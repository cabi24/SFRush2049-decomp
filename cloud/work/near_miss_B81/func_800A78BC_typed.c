/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned short u16;
typedef signed short s16;
typedef float f32;
typedef unsigned char u8;
typedef struct Record88 { s16 active; u16 flags, parameter; unsigned char rest[82]; } Record88;
extern Record88 D_8015B268[];
extern int D_801613B4,D_8014269C;
extern void func_8008C074(Record88 *, int, f32 *, u16, u8 *, u16, int);
Record88 *func_800A78BC(int active, f32 *vertices, u16 parameter, u8 *color, u16 flags, int indexed)
{
    int i;
    Record88 *record;
    for (i = 0; i < D_801613B4; i++) {
        if (!D_8015B268[i].active) break;
    }
    if (i >= 200) return 0;
    record = &D_8015B268[i];
    if (i >= D_801613B4) D_801613B4 = i + 1;
    if (D_8014269C < D_801613B4) D_8014269C = D_801613B4;
    record->flags = flags;
    if (indexed) {
        record->flags |= 0x400;
        record->parameter = parameter;
    }
    func_8008C074(record, active, vertices, parameter, color, 0, indexed);
    return record;
}
