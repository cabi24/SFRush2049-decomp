#!/bin/sh
# w24a (builder, scratch root): LOCK score every lane/$1/*.c for function $2, sorted
TK=$HOME/rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
for f in lane/$1/*.c; do printf '%s ' $f; TK=$TK python3 lane/tools/check.py $f $2 $2 2>/dev/null | grep '^LOCK:' ; done | sort -k3 -n
