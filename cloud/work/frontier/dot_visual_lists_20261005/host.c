/* Unchanged production candidate with a bounded, mutation-aware operation. */
#include <stdio.h>
#include <stddef.h>
#include <stdint.h>
#include <assert.h>
#include "../../../matches/visual_objects_update.c"

static VisualNode nodes[16];
static VisualBody bodies[32];
VisualList D_80144D60[4];
static unsigned mode;
static int events[2560], event_count;

static int ni(VisualNode *p) { return p ? (int)(p - nodes) : -1; }
static int bi(VisualBody *p) { return (int)(p - bodies); }
static void append(int v) { assert(event_count < 2560); events[event_count++] = v; }

s32 MaxPathZeroControls(VisualNode *node, s32 action)
{
    int id = ni(node), k, next = ni(node->body->next);
    append(id); append(action); append(bi(node->body)); append(next);
    append((int)node->body->enabled);
    for (k = 0; k < 4; k++) append(ni(D_80144D60[k].head));
    switch (mode) {
    case 1:
        if (next >= 0) node->body->next = next < 15 ? &nodes[next + 1] : 0;
        break;
    case 2: node->body = &bodies[16 + id]; break;
    case 3:
        if (id / 4 < 3) D_80144D60[id / 4 + 1].head = 0;
        break;
    case 4:
        if (next >= 0) nodes[next].body->enabled = 0;
        break;
    case 5: node->body->next = 0; break;
    case 6:
        if (id / 4 < 3) D_80144D60[id / 4 + 1].head = &nodes[(id / 4 + 1) * 4 + 3];
        break;
    }
    return (s32)(0x81234567u ^ (unsigned)id); /* ignored by the candidate */
}

int main(void)
{
    unsigned seed, m, a;
    int i, j, length;
    while (scanf("%u %u %u", &seed, &m, &a) == 3) {
        mode = m; event_count = 0;
        for (i = 0; i < 16; i++) {
            nodes[i].body = &bodies[i];
            bodies[i].next = i % 4 == 3 ? 0 : &nodes[i + 1];
            bodies[16 + i].next = i % 4 < 2 ? &nodes[i + 2] : 0;
            bodies[i].enabled = ((seed >> (i % 16)) & 1) ? 0x80000001u : 0;
            bodies[16 + i].enabled = 0xFFFFFFFFu;
        }
        for (i = 0; i < 4; i++) {
            length = (int)((seed >> (i * 3)) % 5);
            D_80144D60[i].head = length ? &nodes[4 * i] : 0;
            if (length) bodies[4 * i + length - 1].next = 0;
        }
        visual_objects_update((s32)a);
        printf("%d", event_count);
        for (i = 0; i < event_count; i++) printf(" %u", (unsigned)events[i]);
        for (i = 0; i < 4; i++) printf(" %u", (unsigned)ni(D_80144D60[i].head));
        for (i = 0; i < 16; i++) printf(" %u", (unsigned)bi(nodes[i].body));
        for (i = 0; i < 32; i++) {
            printf(" %u %u", (unsigned)ni(bodies[i].next), bodies[i].enabled);
            for (j = 0; j < 68; j++) assert(bodies[i].unknown04[j] == 0);
        }
        putchar('\n');
    }
    return 0;
}
