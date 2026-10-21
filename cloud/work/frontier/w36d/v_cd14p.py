R='parts/func_8038CCF4.c'
src=open('groups/trackscr/parts/func_8038CCF4.c').read()
s2=src.replace('+= 0.1f','+= up').replace('*= 0.01f','*= scale').replace('* 16.0f','* fix').replace("    saveTrackNum = D_8014978C;\n","    up = 0.1f; scale = 0.01f; fix = 16.0f;\n    saveTrackNum = D_8014978C;\n",1).replace("    f32 sectVerts[8][3];\n","    f32 sectVerts[8][3];\n    f32 up, scale, fix;\n",1)
VARIANTS = {
 'base': [],
 'named': [(R, src, s2)],
}
