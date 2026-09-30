typedef signed int s32;
typedef signed short s16;

typedef struct { s32 a; s32 b; } Pair;
typedef struct { float f[16]; } Rec;     /* 64 bytes */

extern s32 D_80116D0C;
extern Rec D_80116514[];                 /* float at +0 */
extern float D_801248D4;
extern s32 D_801146BC, D_801146C0;
extern s32 D_80118E28, D_80118E2C;
extern s16 D_80116D9C;
extern void dispatch_handler(s32);
extern void state_utility(s32, s32, s32);

void func_8010A7A4(s32 idx, s32 x, s32 y, s32 p3, s32 p4)
{
    s16 yy;
    volatile s32 m1, m2, l1, l2;
    if (D_80116D0C == 1) {
        if (D_80116514[idx].f[0] < D_801248D4) {
            s32 v1 = D_801146BC, v2 = D_801146C0;
            D_80118E28 = v1; l1 = v1;
            D_80118E2C = v2; l2 = v2;
            dispatch_handler(1);
        } else {
            s32 v1 = D_801146BC, v2 = D_801146C0;
            D_80118E28 = v1; m1 = v1;
            D_80118E2C = v2; m2 = v2;
            dispatch_handler(22);
        }
    } else {
        if (idx == D_80116D9C) dispatch_handler(22);
        else dispatch_handler(1);
    }
    yy = y;
    state_utility((s16)(x - 105), yy, p3);
    state_utility((s16)(x + 40), yy, p4);
}

/* stand-in callers (real: func_8010A8D0, func_8010AEAC) */
extern s32 G1[], G2[];
void caller_a(s32 i, s32 y) { func_8010A7A4(1, 160, y, G1[i], G2[i]); func_8010A7A4(2, 160, y, G1[i+1], G2[i+1]); }
void caller_b(s32 i, s32 y, s32 x) { func_8010A7A4(0, x, y, G1[i], G2[i]); }
