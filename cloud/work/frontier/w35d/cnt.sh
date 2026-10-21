#!/bin/bash
# usage: cnt.sh groups/DIR  -> one line "name:diffcount ..." per member/context
cd /home/cburnes/projects/rush2049-decomp || exit 1
L=cloud/work/frontier/w35d
rsync -aq --exclude 'v/' $L/ watchman2:rush2049/scratch/frontier/w35d/lane/ || exit 1
ssh watchman2 "cd ~/rush2049/scratch/frontier/w35d && IDO_DIR=\$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido python3 tools/cloud/score.py group lane/$1 --targets asm/us/ovl_a" 2>&1 | python3 -c "
import sys,re
out=[];name=None
for l in sys.stdin:
    m=re.match(r'^(func_\w+):',l)
    if m: name=m.group(1); continue
    m=re.match(r'\s+(\d+)/(\d+) words differ',l)
    if m: out.append(f'{name}:{m.group(1)}/{m.group(2)}'); continue
    if re.match(r'\s+MATCH',l): out.append(f'{name}:MATCH'); continue
    m=re.match(r'\s+MISMATCH \((.*?)[;)]',l)
    if m: out.append(f'{name}:MISMATCH({m.group(1)[:40]})')
    if 'failed' in l or 'Error' in l: out.append(l.strip()[:200])
print(' '.join(out))
"
