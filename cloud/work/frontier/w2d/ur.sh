#!/bin/sh
cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w2d score reverb_setup func_800B4B00 --internal func_800B4B00 --neighbours --with "$1" 2>&1 | grep -v "^       +0x" | tail -5
python3 cloud/work/frontier/w2d/objdiff.py build/blob_unit/w2d/unit.o reverb_setup | tail -${2:-40}
