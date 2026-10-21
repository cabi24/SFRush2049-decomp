R='parts/func_8038CCF4.c'
src=open('/tmp/claude-1000/-home-cburnes-projects-rush2049-decomp/268d487e-6278-4348-8a37-862b514a0019/scratchpad/gbi_all.c').read()
base=open('groups/trackscr/parts/func_8038CCF4.c').read()
VARIANTS = {
 'g_a': [(R, base, src.replace("    s32 u3, u4, u5, u6, u7;\n", "    s32 u3;\n"))],
 'g_b': [(R, base, src.replace("    s32 u3, u4, u5, u6, u7;\n", ""))],
 'g_c': [(R, base, src.replace("    s32 u3, u4, u5, u6, u7;\n", "    s32 u3, u4;\n"))],
 'g_d': [(R, base, src.replace("    s32 u3, u4, u5, u6, u7;\n", "").replace("    s32 u1, u2;\n", ""))],
}
