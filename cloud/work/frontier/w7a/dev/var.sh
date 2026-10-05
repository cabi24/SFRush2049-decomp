#!/bin/sh
# var.sh PART VARIANT.c FN [gd args] -- build the knot with PART replaced by VARIANT.c, print the aligned diff summary
part=$1; var=$2; fn=$3; shift 3
d=$(mktemp -d); cp part_*.c part_*.h header.txt build.sh standin.c $d/; cp $var $d/$part
sh $d/build.sh $d/k.c $d/standin.c
../tools/gd.sh $d/k.c $fn "$@"
rm -rf $d
