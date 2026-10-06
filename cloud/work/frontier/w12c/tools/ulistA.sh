#!/bin/sh
# ulist.sh FUNC : pre-as1 listing of FUNC from the last blob_unit --tag w12c build
ssh watchman2 "cd ~/rush2049/scratch/frontier/unit/w12c/stage && T=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && cp st st_l && \$T/ugen -G 0 -mips2 -EB -g0 -O3 opt -o gen_l -l unit_l.s -t st_l -temp ugtmp_l >/dev/null 2>&1; awk '/^$1:/{p=1} p{print} /\.end\t$1\$/{exit}' unit_l.s | grep -v '\.loc\|\.livereg'"
