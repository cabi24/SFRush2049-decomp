/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Context: the two pak-channel getters updateControlScreen inlines, byte-identical function text from
 * src/blob/groups/menu_save_options/group.c (locked, kept there): func_800A1A3C = PS_PakInserted,
 * func_80094FC4 = PS_PakRumble. Present so the standalone group compile can inline them as the unit does. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef struct PakChannel {
    s8 present;                /* 0x00 */
    s8 state;                  /* 0x01 */
    u8 unk02[4];
    s8 busy;                   /* 0x06 */
    u8 unk07[0x304 - 7];
} PakChannel;
extern PakChannel D_80144030[];

s8 func_800A1A3C(s32 pak) {
    return D_80144030[pak].state;
}

s8 func_80094FC4(s32 pak) {
    return D_80144030[pak].busy;
}
