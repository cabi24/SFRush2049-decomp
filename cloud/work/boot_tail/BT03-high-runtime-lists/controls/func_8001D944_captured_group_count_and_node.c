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
void func_8001D944(EmitObject *object, float distance)
{
    int i;
    u8 count;
    SmallNode *node;
    EmitGroup *group;
    SmallNode *current;
    SmallNode *previous;
    count = D_8004FF20;
    for (i = 0; i < count; i++) {
        if (D_8004FDA0[i].identifier == object->group38) break;
    }
    group = &D_8004FDA0[i];
    if (i == count) {
        group->large = 0;
        group->small = 0;
        group->identifier = object->group38;
        D_8004FF20 = count + 1;
    }
    previous = 0;
    current = group->small;
    while (current != 0) {
        if (distance < current->distance) break;
        previous = current;
        current = current->next;
    }
    if (previous == 0) group->small = &D_800502B0[D_80050430];
    else previous->next = &D_800502B0[D_80050430];
    node = &D_800502B0[D_80050430];
    node->next = current;
    node->distance = distance;
    node->object = object;
    ++D_80050430;
}
