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
#define NEEDBITS(n) do {while(k<(n)) {u32 word; if(gInflateInPtr<gInflateInEnd) {gInflateInPtr+=2;word=(gInflateInPtr[-1]<<8)|gInflateInPtr[-2];} else word=inflate_read_bits();b|=word<<k;k+=16;}} while(0)
#define DUMPBITS(n) do {b>>=(n);k-=(n);} while(0)
s32 inflate_block(s32 *last) {
 u32 t;
 register u32 b=gInflateBitBuf,k=gInflateBitCount;
 NEEDBITS(1);*last=b&1;DUMPBITS(1);
 NEEDBITS(2);t=b&3;DUMPBITS(2);
 gInflateBitBuf=b;gInflateBitCount=k;
 if(t==2)return inflate_dynamic();
 if(t==0)return inflate_stored();
 if(t==1)return inflate_fixed();
 return 1;
}
