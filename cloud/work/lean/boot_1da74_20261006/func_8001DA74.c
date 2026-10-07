/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
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
    long i;
    LargeNode *current;
    for (i = 0; i < D_8004FF20; ++i) {
        if (object->group38 == D_8004FDA0[i].identifier) break;
    }
    if (i == D_8004FF20) {
        if (D_8004FF20 == 32) return 0;
        D_8004FDA0[i].large = 0;
        D_8004FDA0[i].small = 0;
        D_8004FDA0[i].identifier = object->group38;
        ++D_8004FF20;
    }
    if (D_800502A8 == 32) return 0;
    current = D_8004FDA0[i].large;
    if (current != 0) {
        for (; current->next != 0; current = current->next) {
            if (current->distance < distance) break;
        }
        D_8004FF28[D_800502A8].next = current->next;
        current->next = &D_8004FF28[D_800502A8];
    } else {
        D_8004FF28[D_800502A8].next = D_8004FDA0[i].large;
        D_8004FDA0[i].large = &D_8004FF28[D_800502A8];
    }
    D_8004FF28[D_800502A8].object = object;
    D_8004FF28[D_800502A8].fourth = fourth;
    D_8004FF28[D_800502A8].first = first;
    D_8004FF28[D_800502A8].second = second;
    D_8004FF28[D_800502A8].third = third;
    D_8004FF28[D_800502A8++].distance = distance;
    return 1;
}
