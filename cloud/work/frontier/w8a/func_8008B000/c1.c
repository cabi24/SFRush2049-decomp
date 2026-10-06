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
