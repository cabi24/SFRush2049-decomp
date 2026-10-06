typedef float f32;
typedef short s16;
typedef unsigned char u8;

extern int D_801174B4;
extern int D_8014A110;
extern s16 D_8014A108;
extern f32 D_80149A78[][8];
extern u8 D_80144018[];
extern f32 D_80144DA8[];

void func_800D2054(int player, f32 time)
{
    int i;
    u8 n;
    f32 *t;

    if (!(D_801174B4 & 8) && D_8014A110 != 1 && player < D_8014A108) {
        n = D_80144018[player];
        t = &D_80149A78[player][n];
        *t = time;
        for (i = 0; i < n; i++) {
            *t -= D_80149A78[player][i];
        }
        if (*t < D_80144DA8[player]) {
            D_80144DA8[player] = *t;
        }
        D_80144018[player] = n + 1;
    }
}
