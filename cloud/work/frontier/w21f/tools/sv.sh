#!/bin/sh
# sv.sh FILE.c LABEL: unit build + snapshot + instrumented uopt with SAVELOG; prints strict line and the first
# compute_save summary of the model web (int expr with a bb0 def) and of the int constant-5 web.
f=$(realpath $1); L=$2
cd /home/cburnes/projects/rush2049-decomp; export TAG=w21f; T=cloud/work/frontier/tools/trace
$T/us.sh entity_spawn_init $f | cut -c1-110; $T/snap.sh $L >/dev/null 2>&1
cloud/work/frontier/w21f/tools/oracle.sh $L "SAVELOG=1 ${E:-}" --summary >/dev/null
ssh watchman2 "cd ~/rush2049/scratch/frontier/w21f/st_$L; grep -E 'p1dec phase=p1 proc=94 ' or.spl | grep -nE 'raw|' | grep -E 'web=[0-9]+ ' | head -0; awk '/^\[SAVE\]/' or.spl" > /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/e6d4d7af-aa0e-4f5a-98a8-2e25f3b6dd6b/scratchpad/sv_$L.txt
python3 - /tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/e6d4d7af-aa0e-4f5a-98a8-2e25f3b6dd6b/scratchpad/sv_$L.txt <<'PY'
import sys,re
L=open(sys.argv[1]).read().split('\n')
# group per lr evaluation
cur=[];groups=[]
for l in L:
    if not l.startswith('[SAVE]'): continue
    cur.append(l)
    if ' card=' in l: groups.append(cur); cur=[]
seen=set()
for g in groups:
    lr=re.search(r'lr=(\d+)',g[0]).group(1)
    if lr in seen: continue
    seen.add(lr)
    bbs=[re.search(r'bb=(\d+) u=(\d+) d=(\d+) w=(\d+)',x).groups() for x in g[:-1]]
    m=re.search(r'nocs=(\d+) tot=([-\d.]+)',g[-1]); nocs=int(m.group(1)); tot=float(m.group(2))
    if 40<=nocs<=52 and 25<=tot<=40:
        print(lr, g[-1].split('] ')[1], ' '.join('%s:%s%s/%s'%(b[0],b[1],b[2],b[3]) for b in bbs))
PY
