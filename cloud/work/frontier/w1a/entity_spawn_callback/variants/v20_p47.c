/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
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
extern s16 D_8015B254;

s16 func_8008FFD0(s16 idx);
s16 func_8009002C(s16 idx);
void dbg(void);

void entity_spawn_callback(s16 idx, s32 freeChildren, s32 freeSiblings)
{
    Node *n;

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
        if (0) {}
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
