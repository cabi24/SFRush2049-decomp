/* Test driver includes the unchanged candidate; only valid array indices are used. */
#include <stdio.h>
#include "candidate.c"
s8 D_801407D0, D_80156994, D_8014978C;
s32 D_803AF980, D_8014A110;
s16 D_801164BE;
s8 D_803B9FD0[4], D_803B3020[128];
int main(void)
{
    int item, player, unlock, network, track, active, mode, setting, selector, bits, p;
    while (scanf("%d%d%d%d%d%d%d%d%d%d", &item, &player, &unlock, &network,
                 &track, &active, &mode, &setting, &selector, &bits) == 10) {
        if (player < 0 || player > 3 || selector < 0 || selector > 127) return 2;
        D_801407D0 = unlock; D_80156994 = network; D_8014978C = track;
        D_803AF980 = active; D_8014A110 = mode; D_801164BE = setting;
        for (p=0; p<128; ++p) D_803B3020[p] = -91;
        for (p=0; p<4; ++p) D_803B9FD0[p] = (selector+37*(p-player)+256)%128;
        D_803B9FD0[player] = selector; D_803B3020[selector] = bits;
        printf("%d\n", func_8039D494(item, player));
    }
    return 0;
}
