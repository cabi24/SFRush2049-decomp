#!/bin/sh
# usage: pdiff.sh LABEL1 LABEL2 -- procs whose colouring decisions differ between two ctrace snapshots
ssh watchman2 "cd ~/rush2049/scratch/frontier/w7b && grep -v procindex st_$1/all.txt > /tmp/w7b_a; grep -v procindex st_$2/all.txt > /tmp/w7b_b; diff /tmp/w7b_a /tmp/w7b_b | grep -o 'proc=[0-9]*' | sort | uniq -c; rm -f /tmp/w7b_a /tmp/w7b_b"
