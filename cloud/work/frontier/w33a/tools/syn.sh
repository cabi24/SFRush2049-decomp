#!/bin/sh
# syn.sh file.c [fn]: compile synthetic file on builder at -O3 and print frames of all functions
f=$1
scp -q $f watchman2:rush2049/scratch/frontier/w33a/syn/
ssh watchman2 "cd ~/rush2049/scratch/frontier/w33a/syn; IDO=~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; b=\$(basename $f .c); \$IDO/cc -c -O3 -mips2 -G 0 -non_shared -g0 \$b.c -o \$b.o 2>&1 | grep -v warning | head -5; mips-linux-gnu-objdump -d \$b.o | awk '/^[0-9a-f]+ <.*>:/{n=\$2} /addiu\tsp,sp,-/{print n, \$NF}'; [ -n \"$2\" ] && mips-linux-gnu-objdump -d --no-show-raw-insn \$b.o | awk '/<$2>:/{p=1} p&&/^\$/{exit} p'"
