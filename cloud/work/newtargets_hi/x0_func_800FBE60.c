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
    s32 i;
    D_80152018 = 0.0f;
    D_80152031 = 0;
    for (i = 0; i < D_80152744; i++) {
        D_80152218[i].x = D_80152818[i].pos.x;
        D_80152218[i].y = D_80152818[i].pos.y;
        D_80152218[i].z = D_80152818[i].pos.z;
        D_80152818[i].f264 = 0.0f;
    }
}
