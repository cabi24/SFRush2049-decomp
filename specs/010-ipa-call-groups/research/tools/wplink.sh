#!/bin/bash
# wplink.sh <idodir> <out.o> "<uld opts>" x.u... : run uld..as1 exactly as cc -c -O3 does, with custom uld options
set -e
I=$1; OUT=$2; ULDOPT=$3; shift 3
W=$(mktemp -d); ST=$W/st
# cc passes the shared symbol table (-XS) file produced by cfe; the .u files from ujoin embed it,
# so the -t symtab argument can be a fresh file.
$I/uld -L/usr/lib/mips2/nonshared -_SYSTYPE_SVR4 -mips2 -non_shared -g0 -no_AutoGnum $ULDOPT "$@" -ko $W/linked
$I/usplit -mips2 -o $W/split -t $ST $W/linked
$I/umerge -Olimit 5000 -mips2 -EB -g0 -O3 $W/split -o $W/merged -t $ST
$I/uopt -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 $W/merged $W/opt -t $ST $W/log
$I/ugen -G 0 -mips2 -EB -g0 -O3 $W/opt -o $W/gen -t $ST -temp $W/tmp
$I/as1 -elf -G 0 -p0 -mips2 -EB -g0 -O3 -Olimit 5000 $W/gen -o $OUT -t $ST
rm -rf $W
