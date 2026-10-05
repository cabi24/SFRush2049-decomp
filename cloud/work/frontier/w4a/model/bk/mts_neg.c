/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* NOT A MATCH: 8/101 words differ (same at -O2). Sibling of model_data_load (agentB, 3/93, same
 * loop residual). MBOX_ShowObject-like (arcade MB/mb_util.h SHOW_ALL=0, SHOW_EACHCHILD=1,
 * SHOW_RECURSIVE=2; N64 adds a per-viewport mask): clear the hidden bit 0x80000000 and the mask bits
 * (v << 8) of object record D_8012E700[a] (0x44 bytes, flags@0, child@0x16, sibling@0x18); mode 1 also
 * recurses into the children (tail call -> loop), mode 2 walks siblings recursively. The third branch
 * (`mode == 0` again, which sets the mask bits) is dead code that IDO keeps.
 * Residual: (1) the constants 1 and 0x80000000 swap registers (v1/a2) - every mask/compare spelling,
 * type and loop form leaves it; (2) loop child loaded into a3 + move (retail loads into a0 directly).
 */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];

void model_transform_setup(s32 a, s32 mode, u32 v) {
    s32 t;
    s32 c;

    if (mode == 1) {
        t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = t & ~((v << 8) | 0x80000000);
        t = D_8012E700[a].child;
        if (t != -1) model_transform_setup(t, 2, v);
    } else if (mode == 0) {
        t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = t & ~((v << 8) | 0x80000000);
    } else if (mode == 0) {
        t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = (t & 0x7FFFFFFF) | (v << 8);
    } else if (mode == 2) {
        do {
            t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = t & ~((v << 8) | 0x80000000);
            if (a < 0) ;
            c = D_8012E700[a].child;
            if (c != -1) model_transform_setup(c, 2, v);
            a = D_8012E700[a].sibling;
        } while (a != -1);
    }
}
