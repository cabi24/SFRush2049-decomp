#!/bin/sh
# usage: fl.sh LABEL PROC FORCESPEC NAME -- forced uopt + ugen listing (pre-as1) of NAME from snapshot st_LABEL
lab=$1; proc=$2; spec=$3; name=$4
ssh watchman2 "cd ~/rush2049/scratch/frontier/w4b/st_$lab && T=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && cp st st.run && CDX_PROC=$proc CDX_FORCE='$spec' ../uopt/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.f -t st.run optlog >uopt.flog 2>&1 && \$T/ugen -G 0 -mips2 -EB -g0 -O3 opt.f -o gen.f -l out.s -t st.run -temp ugtmp >/dev/null 2>&1; awk '/\.ent\t$name /,/\.end\t$name$/' out.s"
