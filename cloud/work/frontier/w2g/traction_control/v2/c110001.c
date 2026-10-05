typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    /* 0x00 */ f32 pos[3];
    /* 0x0C */ f32 uvs[3][3];
    /* 0x30 */ f32 right[3];
    /* 0x3C */ f32 fwdL[3];
    /* 0x48 */ f32 fwdR[3];
    /* 0x54 */ f32 halfWidth;
    /* 0x58 */ f32 len0;
    /* 0x5C */ f32 skew0;
    /* 0x60 */ f32 skew1;
    /* 0x64 */ f32 wL0;
    /* 0x68 */ f32 wL1;
    /* 0x6C */ f32 wR0;
    /* 0x70 */ f32 wR1;
    /* 0x74 */ f32 hL;
    /* 0x78 */ f32 hR;
    /* 0x7C */ f32 pad7C[2];
} PathRec; /* 0x84 */

extern PathRec *D_80152034;
extern u16 *D_801526F0;

void func_800ACBC4(f32 *a, f32 *b, f32 t, f32 *out);

void func_800A61B0(f32 *arg0, f32 *arg1, f32 *arg2) {
    arg1[0] = (arg0[0] * arg2[0] + arg0[1] * arg2[1]) + arg0[2] * arg2[2];
    arg1[1] = (arg0[0] * arg2[3] + arg0[1] * arg2[4]) + arg0[2] * arg2[5];
    arg1[2] = (arg0[0] * arg2[6] + arg0[1] * arg2[7]) + arg0[2] * arg2[8];
}

void vector_diff_process(f32 *origin, f32 *basis, f32 *position, f32 *out) {
    f32 delta[3];
    delta[0] = position[0] - origin[0];
    delta[1] = position[1] - origin[1];
    delta[2] = position[2] - origin[2];
    func_800A61B0(delta, out, basis);
}

void traction_control(s32 arg0, u16 idx, f32 *pos, f32 *outPos, f32 (*outMat)[3]) {
    s32 next;
    PathRec *r0;
    PathRec *r1;
    f32 u;
    f32 v;
    s32 i;
    f32 fwd0[3];
    f32 fwd1[3];
    f32 right[3];
    f32 up[3];
    f32 fwd[3];
    f32 p0[3];
    f32 p1[3];
    f32 org[3];
    f32 h0;
    f32 h1;
    f32 h;
    f32 len;
    f32 uu;
    f32 off;
    f32 rel[3];

    next = idx + 1 < *D_801526F0 ? idx + 1 : 0;
    r0 = &D_80152034[idx];
    r1 = &D_80152034[next];
    vector_diff_process(r0->pos, r0->uvs[0], pos, rel);
    len = rel[0] * (r0->skew1 - r0->skew0) + r0->len0;
    u = (rel[2] - rel[0] * r0->skew0) / len;
    if (rel[0] > 0.0f) {
        v = rel[0] / (rel[2] * r0->wR1 + r0->wR0) * 0.5f + 0.5f;
    } else {
        v = rel[0] / (rel[2] * r0->wL1 + r0->wL0) * -0.5f + 0.5f;
    }
    func_800ACBC4(r0->fwdL, r0->fwdR, v, fwd0);
    func_800ACBC4(r1->fwdL, r1->fwdR, v, fwd1);
    func_800ACBC4(fwd0, fwd1, u, fwd);
    func_800ACBC4(r0->right, r1->right, u, right);
    up[0] = right[2] * fwd[1] - fwd[2] * right[1];
    up[1] = fwd[2] * right[0] - right[2] * fwd[0];
    up[2] = fwd[0] * right[1] - fwd[1] * right[0];
    for (i = 0; i < 3; i++) {
        outMat[0][i] = right[i];
        outMat[1][i] = up[i];
        outMat[2][i] = fwd[i];
    }
    for (i = 0; i < 3; i++) {
        p0[i] = r0->pos[i] + r0->right[i] * ((v - 0.5f) * r0->halfWidth);
    }
    for (i = 0; i < 3; i++) {
        p1[i] = r1->pos[i] + r1->right[i] * ((v - 0.5f) * r1->halfWidth);
    }
    func_800ACBC4(p0, p1, u, org);
    h0 = r0->hL + (r0->hR - r0->hL) * v;
    h1 = -(r1->hL + (r1->hR - r1->hL) * v);
    uu = u * u;
    off = (u * uu * (h0 + h1) - uu * (h0 + h0 + h1) + u * h0) * len;
    for (i = 0; i < 3; i++) {
        org[i] -= off * outMat[1][i];
    }
    vector_diff_process(org, outMat[0], pos, outPos);
}
