/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001DA74 - lib_1cf90 (IDO 7.1). Queues an emitter mix request in its group's large list: finds
 * (or appends, max 32) the emitter group whose identifier is object->group, then (max 32 nodes) takes the
 * next free LargeNode D_8004FF28[D_800502A8], links it into the group's list sorted by descending distance
 * (after the last node not quieter than this one) and fills distance / four mix parameters / object.
 * Returns 1, or 0 when either pool is full. Large-list twin of the locked func_8001D944 (small list).
 * w20b levers (D10 true=4290): the locked sibling's style (indexed globals, `long i`, no group pointer);
 * the head test reads `D_8004FDA0[i].large` again (retail keeps head in a2 and the cursor in a1; a
 * head local gives a1/a2 swapped, true=105); LINE LAYOUT (disclosed): the six node stores are written two
 * on one line and four on the next - as1 then places the count increment's sb between them as retail
 * (one store per line: true=60, sb two slots late). No devices otherwise, no unused locals.
 * Types: the TU's SmallNode / LargeNode / EmitGroup (func_8001D944 block) and StateNode. */
#include "boot_tail_sample_contract.h"
typedef struct SmallNode { struct SmallNode *next; float distance; StateNode *object; } SmallNode;
typedef struct LargeNode { struct LargeNode *next; float distance, first, second, third, fourth; StateNode *object; } LargeNode;
typedef struct EmitGroup { unsigned int identifier; LargeNode *large; SmallNode *small; } EmitGroup;
extern EmitGroup D_8004FDA0[32];
extern unsigned char D_8004FF20;
extern LargeNode D_8004FF28[32];
extern unsigned char D_800502A8;

int func_8001DA74(StateNode *object, float distance, float first, float second, float third, float fourth)
{
    long i;
    LargeNode *current;
    LargeNode *next;
    for (i = 0; i < D_8004FF20; ++i) {
        if (object->group == D_8004FDA0[i].identifier) break;
    }
    if (i == D_8004FF20) {
        if (D_8004FF20 == 32) return 0;
        D_8004FDA0[i].large = 0;
        D_8004FDA0[i].small = 0;
        D_8004FDA0[i].identifier = object->group;
        ++D_8004FF20;
    }
    if (D_800502A8 == 32) return 0;
    current = D_8004FDA0[i].large;
    if (D_8004FDA0[i].large != 0) {
        next = current->next;
        while (next != 0) {
            if (current->distance < distance) break;
            current = next;
            next = next->next;
        }
        D_8004FF28[D_800502A8].next = next;
        current->next = &D_8004FF28[D_800502A8];
    } else {
        D_8004FF28[D_800502A8].next = D_8004FDA0[i].large;
        D_8004FDA0[i].large = &D_8004FF28[D_800502A8];
    }
    D_8004FF28[D_800502A8].object = object; D_8004FF28[D_800502A8].first = first;
    D_8004FF28[D_800502A8].distance = distance; D_8004FF28[D_800502A8].fourth = fourth; D_8004FF28[D_800502A8].second = second; D_8004FF28[D_800502A8++].third = third;
    return 1;
}
