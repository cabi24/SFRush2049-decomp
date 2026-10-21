#!/bin/bash
# gscore.sh DIR : AD57C strict line + nonequal-run words
cd /home/cburnes/projects/rush2049-decomp/cloud/work/frontier/w36f/func_803AD57C
../tools/gsc.sh a $1 func_803AD57C | grep -E "words differ|MATCH" | sed 's/(.*//'
../tools/gd.sh group a $1 func_803AD57C 2>&1 | tail -1
