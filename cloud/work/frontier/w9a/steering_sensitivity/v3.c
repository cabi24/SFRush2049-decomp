void steering_sensitivity(s32 arg0, u16 idx, f32 *position, f32 *outPosition, f32 (*outMatrix)[3], f32 threshold) {
    PathRec *r0;
    PathRec *r1;
    f32 height;
    f32 frac;
    f32 dist;
    f32 width;
    f32 x;
    f32 y;
    f32 z;
    f32 t;
    f32 rel[3];

    r0 = &D_80152034[idx];
    if (idx + 1 < *D_801526F0) {
        r1 = r0 + 1;
    } else {
        r1 = D_80152034;
    }
    math_utility(r0->uvs, outMatrix);
    vector_diff_process(r0->pos, r0->uvs[0], position, rel);
    t = r0->skew1;
    z = rel[2];
    if (rel[2] < 0.0f) {
        z = 0.0f;
    } else if (t < rel[2]) {
        z = t;
    }
    frac = z / t;
    x = rel[0] + r0->skew0;
    width = r0->len0 + (r1->len0 - r0->len0) * frac;
    height = r0->halfWidth + (r1->halfWidth - r0->halfWidth) * frac - width - 2.5f;
    y = rel[1];
    if (outMatrix[1][1] < 0.0f) {
        func_800AD090(0.0f, -1.0f, outMatrix);
        x = -x;
        y = height - y + 5.0f;
    } else {
        y = rel[1] - height;
    }
    if (x < -width) {
        x += width;
    } else if (width < x) {
        x -= width;
    } else {
        if (0.0f < y) {
            outPosition[1] = height - y;
            func_800AD090(0.0f, -1.0f, outMatrix);
        } else {
            outPosition[1] = height + y;
        }
        return;
    }
    dist = sqrtf(x * x + y * y);
    if (dist < 0.01f) {
        outPosition[1] += height;
        return;
    }
    outPosition[1] = height - dist;
    if (outPosition[1] < threshold) {
        width = r1->len0 - r0->len0;
        height = r1->halfWidth - r1->len0 - r0->halfWidth + r0->len0;
        if (fabsf(height) > 1.0f) {
            z = sqrtf(height * height + r0->skew1 * r0->skew1);
            func_800ACFF8(-height / z, r0->skew1 / z, outMatrix);
        }
        func_800AD090(-x / dist, -y / dist, outMatrix);
        if (fabsf(width) > 1.0f) {
            width = fabsf(x) * width / dist;
            frac = sqrtf(width * width + r0->skew1 * r0->skew1);
            func_800ACFF8(-width / frac, r0->skew1 / frac, outMatrix);
        }
        if (fabsf(height) > 1.0f) {
            func_800ACFF8(height / z, r0->skew1 / z, outMatrix);
        }
    }
}

