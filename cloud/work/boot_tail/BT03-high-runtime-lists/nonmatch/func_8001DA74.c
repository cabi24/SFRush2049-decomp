/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct EmitObject { u8 unknown00[56]; u32 group38; } EmitObject;
typedef struct SmallNode { struct SmallNode *next; float distance; EmitObject *object; } SmallNode;
typedef struct LargeNode { struct LargeNode *next; float distance, first, second, third, fourth; EmitObject *object; } LargeNode;
typedef struct EmitGroup { u32 identifier; LargeNode *large; SmallNode *small; } EmitGroup;
extern EmitGroup D_8004FDA0[32];
extern u8 D_8004FF20;
extern LargeNode D_8004FF28[32];
extern u8 D_800502A8;
extern SmallNode D_800502B0[32];
extern u8 D_80050430;
int func_8001DA74(EmitObject *object, float distance, float first, float second, float third, float fourth)
{
    int i;
    EmitGroup *group;
    LargeNode *current;
    LargeNode *head;
    LargeNode *next;
    LargeNode *node;
    for (i = 0; i < D_8004FF20; i++) {
        if (D_8004FDA0[i].identifier == object->group38) break;
    }
    if (i == D_8004FF20) {
        if (D_8004FF20 == 32) return 0;
        group = &D_8004FDA0[i];
        group->large = 0;
        group->small = 0;
        group->identifier = object->group38;
        ++D_8004FF20;
    }
    if (D_800502A8 == 32) return 0;
    group = &D_8004FDA0[i];
    head = group->large;
    current = head;
    if (head != 0) {
        next = current->next;
        while (next != 0) {
            if (current->distance < distance) break;
            current = next;
            next = current->next;
        }
        node = &D_8004FF28[D_800502A8];
        node->next = next;
        current->next = node;
    } else {
        node = &D_8004FF28[D_800502A8];
        node->next = head;
        group->large = node;
    }
    node = &D_8004FF28[D_800502A8];
    node->distance = distance;
    node->first = first;
    node->second = second;
    node->third = third;
    node->fourth = fourth;
    node->object = object;
    ++D_800502A8;
    return 1;
}
