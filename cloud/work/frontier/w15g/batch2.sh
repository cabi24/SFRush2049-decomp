cd ~/rush2049/scratch/frontier/w15g
for t in __muldi3 __lshrdi3 __ashldi3 __ashrdi3 __divdi3 __moddi3 __udivdi3 __umoddi3 __fixdfdi __fixsfdi __floatdidf __floatdisf __fixunsdfdi __fixunssfdi __floatundidf __floatundisf; do
 for fl in "-O2 -mips3 -32" "-O1 -mips3 -32" "-O3 -mips3 -32"; do
  sed "1i #define T_$t 1" cand/libgcc.c > /tmp/w15g_lg.c
  echo "$t [$fl]: $(python3 sc.py /tmp/w15g_lg.c $t --flags "-g0 $fl -G 0 -non_shared" 2>&1 | tail -1 | cut -c1-60)"
 done; done
