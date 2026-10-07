#!/usr/bin/env python3
"""uopt_alias_patch.py IN_uopt.c OUT_uopt_al.c -- w12i: trace uopt's .noalias/.alias (Uunal) bookkeeping.
Build on the builder (own scratch only):
  cp ~/rush2049/scratch/ci/tools/ido-static-recomp/build/uopt.c . ; python3 uopt_alias_patch.py uopt.c uopt_al.c
  gcc -std=c11 -Os -fno-strict-aliasing -I. -w -o uopt_al uopt_al.c libc_impl.o -lm   (headers + libc_impl.o from wtk/src)
Run on a snapshot: cp st st.al; ALTRACE=1 [ALU=1] ./uopt_al -G 0 -Olimit 5000 -mips2 -EB -g0 -O3 merged opt.al -t st.al optlog 2>al.log
Output (stderr), per procedure (PROC = f_reemit entry; E05F0 was the 134th PROC in the w12i unit):
  BB n m7=..   bb start in reemission order, colour-7 (t0) live range in this bb's register map
  CHK reg=r rec=LR map=LR'   func_4247a4 at bb start: rec != map -> clear, and emit Uunal d0 (.alias rX,$sp) if the pair flag was 2
  BIR site=k reg=r a1=LR a2=node kind= spna=   f_base_in_reg: records rec[r]=LR; emits .noalias rX,$sp the first time
                       base_sp_noalias(node) is true (kind-1 parent = evaluation of a coloured load temp), else marks 1
  U op=..              (ALU=1) every ucode record written (opcode numbers: workbench ucode OPCODE_NAMES)
"""
import sys
s=open(sys.argv[1]).read()
s="#include <stdio.h>\n#include <stdlib.h>\nstatic int _bir_site;\n"+s
a=s.index("static void func_4247a4(uint8_t *mem, uint32_t sp, uint32_t v0) {")
l=s.index("L4247a4:\n",a)+len("L4247a4:\n")
s=s[:l]+'if(getenv("ALTRACE")){uint32_t _bb=MEM_U32(v0-12); fprintf(stderr,"BB %u m7=%x m2=%x m4=%x\\n",MEM_U16(_bb+8),MEM_U32(_bb+64+28),MEM_U32(_bb+64+8),MEM_U32(_bb+64+16));}\n'+s[l:]
b=s.index("L424818:",a)
c=s.index("t5 = MEM_U32(t4 + 64);",b)+len("t5 = MEM_U32(t4 + 64);")
s=s[:c]+'\nif(getenv("ALTRACE")) fprintf(stderr,"  CHK reg=%u rec=%x map=%x\\n",s5,v0,t5);\n'+s[c:]
d=s.index("static void f_base_in_reg(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1, uint32_t a2) {")
e=s.index("L421018:\n",d)+len("L421018:\n")
s=s[:e]+'if(getenv("ALTRACE")) fprintf(stderr,"BIR site=%d reg=%u a1=%x a1k=%u a2=%x kind=%u m50=%u sym=%u addr=%d sz=%u spna=%u S=%u\\n",_bir_site,a0,a1,MEM_U8(a1),a2,MEM_U8(a2),MEM_U8(a2+50),MEM_U16(a2+4),(int)MEM_U32(a2+40),MEM_U32(a2+44),f_base_sp_noalias(mem,sp-64,a2),MEM_U8(0x100220a0-1+a0));'+s[e:]
DUMP=r"""if(getenv("ALTRACE")&&getenv("ALDUMP")){int _k;fprintf(stderr,"  P:");for(_k=0;_k<64;_k+=4)fprintf(stderr," %08x",MEM_U32(a2+_k));fprintf(stderr,"\n  B:");for(_k=0;_k<64;_k+=4)fprintf(stderr," %08x",MEM_U32(a1+_k));fprintf(stderr,"\n");}
"""
d=s.index("static void f_base_in_reg(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1, uint32_t a2) {")
e=s.index("L421018:\n",d)+len("L421018:\n")
s=s[:e]+DUMP+s[e:]
out=[];i=0;n=0;key="f_base_in_reg(mem, sp, a0, a1, a2);"
while True:
    j=s.find(key,i)
    if j<0: out.append(s[i:]); break
    n+=1; out.append(s[i:j]); out.append("_bir_site=%d; "%n+key); i=j+len(key)
s="".join(out)
d=s.index("static void f_reemit(uint8_t *mem, uint32_t sp) {")
e=s.index("\nL",d)+1; e=s.index("\n",e)+1
s=s[:e]+'if(getenv("ALTRACE")) fprintf(stderr,"PROC\\n");\n'+s[e:]
d=s.index("static void f_uwrite(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3) {")
e=s.index("\nL",d)+1; e=s.index("\n",e)+1
s=s[:e]+'if(getenv("ALTRACE")&&getenv("ALU")) fprintf(stderr,"  U op=%u b1=%u w1=%x w2=%x\\n",MEM_U8(a0),MEM_U8(a0+1),MEM_U32(a0+4),MEM_U32(a0+8));\n'+s[e:]
open(sys.argv[2],'w').write(s)
