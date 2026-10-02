/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
#include "rom_tu.h"
typedef struct C23Huft C23Huft;
struct C23Huft {u8 e,b;union {u16 n;C23Huft *t;} v;};
extern u16 gInflateCplens[],gInflateCplext[],gInflateCpdist[],gInflateCpdext[];
extern s32 huft_alloc(s32);
extern s32 huft_build(u32*,u32,u32,u16*,u16*,C23Huft**,s32*);
extern s32 inflate_free_window(C23Huft*,C23Huft*,s32,s32);
extern u32 gInflateBitBuf,gInflateBitCount;
extern u8 *gInflateInPtr,*gInflateInEnd;
extern s32 inflate_read_bits(void),inflate_dynamic(void),inflate_stored(void),inflate_fixed(void);
s32 inflate_fixed(void) {
 s32 i;
 C23Huft *tl,*td;
 s32 bl,bd;
 u32 *l=(u32*)huft_alloc(288*sizeof(u32));
 for(i=0;i<144;i++) l[i]=8;
 for(;i<256;i++) l[i]=9;
 for(;i<280;i++) l[i]=7;
 for(;i<288;i++) l[i]=8;
 bl=7;
 if((i=huft_build(l,288,257,gInflateCplens,gInflateCplext,&tl,&bl))!=0) {gDisplayListSize=0;return i;}
 for(i=0;i<30;i++) l[i]=5;
 bd=5;
 if((i=huft_build(l,30,0,gInflateCpdist,gInflateCpdext,&td,&bd))>1) {gDisplayListSize=0;return i;}
 if((i=inflate_free_window(tl,td,bl,bd))!=0) return i<0?i:1;
 gDisplayListSize=0;
 return 0;
}
