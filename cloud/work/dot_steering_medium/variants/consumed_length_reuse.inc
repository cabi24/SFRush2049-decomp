void steering_sensitivity(s32 arg0, u16 idx, f32 *position, f32 *outPosition, f32 (*outMatrix)[3], f32 threshold) {
    PathRec *r0;
    PathRec *r1;
    f32 z;
    f32 t;
    f32 width;
    f32 height;
    f32 x;
    f32 y;
    f32 distance;
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
    t = z / t;
    x = rel[0] + r0->skew0;
    width = r0->len0 + (r1->len0 - r0->len0) * t;
    height = r0->halfWidth + (r1->halfWidth - r0->halfWidth) * t - width - 2.5f;
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
