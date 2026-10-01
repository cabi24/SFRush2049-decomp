#!/usr/bin/env python3
"""Run the vendored n64-decomp-workbench without installing it.

    python3 tools/workbench.py diagnose TARGET.o CANDIDATE.o --function FN --objdump mips-linux-gnu-objdump
    python3 tools/workbench.py guide            # field-guide topics (playbooks, verdicts, levers)
    python3 tools/workbench.py guide 15         # one lever;  `guide laws ido53 L64` prints a compiler law

Arguments pass straight through to `decomp-workbench`. See docs/external/README.md.
"""
import sys
from pathlib import Path

SRC = Path(__file__).resolve().parents[1] / "third_party" / "n64-decomp-workbench" / "src"

if __name__ == "__main__":
    sys.path.insert(0, str(SRC))
    from decomp_workbench.cli import main
    sys.exit(main(sys.argv[1:]))
