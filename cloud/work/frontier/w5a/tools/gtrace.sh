#!/bin/sh
# usage: gtrace.sh GROUPDIR LABEL [PROC] -- build an -O3 group (score.py's steps) on the builder up to `merged`,
# snapshot it as ~/rush2049/scratch/frontier/w5a/st_LABEL and run the instrumented uopt (all procs, or PROC detail).
g=$1; lab=$2; proc=$3
ssh watchman2 "rm -rf ~/rush2049/scratch/frontier/w5a/st_$lab && mkdir -p ~/rush2049/scratch/frontier/w5a/st_$lab/src"
tar -C "$g" -cf - . | ssh watchman2 "tar -C ~/rush2049/scratch/frontier/w5a/st_$lab/src -xf -"
ssh watchman2 "cd ~/rush2049/scratch/frontier/w5a/st_$lab/src && T=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido && python3 -c 'import json;g=json.load(open(\"group.json\"));open(\"keep.txt\",\"w\").write(\"\".join(k+\"\\n\" for k in g[\"keep\"]));print(\" \".join(g[\"files\"]))' > files && \$T/cc -j -g0 -O3 -mips2 -G 0 -non_shared \$(cat files) && \$T/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum -kp keep.txt \$(sed 's/\.c/.u/g' files) -ko linked && \$T/usplit -mips2 -o split -t st linked && \$T/umerge -Olimit 5000 -mips2 -EB -g0 -O3 split -o merged -t st && cp merged st .. && echo built"
if [ -z "$proc" ]; then
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w5a && ./tr.sh st_$lab \$PWD/st_$lab/all.txt && grep -c procindex st_$lab/all.txt"
else
  ssh watchman2 "cd ~/rush2049/scratch/frontier/w5a && ./tr.sh st_$lab \$PWD/st_$lab/d.txt CDX_PROC=$proc CDX_DETAIL_WEB=all && ./sum.sh st_$lab/d.txt"
fi
