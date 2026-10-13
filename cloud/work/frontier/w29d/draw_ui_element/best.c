/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * draw_ui_element (0x800C7110, 240 B) = GC save.c _CheckAndRepairGhost(ghosth, data, repair).
 * IPA register params: ghosth t0, data t2, repair t1 (callers func_800C813C / func_800CBF2C set t0, t2, t1 in that
 * order = prototype order). The label is wrong. w29d draft.
 */
typedef signed char s8;typedef unsigned char u8;typedef short s16;typedef unsigned short u16;
typedef signed int s32;typedef unsigned int u32;typedef float f32;

typedef struct GhostHeader {      /* GC __anon_49776; N64 offsets from the retail words */
    u32 headerChecksum;           /* 0x00 */
    u8 revision;                  /* 0x04 */
    u8 pad05[0x3C - 0x05];
    u32 zippedSize;               /* 0x3C */
    u32 unzippedSize;             /* 0x40 */
    u32 zippedChecksum;           /* 0x44 */
    u32 unzippedChecksum;         /* 0x48 */
    u8 pad4C[0x58 - 0x4C];
    u8 data[1];                   /* 0x58 */
} GhostHeader;

typedef struct Ghost {            /* GC __anon_49a9c (N64 layout) */
    void *ll;                     /* 0x00 */
    void **fileh;                 /* 0x04 */
    u8 pad08[0x28 - 0x08];
    GhostHeader **header;         /* 0x28 */
} Ghost;

u32 format_string_parse(u8 *start, u32 size);    /* GC _CalcChecksum */
void pak_delete_file_retry(void **fileh);                  /* pak file delete (label wrong) */

s32 draw_ui_element(Ghost **ghosth, s32 data, s32 repair) {
    s32 result;
    GhostHeader *header;

    result = 0;
    header = *(*ghosth)->header;
    if (header->revision != 17) {
        if (repair) {
            if ((*ghosth)->fileh) {
                pak_delete_file_retry((*ghosth)->fileh);
            }
            return 0;
        }
        result = 1;
    }
    if (header->headerChecksum != format_string_parse(&header->revision, (u8 *)&header->zippedChecksum - (u8 *)header - 4)) {
        if (repair) {
            if ((*ghosth)->fileh) {
                pak_delete_file_retry((*ghosth)->fileh);
            }
            return 0;
        }
        result = 1;
    }
    if (!data) {
        return result;
    }
    if (header->zippedChecksum != format_string_parse(header->data, header->zippedSize)) {
        if (repair) {
            if ((*ghosth)->fileh) {
                pak_delete_file_retry((*ghosth)->fileh);
            }
            return 0;
        }
        result = 1;
    }
    return result;
}
