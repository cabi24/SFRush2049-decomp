/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * model_transform_setup: "show object" on the scene-object table (the twin of
 * model_data_load, which hides).  Shape of the arcade MBOX_ShowObject modes
 * (MB/mb_util.h: SHOW_ALL 0, SHOW_EACHCHILD 1, SHOW_RECURSIVE 2; no body in the
 * reference tree) with an N64 per-viewport mask v: clear the hidden bit
 * 0x80000000 and the mask bits (v << 8) of object record D_8012E700[a]
 * (0x44 bytes: flags@0, child@0x16, sibling@0x18).  Mode 1 shows the root and
 * then its children recursively (IDO turns the call into a jump back to the
 * top); mode 2 walks the sibling chain, recursing into children.  The second
 * `mode == 0` arm is dead (copied from the hide routine's HIDE_ONLY) but IDO
 * keeps it.
 *
 * Shaping: a compiled-out diagnostic after the flag update in the two
 * child-walking arms (`if (a < 0) DEBUG_PRINT(...)`; the condition is not
 * recoverable -- any test of a or t gives the same code).  It ends the basic
 * block before the child load, so the child web no longer shares a block with
 * the incoming index and takes a0 directly (no `move a0,a3`), and it lowers the
 * priority of the 0x80000000 constant web so that `1` takes v1 and 0x80000000
 * takes a2 (uopt trace: save 5.5 vs 5.0 before).  Read index (s16)a, write
 * index a, as in model_data_load.
 */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];

#define DEBUG_PRINT(args)

void model_transform_setup(s32 a, s32 mode, u32 v) {
    s32 t;
    s32 c;

    if (mode == 1) {
        t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = t & ~((v << 8) | 0x80000000);
        if (a < 0) DEBUG_PRINT(("bad object %d\n", a));
        t = D_8012E700[a].child;
        if (t != -1) model_transform_setup(t, 2, v);
    } else if (mode == 0) {
        t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = t & ~((v << 8) | 0x80000000);
    } else if (mode == 0) {
        t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = (t & 0x7FFFFFFF) | (v << 8);
    } else if (mode == 2) {
        do {
            t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = t & ~((v << 8) | 0x80000000);
            if (a < 0) DEBUG_PRINT(("bad object %d\n", a));
            c = D_8012E700[a].child;
            if (c != -1) model_transform_setup(c, 2, v);
            a = D_8012E700[a].sibling;
        } while (a != -1);
    }
}
