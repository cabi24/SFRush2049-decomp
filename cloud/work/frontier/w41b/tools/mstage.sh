#!/bin/sh
# mstage.sh LABEL GROUPDIR(local) : stage a multi-file -O3 group (cc -j each file, uld -kp keep, usplit, umerge)
# into builder snapshot ~/rush2049/scratch/frontier/w41b/st_LABEL, so tools/trace spill.sh/ctrace.sh/force.sh work on it.
set -e
L=$1; G=$2; S=rush2049/scratch/frontier/w41b
ssh watchman2 "rm -rf ~/$S/st_$L && mkdir -p ~/$S/st_$L"
scp -q $G/*.c $G/group.json watchman2:$S/st_$L/
ssh watchman2 "cd ~/$S/st_$L && I=~/$S/bin/ido && python3 -c \"import json;d=json.load(open('group.json'));print('\n'.join(d['keep']))\" > keep.txt && python3 -c \"import json;d=json.load(open('group.json'));print(' '.join(d['files']))\" > files.txt && for f in \$(cat files.txt); do \$I/cc -j -g0 -O3 -mips2 -G 0 -non_shared \$f > cc.log 2>&1 || { tail cc.log; exit 1; }; done && \$I/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt \$(for f in \$(cat files.txt); do echo \${f%.c}.u; done) -ko linked >/dev/null 2>&1 && \$I/usplit -mips2 -o split -t st linked && \$I/umerge -Olimit 5000 -mips2 -EB -g0 -O3 split -o merged -t st && echo staged st_$L"
