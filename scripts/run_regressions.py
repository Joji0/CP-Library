import argparse
import os
import subprocess
from pathlib import Path
from tempfile import TemporaryDirectory


def main():
    parser = argparse.ArgumentParser(description="Run local C++ regression tests.")
    parser.add_argument("--compiler", default=os.environ.get("CXX", "g++"))
    args = parser.parse_args()

    root = Path(__file__).resolve().parents[1]
    tests = root / "test"
    sources = sorted([*tests.rglob("*.regression.cpp"), *tests.rglob("*.regression.test.cpp")])
    if not sources:
        parser.error("no regression tests found")

    with TemporaryDirectory(prefix="cp-library-regressions-") as tmp:
        executable = Path(tmp) / "regression"
        for source in sources:
            subprocess.run(
                [
                    args.compiler, "-std=gnu++20", "-O2", "-D_GLIBCXX_ASSERTIONS",
                    "-I", str(root), str(source), "-o", str(executable),
                ],
                check=True,
            )
            subprocess.run([str(executable)], stdin=subprocess.DEVNULL, check=True, timeout=60)
            print(f"PASS {source.relative_to(root)}", flush=True)
    print(f"{len(sources)} regression tests passed")


if __name__ == "__main__":
    main()
