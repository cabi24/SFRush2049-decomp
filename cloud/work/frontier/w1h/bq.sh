#!/bin/sh
# usage: bq.sh file.c NAME [flags] -- aligned+strict counts for one file
d=$(mktemp -d); cp "$1" $d/x.c; n="$2"; shift; shift; ./batch.sh $d $n "$@"; rm -rf $d
