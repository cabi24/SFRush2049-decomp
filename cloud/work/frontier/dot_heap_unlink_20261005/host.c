/* Same candidate body under a host-width pointer transport contract, C89/UBSan. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
typedef unsigned long u32; /* Only transports pointers in this body; not native u32 ABI proof. */
typedef int s32;
typedef struct Heap { unsigned int magic; struct Heap *next; unsigned int other[6]; } Heap;
typedef struct OSMesgQueue { unsigned int words[6]; } OSMesgQueue;
static Heap nodes[33], expected[33];
static OSMesgQueue D_80152770, queue_expected;
static Heap *D_801527C8;
static int n, chosen_id, shifted, rewrite, order[32], event;
static Heap *selected, *head_expected;
static Heap *func_800A51D8(Heap *heap) { if (heap) return heap; return D_801527C8; }
static void receive_effect(void) {
    if (n) {
        D_801527C8 = &nodes[order[shifted && n > 1 ? 1 : 0]];
        if (rewrite && n >= 3) nodes[order[0]].next = &nodes[order[2]];
    }
}
static s32 osRecvMesg(OSMesgQueue *queue, void **message, s32 blocking) {
    assert(queue == &D_80152770 && message == NULL && blocking == 1 && event++ == 0);
    receive_effect(); return 0;
}
static void audio_reverb_update(u32 address, s32 tag) {
    assert((Heap *)address == selected && tag == 1 && event++ == 1);
    assert(memcmp(nodes, expected, sizeof(nodes)) == 0 && D_801527C8 == head_expected);
    selected->magic = 0xABCDEF01U;
}
static s32 osJamMesg(OSMesgQueue *queue, void *message, s32 blocking) {
    assert(queue == &D_80152770 && message == NULL && blocking == 0 && event++ == 2);
    D_80152770.words[0] = 0x11223344U; return 0;
}
#include "candidate.c"
static void setup(void) {
    int i;
    memset(nodes, 0xA6, sizeof(nodes)); memset(&D_80152770, 0x97, sizeof(D_80152770));
    for (i = 0; i <= n; ++i) nodes[i].next = NULL;
    for (i = 1; i < n; ++i) nodes[order[i - 1]].next = &nodes[order[i]];
    D_801527C8 = n ? &nodes[order[0]] : NULL;
}
int main(void) {
    int scanned, i; Heap *current;
    while ((scanned = scanf("%d%d%d%d", &n, &chosen_id, &shifted, &rewrite)) == 4) {
        assert(n >= 0 && n <= 32 && chosen_id >= -1 && chosen_id <= n);
        for (i = 0; i < n; ++i) assert(scanf("%d", &order[i]) == 1);
        setup(); receive_effect();
        selected = chosen_id < 0 ? D_801527C8 : &nodes[chosen_id];
        assert(selected != NULL);
        for (current = D_801527C8; current; current = current->next) {
            if (current->next == selected) { current->next = selected->next; break; }
        }
        memcpy(expected, nodes, sizeof(nodes)); head_expected = D_801527C8;
        memcpy(&queue_expected, &D_80152770, sizeof(queue_expected)); queue_expected.words[0] = 0x11223344U;
        setup(); event = 0;
        func_800E7A98(chosen_id < 0 ? NULL : &nodes[chosen_id]);
        assert(event == 3 && D_801527C8 == head_expected);
        expected[selected - nodes].magic = 0xABCDEF01U;
        assert(memcmp(nodes, expected, sizeof(nodes)) == 0);
        assert(memcmp(&D_80152770, &queue_expected, sizeof(queue_expected)) == 0);
        printf("%d\n", (int)(selected - nodes));
    }
    assert(scanned == EOF); return 0;
}
