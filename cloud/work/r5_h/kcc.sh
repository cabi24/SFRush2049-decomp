#!/bin/sh
# kcc.sh file.c [flags]  -> prints a.s listing (pre-as1) for file
R=/home/user/SFRush2049-decomp
f=$(readlink -f $1); shift
d=$(mktemp -d); cd $d; cp $f x.c
export USR_LIB=$R/tools/cloud/ido COMPILER_PATH=$R/tools/cloud/ido
$R/tools/cloud/ido/cc -c -K -g0 ${@:--O2} -mips2 -G 0 -non_shared -Wab,-r4300_mul x.c 2>&1 | head
ls; cat x.s 2>/dev/null || cat a.s
