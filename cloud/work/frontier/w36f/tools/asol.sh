#!/bin/bash
# asol.sh LABEL EDIT.s FN IMG OLIMIT [extra as1 flags] : assemble listing with as1 -Olimit OLIMIT
lab=$1; f=$2; n=$3; img=$4; ol=$5; shift 5
I='$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido'
scp -q "$f" watchman2:rush2049/scratch/frontier/w36f/st_$lab/edit.s
ssh watchman2 "cd ~/rush2049/scratch/frontier/w36f/st_$lab && I=$I && \$I/as0 -G 0 -mips2 -EB -g0 -O3 edit.s -o asl.b -t asl.st >/dev/null 2>&1 && \$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit $ol $* asl.b -o asl.o -t asl.st 2>&1 | head -3"
mkdir -p /home/cburnes/projects/rush2049-decomp/build/trace/w36f
scp -q watchman2:rush2049/scratch/frontier/w36f/st_$lab/asl.o /home/cburnes/projects/rush2049-decomp/build/trace/w36f/asl_$lab.o
python3 $(dirname $0)/oscore.py /home/cburnes/projects/rush2049-decomp/build/trace/w36f/asl_$lab.o $n $img
