#!/bin/sh
# usage: trq.sh FILE.c LABEL -- rows + top colouring decisions (proc 6)
D=$(dirname $0)
$D/full.sh $1 | tail -1
$D/tr.sh $1 $2 6 | sed -n 2,14p
