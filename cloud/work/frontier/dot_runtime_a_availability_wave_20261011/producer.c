typedef signed char s8; typedef int s32;
extern s32 D_803BA830[];
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
