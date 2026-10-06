/*
 * func_800C885C -- free the two optional allocations held in D_8011025C and
 * D_80110260 and clear them. Same-shape sibling of wheel_params_set: each
 * free is an inlined call of the deleted internal heap-free helper
 * func_800960CC (defined in wheel_params_set.c; lock, audio_reverb_update
 * (address, 0), unlock), 24 bytes of frame per instance (homes 56/32).
 */
typedef unsigned int u32;

extern u32 D_8011025C, D_80110260;
void func_800960CC(u32 address);

void func_800C885C(void)
{
    if (D_8011025C) {
        func_800960CC(D_8011025C);
        D_8011025C = 0;
    }
    if (D_80110260) {
        func_800960CC(D_80110260);
        D_80110260 = 0;
    }
}
