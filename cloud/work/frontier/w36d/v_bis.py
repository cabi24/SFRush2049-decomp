R='parts/func_8038CCF4.c'
src=open('groups/trackscr/parts/func_8038CCF4.c').read()
L=src.split('\n')
def dele(a,b):  # 1-based inclusive
    return '\n'.join(L[:a-1]+L[b:])
VARIANTS = {
 'd_gfx': [(R, src, dele(128,137))],
 'd_vtx': [(R, src, dele(115,127))],
 'd_sqrt': [(R, src, '\n'.join(L[:73]+['                len0 = len1 = len2 = 1.0f;']+L[76:]))],
 'd_post': [(R, src, dele(141,153))],
 'd_scale': [(R, src, dele(107,114))],
}
