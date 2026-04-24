import argparse
import re
import json
from pathlib import Path
from typing import Optional

ImgRules = list[dict]


def encode_md_link(path_str: str) -> str:
    """
    Encode a Markdown filepath according to Doxygen's rules:
    - prefix with 'md_'
    - replace '/' with '_2'
    - double underscores '__' for underscores '_'
    """
    # Separate anchor if present
    if "#" in path_str:
        filepath, anchor = path_str.split("#", 1)
        anchor = "#" + anchor
    else:
        filepath, anchor = path_str, ""

    # Remove leading slash and .md suffix
    filepath = filepath.removeprefix("/")
    if not filepath.endswith(".md"):
        return path_str  # Not a .md file

    if filepath == "README.md":
        return f"index.html{anchor}"

    filepath = filepath.removesuffix(".md")

    # Encode path separators and underscores
    parts = filepath.split("/")
    encoded_parts = []
    for part in parts:
        # Double underscores
        part = part.replace("_", "__")
        encoded_parts.append(part)
    encoded_path = "_2".join(encoded_parts)

    # Add md_ prefix and .html suffix
    return f"md_{encoded_path}.html{anchor}"


def process_md_refs(content: str) -> str:
    pattern = re.compile(r'href="([^"]+\.md(?:#[^"]*)?)"')

    def replacer(match):
        link = match.group(1)
        new_link = encode_md_link(link)
        return f'href="{new_link}"'

    return pattern.sub(replacer, content)


def process_gfm(content: str) -> str:
    """
    Replace GFM-style callouts with styled inline HTML spans.
    """
    callouts = {
        "[!NOTE]": ("Note", "#1e90ff", "fa fa-info-circle"),
        "[!TIP]": ("Tip", "#28a745", "fa fa-lightbulb-o"),
        "[!IMPORTANT]": ("Important", "#d63384", "fa fa-exclamation-circle"),
        "[!WARNING]": ("Warning", "#fd7e14", "fa fa-exclamation-triangle"),
        "[!CAUTION]": ("Caution", "#dc3545", "fa fa-ban"),
    }

    for tag, (label, color, icon_class) in callouts.items():
        replacement = (
            f'<span style="color: {color}; font-weight: bold; font-size: 1.1em;">'
            f'<i class="{icon_class}" style="margin-right: 6px; vertical-align: middle; font-size: 1.3em; position: relative; top: -0.08em;"></i> {label}:</span>'
        )
        content = content.replace(tag, replacement)

    return content


def process_heading_code_blocks(content: str) -> str:
    content = content.replace("&lt;tt&gt;", "<code>")
    content = content.replace("&lt;/tt&gt;", "</code>")
    return content


def remove_mainpage_title(content: str, filename: str) -> str:
    if filename != "index.html":
        return content

    return re.sub(
        r'<div class="header">\s*<div class="headertitle">.*?</div>\s*</div>',
        "",
        content,
        flags=re.DOTALL,
    )


def process_images(content: str, html_path: Path, rules: list[dict]) -> str:
    def replacer(match):
        img_tag = match.group(0)
        src = match.group(1)
        filename = Path(src).name

        # Skip external or missing files
        if (
            src.startswith(("http", "data:"))
            or not (html_path.parent / filename).exists()
        ):
            return img_tag

        # Fix the src path
        img_tag = img_tag.replace(f'src="{src}"', f'src="{filename}"')

        # Apply style rules
        for rule in rules:
            if (
                rule.get("filename") == filename
                or f'alt="{rule.get("alt")}"' in img_tag
            ):
                style = rule.get("style", "").strip("; ")

                if 'style="' in img_tag:  # Append to existing style
                    img_tag = re.sub(
                        r'style="([^"]*)"',
                        lambda m: f'style="{m.group(1).strip("; ")}; {style}"',
                        img_tag,
                    )
                else:  # Inject new style
                    img_tag = img_tag.replace("/>", f' style="{style}" />').replace(
                        '">', f' style="{style}">'
                    )

        return img_tag

    # Match <img> tags and capture their src attribute
    return re.sub(
        r'<img[^>]*src="([^"]+)"[^>]*>', replacer, content, flags=re.IGNORECASE
    )


def process_file(f: Path, img_rules: Optional[ImgRules] = None):
    content = f.read_text(encoding="utf-8")
    content = process_md_refs(content)
    content = process_gfm(content)
    content = process_heading_code_blocks(content)
    content = remove_mainpage_title(content, f.name)
    if img_rules:
        content = process_images(content, f, img_rules)

    f.write_text(content, encoding="utf-8")


def main():
    parser = argparse.ArgumentParser(
        description="Postprocess Doxygen HTML files to fix links."
    )
    parser.add_argument(
        "directory", type=Path, help="Root directory of generated HTML files"
    )
    parser.add_argument(
        "--img-rules", type=Path, help="Path to JSON file with image style rules"
    )

    args = parser.parse_args()

    img_rules = None
    if args.img_rules:
        with args.img_rules.open(encoding="utf-8") as f:
            img_rules = json.load(f)

    for f in args.directory.rglob("*.html"):
        process_file(f, img_rules)


if __name__ == "__main__":
    main()
