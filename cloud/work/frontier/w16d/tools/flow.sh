#!/bin/sh
# flow.sh LABEL: uopt flow graph (bb, label, preds, succs) of camera_victory on snapshot st_LABEL (W5D_LEVEL=3 listing)
ssh watchman2 "cd ~/rush2049/scratch/frontier/w16d/st_$1 && cp st st.p3 && W5D_LEVEL=3 W5D_PROC=camera_victory ../bin/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.p3 -t st.p3 -l list3 >/dev/null 2>&1; awk '/flow graph for camera_victory/{p=1;next} p&&/SECONDS/{exit} p{print}' list3; grep -E 'EQ_INEQ|UNROLL' list3 | head -20" | python3 -c '
import re,sys
bbs={};cur=None
for l in sys.stdin:
    m=re.match(r"\s+(\d+)\s+(\d+)\s*$",l)
    if m: cur=int(m.group(1)); bbs[cur]={"lab":int(m.group(2)),"pre":[],"suc":[]}; continue
    m=re.match(r"(pre|suc)::::\s+(\d+)",l)
    if m and cur is not None: bbs[cur][m.group(1)].append(int(m.group(2))); continue
    if "EQ_INEQ" in l or "UNROLL" in l: print(l.strip())
for b in sorted(bbs): print(b, "L%d"%bbs[b]["lab"] if bbs[b]["lab"] else "", "pre",bbs[b]["pre"],"suc",bbs[b]["suc"])
'
