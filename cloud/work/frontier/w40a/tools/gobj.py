#!/usr/bin/env python3
"""Builder-side: compile a group dir (score.compile_group) to OUT.o. Usage (scratch root): gobj.py GROUP_DIR OUT.o"""
import sys
from pathlib import Path
sys.path.insert(0, 'tools/cloud')
import score
score.compile_group(Path(sys.argv[1]), Path(sys.argv[2]))
