cd ~/rush2049/scratch/frontier/w15g
O2="-g0 -O2 -mips2 -G 0 -non_shared"; O1="-g0 -O1 -mips2 -G 0 -non_shared"
for t in func_80014650 func_8002517C; do for s in piacs.c siacs.c; do echo "$t <- $s: $(python3 sc.py ul/src/io/$s $t --ver L --flags "$O2"|tail -1|cut -c1-60) / O1 $(python3 sc.py ul/src/io/$s $t --ver L --flags "$O1"|tail -1|cut -c1-40)"; done; done
python3 cslice.py ul/src/io/controller.c __osContGetInitData ul/src/io/cgi.c; echo "func_800103A0 <- __osContGetInitData: $(python3 sc.py ul/src/io/cgi.c func_800103A0 --ver L --flags "$O2"|tail -1|cut -c1-60)"
python3 cslice.py ul/src/os/destroythread.c osDestroyThread ul/src/os/dt.c; echo "__osEnqueueAndYield <- osDestroyThread: $(python3 sc.py ul/src/os/dt.c __osEnqueueAndYield --ver L --flags "$O1"|tail -1|cut -c1-60)"
