typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef struct { float x, y, z; } V3;
typedef struct {
    u8 p0[8]; V3 pos; u8 p1[264 - 20]; float f264; u8 p2[952 - 268];
} B;
extern float D_80152018;
extern u8 D_80152031;
extern s8 D_80152744;
extern V3 D_80152218[];
extern B D_80152818[];
void func_800FBE60(void) {
    V3 *p;
    B *q;
    s32 n = D_80152744;
    D_80152018 = 0.0f;
    D_80152031 = 0;
    q = D_80152818;
    for (p = D_80152218; p < &D_80152218[n]; p++) {
        p->x = q->pos.x;
        p->y = q->pos.y;
        p->z = q->pos.z;
        q->f264 = 0.0f;
        q++;
    }
}
