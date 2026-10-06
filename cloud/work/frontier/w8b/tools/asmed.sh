#!/bin/sh
# asmed.sh LABEL NAME EDIT.py -- on builder copy ul_LABEL/fn.s (NAME's ugen listing from ulist.sh), apply EDIT.py
# (python; s = text of the listing, edit s in place), assemble standalone with the stock as0+as1 (unit flags),
# and print the aligned diff of NAME against retail.
lab=$1; name=$2; ed=$3
S=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/80274e1e-9a2b-4e9a-b2ec-780e794c33c9/scratchpad
cat > $S/asmed_run.py <<EOF
s = open('fn.s').read()
exec(open('edit.py').read())
open('e.s', 'w').write('\t.verstamp\t3 19\n\t.option\tpic0\n\t.text\n\t.globl\t$name\n' + s)
EOF
scp -q "$ed" watchman2:rush2049/scratch/frontier/w8b/ul_$lab/edit.py
scp -q $S/asmed_run.py watchman2:rush2049/scratch/frontier/w8b/ul_$lab/asmed_run.py
ssh watchman2 "cd ~/rush2049/scratch/frontier/w8b/ul_$lab && rm -f e.o && I=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && python3 asmed_run.py && \$I/as0 -G 0 -mips2 -EB -g0 -O3 e.s -o e.b -t e.st && \$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000 e.b -o e.o -t e.st || echo ASMFAIL"
scp -q watchman2:rush2049/scratch/frontier/w8b/ul_$lab/e.o $S/e_$lab.o
cd /home/cburnes/projects/rush2049-decomp && python3 cloud/work/frontier/w3a/tools/udiff.py $name --obj $S/e_$lab.o
