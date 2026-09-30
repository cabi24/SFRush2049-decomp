typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef struct { s16 x, y, z; } V3s;
typedef struct { u16 count; u16 pad; V3s *pts; } Graph;
extern Graph D_801407F0;
extern float D_80124570;
s16 world_bounds_check(V3s *p) {
    float best = D_80124570;
    s32 pad[5];
    s16 bi;
    s32 i;
    for (i = 0; i < D_801407F0.count; i++) {
        float dx = D_801407F0.pts[i].x - p->x;
        float dy = D_801407F0.pts[i].y - p->y;
        float dz = D_801407F0.pts[i].z - p->z;
        float d = dx * dx + dy * dy + dz * dz;
        if (d < best) {
            best = d;
            bi = i;
        }
    }
    return bi;
}
