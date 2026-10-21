#!/bin/sh
# usage: forceo.sh LABEL NAME SPEC IMG [-v]  -- forced-colour oracle on snapshot st_LABEL, scored against the overlay target
SCR=rush2049/scratch/frontier/w36j; B=watchman2
T=/home/cburnes/projects/rush2049-decomp/cloud/work/frontier/tools/trace
lab=$1; n=$2; spec=$3; img=$4; shift 4
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w36j
scp -q $T/remote/tk.sh $B:$SCR/tk/tk.sh && ssh $B "sh ~/$SCR/tk/tk.sh ~/$SCR $lab force $n '$spec'" | grep -i "forced\|declined" ; scp -q $B:$SCR/st_$lab/f.o /home/cburnes/projects/rush2049-decomp/build/trace/w36j/f_$lab.o && python3 /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36j/tools/oscore.py /home/cburnes/projects/rush2049-decomp/build/trace/w36j/f_$lab.o $n $img "$@"
