/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * entity_spawn_callback (historical label): remove scene node idx from the
 * 0x44-byte node table D_8012E700 (u16 id @0x14, s16 child @0x16, s16 sibling
 * @0x18).  Optionally frees the sibling chain and the child chain (recursive
 * (link, 1, 1)), unlinks idx from whoever references it (func_8009002C = node
 * whose child is idx, else the list head D_8015B254, else func_8008FFD0 = node
 * whose sibling is idx), marks the node free (id 0xFFFF, links -1) and trims
 * the high-water count D_80156990.  No arcade ancestor (N64 scene graph).
 *
 * Shaping (from w1a's best, two levers):
 *   - a constant-false debug block at entry (`if (SCENE_DEBUG) ...`): it adds
 *     one block to idx's live range, so uopt's priority for idx drops from
 *     1.2 (5 blocks, gets s0) to 1.0 (6 blocks) and idx is split to its home
 *     slot (`lh 34(sp)` at every use), as retail (uopt trace, w4a);
 *   - QUIRK: D_8015B254 (scene list head) is declared volatile.  That is what
 *     gives retail's `lui v0; addiu v0; lh 0(v0) ... sh 0(v0)` and the else-
 *     block reload of idx.  The locked render_mode_select matches with a plain
 *     `extern s16 D_8015B254` (with volatile it is 1 word off), so this is not
 *     a proven declaration; no non-volatile spelling was found (w1a ~80
 *     variants, w4a: operand order, *&, debug reads of the head).
 *   - the search results reuse the freeChildren parameter (a named local
 *     costs 8 frame bytes).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct Node {
    /* 0x00 */ u32 flags;
    /* 0x04 */ u8 pad4[0x10];
    /* 0x14 */ u16 id;
    /* 0x16 */ s16 child;
    /* 0x18 */ s16 sibling;
    /* 0x1A */ u8 pad1A[0x2A];
} Node; /* 0x44 */

extern Node D_8012E700[];
extern s32 D_80156990;
extern volatile s16 D_8015B254;

s16 func_8008FFD0(s16 idx);
s16 func_8009002C(s16 idx);
#define SCENE_DEBUG 0

void entity_spawn_callback(s16 idx, s32 freeChildren, s32 freeSiblings)
{
    Node *n;

    if (SCENE_DEBUG) {
    }
    if (freeSiblings) {
        n = &D_8012E700[idx];
        if (n->sibling >= 0) {
            entity_spawn_callback(n->sibling, 1, 1);
            n->sibling = -1;
        }
    }
    n = &D_8012E700[idx];
    if (freeChildren) {
        if (n->child >= 0) {
            entity_spawn_callback(n->child, 1, 1);
            n->child = -1;
        }
    }
    freeChildren = func_8009002C(idx);
    if (freeChildren >= 0) {
        D_8012E700[freeChildren].child = n->sibling;
    } else if (idx == D_8015B254) {
        D_8015B254 = n->sibling;
    } else {
        freeChildren = func_8008FFD0(idx);
        if (freeChildren >= 0) {
            D_8012E700[freeChildren].sibling = n->sibling;
        }
    }
    n->id = 0xFFFF;
    n->child = -1;
    n->sibling = -1;
    if (idx + 1 == D_80156990) {
        do {
            D_80156990--;
        } while (D_8012E700[D_80156990 - 1].id == 0xFFFF && D_80156990 > 0);
    }
}
