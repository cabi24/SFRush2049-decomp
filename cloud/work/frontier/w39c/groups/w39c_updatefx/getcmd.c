/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_80091B00: allocate a free 24-byte slot from D_80142DD8[128]: the first
 * slot whose in-use byte (+3) is 0 is marked used, its id (+0) set to -1 and
 * returned; NULL when the table is full.  IPA-internal in the game image (23
 * callers; the four-wide t6..t9 temp ring only appears as an internal -O3
 * procedure), so it is scored in a group with real, locked callers.
 *
 * Shaping: the slot table is volatile.  as1 then keeps the two stores of each
 * unrolled copy in source order (`sb 1,3(v1); li -1; sh 0(v1)`), the last
 * 8-word residual of earlier attempts (proved on the ugen listing first:
 * marking just the sb and sh `.set volatile` gives retail's schedule).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef struct Slot18 { s16 f0; s8 f2; s8 f3; u8 pad[0x14]; } Slot18;
extern volatile Slot18 D_80142DD8[128];
Slot18 *func_80091B00(void)
{
    s32 i;
    for (i = 0; i < 128; i++) {
        if (D_80142DD8[i].f3 == 0) {
            D_80142DD8[i].f3 = 1;
            D_80142DD8[i].f0 = -1;
            return (Slot18 *) &D_80142DD8[i];
        }
    }
    return 0;
}
