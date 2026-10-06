/* Bounded host harness; synthetic backing is deliberately explicit. */
#include <string.h>
#include "candidate.c"
s8 D_803B9FD0[4];
s8 D_80110E85[4][13], D_80111049[4][13], D_8011108D[4][13];
s8 D_801111A9[4][13], D_8011123D[4][13];
float D_803B28C8[3][4], D_803B2948[3][4], D_803B2978[3][4];
float D_803B2A08[3][4], D_803B2A58[3][4], D_803BA190[4][4];
void host_run(int player, int selector, const int *rows,
              const unsigned int *input, unsigned int *output)
{
    int i;
    memset(D_803BA190, 0xA5, sizeof D_803BA190);
    for (i = 0; i < 4; i++) D_803B9FD0[i] = 0;
    memset(D_80110E85, 0xA5, sizeof D_80110E85);
    memset(D_80111049, 0xA5, sizeof D_80111049);
    memset(D_8011108D, 0xA5, sizeof D_8011108D);
    memset(D_801111A9, 0xA5, sizeof D_801111A9);
    memset(D_8011123D, 0xA5, sizeof D_8011123D);
    memset(D_803B28C8, 0xA5, sizeof D_803B28C8);
    memset(D_803B2948, 0xA5, sizeof D_803B2948);
    memset(D_803B2978, 0xA5, sizeof D_803B2978);
    memset(D_803B2A08, 0xA5, sizeof D_803B2A08);
    memset(D_803B2A58, 0xA5, sizeof D_803B2A58);
    D_803B9FD0[player] = selector;
    D_80110E85[player][selector] = rows[0];
    D_80111049[player][selector] = rows[1];
    D_8011108D[player][selector] = rows[2];
    D_801111A9[player][selector] = rows[3];
    D_8011123D[player][selector] = rows[4];
    memcpy(D_803B28C8[rows[0]], input, 16);
    memcpy(D_803B2948[rows[1]], input + 4, 16);
    memcpy(D_803B2978[rows[2]], input + 8, 16);
    memcpy(D_803B2A08[rows[3]], input + 12, 16);
    memcpy(D_803B2A58[rows[4]], input + 16, 16);
    func_8039D300(player);
    memcpy(output, D_803BA190, sizeof D_803BA190);
}
