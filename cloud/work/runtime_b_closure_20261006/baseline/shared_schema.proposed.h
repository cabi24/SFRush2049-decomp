/* PROPOSED RESEARCH SCHEMA ONLY. Not compiled or approved as an original TU.
 * Every named field below is consumed by one of the eight genuine bodies.
 * Unknown ranges preserve established strides; they are not pressure fields.
 * See compatibility.md for source views, aliases and unresolved contracts.
 */
#ifndef RUNTIME_B_CLOSURE_PROPOSED_H
#define RUNTIME_B_CLOSURE_PROPOSED_H
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct BObject {
    struct BObject *next;
    s32 scene_index;
    f32 uv[3][3];
    f32 position[3];
    u32 unknown38;
} BObject;                         /* 0x3C */
typedef struct BAttachedEffect {
    u8 unknown00[8];
    u32 flags;
    u32 unknown0C;
    f32 position[3];
} BAttachedEffect;                 /* 0x1C CONSUMED PREFIX, not allocation size */
typedef struct BStatusEffect {
    s32 scene_index;
    f32 uv[3][3];
    f32 position[3];
} BStatusEffect;                   /* 0x34; F938 global stride */
typedef struct BRecord {
    struct BRecord *next;
    s8 owner;
    s8 hit_mask;                    /* +5: native signed byte in D498 */
    s8 kind;
    s8 flags;
    f32 lifetime;
    f32 timer;
    f32 collision_extent;           /* +0x10: former elapsed/radius labels */
    f32 velocity[3];
    f32 position[3];
    f32 previous_position[3];
    f32 uv[3][3];
    BAttachedEffect *attached_effect;
    BObject *primary;
    BObject *secondary;
} BRecord;                         /* 0x68 */
typedef struct BDebris {
    struct BDebris *next;
    s32 scene_index;
    f32 uv[3][3];
    f32 position[3];
    f32 lifetime;
    f32 velocity[3];
} BDebris;                         /* 0x48 */
typedef struct BInput {
    u32 unknown00;
    u32 pressed;
    u32 held;
    u8 unknown0C[0x30-0x0C];
    u32 reset_mask;
    u8 unknown34[8];
    u32 fire_mask;
} BInput;                          /* 0x40 consumed view */
typedef struct BStatus {
    u32 flags;
    f32 transition_timer;
    f32 scale_timer;
    f32 scale;
    f32 pitch;
} BStatus;                         /* 0x14 */
typedef struct BPlayer {
    u8 unknown00[8];
    f32 position[3];
    f32 velocity[3];
    u8 unknown20[12];
    f32 uv[3][3];
    u8 unknown50[0x308-0x50];
    s8 active;
    u8 unknown309[0x34C-0x309];
    u32 color;
    u8 unknown350[0x359-0x350];
    s8 blocked;
    u8 unknown35A;
    s8 owner;
    u8 unknown35C[0x380-0x35C];
    BInput *input;
    s8 kind;
    s8 ammo;
    u8 unknown386[6];
    BStatus status;
    s8 action;
    u8 alpha;
    s8 transition;
    u8 unknown3A3;
    s32 latched;
    f32 transition_time;
    f32 cooldown;
    f32 pitch;
    f32 yaw;
} BPlayer;                         /* 0x3B8 */
typedef struct BVehicle {
    u8 unknown00[8];
    u8 model;
    u8 unknown09[0xFC-9];
    f32 extent_fc;
    u8 unknown100[8];
    f32 extent_108;
    u8 unknown10C[0x124-0x10C];
    f32 impulse[3];
    u8 unknown130[0x13C-0x130];
    f32 horizontal_impulse[3];
    u8 unknown148[0x640-0x148];
    s8 blocked;
    u8 unknown641[0x654-0x641];
    f32 radius;
    u8 unknown658[0x6C4-0x658];
    s16 state;
    u8 unknown6C6[0x808-0x6C6];
} BVehicle;                        /* 0x808 */
typedef struct BQuad BQuad;         /* A78BC returns a record pointer */
typedef struct BNameEntry BNameEntry;
typedef struct BRelease {
    struct BRelease *next;
    BQuad *quad;
} BRelease;                        /* +4 pointer, 0x08 consumed view */
typedef struct BPool {
    u8 header[16];
    void *head;
} BPool;                           /* 0x14 consumed prefix, not allocation size */
typedef struct BScene {
    u8 unknown00[12];
    f32 scale;
    u8 unknown10[0x44-0x10];
} BScene;                          /* 0x44 */
typedef struct BTexture { u8 bytes[36]; } BTexture;
typedef struct BTextureBank { BTexture *textures; u32 unknown04; } BTextureBank;
typedef union BColor { u32 word; u8 rgba[4]; } BColor;

/* Keep address-spelled symbols distinct. The linker/memory model records
 * byte aliases; it must not infer original C object identity from adjacency.
 */
extern BDebris *D_8039AE90;           /* aliases D_8039AE80 + 0x10 */
extern BRecord *D_8039A530;           /* aliases D_8039A520 + 0x10 */
extern BPool D_8039AE80, D_8039A520, D_8039A5F8, D_80394F70;
extern BPlayer D_80152818[];
extern BVehicle D_8014A250[];
extern BScene D_8012E700[];
extern BTextureBank D_80151AE8[];
extern BStatusEffect D_80394F90[4];
extern BColor D_8039438C;
extern s16 D_801543CA;
extern f32 D_8002EB94, D_80142764;
extern f32 D_8011F844[][4];
extern f32 D_80394358[13];
extern f32 D_803943A4[8][13][3];
extern f32 D_80394AFC[3], D_80394B08[][3];
extern f32 D_8011418C[3][3], D_801141B0[3][3];
extern s32 D_803942C0[];
extern s32 D_80399B18[];             /* indexed base; original extent unproven */
extern s32 D_80399B54;               /* physical byte alias of base + 15*4 */
extern s8 D_8012E67C[];
extern u16 D_80399AF8[], D_80394F88;
extern u8 D_80140BDC;
extern u32 D_80394390, D_803942E8;
extern u8 D_80394CFC[], D_80394D08[];

/* Actual external ABI data, using coherent logical pointee views. */
extern void math_utility(f32 source[3][3], f32 destination[3][3]);
extern void func_80090F44(f32 angle, f32 uv[3][3]);
extern void func_80090E9C(f32 angle, f32 uv[3][3]);
extern void func_8009EA68(f32 scale, f32 uv[3][3]);
extern f32 func_8008B2E4(f32 limit);
extern f32 func_8008C768(f32 y, f32 x);
extern void func_800A61B0(f32 source[3], f32 dest[3], f32 basis[3][3]);
extern void func_8009E820(f32 source[3], f32 dest[3], f32 basis[3][3]);
extern void func_8008B32C(f32 source[3][3], f32 dest[3][3], f32 scale);
extern void *func_8008E3C0(BPool *pool);
extern void func_800AFA84(BPool *pool, void *object);
extern void func_80090254(s16 scene_index);
extern s32 func_8008E398(s32 resource, f32 uv[3][3], s32 parent, u32 flags);
extern void func_8008E06C(s16 scene_index, u32 *color);
extern void func_8008D870(s16 scene_index, BTexture *texture, s32 index);
extern s32 func_800B61A8(s32 sound, s32 owner, s32 arg2, u8 arg3);
extern void func_8008D0C0(s16 *quad_active); /* call with (s16 *)release->quad */
extern BNameEntry *func_800B24EC(char *name, s16 *resource, s8 lo, s8 hi, s32 err);
extern BQuad *func_800A78BC(s32 count, f32 corners[4][3], u16 resource,
                           u32 *color, u16 flags, s32 indexed);
extern void func_8038F568(BPlayer *player);
extern BAttachedEffect *func_8038D054(s32 mode, f32 position[3], f32 homed_position[3]);
extern void func_8038CB20(void);
extern void func_8038DDDC(BRecord *record);
extern void func_8038D3A4(BPlayer *owner, BPlayer *target, s32 damage);
extern void func_8038D798(f32 position[3], f32 previous[3], s32 owner,
                         f32 strength, s32 damage);
extern s32 func_800ADD58(f32 previous[3], f32 position[3], f32 basis[3][3],
                        f32 extent, s32 mode, u32 mask);
extern void func_800ABCC8(void *effect, s32 mode, f32 position[3], s32 arg3,
                         s32 owner, s32 arg5, s32 arg6);
extern void func_800AF06C(f32 position[3], s32 mode, f32 scale, s32 arg3);
extern void func_800AEFE0(f32 position[3], f32 basis[3][3], f32 strength,
                         f32 arg3, f32 arg4, f32 arg5, s32 sound,
                         s32 owner, s32 arg8, s32 flags);
extern f32 fabsf(f32 value);
extern f32 sqrtf(f32 value);
#pragma intrinsic(fabsf, sqrtf)

/* Proposed private visibility, not recovered original signatures/order.
 * No body is supplied here; approved assembly must use every real body.
 * FCE0 is the sole externally kept genuine root, never E114.
 */
static void func_8038D200(BRecord *record, s32 mode);
static BObject *func_8038D328(s32 resource, s32 parent, u32 flags, s32 omit_uv);
static void func_8038D498(BRecord *record);
static BPlayer *func_8038DA78(BRecord *record, f32 previous[3]);
static void func_8038E088(BRecord *record);
static void func_8038E114(void);
static void func_8038F938(BStatus *status, BPlayer *player, BVehicle *vehicle, s32 owner);
void func_8038FCE0(void);
#endif
