/* Bounded semantic harness. This is supporting evidence, not a ROM oracle. */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#include "group.c"

Language countdown_state;
Slot60 D_803B6610[10];
Camera D_80150B70[1];
f32 D_803B68D4[3], D_803B6898[3];
s32 D_803B6A48, D_803B6A68, D_803B6A6C;
s16 D_8014A108;
Color4 D_801146BC, D_801146C0;
char D_803B8CD8[] = "%d", D_803B8CDC[] = "%d";
s8 D_803B65E4[10];
s32 D_80156944;
s8 D_801164A1;
static unsigned writes;

void render_helper(f32 f) { (void)f; }
s32 object_create(s32 a) { return a; }
void dispatch_handler(s32 a) { (void)a; }
void func_800B669C(u32 a, u32 b) { (void)a; (void)b; }
void brake_light_update(s32 a, f32 *p, Camera *c, void *q, s16 *out)
{ (void)a; (void)p; (void)c; (void)q; out[0]=160; out[1]=80; }
s16 sound_pitch_diff_halved(void *p, s16 x)
{ assert(strlen((char *)p) == 1); return x; }
s16 object_bytes_sum_global(void) { return 10; }
void state_utility(s16 x, s16 y, void *p)
{ (void)x; (void)y; assert(strlen((char *)p) == 1); }
void fcvt_wrapper(char *out, char *format, ...)
{
    va_list ap;
    int value, n;
    assert(strcmp(format,"%d") == 0);
    va_start(ap,format); value=va_arg(ap,int); va_end(ap);
    assert(value >= 1 && value <= 4);
    n=sprintf(out,format,value);
    assert(n==1 && out[0]=='0'+value && out[1]==0);
    writes++;
}
void func_800ED66C(f32 f) { (void)f; }
void func_800BEA3C(Color4 a, Color4 b) { (void)a; (void)b; }
void func_800B7360(u8 a,u8 b,u8 c,u8 d) { (void)a; (void)b; (void)c; (void)d; }

int main(void)
{
    static char *titles[40], *options[10];
    static u16 indices[21];
    const f32 angles[4] = {0.1f, 0.5f, 1.0f, 3.0f};
    const s32 selections[4] = {-1, 0, 3, 9};
    int i,mode,angle,unavailable,players,selection,cases=0;
    memset(indices,0,sizeof(indices));
    for(i=0;i<40;i++) titles[i]="T";
    for(i=0;i<10;i++) { options[i]="O"; D_803B65E4[i]=1; }
    countdown_state.text=titles; countdown_state.indices=indices;
    countdown_state.indexed=options;
    D_80156944=0x100; D_801164A1=1;
    for(mode=0;mode<2;mode++) for(angle=0;angle<4;angle++)
    for(unavailable=0;unavailable<5;unavailable++) for(players=1;players<=4;players++)
    for(selection=0;selection<4;selection++) {
        for(i=0;i<10;i++) D_803B6610[i].angle=angles[angle];
        D_803B6A48=mode; D_803B6A6C=unavailable;
        D_8014A108=players; D_803B6A68=selections[selection];
        writes=0;
        assert(func_803AE1C0(0)==1);
        assert(writes == ((mode==1 && angle==2) ? 0U : 4U));
        cases++;
    }
    assert(cases==640);
    return 0;
}
