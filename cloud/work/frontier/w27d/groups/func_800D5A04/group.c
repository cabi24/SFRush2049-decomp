/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800D5A04 (0x800D5A04, 428 B) -- race-to-results transition: stop the doppler/records voices,
 * reset every active car's state (func_800D5828 on model[i].slot), flush the entity list, hand an
 * operation-7 message node to the loader queues, then either flag state 0x40000 (D_80114650) or set
 * up the ghost race and restart play (state 4). Body from cloud/work/frontier/dot_transition_d5a04_
 * research_20261006 with osJamMesg -> osSendMesg (retail calls 0x800075E0 = osSendMesg; the old name
 * predates the libultra renames). `node->operation = 7; message = node;` is the research form (two
 * consumers of the allocated node; no device).
 * WHOLE-PROGRAM UNIT: EQUAL (blob_unit score func_800D5A04 --neighbours, 0 locked bodies differ).
 * score.py fn / score.py group: 13/107 words, all ugen temp-ring register names (t1.. vs t7..): the
 * ring position inherited from the procedures emitted before it in the unit; the context files here
 * (byte-identical locked copies) do not reproduce it. Install through the unit gate, not score.py.
 */
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
s32 osSendMesg(void *,void *,s32);
MessageNode *func_80091B00(void);
void ghost_race_setup(void);
void func_800B912C(s32);
void wheel_render_full(s32);
void playgame_state_change(void);
void func_8038F454();
void entity_audio_update(void);
void func_800D5524(void *);
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
    osSendMesg(D_80142728,0,0);
    osSendMesg(D_801427A8,message,0);
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
