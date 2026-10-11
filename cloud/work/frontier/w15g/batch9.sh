cd ~/rush2049/scratch/frontier/w15g
for fn in __scMain __scHandleRetrace __scAppendList __scExec; do
 python3 cslice.py ul/src/sched/sched.c $fn ul/src/sched/s_$fn.c; sed -i "s/^static //;s/^\(\s*\)static \([a-zA-Z]\)/\1\2/" ul/src/sched/s_$fn.c
 for v in L H; do for fl in "-g1 -O1 -Wab,-r4300_mul" "-g0 -O1" "-g0 -O2" "-g1 -O2 -Wab,-r4300_mul"; do
  echo "$fn $v [$fl]: $(python3 sc.py ul/src/sched/s_$fn.c $fn --ver $v --flags "$fl -mips2 -G 0 -non_shared" 2>&1 | tail -1 | cut -c1-60)"
 done; done; done
