#!/bin/bash
# slots.sh VARIANTFILE FN TEMPLATE : compose group from template, compile on builder, list sp offsets >= 80 used by FN (want vs got)
V=$1; FN=$2; T=$3
D=/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/slots_g
rm -rf $D; mkdir -p $D; cp $T/group.json $D/; python3 -c "
import sys; t=open('$T/group.c').read(); v=open('$V').read(); open('$D/group.c','w').write(t.replace('/*@BODY@*/', v))"
$(dirname $0)/g.sh $D $FN --all > $D.txt
python3 - $D.txt <<'PY'
import re, sys, collections
w = collections.Counter(); g = collections.Counter()
for line in open(sys.argv[1]):
    m = re.match(r'(.)\s+(\+\w+)?\s+(.{34}) (.*)', line.rstrip('\n'))
    if not m: continue
    for side, cnt in ((m.group(3), w), (m.group(4), g)):
        for x in re.findall(r'(\w+) [^,]*,(-?\d+)\(sp\)', side): cnt[(x[0], int(x[1]))] += 1
        for x in re.findall(r'addiu \w+,sp,(\d+)', side): cnt[('addiu', int(x))] += 1
print('want', ' '.join('%s@%d' % (k[0], k[1]) for k in sorted(w, key=lambda k: k[1]) if k[1] >= 80))
print('got ', ' '.join('%s@%d' % (k[0], k[1]) for k in sorted(g, key=lambda k: k[1]) if k[1] >= 80))
PY
tail -1 $D.txt
