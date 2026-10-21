#!/usr/bin/env python3
"""gobj.py (builder-side) GROUPDIR OUT.o : compile a group dir to an object (run from the lane scratch root)."""
import sys
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
score.compile_group(Path(sys.argv[1]), Path(sys.argv[2]))
