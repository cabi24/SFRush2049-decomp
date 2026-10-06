#!/bin/sh
# asmfn.sh FN.s NAME EDIT.py -- edit a local ugen listing of one function (python: s = text), assemble standalone with
# stock as0+as1 on the builder, aligned diff of NAME vs retail.
fn=$1; name=$2; ed=$3
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad
python3 - "$fn" "$ed" "$name" > $S/af_$name.s <<'PY'
import sys
s=open(sys.argv[1]).read()
exec(open(sys.argv[2]).read())
print('\t.verstamp\t3 19\n\t.option\tpic0\n\t.text\n\t.globl\t%s\n' % sys.argv[3] + s)
PY
ssh watchman2 "mkdir -p ~/rush2049/scratch/frontier/w10a/af" && scp -q $S/af_$name.s watchman2:rush2049/scratch/frontier/w10a/af/e.s
ssh watchman2 "cd ~/rush2049/scratch/frontier/w10a/af && rm -f e.o && I=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && \$I/as0 -G 0 -mips2 -EB -g0 -O3 e.s -o e.b -t e.st 2>&1 | head -3 && \$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 e.b -o e.o -t e.st || echo ASMFAIL"
scp -q watchman2:rush2049/scratch/frontier/w10a/af/e.o $S/af_$name.o
cd /home/cburnes/projects/rush2049-decomp && python3 cloud/work/frontier/w3a/tools/udiff.py $name --obj $S/af_$name.o
