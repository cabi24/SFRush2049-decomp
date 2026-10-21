#!/usr/bin/env python3
"""patch_formlivbb.py IN.c OUT.c : on top of patch_fsplit.py's uopt.dbg.c, print every f_formlivbb call (bit, block) and every
f_makelivranges start to the file named by $FLB_OUT (diagnostic only).  The order of formlivbb calls for one bit is the order of
that web's live-unit list, i.e. the flow-graph chain order that f_split's seed search walks (w36a, func_803A1EAC).
Build: gcc -std=c11 -Os -fno-strict-aliasing -I. -o ../bin/uopt OUT.c libc_impl.o -lm   (in a copy of wtk/src)
Use:   XENV="FLB_OUT=/abs/path/flb.txt" SCR=<that scratch> tools/trace/ctrace.sh NAME LABEL"""
import sys
s = open(sys.argv[1]).read()
i = s.index("L4641cc:\n//formlivbb:")
j = s.index("MEM_U32(sp + 92) = a1;\n", i) + len("MEM_U32(sp + 92) = a1;\n")
s = s[:j] + ('{ static FILE *flf; static int flinit; if (!flinit) { flinit=1; if (getenv("FLB_OUT")) flf=fopen(getenv("FLB_OUT"),"a"); } '
             'if (flf) { fprintf(flf, "formlivbb bit=%d bb=%d\\n", (int)MEM_U16(a0 + 2), (int)MEM_U16(a1 + 8)); fflush(flf); } }\n') + s[j:]
k = s.index("L468998:\ndkwb_cdx_lineage_begin();") + len("L468998:\ndkwb_cdx_lineage_begin();")
s = s[:k] + '\n{ FILE *flf2 = getenv("FLB_OUT") ? fopen(getenv("FLB_OUT"),"a") : 0; if (flf2) { fprintf(flf2, "makelivranges begin\\n"); fclose(flf2);} }\n' + s[k:]
open(sys.argv[2], 'w').write(s)
