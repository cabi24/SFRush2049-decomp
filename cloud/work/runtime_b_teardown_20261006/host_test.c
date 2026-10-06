/* Actual C89 source against an independent indexed oracle. */
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include <stdlib.h>
#ifndef CANDIDATE_PATH
#define CANDIDATE_PATH "candidate.c"
#endif
#include CANDIDATE_PATH

void *D_80395ED4;
s16 D_80151AD0;
HudEffectGroup D_803950C0[4];
HudStatus D_80395C00[4];
HudStatus D_80395CD0[4];
HudHandle D_80395DA0[4];

struct State {
    HudEffectGroup group[4];
    HudStatus first[4], second[4];
    HudHandle third[4];
    s16 count;
    void *resource;
};
struct Event { int helper, argument; struct State state; };
static struct Event expected[54];
static unsigned int event_count, event_index;
static int mode, scenario;
static struct State model;
static char resource[2][64];

static void fail(const char *what)
{
    fprintf(stderr, "mismatch: %s\n", what);
    exit(1);
}
static void snapshot(struct State *s)
{
    memcpy(s->group, D_803950C0, sizeof s->group);
    memcpy(s->first, D_80395C00, sizeof s->first);
    memcpy(s->second, D_80395CD0, sizeof s->second);
    memcpy(s->third, D_80395DA0, sizeof s->third);
    s->count = D_80151AD0;
    s->resource = D_80395ED4;
}
static void load(const struct State *s)
{
    memcpy(D_803950C0, s->group, sizeof s->group);
    memcpy(D_80395C00, s->first, sizeof s->first);
    memcpy(D_80395CD0, s->second, sizeof s->second);
    memcpy(D_80395DA0, s->third, sizeof s->third);
    D_80151AD0 = s->count;
    D_80395ED4 = s->resource;
}
static int equal(const struct State *a, const struct State *b)
{
    return memcmp(a->group, b->group, sizeof a->group) == 0 &&
           memcmp(a->first, b->first, sizeof a->first) == 0 &&
           memcmp(a->second, b->second, sizeof a->second) == 0 &&
           memcmp(a->third, b->third, sizeof a->third) == 0 &&
           a->count == b->count && a->resource == b->resource;
}
static void mutate(struct State *s, unsigned int ordinal)
{
    unsigned int x;
    static const s16 counts[5] = {4, 1, 3, 2, 0};
    if (scenario == 1 && ordinal == 1) s->count = 4;
    else if (scenario == 2 && ordinal == 1) s->count = 0;
    else if (scenario == 3) {
        s->resource = resource[1];
        s->second[ordinal % 4].handle = (s16)(32000 - ordinal);
        x = ordinal % 40;
        s->group[x / 10].effect[x % 10].unknown04[4] = (u8)ordinal;
    } else if (scenario == 4) {
        s->count = counts[ordinal % 5];
        s->first[ordinal % 4].state = (s16)(-(int)ordinal);
    } else if (scenario == 5 && ordinal == 1) s->count = 2;
}
static void callback(int helper, int argument)
{
    struct State actual;
    unsigned int ordinal;
    if (mode == 0) {
        if (event_count >= 54) fail("oracle call bound");
        expected[event_count].helper = helper;
        expected[event_count].argument = argument;
        expected[event_count].state = model;
        ordinal = ++event_count;
        mutate(&model, ordinal);
    } else {
        snapshot(&actual);
        if (event_index >= event_count || expected[event_index].helper != helper ||
            expected[event_index].argument != argument ||
            !equal(&expected[event_index].state, &actual)) fail("helper entry");
        ordinal = ++event_index;
        mutate(&actual, ordinal);
        load(&actual);
    }
}
void sound_stop(void *p)
{
    if (p != resource[0] && p != resource[1]) fail("resource pointer");
    callback(0, p == resource[0] ? 0 : 1);
}
void sound_call_minimal(s16 handle)
{
    callback(1, handle);
}
static void oracle_release(s16 *handle)
{
    if (*handle != -1) {
        sound_call_minimal(*handle);
        *handle = -1;
    }
}
static void oracle(void)
{
    int p, i;
    if (model.resource != 0) {
        sound_stop(model.resource);
        model.resource = 0;
    }
    if (model.count > 0) {
        p = 0;
        do {
            if (p >= 4) fail("fixture count domain");
            oracle_release(&model.first[p].handle);
            model.first[p].state = 8;
            oracle_release(&model.second[p].handle);
            model.second[p].state = 0;
            oracle_release(&model.third[p].handle);
            for (i = 0; i < 10; i++) {
                oracle_release(&model.group[p].effect[i].handle);
                model.group[p].effect[i].state = 11;
            }
            p++;
        } while (p < model.count);
    }
}
static void fixture(struct State *s, unsigned int seed, s16 count, int present)
{
    unsigned int state;
    size_t i;
    unsigned char *bytes;
    int player, k;
    static const s16 handles[6] = {-1, 0, 1, -32768, 32767, 0x1234};
    memset(s, 0, sizeof *s);
    state = seed + 1;
    bytes = (unsigned char *)s->group;
    for (i = 0; i < sizeof s->group; i++) {
        state = state * 1664525u + 1013904223u;
        bytes[i] = (unsigned char)(state >> 24);
    }
    bytes = (unsigned char *)s->first;
    for (i = 0; i < sizeof s->first; i++) {
        state = state * 1664525u + 1013904223u;
        bytes[i] = (unsigned char)(state >> 24);
    }
    bytes = (unsigned char *)s->second;
    for (i = 0; i < sizeof s->second; i++) {
        state = state * 1664525u + 1013904223u;
        bytes[i] = (unsigned char)(state >> 24);
    }
    bytes = (unsigned char *)s->third;
    for (i = 0; i < sizeof s->third; i++) {
        state = state * 1664525u + 1013904223u;
        bytes[i] = (unsigned char)(state >> 24);
    }
    for (player = 0; player < 4; player++) {
        s->first[player].handle = handles[(seed + player * 3) % 6];
        s->second[player].handle = handles[(seed + player * 3 + 1) % 6];
        s->third[player].handle = handles[(seed + player * 3 + 2) % 6];
        for (k = 0; k < 10; k++)
            s->group[player].effect[k].handle = handles[(seed + player * 3 + k + 3) % 6];
    }
    s->count = count;
    s->resource = present ? resource[0] : 0;
}
int main(void)
{
    unsigned int seed, n = 0;
    int c, present;
    struct State start, result;
    static const s16 counts[7] = {-32768, -1, 0, 1, 2, 3, 4};
    if (sizeof(HudEffect) != 72 || sizeof(HudEffectGroup) != 720 ||
        sizeof(HudStatus) != 52 || sizeof(HudHandle) != 52 ||
        offsetof(HudEffect, handle) != 2 || offsetof(HudStatus, handle) != 2)
        fail("layout");
    for (seed = 0; seed < 16; seed++) for (c = 0; c < 7; c++)
        for (present = 0; present < 2; present++) for (scenario = 0; scenario < 6; scenario++) {
            fixture(&start, seed, counts[c], present);
            load(&start);
            model = start;
            event_count = event_index = 0;
            mode = 0;
            oracle();
            mode = 1;
            func_8039244C();
            snapshot(&result);
            if (event_index != event_count || !equal(&result, &model)) fail("final state");
            n++;
        }
    printf("%u complete-state host fixtures passed\n", n);
    return 0;
}
