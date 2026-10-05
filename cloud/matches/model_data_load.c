/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * model_data_load: "hide object" on the scene-object table (the twin of
 * model_transform_setup, which shows).  Shape of the arcade MBOX_HideObject
 * modes (MB/mb_util.h: HIDE_ALL 0, HIDE_ONLY 1, HIDE_EACHCHILD 2,
 * HIDE_RECURSIVE 3; no body in the reference tree) with an N64 per-viewport
 * mask v: object record D_8012E700[a] (0x44 bytes: flags@0, child@0x16,
 * sibling@0x18).  Mode 2 sets the mask bits (v << 8) on the root and hides its
 * children recursively (IDO turns the call into a jump back to the top); mode 0
 * sets the hidden bit 0x80000000; mode 1 sets the mask bits on the root only;
 * mode 3 walks the sibling chain, recursing into children.
 *
 * Shaping: a compiled-out diagnostic after the flag update in the two
 * child-walking arms (`if (a < 0) DEBUG_PRINT(...)`; the condition is not
 * recoverable -- any test of a or t gives the same code).  It ends the basic
 * block before the child load, so the child web does not share a block with
 * the incoming index and is loaded straight into a0 (earlier attempts had
 * `lh a3; move a0,a3`).  Read index (s16)a, write index a.
 */
typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef struct { u32 flags; u8 pad4[18]; s16 child; s16 sibling; u8 pad26[42]; } Ent;
extern Ent D_8012E700[];

#define DEBUG_PRINT(args)

void model_data_load(s32 a, s32 mode, u32 v) {
    s32 t;
    s32 c;

    if (mode == 2) {
        t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = t | (v << 8);
        if (a < 0) DEBUG_PRINT(("bad object %d\n", a));
        t = D_8012E700[a].child;
        if (t != -1) model_data_load(t, 3, v);
    } else if (mode == 0) {
        t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = t | 0x80000000;
    } else if (mode == 1) {
        t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = t | (v << 8);
    } else if (mode == 3) {
        do {
            t = D_8012E700[(s16) a].flags; D_8012E700[a].flags = t | (v << 8);
            if (a < 0) DEBUG_PRINT(("bad object %d\n", a));
            c = D_8012E700[a].child;
            if (c != -1) model_data_load(c, 3, v);
            a = D_8012E700[a].sibling;
        } while (a != -1);
    }
}
