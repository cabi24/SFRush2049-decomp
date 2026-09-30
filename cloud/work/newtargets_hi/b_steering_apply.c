typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct { u8 p[2056]; } Big;
typedef struct { u8 p[8]; float x, y, z; u8 q[952 - 20]; } B952;
extern u8 D_8014AA3A[];
extern B952 D_80152818[];
s32 steering_apply(s16 *idx, float *pos, float *rad, float *out) {
    float d[3];
    float p[3];
    float r;
    float dist;
    s32 i = *idx;
    if (((s8 *)D_8014AA3A)[i * 2056] == 0) return 0;
    r = *rad;
    p[0] = pos[0];
    p[1] = pos[1];
    p[2] = pos[2];
    d[0] = D_80152818[i].x - p[0];
    d[1] = D_80152818[i].y - p[1];
    d[2] = D_80152818[i].z - p[2];
    r += 3.5f;
    dist = d[0] * d[0] + d[1] * d[1] + d[2] * d[2];
    if (out) *out = dist - r * r;
    if (dist - r * r <= 0.0f) return 1;
    return 0;
}
