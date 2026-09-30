#!/bin/bash
# gd.sh groupdir fn  -> side-by-side diff (want | got), register-exact
cd /home/user/SFRush2049-decomp
python3 cloud/work/tools/extscore.py $1 --dis $2 2>&1 | grep -E '^\s+[0-9a-f]+:' | sed -E 's/^\s+[0-9a-f]+:\s+[0-9a-f]{8}\s+//; s/\s+/ /g; s/\$//g' > /tmp/r5_got_$2.txt
python3 cloud/work/tools/tdis.py $2 2>&1 | grep -E '^\s+[0-9a-f]{8}:' | sed -E 's/^\s+[0-9a-f]+:\s+//; s/\s+/ /g; s/0x[0-9a-f]{3,}//g' > /tmp/r5_want_$2.txt
wc -l /tmp/r5_got_$2.txt /tmp/r5_want_$2.txt
