import xml.etree.ElementTree as ET
import argparse
import re
import json
from pathlib import Path
from dataclasses import dataclass


@dataclass
class TParamDescriptor:
    name: str
    desc: str


@dataclass
class ConceptDescriptor:
    name: str
    anchor: str
    brief: str
    details: str
    params: list[TParamDescriptor]
    definition: str
    filename: str
    line: int


class ConceptParser:
    def __init__(self, xml_dir: Path, out_dir: Path, config: dict, sort_method: str):
        self.xml_dir = xml_dir
        self.out_dir = out_dir
        self.sort_method = sort_method

        # Extract the index configuration
        self.index_config = config.get("index")
        if not self.index_config:
            raise ValueError(
                "Configuration must include an 'index' block with title, anchor, and description."
            )

        self.groups = config.get("groups", {})

        self.concept_links = {}  # Registry mapping refid -> file#anchor
        self.namespace_links = {}  # Registry mapping full namespace name -> file.md
        self.categorized_concepts = {key: [] for key in self.groups.keys()}

    @staticmethod
    def _get_text(element: ET.Element) -> str:
        """Extracts raw text from an XML element, stripping all tags. (Used for pure C++ blocks)"""
        if element is None:
            return ""
        return "".join(element.itertext()).strip()

    def _xml_to_md(self, elem: ET.Element) -> str:
        """Recursively converts Doxygen XML text nodes to Markdown links and formatting."""
        if elem is None:
            return ""

        # Ignore template parameter lists (they are handled separately for the table)
        if elem.tag == "parameterlist":
            return ""

        res = elem.text or ""
        for child in elem:
            if child.tag == "ref":
                ref_text = "".join(child.itertext())
                refid = child.get("refid", "")

                # Known concept with a registered link
                if refid in self.concept_links:
                    res += f"[`{ref_text}`]({self.concept_links[refid]})"
                # External/Unknown Concepts
                elif refid.startswith("concept"):
                    ref_anchor = ref_text.replace("::", "-").replace("_", "-")
                    res += f"[`{ref_text}`](#{ref_anchor})"
                # Classes, Structs, Namespaces
                elif (
                    refid.startswith("class")
                    or refid.startswith("struct")
                    or refid.startswith("namespace")
                ):
                    res += f"[`{ref_text}`]({refid}.md)"
                else:
                    res += f"`{ref_text}`"

            elif child.tag in ["computeroutput", "preformatted"]:
                inner = self._xml_to_md(child)

                # Prevents wrapping a Markdown link in backticks, which breaks the link.
                # Instead, it places backticks INSIDE the brackets.
                match = re.fullmatch(r"\[(`?)(.*?)\1\]\((.*?)\)", inner.strip())
                if match:
                    res += f"[`{match.group(2)}`]({match.group(3)})"
                elif "](" in inner:
                    # Failsafe for complex strings containing links
                    res += inner
                else:
                    res += f"`{inner}`"

            elif child.tag == "bold":
                res += f"**{self._xml_to_md(child)}**"

            elif child.tag == "emphasis":
                res += f"*{self._xml_to_md(child)}*"

            elif child.tag == "itemizedlist":
                res += "\n\n"
                for item in child.findall("listitem"):
                    res += f"- {self._xml_to_md(item).strip()}\n"
                res += "\n\n"

            elif child.tag == "orderedlist":
                res += "\n\n"
                for i, item in enumerate(child.findall("listitem"), start=1):
                    res += f"{i}. {self._xml_to_md(item).strip()}\n"
                res += "\n\n"

            elif child.tag == "blockquote":
                bq_text = self._xml_to_md(child).strip()
                res += (
                    "\n\n"
                    + "\n".join(f"> {line}" for line in bq_text.splitlines())
                    + "\n\n"
                )

            elif child.tag == "simplesect":
                kind = child.get("kind", "note").upper()
                sect_text = self._xml_to_md(child).strip()
                res += (
                    f"\n\n> [!{kind}]\n"
                    + "\n".join(f"> {line}" for line in sect_text.splitlines())
                    + "\n\n"
                )

            elif child.tag == "para":
                res += self._xml_to_md(child).strip() + "\n\n"

            elif child.tag == "title":
                res += f"### {self._xml_to_md(child).strip()}\n\n"

            else:
                res += self._xml_to_md(child)

            res += child.tail or ""

        return res

    def _parse_concept_xml(self, xml_path: Path) -> ConceptDescriptor | None:
        """Parses a single Doxygen concept XML file."""
        tree = ET.parse(xml_path)
        root = tree.find("compounddef")

        if root is None or root.get("kind") != "concept":
            return None

        name = root.findtext("compoundname")
        anchor = name.replace("::", "-").replace("_", "-")
        brief = self._xml_to_md(root.find("briefdescription")).strip()
        params = []

        detailed_desc = root.find("detaileddescription")
        if detailed_desc is not None:
            for param_list in detailed_desc.findall(
                './/parameterlist[@kind="templateparam"]'
            ):
                for item in param_list.findall("parameteritem"):
                    p_name = self._xml_to_md(item.find(".//parametername")).strip()
                    p_desc = self._xml_to_md(
                        item.find(".//parameterdescription")
                    ).strip()
                    params.append(TParamDescriptor(name=p_name, desc=p_desc))
                param_list.clear()

        details = self._xml_to_md(detailed_desc).strip()

        constraint = self._get_text(root.find("initializer")).strip()
        constraint = re.sub(r"=\s+", "= ", constraint)

        if constraint.startswith("template"):
            definition = constraint
            if not definition.endswith(";"):
                definition += ";"
        else:
            template_decl = "template <"
            tpl_nodes = root.findall(".//templateparamlist/param")
            tpl_strings = [self._get_text(p).strip() for p in tpl_nodes]
            template_decl += ", ".join(tpl_strings) + ">\n"
            definition = (
                f"{template_decl}concept {name.split('::')[-1]} = {constraint};"
            )

        # Extract location data to allow sorting by declaration order
        location_node = root.find("location")
        if location_node is not None:
            file_path = location_node.get("file", "")
            line_num = int(location_node.get("line", "0"))
        else:
            file_path = ""
            line_num = 0

        return ConceptDescriptor(
            name=name,
            anchor=anchor,
            brief=brief,
            details=details,
            params=params,
            definition=definition,
            filename=file_path,
            line=line_num,
        )

    def process(self):
        """Main execution flow: builds registry, parses data, and generates markdown."""
        index_xml = self.xml_dir / "index.xml"
        if not index_xml.exists():
            print(
                f"Error: Could not find Doxygen index at {index_xml}. Run Doxygen first."
            )
            return

        tree = ET.parse(index_xml)

        # PASS 1: Build the concept registry for cross-linking
        for compound in tree.findall("compound[@kind='concept']"):
            name = compound.findtext("name")
            if not name:
                continue

            refid = compound.get("refid")
            anchor = name.replace("::", "-").replace("_", "-")

            for group_key, group_info in self.groups.items():
                if name.startswith(group_info["prefix"]):
                    self.concept_links[refid] = f"{group_info['filename']}#{anchor}"
                    break

        # Build the namespace registry for cross-linking
        for compound in tree.findall("compound[@kind='namespace']"):
            name = compound.findtext("name")
            refid = compound.get("refid")
            if name and refid:
                self.namespace_links[name] = f"{refid}.md"

        # PASS 2: Parse and format the XML
        for compound in tree.findall("compound[@kind='concept']"):
            xml_path = self.xml_dir / f"{compound.get('refid')}.xml"
            if xml_path.exists():
                data = self._parse_concept_xml(xml_path)
                if data:
                    for group_key, group_info in self.groups.items():
                        if data.name.startswith(group_info["prefix"]):
                            self.categorized_concepts[group_key].append(data)
                            break

        # PASS 3: Generate Markdown files
        self.out_dir.mkdir(parents=True, exist_ok=True)
        self._generate_group_files()
        self._generate_index_file()

    def _generate_group_files(self):
        """Generates the specific group documentation files (e.g., gl_concepts.md)."""
        for group_key, group_info in self.groups.items():
            if self.sort_method == "source":
                self.categorized_concepts[group_key].sort(
                    key=lambda x: (x.filename, x.line)
                )
            elif self.sort_method == "alpha":
                self.categorized_concepts[group_key].sort(key=lambda x: x.name)

            concepts = self.categorized_concepts[group_key]

            md = f"# {group_info['title']} {{: #{group_info['anchor']} }}\n\n"
            md += f"{group_info['description']}\n\n---\n\n"

            if not concepts:
                md += "*No concepts are currently documented for this module.*\n"
            else:
                for c in concepts:
                    md += f"## `{c.name}` {{: #{c.anchor} }}\n\n"
                    if c.brief:
                        md += f"{c.brief}\n\n"
                    if c.details:
                        md += f"### Detailed Description\n\n{c.details}\n\n"
                    if c.params:
                        md += "### Template Parameters\n\n| Parameter | Description |\n| :--- | :--- |\n"
                        for p in c.params:
                            md += f"| `{p.name}` | {p.desc} |\n"
                        md += "\n"
                    md += f"### Definition\n\n```cpp\n{c.definition}\n```\n\n---\n\n"

            out_path = self.out_dir / group_info["filename"]
            out_path.write_text(md, encoding="utf-8")
            print(f"Generated {out_path} ({len(concepts)} concepts)")

    def _generate_index_file(self):
        """Generates the central API index mapping to all grouped concepts, nested by namespace."""
        md = f"# {self.index_config['title']} {{: #{self.index_config['anchor']} }}\n\n"

        if self.index_config.get("description"):
            md += f"{self.index_config['description']}\n\n"

        md += "Here are the concepts with brief descriptions:\n\n"

        # Build a global tree of all concepts across all groups
        root_node = {"_concepts": [], "_namespaces": {}}

        for group_key, group_info in self.groups.items():
            concepts = self.categorized_concepts.get(group_key, [])
            for c in concepts:
                c_url = f"{group_info['filename']}#{c.anchor}"
                parts = c.name.split("::")
                ns_parts = parts[:-1]
                short_name = parts[-1]

                # Traverse / Build the tree structure
                curr = root_node
                current_ns_path = ""
                for ns in ns_parts:
                    if current_ns_path:
                        current_ns_path += f"::{ns}"
                    else:
                        current_ns_path = ns

                    if ns not in curr["_namespaces"]:
                        curr["_namespaces"][ns] = {
                            "_concepts": [],
                            "_namespaces": {},
                            "_full_name": current_ns_path,
                        }
                    curr = curr["_namespaces"][ns]

                curr["_concepts"].append(
                    {"short_name": short_name, "url": c_url, "brief": c.brief}
                )

        # Recursive function to render the tree into Markdown
        def _render_tree(node, indent_level):
            tree_md = ""
            indent = "    " * indent_level

            for ns_name, ns_node in sorted(node["_namespaces"].items()):
                full_ns_name = ns_node["_full_name"]
                ns_url = self.namespace_links.get(full_ns_name)
                if ns_url:
                    tree_md += f"{indent}- **namespace** [**{ns_name}**]({ns_url})\n"
                else:
                    tree_md += f"{indent}- **namespace** **{ns_name}**\n"

                tree_md += _render_tree(ns_node, indent_level + 1)

            for c_dict in sorted(node["_concepts"], key=lambda x: x["short_name"]):
                desc_text = f" {c_dict['brief']}" if c_dict["brief"] else ""
                tree_md += f"{indent}- **concept** [**{c_dict['short_name']}**]({c_dict['url']}){desc_text}\n"

            return tree_md

        # 3. Render the tree and append to the document
        rendered_tree = _render_tree(root_node, 0)

        if not rendered_tree.strip():
            md += "*No concepts found.*\n"
        else:
            md += rendered_tree

        md += "\n---\n\n"

        index_path = self.out_dir / "concepts.md"
        index_path.write_text(md, encoding="utf-8")
        print(f"Generated {index_path} (API Index, Nested with Links)")


if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--xml", type=Path, help="Path to Doxygen XML output")
    parser.add_argument("--out", type=Path, help="Path to MkDocs output folder")
    parser.add_argument(
        "--config", type=Path, help="Path to the groups JSON configuration file"
    )
    parser.add_argument(
        "--sort",
        type=str,
        choices=["source", "alpha", "none"],
        default="source",
        help="How to sort the concepts: 'source' (by filename and line), 'alpha' (alphabetically by name), or 'none' (parse order).",
    )
    args = parser.parse_args()

    # Load configuration from JSON
    if not args.config.exists():
        print(f"Error: Configuration file '{args.config}' not found.")
        exit(1)

    with open(args.config, "r", encoding="utf-8") as f:
        config = json.load(f)

    app = ConceptParser(args.xml, args.out, config=config, sort_method=args.sort)
    app.process()
