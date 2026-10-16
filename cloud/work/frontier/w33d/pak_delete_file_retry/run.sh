#!/bin/sh
F=$(realpath "$1"); shift; cd /home/cburnes/projects/rush2049-decomp
python3 -m tools.conveyor.pipeline.blob_unit --tag w33d score pak_delete_file_retry --with "$F" --neighbours --internal func_800A2670 --internal func_800A2678 --internal pak_buffer_free --internal pak_heap_lock --internal pak_heap_unlock $(for h in "$@"; do echo --internal $h; done) > build/trace/w33d.pd.out 2>&1
grep -v '^      ' build/trace/w33d.pd.out | cut -c1-150 | tail -3
echo "  $(TAG=w33d python3 cloud/work/frontier/tools/trace/udiff.py pak_delete_file_retry --summary | sed 's/; unverified.*//') | ops $(TAG=w33d python3 cloud/work/frontier/tools/trace/udiff.py pak_delete_file_retry --summary --ops | sed 's/.*rows \([0-9]*\).*/\1/')"
