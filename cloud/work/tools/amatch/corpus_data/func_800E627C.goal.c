void func_800E627C(CarState *st, InputRecord *in)
{
    f32 x;
    f32 q;
    f32 d;
    f32 prev;

    if (D_8013FECB != 0) {
        st->steer = 0.0f;
        return;
    }
    x = D_80140620[in->pad][0];
    prev = st->steer;
    if (in->steerSrc == 0x19) {
        if (x < D_80124498) {
            f32 c = D_8012449C;
            x += c;
        } else if (D_801244A0 < x) {
            x -= D_801244A0;
        } else {
            x = 0.0;
        }
    } else {
        x = (x >= 0.0f ? x : -x) * (x >= 0.0f ? x : -x) * x;
    }
    q = x * 127.0f;
    x = (f32) (s32) (q < 0.0f ? q - 0.5f : q + 0.5f) / 127;
    d = x - prev;
    if (D_801244A4 < d) {
        prev = x;
    } else if (d < D_801244A8) {
        prev = x;
    }
    if ((D_80151AD8 != 0 || D_80140A04 != 0) &&
        (D_80151AD8 == 0 || D_80140A04 == 0 || D_8013F1D9 != 0)) {
        prev = -prev;
    }
    st->steer = prev;
}