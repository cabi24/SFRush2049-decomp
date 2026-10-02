/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned int u32;
typedef unsigned short u16;
typedef union Gfx {struct {u32 w0,w1;} words; unsigned long long force_alignment;} Gfx;
typedef struct FrameBuffer {void *buffer; unsigned char opaque[124];} FrameBuffer;
extern Gfx *msgq_ptr;
extern int D_8002AFC0,D_8002AFC4;
extern signed char D_8015F72D;
extern FrameBuffer D_80156C5C[];
extern u32 osVirtualToPhysical(void *);
#define _SHIFTL(v,s,w) (((unsigned int)(v)&((0x01U<<(w))-1))<<(s))
#define gDPNoParam(pkt,cmd) { Gfx *_g=(Gfx *)(pkt); _g->words.w0=_SHIFTL(cmd,24,8); _g->words.w1=0; }
#define gSetImage(pkt,cmd,fmt,siz,width,i) { Gfx *_g=(Gfx *)(pkt); _g->words.w0=_SHIFTL(cmd,24,8)|_SHIFTL(fmt,21,3)|_SHIFTL(siz,19,2)|_SHIFTL((width)-1,0,12); _g->words.w1=(unsigned int)(i); }
#define gDPSetColor(pkt,c,d) { Gfx *_g=(Gfx *)(pkt); _g->words.w0=_SHIFTL(c,24,8); _g->words.w1=(unsigned int)(d); }
#define gDPFillRectangle(pkt,ulx,uly,lrx,lry) { Gfx *_g=(Gfx *)(pkt); _g->words.w0=_SHIFTL(0xF6,24,8)|_SHIFTL(lrx,14,10)|_SHIFTL(lry,2,10); _g->words.w1=_SHIFTL(ulx,14,10)|_SHIFTL(uly,2,10); }
void func_800F7448(u16 color) {
    gDPNoParam(msgq_ptr++,0xE7);
    gSetImage(msgq_ptr++,0xFF,0,2,D_8002AFC0,osVirtualToPhysical(D_80156C5C[D_8015F72D].buffer));
    gDPSetColor(msgq_ptr++,0xF7,(color<<16)|color);
    gDPFillRectangle(msgq_ptr++,0,0,D_8002AFC0-1,D_8002AFC4-1);
    gDPNoParam(msgq_ptr++,0xE7);
}
