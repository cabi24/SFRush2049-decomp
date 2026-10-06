#!/bin/sh
# usage: TAG=.. pdiff.sh LABEL1 LABEL2 -- trace every procedure in both snapshots; list the ordinals whose colouring
# decisions differ (with a count). Use it when you need to know which procedures a source change touched
# (e.g. callers through IPA). For one known procedure use ctrace.sh NAME (it finds the ordinal itself).
. "$(dirname "$0")/lib/env.sh"
rtk $1 all >/dev/null; rtk $2 all >/dev/null
rsh "cd ~/$SCR && grep -v procindex st_$1/all.txt > st_$1/all.p; grep -v procindex st_$2/all.txt > st_$2/all.p; diff st_$1/all.p st_$2/all.p | grep -o 'proc=[0-9]*' | sort | uniq -c | sort -k2 -V"
