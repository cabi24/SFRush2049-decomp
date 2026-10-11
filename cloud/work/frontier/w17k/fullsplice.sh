#!/bin/sh
# w17k (builder, scratch root): full matching ROM build with promote_batch's spliced TUs
# (dryrun71.sh -> cloud/work/frontier/w17k/spl/*.c) in place of src/rom, built under the
# 7.1 switch; objects must be newer than their TUs; src/rom is restored afterwards.
set -u
L=cloud/work/frontier/w17k
mkdir -p $L/orig && cp src/rom/*.c $L/orig/
cp $L/spl/*.c src/rom/ && touch $(ls $L/spl/*.c | sed "s|$L/spl|src/rom|")
make VERSION=us COMPILER=ido -j2 > $L/fullsplice.log 2>&1; echo "make exit $?" >> $L/fullsplice.log
make test >> $L/fullsplice.log 2>&1; echo "test exit $?" >> $L/fullsplice.log
for t in $(ls $L/spl/*.c | sed "s|$L/spl/||; s|\.c$||"); do
  [ build/us/src/rom/$t.o -nt src/rom/$t.c ] && echo "$t.o newer than TU" || echo "$t.o STALE"
done >> $L/fullsplice.log
grep -c "PROMOTED (w17k\|^/\* PROMOTED" src/rom/*.c | awk -F: '{s+=$2} END {print "promoted headers in src/rom:", s}' >> $L/fullsplice.log
cp $L/orig/*.c src/rom/ && touch src/rom/*.c
tail -20 $L/fullsplice.log
