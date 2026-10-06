typedef float f32;
typedef unsigned char u8;
typedef unsigned int u32;

extern int D_8011735C;
extern u32 D_80123418[];

static int rand(void)
{
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}

static f32 Random(f32 max)
{
    f32 rannum;

    rannum = ((f32)(rand() & 0x07FFF) * max) / 32768.0f;
    return rannum;
}

u8 func_800B23E0(u8 index)
{
    u8 bit;

    do {
        bit = Random(32.0f);
    } while (!(D_80123418[index] & (1 << bit)));
    return bit;
}
