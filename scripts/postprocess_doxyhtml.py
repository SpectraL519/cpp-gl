import argparse
import re
import json
from pathlib import Path
from typing import Optional
from bs4 import BeautifulSoup


def encode_md_link(path_str: str) -> str:
    """
    Encode a Markdown filepath according to Doxygen's rules:
    - prefix with 'md_'
    - replace '/' with '_2'
    - double underscores '__' for underscores '_'
    """
    # Separate anchor if present
    if '#' in path_str:
        filepath, anchor = path_str.split('#', 1)
        anchor = '#' + anchor
    else:
        filepath, anchor = path_str, ''

    # Remove leading slash and .md suffix
    filepath = filepath.removeprefix('/')
    if not filepath.endswith('.md'):
        return path_str  # Not a .md file
    filepath = filepath.removesuffix('.md')

    # Encode path separators and underscores
    parts = filepath.split('/')
    encoded_parts = []
    for part in parts:
        # Double underscores
        part = part.replace('_', '__')
        encoded_parts.append(part)
    encoded_path = '_2'.join(encoded_parts)

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
        '[!NOTE]':      ('Note', '#1e90ff', 'fa fa-info-circle'),
        '[!TIP]':       ('Tip', '#28a745', 'fa fa-lightbulb-o'),
        '[!IMPORTANT]': ('Important', '#d63384', 'fa fa-exclamation-circle'),
        '[!WARNING]':   ('Warning', '#fd7e14', 'fa fa-exclamation-triangle'),
        '[!CAUTION]':   ('Caution', '#dc3545', 'fa fa-ban'),
    }

    for tag, (label, color, icon_class) in callouts.items():
        replacement = (
            f'<span style="color: {color}; font-weight: bold; font-size: 1.1em;">'
            f'<i class="{icon_class}" style="margin-right: 6px; vertical-align: middle; font-size: 1.3em; position: relative; top: -0.08em;"></i> {label}:</span>'
        )
        content = content.replace(tag, replacement)

    return content


# def remove_mainpage_title(content: str, filename: str) -> str:
#     if filename != "index.html":
#         return content

#     soup = BeautifulSoup(content, 'html.parser')
#     header_div = soup.find("div", class_="header")
#     if header_div:
#         header_div.decompose()
#     return str(soup)


# ImgRules = list[dict]

# def process_images(content: str, html_path: Path, rules: ImgRules) -> str:
#     soup = BeautifulSoup(content, 'html.parser')

#     for img in soup.find_all('img'):
#         src = img.get('src')
#         if not src:
#             continue

#         if src.startswith(('http://', 'https://', 'data:')):
#             continue

#         filename = Path(src).name
#         if (html_path.parent / filename).exists():
#             img['src'] = filename  # Align the file path

#             alt = img.get('alt', '')
#             for rule in rules: # Apply image style rules
#                 if ("filaneme" in rule and filename == rule["filename"]) or ("alt" in rule and alt == rule["alt"]):
#                     img['style'] = f"{img.get('style', '')}; {rule.get('style', '')}".strip('; ')

#     return str(soup)


def process_file(f: Path, img_rules: Optional[ImgRules] = None):
    content = f.read_text(encoding='utf-8')
    content = process_md_refs(content)
    content = process_gfm(content)
    # content = remove_mainpage_title(content, f.name)
    # if img_rules:
    #     content = process_images(content, f, img_rules)

    f.write_text(content, encoding='utf-8')


def main():
    parser = argparse.ArgumentParser(description='Postprocess Doxygen HTML files to fix links.')
    parser.add_argument('directory', type=Path, help='Root directory of generated HTML files')
    parser.add_argument('--img-rules', type=Path, help='Path to JSON file with image style rules')

    args = parser.parse_args()

    img_rules = None
    if args.img_rules:
        with args.img_rules.open(encoding='utf-8') as f:
            img_rules = json.load(f)

    for f in args.directory.rglob('*.html'):
        process_file(f, img_rules)


if __name__ == '__main__':
    main()
