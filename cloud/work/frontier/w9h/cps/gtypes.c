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
void func_803914B4(s8 a, s8 b, s8 c, s8 d);
void camera_play_script(CCar *car, Poly *poly, CCol *col);
