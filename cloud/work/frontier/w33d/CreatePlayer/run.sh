#!/bin/sh
F=$(realpath "$1"); shift; cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w33d score CreatePlayer --with "$F" --neighbours $(for h in "$@"; do echo --internal $h; done) > build/trace/w33d.cp.out 2>&1
grep -v '^      ' build/trace/w33d.cp.out | cut -c1-150 | tail -3
echo "  $(TAG=w33d python3 cloud/work/frontier/tools/trace/udiff.py CreatePlayer --summary | sed 's/; unverified.*//') | ops $(TAG=w33d python3 cloud/work/frontier/tools/trace/udiff.py CreatePlayer --summary --ops | sed 's/.*rows \([0-9]*\).*/\1/')"
