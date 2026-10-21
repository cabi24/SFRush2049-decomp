#!/bin/sh
# usage: asmo.sh LABEL EDIT.s FN IMG  -- assemble an edited ugen listing with stock as0+as1 (snapshot st_LABEL) and score vs overlay target
SCR=rush2049/scratch/frontier/w36f
B=watchman2
T=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace
lab=$1; f=$(realpath "$2"); n=$3; img=$4
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w36f
scp -q $T/remote/tk.sh $B:$SCR/tk/tk.sh && scp -q "$f" $B:$SCR/st_$lab/edit.s && ssh $B "sh ~/$SCR/tk/tk.sh ~/$SCR $lab asm ~/$SCR/st_$lab/edit.s" >/dev/null && scp -q $B:$SCR/st_$lab/asm.o /home/cburnes/projects/rush2049-decomp/build/trace/w36f/asm_$lab.o && python3 /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36f/tools/oscore.py /home/cburnes/projects/rush2049-decomp/build/trace/w36f/asm_$lab.o $n $img
