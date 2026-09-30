typedef signed int s32;
typedef struct {
    float f[32];
} Obj;
void func_800E95DC(s32 a, Obj *o, float *pos, float *out);
void func_800FAD50(Obj *o) {
    float pos[3];
    float out[9];
    pos[0] = o->f[2] + o->f[17] * 80.0f + o->f[11] * 20.0f;
    pos[1] = o->f[3] + 5.0f;
    pos[2] = o->f[4] + o->f[19] * 80.0f + o->f[13] * 20.0f;
    func_800E95DC(0, o, pos, out);
}
