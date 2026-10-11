cd ~/rush2049/scratch/frontier/w15g
for fn in __scAppendList __scExec __scMain __scHandleRetrace; do
 sed -i 's/^#define ASSERT_.*/#define ASSERT_(x) if (!(x)) {}/' ul/src/sched/s_$fn.c
 for fl in "-g1 -O0" "-g2 -O0" "-g -O0" "-g0 -O0" "-g3 -O0" "-g1 -O1" "-g -O1"; do
 echo "$fn [$fl]: $(python3 sc.py ul/src/sched/s_$fn.c $fn --ver L --flags "$fl -Wab,-r4300_mul -mips2 -G 0 -non_shared" 2>&1 | tail -1 | cut -c1-50)"
done; done
