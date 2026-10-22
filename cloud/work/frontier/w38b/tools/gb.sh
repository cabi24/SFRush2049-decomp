#!/bin/sh
# gb.sh ovl_a|ovl_b DIR... : score several group dirs (sequentially) and print one summary line each.
IMG="$1"; shift
R=rush2049/scratch/frontier/w38b
for D in "$@"; do
  LABEL=$(echo "$D" | sed 's#/*$##; s#^.*cloud/work/frontier/w38b/##; s#/#__#g')
  ssh watchman2 "rm -rf ~/$R/cand/$LABEL && mkdir -p ~/$R/cand/$LABEL" && scp -q "$D"/* watchman2:$R/cand/$LABEL/
done
for D in "$@"; do
  LABEL=$(echo "$D" | sed 's#/*$##; s#^.*cloud/work/frontier/w38b/##; s#/#__#g')
  echo "$LABEL"
done | ssh watchman2 "cd ~/$R && export IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido; while read L; do
  python3 tools/cloud/score.py group cand/\$L --targets asm/us/$IMG > /tmp/w38b_gb.txt 2>&1
  python3 - \$L <<'PY'
import re,sys
t=open('/tmp/w38b_gb.txt').read()
mem=t.split('Context')[0]
ctx=t.split('Context')[1] if 'Context' in t else ''
def summ(s):
    out=[]
    for blk in re.split(r'^(?=func_\w+:$|\w+:$)', s, flags=re.M):
        lines=blk.split('\n')
        if not lines[0].endswith(':') or lines[0].startswith(' ') or lines[0] in ('Members:','Context (informational; excluded from exit status):'): continue
        st=next((l.strip() for l in lines[1:] if l.startswith('  ') and not l.startswith('    ')), '?')
        st=st.split(' (')[0]
        out.append(lines[0][:-1][-5:]+'='+('ok' if st=='MATCH' else st.replace(' words differ','')))
    return out
m=summ(mem); c=summ(ctx)
bad=[x for x in m if not x.endswith('=ok')]
err='' if m else ' ERR:'+t.strip().splitlines()[-1][:200] if t.strip() else ' ERR'
print(sys.argv[1].split('__')[-1], 'members', len(m), 'bad', bad, '| ctx', ' '.join(c), err)
PY
done"
