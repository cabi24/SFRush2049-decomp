/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * main_menu_input (0x800DEBAC) is a historical label.  Real semantics: under the queue lock D_80142728
 * (osRecvMesg blocking / osJamMesg), copy four Vec3 (a, b, c, d) into a node at +0x0C, +0x18, +0x24, +0x30.
 * A node of (Node *) -1 is skipped.  No arcade ancestor identified.
 *
 * Whole-program context (cannot match alone): the callee func_800D52CC is an *internal* empty function that
 * retail still calls by jal.  Because it is internal the caller knows it clobbers nothing and keeps
 * node/a/b in t0-t2 and c in a3 across it, and the FP temp ring is four wide.  Its locked source
 * `void func_800D52CC(void) {}` is wrong for callers: the parameter exists and is read (otherwise the
 * argument is stored at 0(sp) instead of moved to a0).  The locked object_activate still matches with this
 * definition (context in this group).
 * INTEGRATION: this group supersedes the locked single func_800D52CC (-O2, `(void)`): `blob_splice revert
 * func_800D52CC`, then splice the group.  In the shadow unit func_800D52CC must be internal (the group
 * defines it and does not keep it); `blob_unit score main_menu_input func_800D52CC object_activate --with
 * main_menu_input.c --internal func_800D52CC` is 3/3 EQUAL (without --internal, i.e. with the old single
 * still kept, main_menu_input is 29/56 off).  The existing inline_blockers entry for func_800D52CC stays.
 */
typedef signed int s32;
typedef float f32;

typedef struct Node {
    /* 0x00 */ s32 unk0[3];
    /* 0x0C */ f32 a[3];
    /* 0x18 */ f32 b[3];
    /* 0x24 */ f32 c[3];
    /* 0x30 */ f32 d[3];
} Node;

typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_80142728;
s32 osRecvMesg(OSMesgQueue *mq, void *msg, s32 flag);
s32 osJamMesg(OSMesgQueue *mq, void *msg, s32 flag);
extern int __unit_dead[];

/* Empty in retail (`jr ra; nop`) but still called by `jal`, and its callers know it clobbers nothing.
 * The parameter must be read for it to be passed in a0; the `if (0)` block is NOT original source: it is the
 * inline blocker of src/blob/unit_overrides.json (umerge sizes a callee before optimisation). */
void func_800D52CC(Node *node) {
    if (node) {
    }
    if (0) {
        __unit_dead[0] = 1;
        __unit_dead[1] = 2;
        __unit_dead[2] = 3;
        __unit_dead[3] = 4;
        __unit_dead[4] = 5;
        __unit_dead[5] = 6;
        __unit_dead[6] = 7;
        __unit_dead[7] = 8;
    }
}

void main_menu_input(Node *node, f32 *a, f32 *b, f32 *c, f32 *d) {
    if (node != (Node *) -1) {
        osRecvMesg(&D_80142728, 0, 1);
        func_800D52CC(node);
        node->a[0] = a[0];
        node->a[1] = a[1];
        node->a[2] = a[2];
        node->b[0] = b[0];
        node->b[1] = b[1];
        node->b[2] = b[2];
        node->c[0] = c[0];
        node->c[1] = c[1];
        node->c[2] = c[2];
        node->d[0] = d[0];
        node->d[1] = d[1];
        node->d[2] = d[2];
        osJamMesg(&D_80142728, 0, 0);
    }
}
