/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * Model-table loader (N64-only): takes the data block of resource slot D_80140AF0 and fills the
 * table header D_8017A4E0 from its six packed sections (u32 count followed by the items; u16 sections
 * are padded to an even count), then relocates the three pointer sections by the block address.
 *
 * `slot_value_get(index)` (src/blob/groups/slot_sound) is inlined by umerge: that is the jal to
 * func_80096288 with the index kept in a3 and the `D_80156D38[index].data` load. The function therefore
 * only matches in the slot_sound group (or the whole-program unit); no quirks.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct {
    /* 0x00 */ u16 count0;
    /* 0x04 */ u32 *ptrs0;
    /* 0x08 */ u16 count1;
    /* 0x0A */ u16 count2;
    /* 0x0C */ u16 *data1;
    /* 0x10 */ u32 *ptrs2;
    /* 0x14 */ u16 count3;
    /* 0x16 */ u16 count4;
    /* 0x18 */ u16 count5;
    /* 0x1C */ u16 *data3;
    /* 0x20 */ u16 *data4;
    /* 0x24 */ u32 *ptrs5;
} ModelTables;

extern s32 D_80140AF0;
extern ModelTables D_8017A4E0;

void *slot_value_get(s32 index);

void func_800A4E58(void) {
    u32 *base;
    u32 *p;
    s32 i;

    base = slot_value_get(D_80140AF0);
    p = base;
    D_8017A4E0.count0 = *p++;
    D_8017A4E0.ptrs0 = p;
    p += D_8017A4E0.count0;
    D_8017A4E0.count1 = *p++;
    D_8017A4E0.data1 = (u16 *) p;
    p = (u32 *) ((u16 *) p + ((D_8017A4E0.count1 + 1) & ~1));
    D_8017A4E0.count2 = *p++;
    D_8017A4E0.ptrs2 = p;
    p += D_8017A4E0.count2;
    D_8017A4E0.count3 = *p++;
    D_8017A4E0.data3 = (u16 *) p;
    p = (u32 *) ((u16 *) p + ((D_8017A4E0.count3 + 1) & ~1));
    D_8017A4E0.count4 = *p++;
    D_8017A4E0.data4 = (u16 *) p;
    p = (u32 *) ((u16 *) p + ((D_8017A4E0.count4 + 1) & ~1));
    D_8017A4E0.count5 = *p++;
    D_8017A4E0.ptrs5 = p;
    for (i = 0; i < D_8017A4E0.count0; i++) {
        D_8017A4E0.ptrs0[i] += (u32) base;
    }
    for (i = 0; i < D_8017A4E0.count2; i++) {
        D_8017A4E0.ptrs2[i] += (u32) base;
    }
    for (i = 0; i < D_8017A4E0.count5; i++) {
        D_8017A4E0.ptrs5[i] += (u32) base;
    }
}
