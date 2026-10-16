#!/bin/sh
# print sp offsets used by retail (left) and ours (right) in the last w33d unit build
cd /home/cburnes/projects/rush2049-decomp
TAG=w33d python3 cloud/work/frontier/tools/trace/udiff.py ${1:-control_settings} --all > build/trace/w33d.offs
echo "retail: $(cut -c1-41 build/trace/w33d.offs | grep -o 'sp,[0-9]*\|[0-9]*(sp)' | grep -o '[0-9]*' | sort -n | uniq -c | awk '{printf "%s(x%s) ", $2, $1}')"
echo "ours:   $(cut -c42- build/trace/w33d.offs | grep -o 'sp,[0-9]*\|[0-9]*(sp)' | grep -o '[0-9]*' | sort -n | uniq -c | awk '{printf "%s(x%s) ", $2, $1}')"
