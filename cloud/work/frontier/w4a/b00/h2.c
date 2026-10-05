/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct Slot18 { s16 f0; s8 f2; s8 f3; u8 pad[0x14]; } Slot18;
extern Slot18 D_80142DD8[128];
void func_80091AF8(Slot18 *s)
{
    s->f0 = -1;
}

Slot18 *func_80091B00(void)
{
    s32 i;
    for (i = 0; i < 128; i++) {
        if (D_80142DD8[i].f3 == 0) {
            D_80142DD8[i].f3 = 1;
            func_80091AF8(&D_80142DD8[i]);
            return &D_80142DD8[i];
        }
    }
    return 0;
}
