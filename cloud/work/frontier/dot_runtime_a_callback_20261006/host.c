/* Hosted call-order oracle seam. Native layout is checked separately at -m32. */
#include <stdarg.h>
#include <stdint.h>
#include <string.h>
#include "../../../matches/ovl_a/func_803A6A28.c"
MenuText menu[2], *D_8017A4E4;
u32 D_80156978;
s8 D_80156994;
u8 D_803B87E8[1];
s32 D_8002E440, D_8002E444;
static u8 text[4];
static int events[32][8], count, hook;
static int textid(void *p) {
    int i;
    if (p == D_803B87E8) return 100;
    for (i=0;i<4;i++) if (p == text+i) return 920+i;
    return -1;
}
static void event(int id, int a, int b, int c, int d, int e, int f, int g) {
    int *row=events[count++];
    row[0]=id;row[1]=a;row[2]=b;row[3]=c;row[4]=d;row[5]=e;row[6]=f;row[7]=g;
    if (count == hook) {
        D_80156978 ^= 0x3C000;
        D_80156994 = !D_80156994;
        D_8017A4E4 = menu+1;
        D_8002E440 = -17;
        D_8002E444 = 2147483647;
    }
}
void render_helper(f32 x) { event(1,x==0.0f?0:-1,0,0,0,0,0,0); }
void *object_create(s32 x) { event(2,x,0,0,0,0,0,0); return 0; }
s8 object_byte9_set(s8 x) { event(3,x,0,0,0,0,0,0);return -3; }
void mode_byte_set(s16 x) { event(4,x,0,0,0,0,0,0); }
void func_800B669C(u32 x,u32 y) { event(5,x,y,0,0,0,0,0); }
void dispatch_handler(s32 x) { event(6,x,0,0,0,0,0,0); }
void camera_auto_follow(s16 a,s16 b,s16 c,s16 d,s16 e,s16 f,u8 *g) { event(7,a,b,c,d,e,f,textid(g)); }
void music_tempo_adjust(s16 a,s16 b,u8 *fmt,...) {
    va_list ap;int c,d;
    va_start(ap,fmt);c=va_arg(ap,int);d=va_arg(ap,int);va_end(ap);
    event(8,a,b,textid(fmt),c,d,0,0);
}
void state_utility(s16 a,s16 b,void *c) { event(9,a,b,textid(c),0,0,0,0); }
int host_run(u32 flags,int enabled,int mutation_at,int *out) {
    int result;
    menu[0].text920=text;menu[0].text936=text+1;
    menu[1].text920=text+2;menu[1].text936=text+3;
    D_8017A4E4=menu;D_80156978=flags;D_80156994=enabled;
    D_8002E440=(-2147483647-1);D_8002E444=23;
    memset(events,0,sizeof(events));count=0;hook=mutation_at;
    result=func_803A6A28(0x12345678);
    if(result!=1)return -1;
    memcpy(out,events,sizeof(events));return count;
}
