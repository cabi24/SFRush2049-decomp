/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed short s16;
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct Descriptor72 {u8 opaque[64];s16 x,y;s8 r,g;u8 b,a;} Descriptor72;
extern Descriptor72 D_8017A510[];
extern u32 D_80124FC8;
void func_800A7480(s16 x,s16 y,s8 r,s8 g,u8 b,u8 a,int index) {
 Descriptor72 *entry=&D_8017A510[index];
 u16 pixel;
 u8 blue=b;
 entry->x=x;
 entry->y=y;
 entry->r=r;
 entry->g=g;
 entry->b=blue;
 entry->a=a;
 pixel=((r<<8)&0xF800)|((g<<3)&0x7C0)|((blue>>2)&0x3E)|1;
 D_80124FC8=((u32)pixel<<16)|pixel;
}
