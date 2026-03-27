#!/usr/bin/env python3
"""
Build automation for CP-Library documentation site.

Run after oj-verify to prepare docs for Jekyll:
  1. Parse .hpp dependency graph (Depends on / Required by)
  2. Sync ## Source Code sections with actual .hpp files
  3. Inject ## Verified with sections from test/verification results
  4. Auto-generate _includes/sidebar.html from docs/ directory

Usage:
    python3 scripts/build_site.py           # run all steps
    python3 scripts/build_site.py --check   # dry-run, exit 1 if changes needed
"""

import os
import re
import sys
import glob
import json

REPO_ROOT = os.path.dirname(os.path.dirname(os.path.abspath(__file__)))
DOCS_DIR = os.path.join(REPO_ROOT, "docs")
TEST_DIR = os.path.join(REPO_ROOT, "test")
INCLUDES_DIR = os.path.join(REPO_ROOT, "_includes")
TIMESTAMPS_FILE = os.path.join(REPO_ROOT, ".verify-helper", "timestamps.remote.json")

# Sidebar section ordering — more specific prefixes first so they match before
# the catch-all parent prefix (e.g. ds/segtree before ds).
SECTIONS = [
	("ds/segtree", "Segment Trees"),
	("ds/unionfind", "Union-Find"),
	("ds", "Other DS"),
	("monoid", "Monoid"),
	("acted_monoid", "ActedMonoid"),
	("graph", "Graph"),
	("mod", "Modular Arithmetic"),
	("nt", "Number Theory"),
	("bigdec", "BigDec"),
]

# All auto-generated section headings, in the order they appear in docs.
AUTO_HEADINGS = ["## Depends on", "## Required by", "## Verified with", "## Source Code"]


# ── Helpers ──────────────────────────────────────────────────────────

def parse_frontmatter(content):
	"""Extract YAML frontmatter fields as a dict."""
	m = re.match(r"^---\s*\n(.*?)\n---\s*\n", content, re.DOTALL)
	if not m:
		return {}
	fields = {}
	for line in m.group(1).split("\n"):
		if ":" in line:
			key, val = line.split(":", 1)
			fields[key.strip()] = val.strip().strip('"').strip("'")
	return fields


def strip_auto_sections(content):
	"""Remove all auto-generated sections."""
	positions = []
	for heading in AUTO_HEADINGS:
		idx = content.find(heading)
		if idx != -1:
			positions.append(idx)
	if positions:
		content = content[:min(positions)].rstrip()
	return content


def problem_name(url):
	"""Derive readable problem name from judge URL."""
	m = re.match(r"https://judge\.yosupo\.jp/problem/(\w+)", url)
	if m:
		return "Library Checker: " + m.group(1)
	m = re.match(r"https://yukicoder\.me/problems/no/(\d+)", url)
	if m:
		return "yukicoder No." + m.group(1)
	return url


def relative_doc_link(from_doc_rel, to_doc_rel):
	"""Compute relative URL between two docs (paths relative to docs/)."""
	from_dir = os.path.dirname(from_doc_rel)
	to_path = os.path.splitext(to_doc_rel)[0]
	return os.path.relpath(to_path, from_dir).replace("\\", "/")


# ── Parse .hpp dependency graph ──────────────────────────────────────

def parse_hpp_deps():
	"""Scan all .hpp files for library #include directives.
	Returns (forward_deps, reverse_deps) dicts keyed by hpp path relative to repo root."""
	forward = {}   # hpp -> [included hpps]
	reverse = {}   # hpp -> [files that include it]

	skip_prefixes = (".", "_")
	for hpp_file in glob.glob(os.path.join(REPO_ROOT, "**", "*.hpp"), recursive=True):
		hpp_rel = os.path.relpath(hpp_file, REPO_ROOT).replace("\\", "/")
		if any(hpp_rel.startswith(p) for p in skip_prefixes):
			continue
		with open(hpp_file) as f:
			src = f.read()
		includes = re.findall(r'#include\s+"([^"]+\.hpp)"', src)
		if includes:
			forward[hpp_rel] = includes
			for inc in includes:
				reverse.setdefault(inc, []).append(hpp_rel)

	return forward, reverse


# ── Parse test files ─────────────────────────────────────────────────

def parse_tests():
	"""Build mapping: hpp_path -> [(test_rel_path, problem_url)]."""
	hpp_to_tests = {}
	for tf in glob.glob(os.path.join(TEST_DIR, "**", "*.test.cpp"), recursive=True):
		with open(tf) as f:
			src = f.read()
		pm = re.search(r'#define\s+PROBLEM\s+"([^"]+)"', src)
		if not pm:
			continue
		url = pm.group(1)
		test_rel = os.path.relpath(tf, REPO_ROOT).replace("\\", "/")
		for inc in re.finditer(r'#include\s+"([^"]+\.hpp)"', src):
			hpp = inc.group(1)
			hpp_to_tests.setdefault(hpp, []).append((test_rel, url))
	return hpp_to_tests


# ── Process docs ─────────────────────────────────────────────────────

def process_docs(forward_deps, reverse_deps, hpp_to_tests, hpp_to_doc, timestamps, check_only=False):
	"""Sync source code and inject dependency/verification sections."""
	changed = 0

	for doc_path in sorted(glob.glob(os.path.join(DOCS_DIR, "**", "*.md"), recursive=True)):
		with open(doc_path) as f:
			original = f.read()

		fm = parse_frontmatter(original)
		source_rel = fm.get("documentation_of", "")
		if not source_rel:
			continue

		doc_rel = os.path.relpath(doc_path, DOCS_DIR).replace("\\", "/")
		base = strip_auto_sections(original)
		parts = [base]

		# ── Depends on ──
		deps = forward_deps.get(source_rel, [])
		if deps:
			lines = ["## Depends on", ""]
			for dep in sorted(deps):
				if dep in hpp_to_doc:
					link = relative_doc_link(doc_rel, hpp_to_doc[dep])
					lines.append("* [" + dep + "](" + link + ")")
				else:
					lines.append("* `" + dep + "`")
			parts.append("\n".join(lines))

		# ── Required by ──
		rdeps = reverse_deps.get(source_rel, [])
		if rdeps:
			lines = ["## Required by", ""]
			for rdep in sorted(rdeps):
				if rdep in hpp_to_doc:
					link = relative_doc_link(doc_rel, hpp_to_doc[rdep])
					lines.append("* [" + rdep + "](" + link + ")")
				else:
					lines.append("* `" + rdep + "`")
			parts.append("\n".join(lines))

		# ── Verified with ──
		tests = hpp_to_tests.get(source_rel, [])
		if tests:
			lines = ["## Verified with", ""]
			for test_rel, url in sorted(tests):
				name = problem_name(url)
				test_name = os.path.basename(test_rel)
				status = "AC" if test_rel in timestamps else "WJ"
				lines.append("* **" + status + "** [" + name + "](" + url + ")" + " &mdash; `" + test_name + "`")
			parts.append("\n".join(lines))

		# ── Source Code ──
		source_path = os.path.join(REPO_ROOT, source_rel)
		if os.path.exists(source_path):
			with open(source_path) as f:
				code = f.read()
			if not code.endswith("\n"):
				code += "\n"
			parts.append("## Source Code\n\n```cpp\n" + code + "```")

		new_content = "\n\n".join(parts) + "\n"
		if new_content != original:
			rel = os.path.relpath(doc_path, REPO_ROOT)
			if check_only:
				print("  OUT OF SYNC: " + rel)
			else:
				with open(doc_path, "w") as f:
					f.write(new_content)
				print("  UPDATED: " + rel)
			changed += 1

	return changed


# ── Generate sidebar ─────────────────────────────────────────────────

def generate_sidebar(check_only=False):
	"""Auto-generate _includes/sidebar.html from docs/ directory."""
	entries = []
	for doc_path in sorted(glob.glob(os.path.join(DOCS_DIR, "**", "*.md"), recursive=True)):
		with open(doc_path) as f:
			content = f.read()
		fm = parse_frontmatter(content)
		title = fm.get("title", "")
		if not title:
			continue
		rel = os.path.relpath(doc_path, DOCS_DIR).replace("\\", "/")
		url_path = os.path.splitext(rel)[0]
		entries.append((rel, url_path, title))

	used = set()
	sections_data = []
	for prefix, section_name in SECTIONS:
		items = []
		for rel, url_path, title in entries:
			if rel in used:
				continue
			doc_dir = os.path.dirname(rel).replace("\\", "/")
			if doc_dir == prefix or doc_dir.startswith(prefix + "/"):
				items.append((url_path, title))
				used.add(rel)
		if items:
			sections_data.append((section_name, items))

	remaining = [(url_path, title) for rel, url_path, title in entries if rel not in used]
	if remaining:
		sections_data.append(("Other", remaining))

	html = []
	for section_name, items in sections_data:
		html.append('<div class="sidebar-section">')
		html.append('  <div class="sidebar-section-title">')
		html.append('    <span class="arrow">&#9660;</span> <span>' + section_name + '</span>')
		html.append('  </div>')
		html.append('  <ul class="sidebar-links">')
		for url_path, title in items:
			href = "{{ '/docs/" + url_path + "' | relative_url }}"
			html.append('    <li><a href="' + href + '">' + title + '</a></li>')
		html.append('  </ul>')
		html.append('</div>')
		html.append('')

	new_content = "\n".join(html)

	sidebar_path = os.path.join(INCLUDES_DIR, "sidebar.html")
	try:
		with open(sidebar_path) as f:
			old = f.read()
	except FileNotFoundError:
		old = ""

	if new_content != old:
		if check_only:
			print("  OUT OF SYNC: _includes/sidebar.html")
			return 1
		with open(sidebar_path, "w") as f:
			f.write(new_content)
		print("  UPDATED: _includes/sidebar.html")
		return 1
	return 0


# ── Main ─────────────────────────────────────────────────────────────

def main():
	check_only = "--check" in sys.argv

	# Load verification timestamps
	timestamps = {}
	if os.path.exists(TIMESTAMPS_FILE):
		with open(TIMESTAMPS_FILE) as f:
			timestamps = json.load(f)

	# Build hpp -> doc mapping (for linking dependencies)
	print("=== Indexing docs ===")
	hpp_to_doc = {}
	for doc_path in glob.glob(os.path.join(DOCS_DIR, "**", "*.md"), recursive=True):
		with open(doc_path) as f:
			content = f.read()
		fm = parse_frontmatter(content)
		doc_of = fm.get("documentation_of", "")
		if doc_of:
			doc_rel = os.path.relpath(doc_path, DOCS_DIR).replace("\\", "/")
			hpp_to_doc[doc_of] = doc_rel
	print("  Indexed " + str(len(hpp_to_doc)) + " docs")

	# Parse dependency graph
	print("\n=== Parsing .hpp dependencies ===")
	forward_deps, reverse_deps = parse_hpp_deps()
	total_edges = sum(len(v) for v in forward_deps.values())
	print("  Found " + str(total_edges) + " dependency edges across " + str(len(forward_deps)) + " files")

	# Parse test -> hpp mappings
	print("\n=== Parsing test files ===")
	hpp_to_tests = parse_tests()
	total_links = sum(len(v) for v in hpp_to_tests.values())
	print("  Found " + str(total_links) + " test links across " + str(len(hpp_to_tests)) + " headers")

	# Process all docs
	print("\n=== Processing docs ===")
	doc_changes = process_docs(forward_deps, reverse_deps, hpp_to_tests, hpp_to_doc, timestamps, check_only)

	# Generate sidebar
	print("\n=== Generating sidebar ===")
	sidebar_changes = generate_sidebar(check_only)

	total = doc_changes + sidebar_changes
	if total:
		if check_only:
			print("\n" + str(total) + " file(s) out of sync. Run: python3 scripts/build_site.py")
			return 1
		print("\nUpdated " + str(total) + " file(s).")
	else:
		print("\nEverything up to date.")
	return 0


if __name__ == "__main__":
	sys.exit(main())
