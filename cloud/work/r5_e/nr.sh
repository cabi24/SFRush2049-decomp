#!/bin/bash
# nr.sh file fn [flags] -> aligned exact count + words
cd /home/user/SFRush2049-decomp
python3 cloud/work/bigfish/near.py "$1" "$2" ${3:+--flags "$3"} 2>&1 | tail -1 | sed -E "s/.*got ([0-9]+) words.*strict-equal ([0-9]+); aligned exact ([0-9]+).*/got=\1 strict=\2 exact=\3/"
