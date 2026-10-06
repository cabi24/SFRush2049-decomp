/* Literal candidate inclusion; no independent rewritten source algorithm. */
#include <stddef.h>
#include <string.h>
#include CANDIDATE_FILE
#define N 4
int D_8014A110;
s16 D_8014A108;
s8 D_80152744, D_8010FFC0, D_80114650;
u32 D_801174B4;
PlayerState D_80152818[N];
ModelState D_8014A250[N];
ScoreRecord D_8014A118[N];
EffectRecord D_80150B70[N];
ObjectRef *D_80152698[N];
u32 D_801392D8[N];
SlotStatus D_80153E88[N];
u8 D_801141B0[16];
static ObjectRef refs[N];
static ObjectBody bodies[N];
static ObjectLink links[N];
static ObjectHeader headers[N];
static const int *config;
static u32 events[128][11];
static int event_count;
static u32 bits(float f) { u32 u; memcpy(&u, &f, 4); return u; }
static float frombits(u32 u) { float f; memcpy(&f, &u, 4); return f; }
static u32 *event(u32 id)
{
    u32 *e = events[event_count++];
    memset(e, 0, 44);
    e[0] = id;
    return e;
}
static void (*review_hook)(int);
void register_hook(void (*hook)(int)) { review_hook=hook; }
static void mutate(void)
{
    if (review_hook) review_hook(event_count);
    int j, mask;
    if (event_count != config[7]) return;
    j = config[14]; mask = config[8];
    if (mask & 1) D_8014A110 = config[9];
    if (mask & 2) D_80152744 = (s8)config[10];
    if (mask & 4) D_8014A250[j].slot = (s16)config[11];
    if (mask & 8) D_8014A250[j].mode = (s8)config[12];
    if (mask & 16) D_8010FFC0 = (s8)config[13];
    if (mask & 32) D_80153E88[j].state = (u8)config[15];
    if (mask & 64) D_80114650 = (s8)config[16];
    if (mask & 128) D_80152818[j].transition = (s8)config[17];
}
#define SIMPLE(NAME, ID) void NAME(void) { event(ID); mutate(); }
SIMPLE(race_init_helper,1)
SIMPLE(camera_scene_manager,2)
SIMPLE(func_8038FCE0,3)
SIMPLE(func_80390F60,4)
SIMPLE(func_800D169C,5)
SIMPLE(render_large_objects,6)
SIMPLE(func_800F8EC8,7)
SIMPLE(hud_render,8)
SIMPLE(camera_position_update,9)
SIMPLE(race_setup_1,18)
SIMPLE(menu_controller_remap,19)
SIMPLE(skid_mark_render,20)
void save_write_data(void *p, int mode, float scale, int sound)
{
    u32 *e = event(10);
    e[1]=(u32)*(s16 *)p; e[2]=(u32)mode; e[3]=bits(scale); e[4]=(u32)sound;
    mutate();
}
void func_800D5524(ModelState *m)
{
    event(11)[1]=(u32)(m-D_8014A250); mutate();
}
int entity_flags_apply(int a,int b,int c,u8 d)
{
    u32 *e=event(12); e[1]=(u32)a;e[2]=(u32)b;e[3]=(u32)c;e[4]=d;
    mutate(); return 123;
}
int camera_target_track(float *p,const void *q,float a,float b,float c,float d,
                        int e0,int f,int g,u8 h)
{
    int i;
    u32 *e=event(13);
    e[1]=0xffffffffu;
    for(i=0;i<N;i++) if(p==D_8014A250[i].position) e[1]=(u32)i;
    e[2]=q==D_801141B0 ? 0x801141B0u : 0xffffffffu;
    e[3]=bits(a);e[4]=bits(b);e[5]=bits(c);e[6]=bits(d);
    e[7]=(u32)e0;e[8]=(u32)f;e[9]=(u32)g;e[10]=h;
    mutate(); return 234;
}
void func_800C3578(int x) { event(14)[1]=(u32)x; mutate(); }
void func_800F8E90(s16 x) { event(15)[1]=(u32)x; mutate(); }
void cpak_read(s16 x) { event(16)[1]=(u32)x; mutate(); }
void race_setup_2(s16 x) { event(17)[1]=(u32)x; mutate(); }
int layouts(void)
{
    return sizeof(PlayerState)==0x3B8 && sizeof(ModelState)==0x808 &&
        sizeof(ScoreRecord)==76 && sizeof(EffectRecord)==152 && sizeof(SlotStatus)==8 &&
        offsetof(PlayerState,transition)==0xED && offsetof(PlayerState,effect_index)==0x35C &&
        offsetof(ModelState,position)==0x22C && offsetof(ModelState,slot)==0x7C6 &&
        offsetof(ModelState,mode)==0x7CC && offsetof(ScoreRecord,transitions)==0x40 &&
        offsetof(EffectRecord,position)==0x24 && offsetof(EffectRecord,saved_position)==0x84 &&
        offsetof(ObjectBody,link)==0x28 && offsetof(ObjectHeader,flags)==5;
}
int run(const int *c,u32 *out)
{
    int i,j,k=0,b;
    config=c;event_count=0;
    D_8014A110=c[0];D_80152744=(s8)c[1];D_8014A108=(s16)c[2];
    D_801174B4=(u32)c[3];D_8010FFC0=(s8)c[4];D_80114650=(s8)c[5];
    memset(D_80152818,0x5a,sizeof(D_80152818));
    memset(D_8014A250,0xa5,sizeof(D_8014A250));
    memset(D_8014A118,0x69,sizeof(D_8014A118));
    memset(D_80150B70,0x96,sizeof(D_80150B70));
    for(i=0;i<N;i++) {
        b=32+i*16;
        D_80152818[i].transition=(s8)c[b];
        D_80152818[i].effect_index=(s8)c[b+1];
        D_80152818[i].effect_mode=(s8)c[b+2];
        D_80152818[i].saved_effect_mode=(s8)c[b+3];
        D_8014A250[i].slot=(s16)c[b+4];D_8014A250[i].mode=(s8)c[b+5];
        D_8014A118[i].transitions=(u16)c[b+6];D_801392D8[i]=(u32)c[b+7];
        D_80153E88[i].state=(u8)c[b+8];
        D_80152698[i]=c[b+9] ? &refs[i] : 0;
        refs[i].body=&bodies[i];bodies[i].link=&links[i];links[i].header=&headers[i];
        headers[i].flags=(s8)c[b+10];
        for(j=0;j<3;j++) {
            D_80150B70[i].position[j]=frombits((u32)c[b+11+j]);
            D_80150B70[i].saved_position[j]=frombits(0x40400000u+j*0x40000u);
            D_8014A250[i].position[j]=(float)(i*10+j);
        }
    }
    render_viewport_init();
    out[k++]=(u32)event_count;
    for(i=0;i<event_count;i++) for(j=0;j<11;j++) out[k++]=events[i][j];
    out[k++]=(u32)D_8014A110;out[k++]=(u32)D_80152744;out[k++]=(u32)D_8010FFC0;
    out[k++]=(u32)D_80114650;
    for(i=0;i<N;i++) {
        out[k++]=(u32)D_80152818[i].transition;
        out[k++]=(u32)D_80152818[i].effect_mode;
        out[k++]=(u32)D_80152818[i].saved_effect_mode;
        out[k++]=(u32)D_8014A250[i].slot;out[k++]=(u32)D_8014A250[i].mode;
        out[k++]=(u32)D_8014A118[i].transitions;out[k++]=D_801392D8[i];
        out[k++]=D_80153E88[i].state;
        for(j=0;j<3;j++) out[k++]=bits(D_80150B70[i].saved_position[j]);
    }
    return k;
}
#ifdef HOST_MAIN
#include <stdio.h>
int main(void)
{
    int c[128], count=0;
    u32 out[2048];
    if (!layouts()) return 2;
    while (fread(c,sizeof(c),1,stdin)==1) {
        if (run(c,out)<=0) return 3;
        count++;
    }
    if (ferror(stdin)) return 4;
    printf("%d literal-source host cases passed\n",count);
    return 0;
}
#endif

void review_event(unsigned *out) { memcpy(out,events[event_count-1],44); }
void review_ref(int i, int enabled) { D_80152698[i] = enabled ? &refs[i] : 0; }
int review_ref_state(int i) { return D_80152698[i] != 0; }
void review_header(int i, int value) { headers[i].flags = (s8)value; }
int review_header_state(int i) { return headers[i].flags; }
