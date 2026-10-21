#!/bin/bash
# run.sh vX : build group from src/vX.c into gv/vX and print 923B0 section
V=$1
./mk.sh src/$V.c gv/$V >/dev/null && ../tools/gsc.sh a gv/$V func_803923B0 | grep -v "not verified"
