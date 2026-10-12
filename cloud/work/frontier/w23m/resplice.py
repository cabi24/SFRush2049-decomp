#!/usr/bin/env python3
"""w23m: w23g's rename_resplice.py (prove/apply a pure rename of locked game
sources, recompile, image gate, re-pin) run in this lane's builder scratch and
writing its proof next to this script. Same arguments as rename_resplice.py."""
import sys
from pathlib import Path

HERE = Path(__file__).resolve().parent
sys.path.insert(0, str(HERE.parent / "w23g"))
import rename_resplice as rr  # noqa: E402

rr.WORK = "rush2049/scratch/frontier/w23m"
rr.OUT_DIR = HERE

if __name__ == "__main__":
    sys.exit(rr.main())
