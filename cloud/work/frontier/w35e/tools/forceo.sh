#!/bin/sh
# usage: forceo.sh LABEL NAME SPEC IMG [-v]  -- forced-colour oracle on snapshot st_LABEL, scored against the overlay target
SCR=rush2049/scratch/frontier/w35e; B=watchman2
T=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace
lab=$1; n=$2; spec=$3; img=$4; shift 4
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w35e
scp -q $T/remote/tk.sh $B:$SCR/tk/tk.sh && ssh $B "sh ~/$SCR/tk/tk.sh ~/$SCR $lab force $n '$spec'" | grep -i "forced\|declined" ; scp -q $B:$SCR/st_$lab/f.o /home/cburnes/projects/rush2049-decomp/build/trace/w35e/f_$lab.o && python3 /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w35e/tools/oscore.py /home/cburnes/projects/rush2049-decomp/build/trace/w35e/f_$lab.o $n $img "$@"
