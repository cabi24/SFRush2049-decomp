exec(open('/tmp/static_C23_sources.py').read().split("p.joinpath('inflate_fixed.c')")[0])
macro=Path('cloud/work/static_C23/inflate_block.c').read_text().split('#define NEEDBITS')[1].split('s32 inflate_block')[0]
macro='#define NEEDBITS'+macro
src=Path('reference/repos/perfect_dark/src/inflate/inflate.c').read_text()
dyn=src[src.index('s32 inflate_dynamic(void)'):src.index('s32 inflate_block(s32 *e)')]
dyn=dyn.replace('struct huft','C23Huft').replace('u32 ll[286+30];','u32 *ll=(u32*)huft_alloc((286+30)*sizeof(u32));')
dyn=dyn.replace('b = bb;\n\tk = bk;', 'k = gInflateBitCount;\n\tb = gInflateBitBuf;')
dyn=dyn.replace('DUMPBITS(4)', 'DUMPBITS(4)\n\tif (nl>286 || nd>30) return 1;')
dyn=dyn.replace('huft_build(ll, 19, 19, NULL, NULL, &tl, &bl);','if ((i=huft_build(ll, 19, 19, NULL, NULL, &tl, &bl))!=0) {gDisplayListSize=0;return i;}')
dyn=dyn.replace('\t\t\twhile (j--) {','\t\t\tif (i+j>n) return 1;\n\t\t\twhile (j--) {')
dyn=dyn.replace('bb = b;\n\tbk = k;', 'gDisplayListSize=0x4F0;\n\tgInflateBitBuf=b;\n\tgInflateBitCount=k;')
dyn=dyn.replace('huft_build(ll, nl, 257, cplens, cplext, &tl, &bl);','if ((i=huft_build(ll, nl, 257, gInflateCplens, gInflateCplext, &tl, &bl))!=0) {gDisplayListSize=0;return i;}')
dyn=dyn.replace('huft_build(ll + nl, nd, 0, cpdist, cpdext, &td, &bd);','if ((i=huft_build(ll + nl, nd, 0, gInflateCpdist, gInflateCpdext, &td, &bd))!=0) {gDisplayListSize=0;return i;}')
dyn=dyn.replace('inflate_codes(tl, td, bl, bd);','if ((i=inflate_free_window(tl, td, bl, bd))!=0) return i<0?i:1;\n\tgDisplayListSize=0;')
for a,b in [('border','gInflateBorder'),('mask_bits','gInflateMaskBits'),('lbits','gInflateHuftPointerLit'),('dbits','gInflateHuftPointerDist')]: dyn=dyn.replace(a,b)
# Macro callers originally omit semicolons; add to definitions.
macro=macro.replace('while(0)\n','while(0);\n')
p.joinpath('inflate_dynamic.c').write_text(context+'extern u32 gInflateBorder[];\nextern u16 gInflateMaskBits[];\nextern s32 gInflateHuftPointerLit,gInflateHuftPointerDist;\n'+macro+dyn)
huft=src[src.index('s32 huft_build('):src.index('s32 inflate_codes(')]
huft=huft.replace('struct huft','C23Huft').replace('u8 *e','u16 *e').replace('u32 v[N_MAX];','u32 *v;').replace('u32 i2;','')
start=huft.index('\tfor (i2 = 0;');end=huft.index('\n\tp = b;',start)
huft=huft[:start]+'\tmemset(c,0,sizeof(c));\n'+huft[end:]
huft=huft.replace('y -= c[j];','if ((y -= c[j]) < 0) return 2;').replace('y -= c[i];','if ((y -= c[i]) < 0) return 2;')
huft=huft.replace('/* Make a table of values in order of bit lengths */','/* Make a table of values in order of bit lengths */\n\tv=(u32*)huft_alloc(288*sizeof(u32));')
huft=huft.replace('q = &huftlist[hufts];\n\n\t\t\t\thufts += z + 1;','q = (C23Huft*)huft_alloc((z+1)*sizeof(C23Huft));\n\t\t\t\tif(q==NULL) return 3;')
p.joinpath('huft_build.c').write_text(context+'#define BMAX 16\nextern void *memset(void*,s32,u32);\n'+huft)
