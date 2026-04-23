import xml.etree.ElementTree as ET
import argparse
import re
from pathlib import Path

class ConceptParser:
    # Configuration for module groups
    GROUPS = {
        "GL": {
            "prefix": "gl::",
            "filename": "gl_traits.md",
            "anchor": "gl-traits-concepts-documentation",
            "title": "GL Traits & Concepts",
            "description": "This page documents the C++20 concepts and type traits used to constrain templates across the GL library."
        },
        "HGL": {
            "prefix": "hgl::",
            "filename": "hgl_traits.md",
            "anchor": "hgl-traits-concepts-documentation",
            "title": "HGL Traits & Concepts",
            "description": "This page documents the C++20 concepts and type traits used to constrain templates across the HGL library."
        }
    }

    # Hidden from the rendered website, but visible to developers opening the raw .md file
    DEV_WARNING = "\n"

    def __init__(self, xml_dir: Path, out_dir: Path):
        self.xml_dir = xml_dir
        self.out_dir = out_dir
        self.concept_links = {}         # Registry mapping refid -> file#anchor
        self.categorized_concepts = {key: [] for key in self.GROUPS.keys()}

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
        if elem.tag == 'parameterlist':
            return ""

        res = elem.text or ""
        for child in elem:
            if child.tag == 'ref':
                ref_text = "".join(child.itertext())
                refid = child.get('refid', '')

                # 1. Known Concepts: Link using the class registry
                if refid in self.concept_links:
                    res += f"[`{ref_text}`]({self.concept_links[refid]})"
                # 2. External/Unknown Concepts
                elif refid.startswith('concept'):
                    ref_anchor = ref_text.replace("::", "-").replace("_", "-")
                    res += f"[`{ref_text}`](#{ref_anchor})"
                # 3. Classes, Structs, Namespaces
                elif refid.startswith('class') or refid.startswith('struct') or refid.startswith('namespace'):
                    res += f"[`{ref_text}`]({refid}.md)"
                else:
                    res += f"`{ref_text}`"

            elif child.tag in ['computeroutput', 'preformatted']:
                res += f"`{self._xml_to_md(child)}`"
            elif child.tag == 'bold':
                res += f"**{self._xml_to_md(child)}**"
            elif child.tag == 'emphasis':
                res += f"*{self._xml_to_md(child)}*"
            elif child.tag == 'itemizedlist':
                res += "\n\n"
                for item in child.findall('listitem'):
                    res += f"- {self._xml_to_md(item).strip()}\n"
                res += "\n\n"
            elif child.tag == 'blockquote':
                bq_text = self._xml_to_md(child).strip()
                res += "\n\n" + "\n".join(f"> {line}" for line in bq_text.splitlines()) + "\n\n"
            elif child.tag == 'simplesect':
                kind = child.get('kind', 'note').upper()
                sect_text = self._xml_to_md(child).strip()
                res += f"\n\n> [!{kind}]\n" + "\n".join(f"> {line}" for line in sect_text.splitlines()) + "\n\n"
            elif child.tag == 'para':
                res += self._xml_to_md(child).strip() + "\n\n"
            elif child.tag == 'title':
                res += f"### {self._xml_to_md(child).strip()}\n\n"
            else:
                res += self._xml_to_md(child)

            res += child.tail or ""

        return res

    def _parse_concept_xml(self, xml_path: Path) -> dict | None:
        """Parses a single Doxygen concept XML file."""
        tree = ET.parse(xml_path)
        root = tree.find('compounddef')

        if root is None or root.get('kind') != 'concept':
            return None

        name = root.findtext('compoundname')
        anchor = name.replace("::", "-").replace("_", "-")
        brief = self._xml_to_md(root.find('briefdescription')).strip()
        params = []

        detailed_desc = root.find('detaileddescription')
        if detailed_desc is not None:
            for param_list in detailed_desc.findall('.//parameterlist[@kind="templateparam"]'):
                for item in param_list.findall('parameteritem'):
                    p_name = self._xml_to_md(item.find('.//parametername')).strip()
                    p_desc = self._xml_to_md(item.find('.//parameterdescription')).strip()
                    params.append({"name": p_name, "desc": p_desc})
                param_list.clear()

        details = self._xml_to_md(detailed_desc).strip()

        constraint = self._get_text(root.find('initializer')).strip()
        constraint = re.sub(r'=\s+', '= ', constraint)

        if constraint.startswith("template"):
            definition = constraint
            if not definition.endswith(";"):
                definition += ";"
        else:
            template_decl = "template <"
            tpl_nodes = root.findall('.//templateparamlist/param')
            tpl_strings = [self._get_text(p).strip() for p in tpl_nodes]
            template_decl += ", ".join(tpl_strings) + ">\n"
            definition = f"{template_decl}concept {name.split('::')[-1]} = {constraint};"

        return {
            "name": name,
            "anchor": anchor,
            "brief": brief,
            "details": details,
            "params": params,
            "definition": definition
        }

    def process(self):
        """Main execution flow: builds registry, parses data, and generates markdown."""
        index_xml = self.xml_dir / "index.xml"
        if not index_xml.exists():
            print(f"Error: Could not find Doxygen index at {index_xml}. Run Doxygen first.")
            return

        tree = ET.parse(index_xml)

        # PASS 1: Build the global concept dictionary for cross-linking
        for compound in tree.findall("compound[@kind='concept']"):
            name = compound.findtext('name')
            if not name:
                continue

            refid = compound.get("refid")
            anchor = name.replace("::", "-").replace("_", "-")

            for group_key, group_info in self.GROUPS.items():
                if name.startswith(group_info["prefix"]):
                    self.concept_links[refid] = f"{group_info['filename']}#{anchor}"
                    break

        # PASS 2: Parse and format the XML
        for compound in tree.findall("compound[@kind='concept']"):
            xml_path = self.xml_dir / f"{compound.get('refid')}.xml"
            if xml_path.exists():
                data = self._parse_concept_xml(xml_path)
                if data:
                    for group_key, group_info in self.GROUPS.items():
                        if data["name"].startswith(group_info["prefix"]):
                            self.categorized_concepts[group_key].append(data)
                            break

        # PASS 3: Generate Markdown files
        self.out_dir.mkdir(parents=True, exist_ok=True)
        self._generate_group_files()
        self._generate_index_file()

    def _generate_group_files(self):
        """Generates the specific group documentation files (e.g., gl_traits.md)."""
        for group_key, group_info in self.GROUPS.items():
            self.categorized_concepts[group_key].sort(key=lambda x: x['name'])
            concepts = self.categorized_concepts[group_key]

            md = f"{self.DEV_WARNING}\n"
            md += f"# {group_info['title']} {{: #{group_info['anchor']} }}\n\n"
            md += f"{group_info['description']}\n\n---\n\n"

            if not concepts:
                md += "*No concepts are currently documented for this module.*\n"
            else:
                for c in concepts:
                    md += f"## `{c['name']}` {{: #{c['anchor']} }}\n\n"
                    if c['brief']:
                        md += f"{c['brief']}\n\n"
                    if c['details']:
                        md += f"### Detailed Description\n\n{c['details']}\n\n"
                    if c['params']:
                        md += "### Template Parameters\n\n| Parameter | Description |\n| :--- | :--- |\n"
                        for p in c['params']:
                            md += f"| `{p['name']}` | {p['desc']} |\n"
                        md += "\n"
                    md += f"### Definition\n\n```cpp\n{c['definition']}\n```\n\n---\n\n"

            out_path = self.out_dir / group_info['filename']
            out_path.write_text(md, encoding="utf-8")
            print(f"Generated {out_path} ({len(concepts)} concepts)")

    def _generate_index_file(self):
        """Generates the central API index mapping to all grouped concepts."""
        md = f"{self.DEV_WARNING}\n"
        md += "# Concepts API Reference {: #concepts-api-reference }\n\n"
        md += "This page serves as the central index for all C++20 concepts used across the library to enforce type safety and template constraints.\n\n---\n\n"

        for group_key, group_info in self.GROUPS.items():
            md += f"## {group_key} Concepts\n\n"
            md += f"- **[{group_info['title']}]({group_info['filename']})**: Full API reference.\n"

            concepts = self.categorized_concepts.get(group_key, [])
            if not concepts:
                md += f"    - *(Documentation coming soon)*\n"
            else:
                for c in concepts:
                    desc_text = f": {c['brief']}" if c['brief'] else ""
                    md += f"    - [`{c['name']}`]({group_info['filename']}#{c['anchor']}){desc_text}\n"
            md += "\n---\n\n"

        index_path = self.out_dir / "concepts.md"
        index_path.write_text(md, encoding="utf-8")
        print(f"Generated {index_path} (API Index)")

if __name__ == "__main__":
    parser = argparse.ArgumentParser()
    parser.add_argument("--xml", default="xml", type=Path, help="Path to Doxygen XML output")
    parser.add_argument("--out", default="docs/cpp-gl", type=Path, help="Path to MkDocs output folder")
    args = parser.parse_args()

    app = ConceptParser(args.xml, args.out)
    app.process()
