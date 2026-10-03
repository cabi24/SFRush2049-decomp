#include <string.h>
#include "func_800F1D04.c"
u8 D_80151968[20][13];
u16 D_80151A78[20];
s32 D_80151690[12][3][5];
s32 func_8008AD04(u8 *a,u8 *b) { while (*a && *a==*b) {a++;b++;} return *a-*b; }
u8 *func_800A473C(u8 *a,u8 *b) {u8 *r=a;do {*a++=*b;}while(*b++);return r;}
int run_case(const unsigned char *names,const unsigned short *ages,const int *refs,const unsigned char *name,unsigned char *outnames,unsigned short *outages,int *outrefs) {
 int result;
 memcpy(D_80151968,names,260);memcpy(D_80151A78,ages,40);memcpy(D_80151690,refs,720);
 result=func_800F1D04((u8 *)name);
 memcpy(outnames,D_80151968,260);memcpy(outages,D_80151A78,40);memcpy(outrefs,D_80151690,720);return result;
}
