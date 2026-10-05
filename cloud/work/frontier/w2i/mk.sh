#!/bin/sh
# mk.sh body.c out.c : prepend flags line + gbi_min.h
d=$(dirname "$0")
{ echo '/* flags: -g0 -O3 -mips2 -G 0 -non_shared */'; cat $d/object_render/gbi_min.h "$1"; } > "$2"
