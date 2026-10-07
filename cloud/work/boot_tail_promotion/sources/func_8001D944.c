/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Shared native contract adaptation of cloud/matches/func_8001D944.c (Astra PR #177);
 * compile with -Iinclude. EmitObject.group38 is StateNode.group (offset 0x38). */
#include "boot_tail_sample_contract.h"
typedef struct SmallNode { struct SmallNode *next; float distance; StateNode *object; } SmallNode;
typedef struct LargeNode { struct LargeNode *next; float distance, first, second, third, fourth; StateNode *object; } LargeNode;
typedef struct EmitGroup { unsigned int identifier; LargeNode *large; SmallNode *small; } EmitGroup;
extern EmitGroup D_8004FDA0[32];
extern unsigned char D_8004FF20;
extern LargeNode D_8004FF28[32];
extern unsigned char D_800502A8;
extern SmallNode D_800502B0[32];
extern unsigned char D_80050430;
void func_8001D944(StateNode *object, float distance)
{
    long i;
    SmallNode *current;
    SmallNode *previous;
    for (i = 0; i < D_8004FF20; ++i) {
        if (object->group == D_8004FDA0[i].identifier) break;
    }
    if (i == D_8004FF20) {
        D_8004FDA0[i].large = 0;
        D_8004FDA0[i].small = 0;
        D_8004FDA0[i].identifier = object->group;
        ++D_8004FF20;
    }
    previous = 0;
    for (current = D_8004FDA0[i].small; current != 0; current = current->next) {
        if (current->distance > distance) break;
        previous = current;
    }
    if (previous == 0) D_8004FDA0[i].small = &D_800502B0[D_80050430];
    else previous->next = &D_800502B0[D_80050430];
    D_800502B0[D_80050430].next = current;
    D_800502B0[D_80050430].object = object;
    D_800502B0[D_80050430++].distance = distance;
}
