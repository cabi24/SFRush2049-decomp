#!/bin/sh
# usage: usb.sh cand.c -- AdjustSpeed in the unit: summary line (frame, sp slots), no listing
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w2d score AdjustSpeed $USB_EXTRA --internal func_800960CC --internal func_800A2670 --internal func_800A2678 --with "$1" 2>&1 | grep "AdjustSpeed:\|rror"
grep -A12 "^  AdjustSpeed" build/blob_unit/w2d/umerge.log | grep inlining | sed 's/inlining//' | tr -s '\n ' ' '; echo
python3 cloud/work/frontier/w2d/objdiff.py build/blob_unit/w2d/unit.o AdjustSpeed --all > /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/last.txt
cut -c42- /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/last.txt | grep -o "[a-z0-9]*,[0-9]*(sp)\|a1,sp,[0-9]*\|t[0-9],sp,[0-9]*\|sp,sp,-[0-9]*" | grep -v "^s[0-9]\|^ra" | sort -u | tr '\n' ' '; echo
tail -1 /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad/last.txt
