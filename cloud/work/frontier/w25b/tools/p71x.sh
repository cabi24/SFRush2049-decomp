#!/bin/sh
# p71x.sh SRC FN REGEX [VECS]  -- resolve REGEX against the cm bit table (first run), then rerun with
# P71_CLR=<vec>:<bit> for every matching bit and every vec in VECS (default "insert,delete"); prints both row lines.
d=$(dirname $0); src=$1; fn=$2; re=$3; vecs=${4:-insert,delete}
F=${LOUT:-/home/cburnes/projects/rush2049-decomp/build/trace71/w25b}/$fn.p71.txt
echo "plain:  $($d/p71.sh $src $fn | tail -1 | cut -c1-60)"
bits=$(python3 -c "
import re,sys
bs=[]
for l in open('$F'):
    m=re.match(r'\[P71\] bit at=cm bit=(\d+) .*?text=(\S*)', l)
    if m and re.search(sys.argv[1], m.group(2)): bs.append(m.group(1))
print(' '.join(bs))" "$re")
spec=$(for b in $bits; do for v in $(echo $vecs | tr , ' '); do printf '%s:%s,' $v $b; done; done)
echo "bits:   $bits"
echo "oracle: $($d/p71.sh $src $fn P71_CLR=${spec%,} | tail -1 | cut -c1-60)"
