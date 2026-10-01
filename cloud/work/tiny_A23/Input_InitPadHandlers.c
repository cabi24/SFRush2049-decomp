typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct {u32 word0,word4;u16 half8,halfA,halfC,padE,half10,half12;u8 tail[12];} Record;
extern Record D_80140BF0[];
void Input_InitPadHandlers(u32 index,u32 h8,u32 w4,u32 w0,u32 hA,u32 hC,u32 h10,u32 h12) {Record *r=&D_80140BF0[index];r->word4=w4;r->word0=w0;r->half8=h8;r->halfA=hA;r->halfC=hC;r->half10=h10;r->half12=h12;}
