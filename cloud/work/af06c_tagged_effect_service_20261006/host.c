/* Host-only adapter. The included target source is compiled without edits. */
#include <stddef.h>
#include "candidate.c"
typedef unsigned long long host_word;
typedef host_word (*HostCall)(u32, host_word, host_word, host_word, host_word,
                              host_word, host_word, host_word, host_word,
                              host_word, host_word);
static HostCall host_call;
void set_host_call(HostCall call) { host_call = call; }
s8 D_80156994, D_8014978C, D_8010FFC0;
s16 D_8014A108, D_8013C094, D_8012E66C, D_8012E678;
u32 D_8011735C, D_8011B554, D_8011B550;
f32 D_801543CC;
f32 D_801239A8, D_801239AC, D_801239B0;
f32 D_801239B4, D_801239B8, D_801239BC;
f32 D_801239C0, D_801239C4, D_80123C00, D_80123C04;
f32 D_8011418C[9], D_801141B0[3];
u16 D_801428FC[4], D_80142904, D_80142906, D_80142908;
Player D_80152818[6];
EffectGroup D_80154660[6];
ExtraEffect D_80154FD8[6];
Scene D_8012E700[128];
Transform *D_8013C238[50];
EffectNode *D_801391F0, *D_801392C8;
EffectNode host_nodes[8];
Transform host_transforms[50];
u32 host_input[3];

static u32 bits(f32 value) { union { f32 f; u32 u; } v; v.f=value; return v.u; }
EffectNode *func_80090284(void) {
    return (EffectNode *)host_call(0x80090284u,0,0,0,0,0,0,0,0,0,0);
}
void math_utility(const f32 *src, f32 *dst) {
    host_call(0x8008D6B0u,(host_word)src,(host_word)dst,0,0,0,0,0,0,0,0);
}
void entity_spawn_callback(s16 scene,s32 a,s32 b) {
    host_call(0x80090088u,(u32)scene,(u32)a,(u32)b,0,0,0,0,0,0,0);
}
s32 func_8008E26C(s32 resource,void *matrix,s16 parent,s32 flags) {
    return (s32)host_call(0x8008E26Cu,(u32)resource,(host_word)matrix,
                         (u32)parent,(u32)flags,0,0,0,0,0,0);
}
void entity_spawn_init(s16 index,s32 slot,s32 gate,s32 mode) {
    host_call(0x8008EA10u,(u32)index,(u32)slot,(u32)gate,(u32)mode,0,0,0,0,0,0);
}
s32 camera_target_track(const f32 *position,const void *unused,f32 volume,
                        f32 reserved,f32 gain,f32 pan,u32 sound,s32 scene,
                        u32 value,u8 mode) {
    return (s32)host_call(0x800AED64u,(host_word)position,(host_word)unused,
                         bits(volume),bits(reserved),bits(gain),bits(pan),
                         sound,(u32)scene,value,mode);
}
/* These identities are stored; their implementations are not executed. */
void entity_collision_detect(EffectNode *node,s16 step) {(void)node;(void)step;}
void entity_physics_update(EffectNode *node,s16 step) {(void)node;(void)step;}
void entity_update_callback(EffectNode *node,s16 step) {(void)node;(void)step;}

unsigned long host_layout(unsigned int id) {
    switch (id) {
    case 0:return sizeof(EffectNode);
    case 1:return sizeof(Transform);
    case 2:return sizeof(Player);
    case 3:return sizeof(ExtraEffect);
    case 4:return sizeof(Debris);
    case 5:return sizeof(EffectGroup);
    case 6:return sizeof(Scene);
    case 7:return offsetof(EffectNode,callback);
    case 8:return offsetof(ExtraEffect,timer);
    case 9:return offsetof(Debris,lifetime);
    case 10:return offsetof(EffectGroup,extra);
    case 11:return offsetof(Scene,color);
    }
    return 0;
}
