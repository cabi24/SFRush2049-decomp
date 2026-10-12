/* Actual-source logical-pointer fixtures; native layout is verified separately. */
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "candidate.c"
Player D_80152818[4]; Control D_80139320[4]; Vehicle D_8014A250[4];
Vec D_803943A4[269]; f32 D_8002EB90[1]; u16 D_80142972;
static unsigned seed; static int selected, count, mutate; static s32 result;
static void demand(int ok) { if (!ok) abort(); }
static unsigned random_word(void) { seed=seed*1664525u+1013904223u;return seed; }
void func_8038AA8C(void *context) {(void)context;abort();}
s32 func_800AF5E0(s32 id,s32 layer,s32 parent,s32 flags,s32 row,s32 col,s32 dynamic)
{
    Callback *c=&D_80152818[selected].callback;
    demand(count++ == 0 && id == D_80142972 && layer == 15);
    demand(parent == D_80139320[selected].parent && flags == 0x2000);
    demand(row == selected && col == -1 && dynamic == 1);
    demand(c->state == 0 && c->player == selected && c->callback == func_8038AA8C);
    demand(memcmp(&c->time,D_8002EB90,4) == 0);
    if (mutate) {
        D_8014A250[selected].model ^= 0xFF;
        c->state=123; c->handle=-222; D_80139320[selected].handle=0x12345678;
    }
    return result;
}
void func_8008D6FC(s16 handle,void *position,void *matrix)
{
    demand(count++ == 1 && handle == (s16)result && matrix == 0);
    demand(position == D_803943A4+D_8014A250[selected].model+13);
    demand(D_80139320[selected].handle == result);
    demand(D_80152818[selected].callback.handle == (s16)result);
}
int main(void)
{
    unsigned k,j; Player before_p[4];Control before_c[4];Vehicle before_v[4];
    for(k=0;k<8192;k++) {
        seed=k+1;selected=k%4;mutate=(k>>2)&1;count=0;
        memset(D_80152818,0xA5,sizeof D_80152818);
        memset(D_80139320,0x5A,sizeof D_80139320);
        memset(D_8014A250,0x73,sizeof D_8014A250);
        for(j=0;j<4;j++) {
            D_80152818[j].callback.callback=0;
            D_80152818[j].callback.handle=(s16)(random_word()>>16);
            D_80139320[j].parent=(s32)random_word();
            D_8014A250[j].model=(u8)(random_word()>>24);
        }
        D_80152818[selected].callback.handle=(k&8)?-1:(s16)(k&32767);
        D_8002EB90[0]=(f32)((int)(k%4097)-2048)/8.0f;
        D_80142972=(u16)random_word();result=(s16)(random_word()>>16);
        memcpy(before_p,D_80152818,sizeof before_p);memcpy(before_c,D_80139320,sizeof before_c);memcpy(before_v,D_8014A250,sizeof before_v);
        if(k&8) {
            Callback *c=&before_p[selected].callback;
            c->state=mutate?123:0;c->player=selected;c->callback=func_8038AA8C;c->time=D_8002EB90[0];c->handle=(s16)result;
            before_c[selected].handle=result;
            if(mutate)before_v[selected].model^=0xFF;
        }
        func_8038C910((s16)selected);
        demand(count==((k&8)?2:0));
        demand(memcmp(before_p,D_80152818,sizeof before_p)==0);
        demand(memcmp(before_c,D_80139320,sizeof before_c)==0);
        demand(memcmp(before_v,D_8014A250,sizeof before_v)==0);
    }
    puts("8192 actual-source logical-host fixtures passed");return 0;
}
