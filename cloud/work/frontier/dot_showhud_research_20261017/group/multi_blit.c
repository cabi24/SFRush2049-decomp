/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * N64 NewMultiBlit (historical symbol sound_control), 0x800B37E8..0x800B39BC.
 * Donor: historicalsource/rushtherock 845329d7b36f5a384c5625ed9a0aef584ab46139,
 * LIB/blit.c NewMultiBlit and LIB/blit.h. Native fields and 36-byte descriptor
 * layout differ from arcade. In particular the N64 sentinel texture name -1
 * uses info as an opaque callback/address carrier and palette as the Blit itself.
 *
 * The fourth NewBlit call argument is the native descriptor high bit. Its
 * callee does not read it, so the declaration remains old-style rather than
 * inventing a fourth formal in that callee. The direct descriptor indexing
 * and call through the just-assigned output animFunc follow the authentic
 * donor and eliminate the prior cursor/local-callback reconstruction residual.
 * No unused locals, dummy reads, volatile qualifiers or compiler shaping.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Blit Blit;
struct Blit {
    const char *name;
    void *palette;
    void *info;
    u16 texIndex;
    s16 x, y;
    u16 state;
    s16 width, height;
    u8 alpha, flip;
    s8 hide, init;
    s16 top, bot, left, right, color;
    u16 pad26;
    s32 (*animFunc)(Blit *);
    u32 animID;
    s32 animDTA;
    u16 blIdx;
    u16 pad36;
    void *data;
    Blit *child;
};
typedef struct MultiBlit {
 const char *texname;
 s16 dulx,duly,width,height,top,bot,left,right;
 u32 zdepth,alpha;
 s32 (*animfunc)(Blit *);
 u32 animid;
} MultiBlit;
extern Blit *func_800B3704();
extern void Input_ApplyPadConfig(Blit *);
extern void sound_stop(Blit *);
Blit *sound_control(s16 ulx, s16 uly, const MultiBlit *mblit, s16 nblits)
{
 Blit *rootblit,*lastblit,*curblit;
 int i;
 if(nblits<=0) return (Blit *)0;
 rootblit=(Blit *)0;
 lastblit=(Blit *)0;
 for(i=0;i<nblits;i++) {
  curblit=func_800B3704(mblit[i].texname,ulx+mblit[i].dulx,uly+mblit[i].duly,mblit[i].animid & 0x80000000U);
  curblit->alpha=mblit[i].alpha;
  curblit->state=mblit[i].zdepth;
  curblit->left=mblit[i].left;
  curblit->top=mblit[i].top;
  curblit->right=mblit[i].right;
  curblit->bot=mblit[i].bot;
  if(mblit[i].width>=0) curblit->width=mblit[i].width;
  if(mblit[i].height>=0) curblit->height=mblit[i].height;
  Input_ApplyPadConfig(curblit);
  if(mblit[i].animfunc) {
   curblit->animID=mblit[i].animid & 0x7fffffffU;
   if(mblit[i].texname==(char *)-1) {
    curblit->info=(void *)mblit[i].animfunc;
    curblit->palette=curblit;
    Input_ApplyPadConfig(curblit);
   } else {
    curblit->animFunc=mblit[i].animfunc;
    if(!curblit->animFunc(curblit)) {
     sound_stop(curblit);
     return rootblit;
    }
   }
  }
  if(i==0) rootblit=curblit;
  else lastblit->child=curblit;
  lastblit=curblit;
 }
 return rootblit;
}
