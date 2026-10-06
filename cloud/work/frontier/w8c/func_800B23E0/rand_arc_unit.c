typedef float f32;

int func_8008B2B4(void);

f32 func_8008B2E4(f32 max)
{
    f32 rannum;

    rannum = ((f32)(func_8008B2B4() & 0x07FFF) * max) / 32768.0f;
    return rannum;
}
