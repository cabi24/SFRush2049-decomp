/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_8008B000(u16 id, s16 part, u16 value): set a model part's 16-bit value and mark it dirty (flags |=
 * 0x8000). id packs (list << 10) | model: model = &D_801161F4[id >> 10].models[id & 0x3FF] (the same 88-byte
 * Model / 16-byte Part table entity_render_mode walks; part count s16 at +0x16, parts at +0x18). part >= 0:
 * returns 0 when out of range, else sets that part. part < 0: sets every part's value -- and, as in retail,
 * ORs the dirty bit into m->part[part] (the negative index) on every iteration, an original bug. Returns 1.
 * Matched on the first compile; no quirks.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct { u16 value; u16 flags; u8 pad4[4]; u32 *dl; u8 padC[4]; } Part;
typedef struct { u8 pad0[0x16]; s16 count; Part part[4]; } Model;
typedef struct { Model *models; s32 count; } ModelList;

extern ModelList D_801161F4[];

s32 func_8008B000(u16 id, s16 part, u16 value)
{
    Model *m;
    s32 i;

    m = &D_801161F4[id >> 10].models[id & 0x3FF];
    if (part >= 0) {
        if (part >= m->count) {
            return 0;
        }
        m->part[part].value = value;
        m->part[part].flags |= 0x8000;
    } else {
        for (i = 0; i < m->count; i++) {
            m->part[i].value = value;
            m->part[part].flags |= 0x8000;
        }
    }
    return 1;
}
