/* Complete genuine A:931C0 -> {930B8,93004,92F28} call family. */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
extern s8 D_803BA858;
extern s32 D_803BA880;
extern s8 D_803BA860;
extern u8 D_803BA7FC[];
extern u8 D_803BA7FE[];

void func_80392F28(void)
{
    s32 i;
    s32 flag;
    s8 level = D_803BA858;

    if (level < 12) {
        D_803BA880 = 0;
        flag = 1;
    } else if (level < 20) {
        D_803BA880 = 1;
        flag = 1;
    } else if (level < 24) {
        D_803BA880 = 2;
        flag = 1;
    } else if (level < 25) {
        D_803BA880 = 3;
        flag = 1;
    } else if (level < 29) {
        D_803BA880 = 4;
        flag = 1;
        D_803BA860 = level - 25;
    } else {
        D_803BA880 = 5;
        flag = 1;
    }
    for (i = 0; i < 2; i++) {
        D_803BA7FC[i] = flag;
        D_803BA7FE[i] = flag;
    }
}

extern u8 D_803BA7E0[];
extern u8 D_803BA7F0[];
extern s32 D_803BA830[];
extern s8 D_803B65A4;

void func_80393004(void)
{
    unsigned int i;
    s32 count;

    D_803BA7E0[0] = 1;
    D_803BA7F0[0] = 1;
    D_803BA7E0[1] = 1;
    D_803BA7F0[1] = 1;
    D_803BA7E0[2] = 0;
    count = D_803BA830[D_803B65A4];
    D_803BA7F0[2] = count < 1;
    for (i = 0; i < 8; i++) {
        D_803BA7E0[i + 3] = (s32)i < count;
        D_803BA7F0[i + 3] = (s32)i < count;
    }
}


/* Bounded storage views follow only observed offsets; names are descriptive. */
typedef struct { s8 enabled; unsigned char remaining[15]; } PlayerSlot;
typedef struct { unsigned char prefix[16]; unsigned char slot; } ListOwner;
typedef struct { ListOwner *owner; } ListObject;
typedef struct ListNode ListNode;
typedef struct { ListNode *next; s32 unknown; ListObject *object; } ListPayload;
struct ListNode { ListPayload *payload; };
typedef struct { s32 prefix[2]; ListNode *head; } ListRoot;
extern PlayerSlot D_80156CF0[];
extern ListRoot D_8012E6D8;
extern s32 func_800A1A3C(s32);
extern s32 func_800A35F8(s32);
extern s32 func_80094FC4(s32);
void func_803930B8(void) {
    s32 slot;
    ListNode *node;
    for (slot = 0; slot < 4; slot++) {
        if (!D_80156CF0[slot].enabled) { D_803BA830[slot] = -4; }
        else if (!func_800A1A3C(slot)) { D_803BA830[slot] = -1; }
        else if (func_800A35F8(slot)) { D_803BA830[slot] = -2; }
        else if (func_80094FC4(slot)) { D_803BA830[slot] = -3; }
        else {
            node = D_8012E6D8.head;
            D_803BA830[slot] = 0;
            while (node != 0) {
                if (node->payload->object->owner->slot == slot) {
                    D_803BA830[slot]++;
                }
                node = node->payload->next;
            }
            if (D_803BA830[slot] == 0) { D_803BA830[slot] = 0; }
        }
    }
}

void func_803930B8(void);
void func_80393004(void);
void func_80392F28(void);
extern s32 D_803BA804;
extern s32 D_803BA800;
extern s8 D_803BA84D;
extern s8 D_803BA854;
extern s8 D_803BA850;
extern s8 D_803BA80C;
extern s8 D_803BA858;
extern s32 D_803BA880;
extern s32 D_803BA818[];
extern s8 D_803B6554[][13];
extern s32 D_803BA7D4;

void func_803931C0(s32 mode)
{
    D_803BA804 = mode;
    switch (D_803BA804) {
    case 0:
        func_803930B8();
        func_80393004();
        D_803BA84D = 0;
        D_803BA854 = 0;
        D_803BA850 = 0;
        D_803BA80C = 0;
        break;
    case 1:
        if (D_803BA800 == 0 && D_803BA858 >= 29) {
            D_803BA858 = 0;
            while (D_803B6554[D_803BA880][D_803BA818[D_803BA880]] == 0) {
                if (++D_803BA818[D_803BA880] >= 13) {
                    D_803BA818[D_803BA880] = 0;
                }
            }
        }
        func_80392F28();
        break;
    }
    D_803BA7D4 = 1;
}
