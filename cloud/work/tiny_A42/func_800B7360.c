/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef int s32;typedef unsigned int u32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
extern u8 D_80149B48[4];extern u32 D_8012E6D0;
void func_800B7360(u8 a,u8 b,u8 c,u8 d) {
 u8 *cache=D_80149B48;
 if(cache[0]!=a || cache[1]!=b || cache[2]!=c || cache[3]!=d) {
 cache[0]=a;cache[1]=b;cache[2]=c;cache[3]=d;D_8012E6D0=0;
 }
}
