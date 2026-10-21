#!/bin/sh
# usage: cols.sh LABEL NAME  -- compact web -> register list from ctrace (TAG=w36x3)
export TAG=w36x3
cloud/work/frontier/tools/trace/ctrace.sh "$2" "$1" 2>&1 | awk '/^p[12] /{r=""; for(i=1;i<=NF;i++) if($i=="->") r=$(i+1); printf "%s:%s=%s ", $1, $2, r} END{print ""}'
