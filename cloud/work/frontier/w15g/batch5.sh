cd ~/rush2049/scratch/frontier/w15g
for t in __osContDataCrc __osPfsDataChecksum; do for v in L H; do for o in -O2 -O1; do
 echo "$t $v $o: $(python3 sc.py ul/src/io/crc.c $t --ver $v --flags "-g0 $o -mips2 -G 0 -non_shared" 2>&1|tail -1|cut -c1-70)"; done;done;done
for v in L H G F D; do for o in -O2 -O1; do echo "motor $v $o: $(python3 sc.py ul/src/io/motor.c osMotorStop --ver $v --flags "-g0 $o -mips2 -G 0 -non_shared" 2>&1|tail -1|cut -c1-110)"; done; done
