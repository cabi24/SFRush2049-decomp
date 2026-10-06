src=open('v2/a_lo.c').read()
L="                lo = curve->level;\n"
assert L in src
V={'eec':"                lo = D_80120EEC[i].level;\n",
   'cast':"                lo = (s32 *)curve;\n",
   'amp0':"                lo = &curve->level[0];\n",
   'eec0':"                lo = &D_80120EEC[i].level[0];\n",
   'plus':"                lo = curve->level + 0;\n",
   'u32':"                lo = (s32 *)(u32)curve->level;\n",
}
for k,v in V.items(): open('v6/%s.c'%k,'w').write(src.replace(L,v))
# hi from different bases
s=src.replace("                hi = lo[4];\n","                hi = curve->level[4];\n")
open('v6/hicv.c','w').write(s)
for k,v in V.items(): open('v6/hicv_%s.c'%k,'w').write(s.replace(L,v))
