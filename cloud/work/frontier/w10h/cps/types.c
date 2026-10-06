float fabsf(float);
#pragma intrinsic (fabsf)
extern s8 D_801174C4[];
typedef struct Poly {
    u16 type;
    u16 cnt;
    u8 body[0x12];
    u16 off;
} Poly;
typedef struct CCol {
    Poly *poly;
    f32 m[9];
    s16 idx;
    s16 edge;
    f32 k;
} CCol;
typedef struct CCar {
    u8 p0[628];
    f32 B628[4][3];
    f32 C676[4][3];
    u8 p724[24];
    f32 mat[9];
    u8 p784[816];
    s8 b1600;
    u8 p1601[131];
    s16 f6C4;
    u8 p1734[7];
    s8 b1741;
    u8 p1742[248];
    s16 pidx;
} CCar;
typedef struct PCar {
    u8 p0[0x358];
    s8 f358;
    s8 f359;
    u8 p35A[0x3B8 - 0x35A];
} PCar;
typedef struct Link {
    u8 p0[7];
    u8 kind;
} Link;
extern Link D_80153E88[];
void func_800C36A0(CCar *car, CCol *col);
void func_803914B4(s8 a, s8 b, s8 c, s8 d);
void camera_play_script(CCar *car, Poly *poly, CCol *col);

#define DECODE(v, e) \
    (v)[0] = (f32) (((e)->x << 5) + (((e)->w & 0x7C00) >> 10)) * 0.03125f; \
    (v)[1] = (f32) (((e)->y << 5) + (((e)->w & 0x3E0) >> 5)) * 0.03125f; \
    (v)[2] = (f32) (((e)->z << 5) + ((e)->w & 0x1F)) * 0.03125f

