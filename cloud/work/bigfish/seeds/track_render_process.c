/* flags: -g0 -O2 -mips2 -G 0 -non_shared ; repaired raw m2c seed, 772/800 words, NOT a match */

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef signed long long s64;
typedef unsigned long long u64;
typedef float f32;
typedef double f64;
typedef u32 uintptr_t;
typedef s32 intptr_t;
typedef volatile u8 vu8;
typedef volatile u16 vu16;
typedef volatile u32 vu32;
typedef volatile u64 vu64;
typedef volatile s8 vs8;
typedef volatile s16 vs16;
typedef volatile s32 vs32;
typedef volatile s64 vs64;
typedef union 
{
  struct 
  {
    u32 w0;
    u32 w1;
  } words;
  u64 force_structure_alignment;
} Gfx;
typedef u32 Mtx[4][4];
typedef f32 F32;
typedef s32 S32;
typedef s16 S16;
typedef s8 S08;
typedef u32 U32;
typedef u16 U16;
#define NULL ((void*)0)
/*
 * This header contains macros emitted by m2c in "valid syntax" mode,
 * which can be enabled by passing `--valid-syntax` on the command line.
 *
 * In this mode, unhandled types and expressions are emitted as macros so
 * that the output is compilable without human intervention.
 */

#ifndef M2C_MACROS_H
#define M2C_MACROS_H

/* Unknown types */
typedef s32 M2C_UNK;
typedef s8  M2C_UNK8;
typedef s16 M2C_UNK16;
typedef s32 M2C_UNK32;
typedef s64 M2C_UNK64;

/* Unknown field access, like `*(type_ptr) &expr->unk_offset` */
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))

/* Bitwise (reinterpret) cast */
#define M2C_BITWISE(type, expr) ((type)(expr))

/* Unaligned reads */
#define M2C_LWL(expr) (expr)
#define M2C_FIRST3BYTES(expr) (expr)
#define M2C_UNALIGNED32(expr) (expr)

/* Unhandled instructions */
#define M2C_ERROR(desc) (0)
#define M2C_TRAP_IF(cond) (0)
#define M2C_BREAK() (0)
#define M2C_SYNC() (0)

#define GLUE_F64(a, b) (0.0)
#define MULT_HI(a, b) (0)
#define MULTU_HI(a, b) (0)
#define DMULT_HI(a, b) (0)
#define DMULTU_HI(a, b) (0)
#define CLZ(x) (0)
#define REVERSE_BITS(x) (0)
#define ROTATE_RIGHT(x, shift) (0)
#define ARM_RRX(x, carry) (0)
#define BSWAP32(x) (0)
#define BSWAP16(x) (0)
#define BSWAP16X2(x) (0)

/* Carry/overflow bits from partially-implemented instructions */
#define M2C_CARRY 0
#define M2C_OVERFLOW(a) (0)

/* Memcpy patterns */
#define M2C_MEMCPY_ALIGNED memcpy
#define M2C_MEMCPY_UNALIGNED memcpy
#define M2C_STRUCT_COPY memcpy

#endif
float sqrtf(float);
float fabsf(float);
#pragma intrinsic (sqrtf)
#pragma intrinsic (fabsf)
extern M2C_UNK func_8008A704();
extern M2C_UNK func_8009211C();
extern M2C_UNK func_800A1644();
extern s32 func_800A1910();
extern s32 func_800A1E94();
extern M2C_UNK func_800A3640();
extern M2C_UNK func_800A3724();
extern M2C_UNK memcpy();
extern M2C_UNK no_catchup();
extern M2C_UNK osCreateMesgQueue();
extern M2C_UNK osJamMesg();
extern s32 osMotorStart();
extern s32 osMotorStop();
extern s32 osPfsDeleteFile();
extern M2C_UNK osPfsFreeBlocks();
extern s32 osPfsInitPak();
extern M2C_UNK osRecvMesg();
extern s32 track_collision_setup();
extern u8 D_80035458[];
extern u8 D_8008A6A4[];
extern s8 D_8011194C;
extern s8 D_8011EAE0;
extern s8 D_8011EAE8;
extern M2C_UNK (*D_80144008)(s8, M2C_UNK, M2C_UNK, M2C_UNK, s8 *, s8 *, M2C_UNK *);
extern s8 D_80144030;
extern s8 D_80144334;
extern s8 D_80144638;
extern s8 D_8014493C;
extern u8 D_80144D60[];
extern u8 D_80144D68[];
extern u8 D_801460E0[];
extern void **D_801460E8;
extern u8 D_801497D0[];
extern u8 D_801527E4[];
extern u8 D_80156CF0[];

void track_render_process(s32 arg0, s32 arg1) {
    s8 sp130[16];
    s32 sp1A8;
    M2C_UNK sp144;
    s32 sp13C;
    s8 sp12F;
    s8 sp12E;
    void **sp128;
    M2C_UNK sp10C;
    M2C_UNK spFC;
    M2C_UNK spE4;
    M2C_UNK spD4;
    M2C_UNK spA4;
    M2C_UNK sp94;
    void **sp84;
    s8 *sp7C;
    s32 sp78;                                       /* compiler-managed */
    s8 *sp74;
    s8 *sp70;
    void *sp6C;
    s8 *sp68;
    void *sp64;
    void *sp60;
    s8 *sp5C;
    s8 *sp58;
    s8 *sp54;
    M2C_UNK **temp_a2;
    M2C_UNK **var_t0;
    s32 *temp_a1_2;
    s32 temp_a3;
    s32 temp_s0;
    s32 temp_t6;
    s32 temp_t9;
    s32 temp_t9_2;
    s32 temp_v0;
    s32 temp_v0_2;
    s32 var_s0;
    s32 var_v0_2;
    s8 *temp_a0;
    s8 *temp_a1;
    s8 *temp_v1;
    s8 *var_s1;
    s8 *var_s4;
    s8 *var_v1;
    s8 var_s8;
    u32 var_v0;
    u8 *var_v1_2;
    void **var_t0_2;
    void *temp_s0_2;
    void *var_s0_2;
    void *var_s1_2;

    /* Flowgraph is not reducible, falling back to gotos-only mode. */
    sp130[6] = 0;
    sp130[5] = 0;
    if (D_8011EAE0 == 0) {
        goto block_118;
    }
    var_v1 = sp130;
loop_2:
    var_v1 += 1;
    M2C_FIELD(var_v1, s8 *, -1) = 0;
    if ((u32) var_v1 < (u32) &sp130[4]) {
        goto loop_2;
    }
    func_8008A704();
    var_s8 = 0;
loop_4:
    temp_a3 = var_s8 * 0x10;
    if (D_8011EAE8 < 0) {
        goto block_6;
    }
    if (var_s8 != D_8011EAE8) {
        goto block_116;
    }
block_6:
    var_s4 = (var_s8 * 0x304) + &D_80144030;
    if (*((u8 *) &D_80156CF0 + temp_a3) == 0) {
        goto block_9;
    }
    if (M2C_FIELD(var_s4, s8 *, 2) == 0) {
        goto block_9;
    }
    if (M2C_FIELD(var_s4, s8 *, 3) == 0) {
        goto block_24;
    }
block_9:
    M2C_FIELD(var_s4, s8 *, 3) = 0;
    if (M2C_FIELD(var_s4, s8 *, 1) == 0) {
        goto block_19;
    }
    M2C_FIELD(var_s4, s8 *, 1) = 0;
    M2C_FIELD(var_s4, s8 *, 9) = 0;
    M2C_FIELD(var_s4, s32 *, 0x74) = 0;
    sp7C = &(sp130)[var_s8];
    if (M2C_FIELD(var_s4, s8 *, 6) == 0) {
        goto block_12;
    }
    M2C_FIELD(var_s4, s8 *, 6) = 0;
    M2C_FIELD(var_s4, s16 *, 0x80) = 0;
    M2C_FIELD(var_s4, s8 *, 0x7E) = 0;
    M2C_FIELD(var_s4, s8 *, 0x7D) = 0;
    M2C_FIELD(var_s4, s8 *, 0x7C) = 0;
    M2C_FIELD(sp7C, s8 *, 0) = 1;
    goto block_116;
block_12:
    if (M2C_FIELD(var_s4, s8 *, 0xA) == 0) {
        goto block_14;
    }
    goto block_18;
block_14:
    if (M2C_FIELD(var_s4, s8 *, 0xB) != 0) {
        goto block_18;
    }
    if (M2C_FIELD(var_s4, s8 *, 7) == 0) {
        goto block_17;
    }
    goto block_18;
block_17:
    sp74 = var_s4;
    func_800A3640(var_s8);
    func_800A3724();
block_18:
    M2C_FIELD(var_s4, s8 *, 5) = 0;
    M2C_FIELD(sp7C, s8 *, 0) = 1;
    goto block_116;
block_19:
    if (M2C_FIELD(var_s4, s8 *, 4) == 0) {
        goto block_116;
    }
    if (M2C_FIELD(var_s4, s8 *, 0xA) != 0) {
        goto block_116;
    }
    if (M2C_FIELD(var_s4, s8 *, 0xB) != 0) {
        goto block_116;
    }
    if (M2C_FIELD(var_s4, s8 *, 7) != 0) {
        goto block_116;
    }
    func_800A3640(var_s8);
    func_800A3724();
    goto block_116;
block_24:
    temp_v1 = &(sp130)[var_s8];
    if (M2C_FIELD(var_s4, s8 *, 1) == 0) {
        goto block_26;
    }
    goto block_116;
block_26:
    *temp_v1 = 1;
    temp_a1 = var_s4 + 0xC;
    M2C_FIELD(var_s4, s8 *, 1) = 1;
    sp70 = temp_a1;
    sp78 = temp_a3;
    sp7C = temp_v1;
    sp12E = 0;
    sp12F = 0;
    memcpy(&sp130[8], temp_a1, 0x68, temp_a3);
loop_27:
    sp13C = 0;
    var_s0 = osPfsInitPak(&D_80035458, &sp130[8], var_s8);
    if (sp13C != 0) {
        goto block_29;
    }
    var_s0 = 1;
block_29:
    M2C_FIELD(var_s4, s32 *, 0x74) = func_800A1E94(var_s0);
    if (M2C_FIELD(var_s4, s32 *, 0x74) != 0xA) {
        goto block_56;
    }
    if (osMotorStart(&D_80035458, &sp130[8], var_s8) != 0) {
        goto block_34;
    }
    if (M2C_FIELD(var_s4, s8 *, 6) != 0) {
        goto block_33;
    }
    M2C_FIELD(var_s4, s8 *, 6) = 1;
    goto block_116;
block_33:
    M2C_FIELD(sp7C, s8 *, 0) = 0;
    goto block_116;
block_34:
    if (arg1 == 0) {
        goto block_36;
    }
    M2C_FIELD(var_s4, s8 *, 1) = 0;
    M2C_FIELD(sp7C, s8 *, 0) = 0;
    goto block_116;
block_36:
    osJamMesg(&D_801497D0, 0, 0);
loop_37:
    D_8011EAE8 = var_s8;
    D_80144008(var_s8, 0xA, 0, 0, &sp130[5], &sp130[6], &D_8008A6A4);
    D_8011EAE8 = -1;
    if (M2C_FIELD(var_s4, s8 *, 1) != 0) {
        goto block_41;
    }
    if (D_8011194C != 0) {
        goto block_40;
    }
    D_8011194C = 1;
    osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
    osJamMesg(&D_801497D0, 0, 0);
block_40:
    osRecvMesg(&D_801497D0, &sp10C, 1);
    M2C_FIELD(sp7C, s8 *, 0) = 0;
    goto block_116;
block_41:
    if (sp130[6] == 0) {
        goto block_48;
    }
    if (D_8011194C != 0) {
        goto block_44;
    }
    D_8011194C = 1;
    osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
    osJamMesg(&D_801497D0, 0, 0);
block_44:
    osRecvMesg(&D_801497D0, &spFC, 1);
    temp_v0 = osPfsInitPak(&D_80035458, &sp130[8], var_s8);
    var_s0 = temp_v0;
    if (temp_v0 == 0) {
        goto block_56;
    }
    temp_s0 = osMotorStop(&sp130[8]);
    osJamMesg(&D_801497D0, 0, 0);
    if (temp_s0 == 0) {
        goto block_47;
    }
    D_8011EAE8 = var_s8;
    D_80144008(var_s8, 0xC, 0, 0, &sp130[5], &sp130[6], NULL);
    D_8011EAE8 = -1;
    goto loop_37;
block_47:
    D_8011EAE8 = var_s8;
    D_80144008(var_s8, 0xD, 0, 0, &sp130[5], &sp130[6], NULL);
    goto block_53;
block_48:
    if (sp130[5] == 0) {
        goto block_52;
    }
    if (D_8011194C != 0) {
        goto block_51;
    }
    D_8011194C = 1;
    osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
    osJamMesg(&D_801497D0, 0, 0);
block_51:
    osRecvMesg(&D_801497D0, &spE4, 1);
    M2C_FIELD(var_s4, s8 *, 5) = 1;
    M2C_FIELD(var_s4, s8 *, 9) = 0;
    M2C_FIELD(sp7C, s8 *, 0) = 0;
    goto block_116;
block_52:
    D_8011EAE8 = var_s8;
    D_80144008(var_s8, 0xB, 0, 0, &sp130[5], &sp130[6], NULL);
block_53:
    D_8011EAE8 = -1;
    if (D_8011194C != 0) {
        goto block_55;
    }
    D_8011194C = 1;
    osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
    osJamMesg(&D_801497D0, 0, 0);
block_55:
    osRecvMesg(&D_801497D0, &spD4, 1);
    goto loop_27;
block_56:
    M2C_FIELD(var_s4, s8 *, 6) = 0;
    M2C_FIELD(var_s4, s16 *, 0x80) = 0;
    M2C_FIELD(var_s4, s8 *, 0x7E) = 0;
    M2C_FIELD(var_s4, s8 *, 0x7D) = 0;
    M2C_FIELD(var_s4, s8 *, 0x7C) = 0;
    if (var_s0 != 2) {
        goto block_58;
    }
    M2C_FIELD(var_s4, s32 *, 0x74) = 0;
    goto block_60;
block_58:
    if (var_s0 == 0) {
        goto block_60;
    }
    M2C_FIELD(sp7C, s8 *, 0) = 0;
    M2C_FIELD(var_s4, s8 *, 5) = 1;
    M2C_FIELD(var_s4, s8 *, 9) = 0;
    goto block_116;
block_60:
    if (M2C_FIELD(var_s4, s8 *, 4) != 0) {
        goto block_62;
    }
    M2C_FIELD(var_s4, s8 *, 4) = 1;
    goto block_77;
block_62:
    if (func_800A1910(var_s4 + 0x18, &sp144, 0x20) != 0) {
        goto block_77;
    }
    M2C_FIELD(sp7C, s8 *, 0) = 0;
    sp6C = var_s4 + 0x78;
    memcpy(sp70, &sp130[8], 0x68);
    osPfsFreeBlocks(sp70, sp6C);
    M2C_FIELD(var_s4, s8 *, 9) = 1;
    if (M2C_FIELD(var_s4, s8 *, 7) != 0) {
        goto block_116;
    }
    var_t0 = *((u8 *) &D_80144D68 + sp78);
    if (var_t0 == NULL) {
        goto block_75;
    }
loop_65:
    temp_a2 = *var_t0;
    temp_a1_2 = M2C_FIELD(temp_a2, s32 **, 0x48);
    if (temp_a1_2 != NULL) {
        goto block_67;
    }
    goto block_71;
block_67:
    var_v0 = 0;
    temp_a0 = &(&D_80144030)[(M2C_FIELD(temp_a2, u8 *, 0x10) * 0x304) + (M2C_FIELD(temp_a2, u8 *, 0x11) * 0x28)];
    var_v1_2 = *temp_a1_2 + M2C_FIELD(temp_a2, s32 *, 0x40);
    if (M2C_FIELD(temp_a0, u16 *, 0x88) == 0) {
        goto block_71;
    }
loop_68:
    if (*var_v1_2 == 0) {
        goto block_70;
    }
    var_v0_2 = 1;
    goto block_72;
block_70:
    var_v0 += 1;
    var_v1_2 += 1;
    if (var_v0 < (u16) M2C_FIELD(temp_a0, u16 *, 0x88)) {
        goto loop_68;
    }
block_71:
    var_v0_2 = 0;
block_72:
    if (var_v0_2 == 0) {
        goto block_74;
    }
    M2C_FIELD((var_s4 + (M2C_FIELD(temp_a2, u8 *, 0x11) * 0x28)), s8 *, 0x85) = 1;
    M2C_FIELD(var_s4, s8 *, 0xB) = 1;
block_74:
    var_t0 = M2C_FIELD(*var_t0, M2C_UNK ***, 0);
    if (var_t0 != NULL) {
        goto loop_65;
    }
block_75:
    sp12E = 1;
    goto block_96;
block_77:
    sp1A8 = 0;
loop_78:
    if (var_s8 == sp1A8) {
        goto block_90;
    }
    var_s0_2 = (sp1A8 * 0x304) + &D_80144030;
    if (M2C_FIELD(var_s0_2, s8 *, 4) == 0) {
        goto block_90;
    }
    if (func_800A1910(&sp144, (u8 *) var_s0_2 + 0x18, 0x20) != 0) {
        goto block_90;
    }
    if (M2C_FIELD(var_s4, s8 *, 4) == 0) {
        goto block_83;
    }
    sp12F = 1;
    goto block_94;
block_83:
    sp6C = var_s4 + 0x78;
    sp68 = sp78 + (u8 *) &D_80144D60;
    var_s1 = (sp1A8 * 0x10) + (u8 *) &D_80144D60;
    sp5C = var_s4 + 0x84;
    sp54 = &(sp130)[sp1A8];
    sp58 = (u8 *) var_s0_2 + 0x84;
block_84:
    memcpy(sp70, &sp130[8], 0x68);
    osPfsFreeBlocks(sp70, sp6C);
    M2C_FIELD(var_s4, s8 *, 9) = 1;
    memcpy(sp68, var_s1, 0x10);
    M2C_FIELD(var_s1, s32 *, 8) = 0;
    M2C_FIELD(var_s1, s32 *, 0xC) = 0;
    M2C_FIELD(var_s1, s32 *, 4) = 0;
    var_t0_2 = M2C_FIELD(sp68, void ***, 8);
    if (var_t0_2 == NULL) {
        goto block_86;
    }
loop_85:
    M2C_FIELD(*var_t0_2, s8 *, 0x10) = var_s8;
    var_t0_2 = M2C_FIELD(*var_t0_2, void ***, 0);
    if (var_t0_2 != NULL) {
        goto loop_85;
    }
block_86:
    memcpy(sp5C, sp58, 0x280);
    M2C_FIELD(sp7C, s8 *, 0) = 0;
    M2C_FIELD(var_s0_2, s8 *, 4) = 0;
    M2C_FIELD(var_s0_2, s8 *, 9) = 0;
    if (M2C_FIELD(var_s0_2, s8 *, 1) == 0) {
        goto block_89;
    }
    if (M2C_FIELD(var_s0_2, s8 *, 5) != 0) {
        goto block_89;
    }
    M2C_FIELD(var_s0_2, s8 *, 3) = 1;
block_89:
    *sp54 = 1;
    goto block_91;
block_90:
    temp_t9 = sp1A8 + 1;
    sp1A8 = temp_t9;
    if (temp_t9 != 4) {
        goto loop_78;
    }
block_91:
    if (sp1A8 < 4) {
        goto block_116;
    }
    if (M2C_FIELD(var_s4, s8 *, 7) == 0) {
        goto block_94;
    }
    M2C_FIELD(sp7C, s8 *, 0) = 0;
    goto block_116;
block_94:
    if (M2C_FIELD(var_s4, s8 *, 0xA) == 0) {
        goto block_96;
    }
    M2C_FIELD(sp7C, s8 *, 0) = 0;
    goto block_116;
block_96:
    osJamMesg(&D_801497D0, 0, 0);
    if (track_collision_setup(var_s8, 0) != 0) {
        goto block_100;
    }
    M2C_FIELD(sp7C, s8 *, 0) = 0;
    if (D_8011194C != 0) {
        goto block_99;
    }
    D_8011194C = 1;
    osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
    osJamMesg(&D_801497D0, 0, 0);
block_99:
    osRecvMesg(&D_801497D0, &spA4, 1);
    goto block_116;
block_100:
    if (D_8011194C != 0) {
        goto block_102;
    }
    D_8011194C = 1;
    osCreateMesgQueue(&D_801497D0, &D_801527E4, 1);
    osJamMesg(&D_801497D0, 0, 0);
block_102:
    osRecvMesg(&D_801497D0, &sp94, 1);
    if (M2C_FIELD(var_s4, s8 *, 8) == 0) {
        goto block_105;
    }
    if (sp12E != 0) {
        goto block_105;
    }
    sp12F = 0;
    sp12E = 0;
    goto loop_27;
block_105:
    if (sp12E != 0) {
        goto block_116;
    }
    sp6C = var_s4 + 0x78;
    sp74 = var_s4;
    func_800A3640(var_s8);
    if (sp12F == 0) {
        goto block_108;
    }
    sp68 = sp78 + (u8 *) &D_80144D60;
    var_s1 = (sp1A8 * 0x10) + (u8 *) &D_80144D60;
    var_s0_2 = (sp1A8 * 0x304) + &D_80144030;
    sp54 = &(sp130)[sp1A8];
    sp5C = var_s4 + 0x84;
    sp58 = (u8 *) var_s0_2 + 0x84;
    goto block_84;
block_108:
    M2C_FIELD(var_s4, s8 *, 9) = 1;
    M2C_FIELD(var_s4, s8 *, 0xA) = 0;
    M2C_FIELD(var_s4, s8 *, 0xB) = 0;
    sp1A8 = 0;
    memcpy(sp70, &sp130[8], 0x68);
    osPfsFreeBlocks(sp70, sp6C);
    sp78 = var_s4 + 0x8C;
    var_s1_2 = (var_s8 * 0x304) + &D_80144030;
    sp7C = var_s4;
loop_109:
    temp_v0_2 = osPfsDeleteFile(sp70, sp1A8, sp78);
    if (temp_v0_2 == 0) {
        goto block_111;
    }
    M2C_FIELD(var_s1_2, s32 *, 0x8C) = 0;
block_111:
    M2C_FIELD(var_s1_2, s8 *, 0x84) = 0;
    M2C_FIELD(var_s1_2, s8 *, 0x85) = 0;
    M2C_FIELD(var_s1_2, s8 *, 0x86) = 0;
    M2C_FIELD(var_s1_2, s16 *, 0x88) = (s16) ((u32) (((u32) (M2C_FIELD(var_s1_2, s32 *, 0x8C) + 0x1F) >> 5) + 7) >> 3);
    M2C_FIELD(var_s4, s32 *, 0x74) = func_800A1E94(temp_v0_2);
    if (M2C_FIELD(sp7C, s32 *, 0x8C) == 0) {
        goto block_115;
    }
    sp60 = (u8 *) var_s1_2 + 0x96;
    sp64 = (u8 *) var_s1_2 + 0x9A;
    sp84 = D_801460E8;
    func_8009211C(&D_801460E0, D_801460E8);
    temp_s0_2 = *sp84;
    M2C_FIELD(temp_s0_2, s32 *, 8) = 0;
    M2C_FIELD(temp_s0_2, s32 *, 0xC) = 0;
    M2C_FIELD(temp_s0_2, s8 *, 0x10) = var_s8;
    M2C_FIELD(temp_s0_2, s8 *, 0x11) = (s8) sp1A8;
    temp_t9_2 = M2C_FIELD(var_s1_2, s32 *, 0x8C);
    M2C_FIELD(temp_s0_2, u32 *, 0x44) = (u32) ((u32) (temp_t9_2 + 0xFF) >> 8);
    M2C_FIELD(temp_s0_2, s32 *, 0x40) = temp_t9_2;
    sp128 = sp84;
    func_800A1644((u8 *) temp_s0_2 + 0x12, sp64, 0x10);
    func_800A1644((u8 *) temp_s0_2 + 0x35, sp60, 4);
    if (M2C_FIELD(temp_s0_2, u8 *, 0x35) == 0) {
        goto block_114;
    }
    M2C_FIELD(temp_s0_2, s8 *, 0x36) = 0;
block_114:
    M2C_FIELD(temp_s0_2, s32 *, 0x48) = 0;
    no_catchup();
block_115:
    temp_t6 = sp1A8 + 1;
    sp7C += 0x28;
    sp78 += 0x28;
    sp1A8 = temp_t6;
    var_s1_2 = (u8 *) var_s1_2 + 0x28;
    if (temp_t6 != 0x10) {
        goto loop_109;
    }
block_116:
    var_s8 += 1;
    if (var_s8 < 4) {
        goto loop_4;
    }
    osJamMesg(&D_801497D0, 0, 0);
    D_80144030 = sp130[0];
    D_80144334 = sp130[1];
    D_80144638 = sp130[2];
    D_8014493C = sp130[3];
block_118:
    return;
}

