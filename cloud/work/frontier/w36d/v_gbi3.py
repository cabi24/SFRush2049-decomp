R='parts/func_8038CCF4.c'
src=open('/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/gbi_all.c').read()
base=open('groups/trackscr/parts/func_8038CCF4.c').read()
def mk(fill, loopdecl):
    s=src.replace("    s32 u3, u4, u5, u6, u7;\n    f32 sectVerts[8][3];\n", fill)
    s=s.replace("            for (pointIdx = 0; pointIdx < D_803B7CD8; pointIdx++) {\n", "            for (pointIdx = 0; pointIdx < D_803B7CD8; pointIdx++) {\n" + loopdecl, 1)
    return [(R, base, s)]
VARIANTS = {
 'sv_loop_0': mk("", "                f32 sectVerts[8][3];\n"),
 'sv_loop_1': mk("    s32 u3;\n", "                f32 sectVerts[8][3];\n"),
 'sv_loop_5': mk("    s32 u3, u4, u5, u6, u7;\n", "                f32 sectVerts[8][3];\n"),
}
