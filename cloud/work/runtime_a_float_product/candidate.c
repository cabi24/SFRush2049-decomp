/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Image A, 0x8039D300: update four selected car-stat bar products.
 * N64 front-end adaptation; related arcade consumer: game/select.c:AnimateBar.
 * Five selected vectors, preserving the native three stores per component.
 */
typedef signed char s8;
typedef signed short s16;
typedef int s32;

extern s8 D_803B9FD0[];
extern s8 D_80110E85[][13];
extern s8 D_80111049[][13];
extern s8 D_8011108D[][13];
extern s8 D_801111A9[][13];
extern s8 D_8011123D[][13];
extern float D_803B28C8[][4];
extern float D_803B2948[][4];
extern float D_803B2978[][4];
extern float D_803B2A08[][4];
extern float D_803B2A58[][4];
extern float D_803BA190[][4];

void func_8039D300(s32 player)
{
    float *dest;
    float *first;
    float *second;
    float *third;
    float *fourth;
    float *fifth;
    s16 component;

    dest = D_803BA190[player];
    first = D_803B28C8[D_80110E85[player][D_803B9FD0[player]]];
    second = D_803B2948[D_80111049[player][D_803B9FD0[player]]];
    third = D_803B2978[D_8011108D[player][D_803B9FD0[player]]];
    fourth = D_803B2A08[D_801111A9[player][D_803B9FD0[player]]];
    fifth = D_803B2A58[D_8011123D[player][D_803B9FD0[player]]];
    for (component = 0; component < 4; component++) {
        dest[component] = (first[component] * second[component]) * third[component];
        dest[component] *= fourth[component];
        dest[component] *= fifth[component];
    }
}
