cd ~/rush2049/scratch/frontier/w15g
for fn in __scAppendList __scExec __scMain __scHandleRetrace; do
 python3 cslice.py ul/src/sched/sched.c $fn ul/src/sched/s_$fn.c; sed -i "s/^static //;s/^\(\s*\)static \([a-zA-Z]\)/\1\2/" ul/src/sched/s_$fn.c
 sed -i 's/\bassert *(/ASSERT_(/g' ul/src/sched/s_$fn.c; sed -i '1i #define ASSERT_(x) if (x) {}' ul/src/sched/s_$fn.c
 echo "$fn: $(python3 sc.py ul/src/sched/s_$fn.c $fn --ver L --flags "-g1 -O1 -Wab,-r4300_mul -mips2 -G 0 -non_shared" 2>&1 | tail -1 | cut -c1-60)"
done
