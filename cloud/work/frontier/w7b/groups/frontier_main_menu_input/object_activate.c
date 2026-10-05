/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * object_activate (0x800D52D4) and func_800D5374 (0x800D5374), one translation unit.
 * object_activate(node): unless node == (Node *)-1, under the queue lock D_80142728 (osRecvMesg blocking /
 * osJamMesg) call the debug check func_800D52CC(node), unlink the node from list D_80146188 if its
 * "in that list" byte (+9) is set, push it at the head of list D_80146170 (func_80091FBC = list insert,
 * func_8009211C = list remove; List {u8 indirect, doubly; u32 count; head; tail}) and set byte +8.
 * func_800D5374: for every active player record (D_8014A118, 76 bytes, count D_8014A108 s16) activate the
 * pending node at +0x44 and clear the slot to -1. No arcade ancestor (N64 object lists).
 *
 * Whole-program shape (proved with score.py group and blob_unit, see w7b/RESULTS.md):
 *  - object_activate is a *kept* function that umerge inlines into func_800D5374 because it is defined
 *    `__inline` in the same file (w6c mechanism). Its inlined statements carry the call's .loc, which is
 *    what makes as1 put the loop-end `sll` in the bne delay slot (an internal static helper or a goto in the
 *    loop body leaves it in the load-delay slot instead).
 *  - early `return` for the -1 node (not `if (node != -1) {...}`): the return gives the skip path its own
 *    block, so uopt's test replacement turns `i < D_8014A108` into the retail pointer compare against
 *    D_8014A118 + count*76 recomputed on both paths; with the if-block form the loop keeps `i` and needs
 *    a ninth s-register.
 *  - the slot field is an int (`= -1`) while the node is a pointer: two distinct -1 webs (s4 compare,
 *    s5 store) as in retail.
 *  - func_800D52CC is the group's internal empty function (main_menu_input.c), called by jal.
 * INTEGRATION: supersedes frontier_main_menu_input (revert it, splice this group); the old locked
 * object_activate source (if-block form, not __inline) is replaced by this one.
 */
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned char u8;
typedef unsigned int u32;

typedef struct Link { struct Link *next, *prev; } Link;
typedef struct List { u8 indirect, doubly, pad[2]; u32 count; Link *head, *tail; } List;
typedef struct OSMesgQueue OSMesgQueue;

typedef struct Entry {
    /* 0x00 */ s32 unk0[17];
    /* 0x44 */ s32 object;
    /* 0x48 */ s32 unk48;
} Entry;

extern OSMesgQueue D_80142728;
extern List D_80146170;
extern List D_80146188;
extern s16 D_8014A108;
extern Entry D_8014A118[];
s32 osRecvMesg(OSMesgQueue *mq, void *msg, s32 flag);
s32 osJamMesg(OSMesgQueue *mq, void *msg, s32 flag);
void func_8009211C(List *list, s32 *object);
void func_80091FBC(List *list, s32 *object, Link *before);

void func_800D52CC(s32 *arg0);

__inline void object_activate(s32 *arg0)
{
    if (arg0 == (s32 *)-1) {
        return;
    }
    osRecvMesg(&D_80142728, 0, 1);
    func_800D52CC(arg0);
    if (((s8 *)arg0)[9] != 0) {
        func_8009211C(&D_80146188, arg0);
        ((s8 *)arg0)[9] = 0;
    }
    func_80091FBC(&D_80146170, arg0, D_80146170.head);
    ((s8 *)arg0)[8] = 1;
    osJamMesg(&D_80142728, 0, 0);
}

void func_800D5374(void)
{
    s32 i;

    for (i = 0; i < D_8014A108; i++) {
        object_activate((s32 *)D_8014A118[i].object);
        D_8014A118[i].object = -1;
    }
}
