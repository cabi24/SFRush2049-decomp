#!/usr/bin/env python3
"""Run cloud/work/boot_tail_promotion/context_check.py (unchanged) with IDO 7.1 as the ROM-TU compiler
(tk71/ido/cc, scratch root). Usage: python3 lane/ctx71.py OUT.jsonl fn=src ..."""
import sys
from pathlib import Path
sys.path.insert(0, str(Path.cwd() / "cloud/work/boot_tail_promotion"))
import context_check as cc  # noqa: E402
cc.CC = Path.cwd() / "tk71/ido/cc"
print("context_check CC:", cc.CC)
cc.main()
