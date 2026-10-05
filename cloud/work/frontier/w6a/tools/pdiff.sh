#!/bin/sh
# usage: pdiff.sh LABEL1 LABEL2 -- procs whose colouring decisions differ between two ctrace snapshots
ssh watchman2 "cd ~/rush2049/scratch/frontier/w6a && grep -v procindex st_$1/all.txt > /tmp/w6a_a; grep -v procindex st_$2/all.txt > /tmp/w6a_b; diff /tmp/w6a_a /tmp/w6a_b | grep -o 'proc=[0-9]*' | sort | uniq -c; rm -f /tmp/w6a_a /tmp/w6a_b"
