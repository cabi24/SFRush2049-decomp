/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * N64 NewMultiBlit (historical symbol sound_control), 0x800B37E8..0x800B39BC.
 * Donor: historicalsource/rushtherock 845329d7b36f5a384c5625ed9a0aef584ab46139,
 * LIB/blit.c NewMultiBlit and LIB/blit.h. Native fields and 36-byte descriptor
 * layout differ from arcade. In particular the N64 sentinel texture name -1
 * uses Info as an opaque callback/address carrier and Image as the Blit itself.
 *
 * The fourth NewBlit call argument is the native descriptor high bit. Its
 * callee does not read it, so the declaration remains old-style rather than
 * inventing a fourth formal in that callee. The direct descriptor indexing
 * and call through the just-assigned output AnimFunc follow the authentic
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
    const char *Name;
    void *Image;
    void *Info;
    u16 TexIndex;
    s16 X, Y;
    u16 Z;
    s16 Width, Height;
    u8 Alpha, Flip;
    s8 Hide, Init;
    s16 Top, Bot, Left, Right, color;
    u16 reserved;
    s32 (*AnimFunc)(Blit *);
    u32 AnimID;
    s32 AnimDTA;
    u16 BLIdx;
    u16 reserved36;
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
  curblit->Alpha=mblit[i].alpha;
  curblit->Z=mblit[i].zdepth;
  curblit->Left=mblit[i].left;
  curblit->Top=mblit[i].top;
  curblit->Right=mblit[i].right;
  curblit->Bot=mblit[i].bot;
  if(mblit[i].width>=0) curblit->Width=mblit[i].width;
  if(mblit[i].height>=0) curblit->Height=mblit[i].height;
  Input_ApplyPadConfig(curblit);
  if(mblit[i].animfunc) {
   curblit->AnimID=mblit[i].animid & 0x7fffffffU;
   if(mblit[i].texname==(char *)-1) {
    curblit->Info=(void *)mblit[i].animfunc;
    curblit->Image=curblit;
    Input_ApplyPadConfig(curblit);
   } else {
    curblit->AnimFunc=mblit[i].animfunc;
    if(!curblit->AnimFunc(curblit)) {
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
