/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Context copy (not a claim): the locked kept entity-flags getter func_8008AE64 (src/blob/func_8008AE64.c,
 * same expression). transmission_ratio_get inlines it; the group needs its definition for umerge. */
typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;
extern u8 D_8012E700[];

s32 func_8008AE64(s16 arg0) {
    return *((s32 *) ((u8 *) &D_8012E700 + (arg0 * 0x44)));
}
