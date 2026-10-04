#!/bin/sh
# dis.sh OBJ FUNC [full] : raw target vs relocated candidate (from verify.py dump)
cd /home/cburnes/projects/rush2049-decomp/cloud/work/s20261004/E
T=$(mktemp); C=$(mktemp)
python3 tdis.py $2 > $T; python3 tdis.py $2 obj/$1.$2.bin > $C 2>/dev/null
echo "diffs: $(diff $T $C | grep -c '^[<>]')"
[ -n "$3" ] && diff -y -W 90 $T $C
rm -f $T $C
