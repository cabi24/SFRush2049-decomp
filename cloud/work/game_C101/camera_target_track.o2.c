/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef unsigned int u32;typedef int s32;typedef float f32;
typedef struct Link {struct Link *next,*prev;} Link;
typedef struct List {u8 indirect,doubly,pad[2];u32 count;Link *head,*tail;} List;
typedef struct Node68 {Link link;List *list;u32 tag,state,p20;u8 flag,code,live,mode;u32 p28;f32 f32,f36,f40,f44,f48;u32 index,value,p60,p64;} Node68;
typedef struct Object24 {u8 p0,p1,kind,p3;u32 value;f32 f8,f12,f16;Node68 *node;} Object24;
typedef struct Queue24 {u32 opaque[6];} Queue24;
extern List D_80143FF8,D_80144C50;extern u32 D_80110254,D_801460F8,D_80146104;
extern Queue24 D_80142728,D_801427A8;extern u8 D_8011F5CC[];
s32 osRecvMesg(Queue24 *,void **,s32);s32 osJamMesg(Queue24 *,void *,s32);
Object24 *func_80091B00(void);
void func_8009211C(List *,Link *);void func_80091FBC(List *,Link *,Link *);

typedef struct Effect44 { Link link; signed char active, ready; u8 unknown10[2]; f32 position[3], radius, gain, blend, scale; u32 tag; } Effect44;
extern u32 D_80110260; extern List D_801461E8;
Effect44 *func_800AED20(void);

Node68 *func_80092278(void);
u32 camera_target_track(f32 *position, s32 channel, f32 radius, f32 attenuation,
                        f32 gain, f32 blend, u32 index, u32 value,
                        u32 object_value, u8 mode) {
    Effect44 *effect;
    Object24 *object;
    u32 tag;
    if (!D_80110260) return -1;
    osRecvMesg(&D_80142728, 0, 1);
    effect = func_800AED20();
    if (!effect) {
        osJamMesg(&D_80142728, 0, 0);
        return -1;
    }
    effect->position[0] = position[0];
    effect->position[1] = position[1];
    effect->position[2] = position[2];
    effect->radius = radius;
    if (gain < 0.0f) effect->gain = 0.0f;
    else effect->gain = gain > 1.0f ? 1.0f : gain;
    if (blend < 0.0f) effect->blend = 0.0f;
    else effect->blend = blend > 1.0f ? 1.0f : blend;
    effect->scale = 1.0f;
    object = func_80091B00();
    object->kind = 2;
    object->value = object_value;
    object->node = func_80092278();
    object->node->p20 = 1;
    object->node->index = index;
    object->node->value = value;
    object->node->mode = mode;
    object->node->state = 1;
    object->node->flag = 0;
    object->node->live = 1;
    object->node->code = D_8011F5CC[index];
    object->f8 = 0.0f;
    object->f12 = 0.0f;
    object->f16 = -2.0f;
    object->node->p64 = (u32)effect;
    tag = object->node->tag;
    effect->tag = tag;
    osJamMesg(&D_80142728, 0, 0);
    osJamMesg(&D_801427A8, object, 0);
    osRecvMesg(&D_80142728, 0, 1);
    if (!effect->active) {
        func_80091FBC(&D_801461E8, (Link *)effect, D_801461E8.head);
        effect->ready = 1;
    }
    osJamMesg(&D_80142728, 0, 0);
    return tag;
}
