import argparse
import os
import subprocess
from concurrent.futures import ThreadPoolExecutor
from pathlib import Path


def main():
    parser = argparse.ArgumentParser(description="Compile each header and all headers together.")
    parser.add_argument("--compiler", default=os.environ.get("CXX", "g++"))
    args = parser.parse_args()
    root = Path(__file__).resolve().parents[1]
    headers = sorted(
        path.relative_to(root).as_posix()
        for path in root.rglob("*.hpp")
        if not any(part.startswith(".") for part in path.relative_to(root).parts)
    )
    if not headers:
        parser.error("no library headers found")

    command = [args.compiler, "-std=gnu++20", "-I", str(root), "-fsyntax-only", "-x", "c++", "-"]

    def check(job):
        label, source = job
        result = subprocess.run(command, input=source, text=True, capture_output=True)
        return label, result

    includes = [f'#include "{header}"\n' for header in headers]
    jobs = list(zip(headers, includes)) + [("all headers together", "".join(includes))]
    failures = 0
    with ThreadPoolExecutor(max_workers=min(4, os.cpu_count() or 1)) as workers:
        for label, result in workers.map(check, jobs):
            if result.returncode:
                failures += 1
                print(f"{label}\n{result.stderr}")

    print(f"{len(jobs) - failures}/{len(jobs)} header checks passed")
    return bool(failures)


if __name__ == "__main__":
    raise SystemExit(main())
