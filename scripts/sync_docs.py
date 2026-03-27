#!/usr/bin/env python3
"""
Sync source code sections in documentation files with actual source files.

Each doc in docs/ has a YAML frontmatter field `documentation_of: path/to/file.hpp`.
This script reads that field, grabs the current source code, and updates the
`## Source Code` section in the doc automatically.

Usage:
    python3 scripts/sync_docs.py          # sync all docs
    python3 scripts/sync_docs.py --check  # check only, exit 1 if out of sync
"""

import os
import re
import sys
import glob

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DOCS_DIR = os.path.join(REPO_ROOT, "docs")


def parse_frontmatter(content):
    match = re.match(r"^---\s*\n(.*?)\n---\s*\n", content, re.DOTALL)
    if not match:
        return None
    for line in match.group(1).split("\n"):
        if line.startswith("documentation_of:"):
            return line.split(":", 1)[1].strip()
    return None


def update_source_section(content, source_code):
    idx = content.find("## Source Code")
    if idx != -1:
        before = content[:idx].rstrip()
    else:
        before = content.rstrip()
    return before + f"\n\n## Source Code\n\n```cpp\n{source_code}```\n"


def sync_doc(doc_path, check_only=False):
    with open(doc_path, "r") as f:
        content = f.read()

    source_rel = parse_frontmatter(content)
    if not source_rel:
        return False

    source_path = os.path.join(REPO_ROOT, source_rel)
    if not os.path.exists(source_path):
        rel = os.path.relpath(doc_path, REPO_ROOT)
        print(f"  WARN: {rel} -> source not found: {source_rel}", file=sys.stderr)
        return False

    with open(source_path, "r") as f:
        source_code = f.read()

    if not source_code.endswith("\n"):
        source_code += "\n"

    new_content = update_source_section(content, source_code)

    if new_content != content:
        rel = os.path.relpath(doc_path, REPO_ROOT)
        if check_only:
            print(f"  OUT OF SYNC: {rel}")
        else:
            with open(doc_path, "w") as f:
                f.write(new_content)
            print(f"  UPDATED: {rel}")
        return True

    return False


def main():
    check_only = "--check" in sys.argv

    docs = glob.glob(os.path.join(DOCS_DIR, "**", "*.md"), recursive=True)
    changed = 0
    for doc in sorted(docs):
        if sync_doc(doc, check_only):
            changed += 1

    if changed:
        if check_only:
            print(f"\n{changed} doc(s) out of sync. Run: python3 scripts/sync_docs.py")
            return 1
        else:
            print(f"\nSynced {changed} doc(s).")
    else:
        print("All docs up to date.")

    return 0


if __name__ == "__main__":
    sys.exit(main())
