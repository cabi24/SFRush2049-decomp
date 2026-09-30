f32 func_800C9590(f32 range, f32 calib, s32 raw)
{
    f32 v;

    v = ((f32) raw * range) / calib;
    if (v < -range) {
        v = -range;
    } else if (range < v) {
        v = range;
    }
    return v;
}