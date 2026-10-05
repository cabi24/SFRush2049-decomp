/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research-only natural steering reconstruction. No matched claims. */
typedef signed char s8;
typedef unsigned char u8;
typedef unsigned short u16;
typedef int s32;
typedef float f32;
#define M2C_FIELD(expr, type_ptr, offset) (*(type_ptr)((s8 *)(expr) + (offset)))
typedef struct {
    f32 pos[3];
    f32 uvs[3][3];
    f32 right[3];
    f32 fwdL[3];
    f32 fwdR[3];
    f32 halfWidth, len0, skew0, skew1;
    f32 wL0, wL1, wR0, wR1, hL, hR;
    f32 pad7C[2];
} PathRec;
extern PathRec *D_80152034;
extern u16 *D_801526F0;
extern f32 D_80123BFC;
void func_800A61B0(f32 *, f32 *, f32 *);
void math_utility(void *, void *);
void func_800ACBC4(f32 *, f32 *, f32, f32 *);
void func_800ACFF8(f32, f32, void *);
void func_800AD090(f32, f32, void *);
f32 sqrtf(f32);
f32 fabsf(f32);
#pragma intrinsic (sqrtf)
#pragma intrinsic (fabsf)

void vector_diff_process(f32 *origin, f32 *basis, f32 *position, f32 *out) {
    f32 delta[3];
    delta[0] = position[0] - origin[0];
    delta[1] = position[1] - origin[1];
    delta[2] = position[2] - origin[2];
    func_800A61B0(delta, out, basis);
}

void steering_sensitivity(s32 arg0, u16 idx, f32 *position, f32 *outPosition, f32 (*outMatrix)[3], f32 threshold) {
    PathRec *r0;
    PathRec *r1;
    f32 height;
    f32 t;
    f32 distance;
    f32 width;
    f32 x;
    f32 y;
    f32 z;
    f32 absHeightChange;
    f32 rel[3];

    r0 = &D_80152034[idx];
    if (idx + 1 < *D_801526F0) r1 = r0 + 1;
    else r1 = D_80152034;
    math_utility(r0->uvs, outMatrix);
    vector_diff_process(r0->pos, r0->uvs[0], position, rel);
    t = r0->skew1;
    z = rel[2];
    if (rel[2] < 0.0f) z = 0.0f;
    else if (t < rel[2]) z = t;
    x = rel[0] + r0->skew0;
    width = r0->len0 + (r1->len0 - r0->len0) * (z / t);
    height = r0->halfWidth + (r1->halfWidth - r0->halfWidth) * (z / t) - width - 2.5f;
    y = rel[1];
    if (outMatrix[1][1] < 0.0f) {
        func_800AD090(0.0f, -1.0f, outMatrix);
        x = -x;
        y = height - y + 5.0f;
    } else y -= height;
    if (x < -width) x += width;
    else if (x > width) x -= width;
    else {
        if (y > 0.0f) {
            outPosition[1] = height - y;
            func_800AD090(0.0f, -1.0f, outMatrix);
        } else outPosition[1] = height + y;
        return;
    }
    distance = sqrtf(x * x + y * y);
    if (distance < D_80123BFC) {
        outPosition[1] += height;
        return;
    }
    outPosition[1] = height - distance;
    if (outPosition[1] < threshold) {
        width = r1->len0 - r0->len0;
        height = (r1->halfWidth - r1->len0) - r0->halfWidth + r0->len0;
        absHeightChange = fabsf(height);
        if (absHeightChange > 1.0f) {
            t = r0->skew1;
            z = sqrtf(height * height + t * t);
            func_800ACFF8(-height / z, t / z, outMatrix);
        }
        func_800AD090(-x / distance, -y / distance, outMatrix);
        if (fabsf(width) > 1.0f) {
            t = r0->skew1;
            width = fabsf(x) * width / distance;
            distance = sqrtf(width * width + t * t);
            func_800ACFF8(-width / distance, t / distance, outMatrix);
        }
        if (absHeightChange > 1.0f) {
            func_800ACFF8(height / z, r0->skew1 / z, outMatrix);
        }
    }
}

void func_800A61B0(f32 *arg0, f32 *arg1, f32 *arg2) {
    arg1[0] = (arg0[0] * arg2[0] + arg0[1] * arg2[1]) + arg0[2] * arg2[2];
    arg1[1] = (arg0[0] * arg2[3] + arg0[1] * arg2[4]) + arg0[2] * arg2[5];
    arg1[2] = (arg0[0] * arg2[6] + arg0[1] * arg2[7]) + arg0[2] * arg2[8];
}

void math_utility(void *arg0, void *arg1) {
    M2C_FIELD(arg1, f32 *, 0) = (f32) M2C_FIELD(arg0, f32 *, 0);
    M2C_FIELD(arg1, f32 *, 4) = (f32) M2C_FIELD(arg0, f32 *, 4);
    M2C_FIELD(arg1, f32 *, 8) = (f32) M2C_FIELD(arg0, f32 *, 8);
    M2C_FIELD(arg1, f32 *, 0xC) = (f32) M2C_FIELD(arg0, f32 *, 0xC);
    M2C_FIELD(arg1, f32 *, 0x10) = (f32) M2C_FIELD(arg0, f32 *, 0x10);
    M2C_FIELD(arg1, f32 *, 0x14) = (f32) M2C_FIELD(arg0, f32 *, 0x14);
    M2C_FIELD(arg1, f32 *, 0x18) = (f32) M2C_FIELD(arg0, f32 *, 0x18);
    M2C_FIELD(arg1, f32 *, 0x1C) = (f32) M2C_FIELD(arg0, f32 *, 0x1C);
    M2C_FIELD(arg1, f32 *, 0x20) = (f32) M2C_FIELD(arg0, f32 *, 0x20);
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
    up[0] = fwd[1] * right[2] - fwd[2] * right[1];
    up[1] = fwd[2] * right[0] - fwd[0] * right[2];
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
