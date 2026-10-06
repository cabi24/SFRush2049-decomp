/* COMPLETE-NONMATCH semantic units; private argument order/TU are unproven.
 * Native image B: D200, D328, E088. These are the real E114 callees, not
 * standalone matching submissions. No optimizer-visible stand-in caller.
 * N64 battle-object logic; no whole-function arcade donor established.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct BObject {
    struct BObject *next;
    s32 scene_index;
    f32 uv[3][3];
    f32 position[3];
    u32 unknown38;
} BObject;

typedef struct BRecord {
    struct BRecord *next;
    s8 owner;
    u8 unknown05;
    s8 kind;
    s8 flags;
    f32 lifetime;
    f32 timer;
    f32 elapsed;
    f32 velocity[3];
    f32 position[3];
    f32 previous_position[3];
    f32 uv[3][3];
    void *attached_effect;
    BObject *primary;
    BObject *secondary;
} BRecord;

typedef struct BPool BPool;
extern BPool D_80394F70;
extern f32 D_8011418C[3][3];
extern s32 D_80399B18[17];
extern void math_utility(f32 source[3][3], f32 destination[3][3]);
extern void func_80090F44(f32 angle, f32 uv[3][3]);
extern BObject *func_8008E3C0(BPool *pool);
/* Native E398 leaves E26C's sign-extended scene index in v0. The accepted
 * wrapper currently declares void; this research contract keeps its output. */
extern s32 func_8008E398(s32 resource, f32 transform[3][3],
                       s32 parent, u32 flags);
extern void sound_call_minimal(s16 scene_index);
extern void func_800AFA84(BPool *pool, BObject *object);

void func_8038D200(BRecord *record, s32 mode)
{
    BObject *object;
    s32 flags;
    f32 scale;

    object = record->primary;
    object->position[0] = record->position[0];
    object->position[1] = record->position[1];
    object->position[2] = record->position[2];
    if (mode == 1) {
        math_utility(D_8011418C, object->uv);
    } else if (mode != 2) {
        math_utility(record->uv, object->uv);
    }
    flags = record->flags;
    if (flags & 0x70) {
        if (((flags & 0x80) && (flags & 0x10) &&
             (record->kind == 1 || record->kind == 8)) ||
            record->kind == 0) {
            if (flags & 0x80) {
                scale = 0.2f;
            } else {
                scale = 0.4f;
            }
            object->uv[2][0] *= scale;
            object->uv[2][1] *= scale;
            object->uv[2][2] *= scale;
        }
    }
    if (record->flags & 2) {
        func_80090F44(0.18f, object->uv);
    }
}

BObject *func_8038D328(s32 resource_index, s32 parent,
                      u32 flags, s32 omit_transform)
{
    BObject *object;
    object = func_8008E3C0(&D_80394F70);
    if (omit_transform) {
        object->scene_index = func_8008E398(D_80399B18[resource_index],
                                          0, parent, flags);
    } else {
        object->scene_index = func_8008E398(D_80399B18[resource_index],
                                          object->uv, parent, flags);
    }
    return object;
}

void func_8038E088(BRecord *record)
{
    BObject *object;
    switch ((u8) record->kind) {
    case 3:
        object = record->secondary;
        sound_call_minimal((s16) object->scene_index);
        func_800AFA84(&D_80394F70, object);
        /* The primary pointer is reread after both external calls. */
    case 0:
    case 1:
    case 2:
    case 4:
    case 6:
    case 7:
    case 8:
        object = record->primary;
        sound_call_minimal((s16) object->scene_index);
        func_800AFA84(&D_80394F70, object);
        break;
    }
}
