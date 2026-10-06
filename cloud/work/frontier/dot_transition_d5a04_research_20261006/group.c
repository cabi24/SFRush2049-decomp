/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef short s16;typedef unsigned char u8;typedef signed int s32;
typedef struct Vehicle2056 {u8 prefix[1990];s16 resource;u8 tail[64];} Vehicle2056;
typedef struct MessageNode {s16 id;s8 operation,used;float payload[4];void *entity;} MessageNode;
extern MessageNode D_80142DD8[128];
extern Vehicle2056 D_8014A250[];
extern s8 D_80152744,D_80114650,D_8014978C;
extern void *D_801541A4,*D_801391F0;
extern u8 D_80142728[],D_801427A8[];
extern s16 D_80151AD0;
extern s32 state_word_b;
void audio_doppler_full(s32);
void records_screen(void);
void sound_stop(void *);
void func_800D5828(s16);
void players_frame_update(void);
void func_800D5374(void);
void entity_transform_apply(void *,s32);
void func_800B0580(void);
s32 osRecvMesg(void *,void **,s32);
s32 osJamMesg(void *,void *,s32);
MessageNode *func_80091B00(void);
void ghost_race_setup(void);
void func_800B912C(s32);
void wheel_render_full(s32);
void playgame_state_change(void);
void func_8038F454();
void entity_audio_update(void);
MessageNode *func_80091B00(void) {
    s32 i;
    for(i=0;i<128;i++) {
        if(D_80142DD8[i].used==0) {
            D_80142DD8[i].used=1;
            D_80142DD8[i].id=-1;
            return &D_80142DD8[i];
        }
    }
    return 0;
}
void func_800D5524(void *);
void func_800D5828(s16 index) {
    u8 *vehicle=(u8 *)&D_8014A250[index];
    *(s16 *)(vehicle+1820)=0;
    func_800D5524(vehicle);
    *(float *)(vehicle+2044)=0.0f;
    *(float *)(vehicle+2048)=0.0f;
    *(float *)(vehicle+2052)=0.0f;
    *(float *)(vehicle+2040)=0.0f;
}
void func_800D5A04(void)
{
    s16 i;
    void *node;
    MessageNode *message;
    audio_doppler_full(0);
    records_screen();
    if(D_801541A4) {
        sound_stop(D_801541A4);
        D_801541A4=0;
    }
    for(i=0;i<D_80152744;i++) func_800D5828(D_8014A250[i].resource);
    players_frame_update();
    func_800D5374();
    while((node=D_801391F0)!=0) entity_transform_apply(node,1);
    func_800B0580();
    osRecvMesg(D_80142728,0,1);
    node=func_80091B00();
    ((MessageNode *)node)->operation=7;
    message=node;
    osJamMesg(D_80142728,0,0);
    osJamMesg(D_801427A8,message,0);
    if(D_80114650) state_word_b=0x40000;
    else {
        ghost_race_setup();
        func_800B912C(D_8014978C);
        D_80151AD0=1;
        wheel_render_full(1);
        state_word_b=4;
        playgame_state_change();
        func_8038F454();
    }
    entity_audio_update();
}
