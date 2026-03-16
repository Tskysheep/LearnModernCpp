#!/usr/bin/env python3
"""
Add '→ 查看完整代码' links to each feature section in index.md overview pages.
Matches lines like: ### XX. 特性名 (`XX_filename.cpp`)
Inserts a link to the corresponding doc page after the section header.
"""

import re
import os

DOCS_DIR = "G:/Test/Claude/LearnCpp/website/docs"
STANDARDS = ["cpp11", "cpp14", "cpp17", "cpp20", "cpp23"]


def cpp_to_doc_path(cpp_filename):
    """Convert .cpp filename to doc page path: 01_auto_and_decltype.cpp → ./01-auto-and-decltype"""
    name = os.path.splitext(cpp_filename)[0]
    name = name.replace("_", "-")
    return f"./{name}"


def process_index(filepath):
    with open(filepath, "r", encoding="utf-8") as f:
        content = f.read()

    lines = content.split("\n")
    new_lines = []
    modified = 0

    i = 0
    while i < len(lines):
        line = lines[i]
        new_lines.append(line)

        # Match section headers like: ### 01. auto 与 decltype (`01_auto_and_decltype.cpp`)
        m = re.match(r'^###\s+\d+\.\s+.+\(`(\d+\w+\.cpp)`\)', line)
        if m:
            cpp_file = m.group(1)
            doc_path = cpp_to_doc_path(cpp_file)
            link_line = f"\n> **[→ 查看完整代码]({doc_path})**"

            # Check if next line is already a code link (avoid duplicates)
            next_line = lines[i + 1] if i + 1 < len(lines) else ""
            if "查看完整代码" not in next_line:
                new_lines.append(link_line)
                modified += 1

        i += 1

    if modified > 0:
        with open(filepath, "w", encoding="utf-8") as f:
            f.write("\n".join(new_lines))

    return modified


def main():
    total = 0
    for std_dir in STANDARDS:
        index_path = os.path.join(DOCS_DIR, std_dir, "index.md")
        if os.path.exists(index_path):
            count = process_index(index_path)
            print(f"[{std_dir}] Added {count} code links")
            total += count
        else:
            print(f"[{std_dir}] index.md not found")

    print(f"\nTotal: {total} links added.")


if __name__ == "__main__":
    main()
