typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define NULL ((void *)0)
float fabsf(float);
#pragma intrinsic (fabsf)

typedef struct QNode {
    s16 next;
    u8 pad2;
    u8 mask;
    s16 x0;
    s16 x1;
    s16 y0;
    s16 y1;
    u16 child[4];
} QNode;
extern QNode *D_80124EEC;

typedef struct Poly {
    u16 type;
    u16 cnt;
    u8 body[0x12];
    u16 off;
} Poly;
extern Poly *D_801497F8;
extern u8 *D_80152460;

void *handbrake_apply(QNode *n, s16 x, s16 y, s16 *out);
void func_800ADCE0(u8 *input, s32 count, u16 *front, u32 marker);
s16 func_800C3AD0(Poly *poly, f32 *wp, f32 *q, f32 *pt, f32 *bound, s16 *outIdx, f32 *mat, f32 zmin);
void math_utility(void *, void *);
void steering_sensitivity(s32 arg0, u16 idx, f32 *position, f32 *outPosition, f32 (*outMatrix)[3], f32 threshold);
void traction_control(s32 arg0, u16 idx, f32 *pos, f32 *outPos, f32 (*outMat)[3]);
f32 func_8008E0B8(f32 *v);

typedef struct EUCar {
    u8 pad0[928];
    QNode *node[4];
    u8 pad944[1440 - 944];
    u16 hint[4];
    u8 pad1448[1564 - 1448];
    s16 kind[4];
    s16 slope[4];
    u16 attr[4];
    u8 pad1588[1600 - 1588];
    u8 b1600;
    u8 pad1601[7];
    Poly *p1608;
    u8 pad1612[1732 - 1612];
    s16 s1732;
    u8 pad1734[1741 - 1734];
    u8 b1741;
    u8 pad1742[1990 - 1742];
    s16 id;
} EUCar;
typedef struct EUSlot {
    f32 dr_pos[3];          /* 0x000 arcade CAR_DATA dr_pos */
    u8 pad00C[856 - 12];
    s8 b856;
    s8 b857;                /* 0x359 state (battle_mode_setup) */
    u8 pad35A;
    s8 place;               /* 0x35B (func_800EC914) */
    u8 pad35C[952 - 860];
} EUSlot;
extern EUSlot D_80152818[];
extern s8 D_8010FFC0;
extern f32 D_8011418C[];
void effect_cleanup(s8 a, s8 b, s8 c);
void func_800C54F0(s16 arg0, s32 arg1);
u32 entity_flags_apply(u32 index, u32 other, u32 value, u8 mode);
s16 input_process_controller(f32 *p1, f32 *p2, f32 *out, Poly *poly, s16 *outIdx, s32 flag, f32 *vcOut, f32 *mat, f32 rad2);
s32 func_800B61A8(s32 arg0, s32 arg1, s32 arg2, unsigned char arg3);
