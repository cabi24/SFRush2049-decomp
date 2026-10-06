#!/bin/sh
# vrun.sh NAME... : build runs/NAME/group.c from runs/NAME.c (as the Input_ProcessGameplayPad part) and print diff count
cd $(dirname $0)/..
for v in "$@"; do
  IPGP=runs/$v.c sh dev/mk.sh runs/$v >/dev/null && KEEP=camera_update_c,select_screen_update,Input_ProcessGameplayPad tools/gd.sh runs/$v/group.c Input_ProcessGameplayPad > runs/$v/diff.txt 2>&1
  echo "$v $(grep -c '^|' runs/$v/diff.txt) $(tail -1 runs/$v/diff.txt | cut -c1-40)"
done
