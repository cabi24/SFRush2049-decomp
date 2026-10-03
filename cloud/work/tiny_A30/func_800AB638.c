/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
typedef struct Header {u16 count[6];u8 pad12[4];} Header;
extern Header *D_801526DC,*D_801526F0;extern u8 *D_80152034,*D_80124EEC,*D_801497F8,*D_8015201C,*D_801525EC,*D_80152568,*D_80152460;extern u16 D_8015267C;
void func_800AB638(void) {
 Header *header=D_801526DC;u8 *p;
 D_801526F0=header;
 p=(u8 *)header+16;D_80152034=p;
 p+=header->count[0]*132;D_80124EEC=p;
 p+=header->count[1]*20;D_801497F8=p;
 p+=header->count[2]*24;D_8015201C=p;
 p+=header->count[3]*8;D_801525EC=p;
 D_8015267C=header->count[4];
 p=D_801525EC+header->count[4]*32;D_80152568=p;
 D_80152460=p+header->count[5];
}
