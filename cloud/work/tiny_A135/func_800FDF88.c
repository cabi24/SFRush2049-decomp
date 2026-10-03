/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Save76 {
    u8 prefix[4];
    s8 active,secondary,unused6,variant,id;
    u8 gap[3];
    s32 timestamp;
    f32 value;
    u8 tail[56];
} Save76;
typedef struct ModelView { u8 prefix[1780]; Save76 save; } ModelView;
typedef struct ContextView {
    u8 prefix[8]; void *input;
    u8 gap[32]; ModelView **model;
} ContextView;
typedef struct Session76 { ContextView **context; u8 rest[72]; } Session76;
extern Session76 D_8014A160[];
extern u8 D_801543D4;
extern s32 D_8002EB90;
extern s8 D_80146127;
extern s32 D_801117B4[];
extern void *memset(void *,s32,u32);
extern void slot_state_lookup(void *,void *,s32);
void func_800FDF88(s32 id)
{
    Save76 *save=&(*(*D_8014A160[D_801543D4].context)->model)->save;
    memset(save,0,76);
    slot_state_lookup((*D_8014A160[D_801543D4].context)->input,save,76);
    save->id=id;
    save->timestamp=D_8002EB90 & 0x000FFFFF;
    save->active=1;
    save->value=0.0f;
    if(D_80146127) save->secondary=1;
    save->variant=D_801117B4[id];
}
