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
    f32 heightChange;
    f32 widthChange;
    f32 absHeightChange;
    f32 slopeLength;
    f32 length;
    f32 rel[3];

    r0 = &D_80152034[idx];
    if (idx + 1 < *D_801526F0) r1 = r0 + 1;
    else r1 = D_80152034;
    math_utility(r0->uvs, outMatrix);
    vector_diff_process(r0->pos, r0->uvs[0], position, rel);
    z = rel[2];
    if (z < 0.0f) z = 0.0f;
    else if (r0->skew1 < z) z = r0->skew1;
    t = z / r0->skew1;
    width = r0->len0 + (r1->len0 - r0->len0) * t;
    height = r0->halfWidth + (r1->halfWidth - r0->halfWidth) * t - width - 2.5f;
    x = rel[0] + r0->skew0;
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
        widthChange = r1->len0 - r0->len0;
        heightChange = (r1->halfWidth - r1->len0) - r0->halfWidth + r0->len0;
        absHeightChange = fabsf(heightChange);
        if (absHeightChange > 1.0f) {
            length = r0->skew1;
            slopeLength = sqrtf(heightChange * heightChange + length * length);
            func_800ACFF8(-heightChange / slopeLength, length / slopeLength, outMatrix);
        }
        func_800AD090(-x / distance, -y / distance, outMatrix);
        if (fabsf(widthChange) > 1.0f) {
            length = r0->skew1;
            widthChange = fabsf(x) * widthChange / distance;
            length = sqrtf(widthChange * widthChange + length * length);
            func_800ACFF8(-widthChange / length, r0->skew1 / length, outMatrix);
        }
        if (absHeightChange > 1.0f) {
            func_800ACFF8(heightChange / slopeLength, r0->skew1 / slopeLength, outMatrix);
        }
    }
}
