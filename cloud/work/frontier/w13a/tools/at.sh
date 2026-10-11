#!/bin/sh
# at.sh LABEL [REG]: traced uopt (bin/uopt_al, w12i patch) on snapshot st_LABEL; E05F0's BB/CHK/BIR lines for colour REG (default 7=t0)
R=${2:-7}
ssh watchman2 "cd ~/rush2049/scratch/frontier/w13a && o=\$(sh tk/tk.sh \$PWD $1 ord func_800E05F0) && cd st_$1 && cp st st.al && ALTRACE=1 ../bin/uopt_al -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.al -t st.al optlog 2>al.log; awk -v n=\$((o+1)) '/^PROC/{k++} k==n' al.log > e.al; grep -E '^BB|reg=$R ' e.al | sed 's/ m2=.*//' | awk '/^BB/{b=\$0; next} {print b \" :: \" \$0}'"
