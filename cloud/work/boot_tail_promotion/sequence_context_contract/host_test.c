/* Semantic tests use host pointers; native byte layout is checked by IDO. */
#include <assert.h>
#include <string.h>
#include "sequence_context.h"
#include "sources/func_80017540.c"
#include "sources/func_80018A30.c"
#include "sources/func_80018B3C.c"
#include "sources/func_80018C2C.c"
#include "sources/func_80018D40.c"
#include "sources/func_80019194.c"
#include "sources/func_800198C8.c"

SequenceNode *D_80043EB0;
SequenceContext D_80043EB8[8];
SequenceContext *D_8004BE80;
unsigned int D_8004BE78;
unsigned char D_8004BE7C;
unsigned int D_8004F808;
static unsigned int lookup, stop_calls, release_calls, removed, rate_calls;
static unsigned int rate_value, rate_channel, channel_calls, channel_index;
static unsigned int channels[65], flags_seen[65], identifiers[65];
static unsigned int visit_mask, service_step;
static SequenceContext *replace_context;
static SequenceContext *stop_context, *release_context;

unsigned int func_80017644(unsigned int id) { (void)id; return lookup; }
unsigned int func_800201D0(unsigned int id)
{
    return id == 11 ? 0xFFFFFFFFU : id;
}
void func_80017470(SequenceNode *node)
{
    removed++;
    assert(node->identifier == 11);
    D_8004BE80->pending = node->next;
    node->next = 0; /* A walker must save next before removing this node. */
}
void func_8001734C(SequenceContext *state)
{
    stop_calls++;
    stop_context = state;
    assert(!state->activeFC0);
}
void func_8001729C(SequenceContext *state)
{
    release_calls++;
    release_context = state;
    assert(stop_calls == release_calls);
}
void func_80019A60(unsigned int rate, unsigned char channel)
{
    rate_calls++;
    rate_value = rate;
    rate_channel = channel;
    if (replace_context) {
        D_8004BE80 = replace_context;
        replace_context = 0;
    }
}
void func_8001B9F8(unsigned char channel, unsigned short duration,
                  unsigned char index, unsigned char flags, unsigned int id)
{
    assert(channel == 250 && duration == 65530);
    assert(channel_calls < 65);
    channels[channel_calls] = index;
    flags_seen[channel_calls] = flags;
    identifiers[channel_calls] = id;
    channel_calls++;
}
int func_8001BDB8(unsigned char channel)
{
    assert(channel == D_8004BE80->channelFC4);
    channel_index = channel;
    service_step = 0;
    return 1;
}
void func_8001897C(void)
{
    if (D_8004F808) {
        assert(service_step++ == 0);
        assert(D_8004BE80 == &D_80043EB8[D_8004BE78]);
        assert(D_8004BE7C == 1 && channel_index == D_8004BE80->channelFC4);
        visit_mask |= 1U << D_8004BE78;
    }
}
void func_80018AEC(void) { assert(service_step++ == 1); }
unsigned char func_80018634(void)
{
    assert(service_step++ == 2);
    return D_8004BE78 == 1 ? 128 : 0;
}
unsigned char func_80017D38(void)
{
    assert(service_step++ == 3);
    return D_8004BE78 == 2 ? 255 : 0;
}
void func_8001824C(void) { assert(service_step++ == 4); }
void func_80018448(void) { assert(service_step++ == 5); }
void func_800180A0(void) { assert(service_step++ == 6); }
int func_80018184(void)
{
    assert(service_step++ == 7);
    return D_8004BE78 == 3 ? 1 : 0;
}

static void reset(void)
{
    memset(D_80043EB8, 0, sizeof(D_80043EB8));
    D_8004BE80 = &D_80043EB8[0];
    D_8004F808 = 0;
    stop_calls = release_calls = removed = rate_calls = channel_calls = 0;
    visit_mask = 0;
    replace_context = 0;
}

static void test_pending_list(void)
{
    SequenceNode a, b, c;
    reset();
    memset(&a, 0, sizeof(a)); memset(&b, 0, sizeof(b)); memset(&c, 0, sizeof(c));
    a.identifier = 11; b.identifier = 12; c.identifier = 11;
    a.next = &b; b.next = &c;
    D_8004BE80->pending = &a;
    func_80017540();
    assert(removed == 2);
}

static void test_timing(void)
{
    SequenceTimedValue values[3], other[2];
    reset();
    values[0].time = 8; values[0].value = 7;
    values[1].time = 11; values[1].value = 9;
    values[2].time = 0xFFFFFFFFU; values[2].value = 10;
    D_8004BE80->timingStartF68 = values;
    D_8004BE80->currentF6C = values;
    D_8004BE80->highF74 = 7; D_8004BE80->half120 = 1;
    D_8004BE78 = 0x123;
    func_80018A30();
    assert(rate_calls == 1 && rate_value == 7 && rate_channel == 0x23);
    assert(D_8004BE80->currentF6C == values + 1 && D_8004BE80->rate124 == 7);
    D_8004BE80->lowF70 = 999;
    func_80018B3C(10);
    assert(rate_calls == 3 && D_8004BE80->currentF6C == values + 2);
    assert(D_8004BE80->lowF70 == 0 && D_8004BE80->highF74 == 10);
    /* Unsigned comparison must reject a very large future time. */
    values[0].time = 0x80000000U;
    D_8004BE80->currentF6C = values;
    func_80018A30();
    assert(rate_calls == 3);
    /* Cursor advancement uses the global reloaded after the callback. */
    values[0].time = 0;
    other[0].time = 100; other[1].time = 0xFFFFFFFFU;
    D_80043EB8[1].currentF6C = other;
    replace_context = &D_80043EB8[1];
    func_80018A30();
    assert(rate_calls == 4 && D_80043EB8[0].currentF6C == values);
    assert(D_80043EB8[1].currentF6C == other + 1);
    D_8004BE80->highF74 = 123; D_8004BE80->lowF70 = 456;
    func_80018B3C(0);
    assert(D_8004BE80->highF74 == 123 && D_8004BE80->lowF70 == 456);
}

static void test_stop_and_retire(void)
{
    SequenceContext *r;
    reset(); r = &D_80043EB8[3]; lookup = 3;
    r->activeFC0 = 1;
    func_80018C2C(9);
    assert(!r->activeFC0 && !r->inactiveFC1);
    assert(stop_calls == 1 && release_calls == 1 && stop_context == r && release_context == r);
    func_80018D40(9);
    assert(r->inactiveFC1 && stop_calls == 1);
    r->activeFC0 = 1; r->inactiveFC1 = 0;
    lookup = 0x80000003U; r->pendingFF0 = 99; r->flagsFEE = 0x10;
    func_80018C2C(9);
    assert(r->flagsFEE == 0x18 && r->activeFC0);
    func_80018D40(9);
    assert(r->pendingFF0 == 0 && !r->inactiveFC1 && stop_calls == 1);
    r->inactiveFC1 = 1; r->pendingFF0 = 99; r->flagsFEE = 0;
    func_80018C2C(9); func_80018D40(9);
    assert(r->pendingFF0 == 99 && !r->flagsFEE);
    lookup = 0xFFFFFFFFU; func_80018C2C(9); func_80018D40(9);
    assert(stop_calls == 1);
}

static void test_channel_dispatch(void)
{
    SequenceContext *r;
    unsigned int i;
    reset(); lookup = 7; r = &D_80043EB8[7]; r->channelFC4 = 30;
    for (i = 0; i < 64; i++) r->channels528[i] = i;
    func_80019194(250, 65530, 0xCAFEBABEU, 0x82);
    assert(channel_calls == 64 && channels[0] == 30);
    assert(flags_seen[0] == 0x82 && identifiers[0] == 0xCAFEBABEU);
    for (i = 1; i < 64; i++) assert(flags_seen[i] == 0 && identifiers[i] == 0);
    lookup = 0x80000007U;
    func_80019194(250, 65530, 0, 0xF0); assert(r->valueFE0 == 250);
    r->pendingFF0 = 3;
    func_80019194(250, 65530, 0, 1); assert(r->pendingFF0 == 0);
    func_80019194(250, 65530, 0, 2); assert(r->flagsFEE == 8);
    func_80019194(250, 65530, 0, 3); assert(r->flagsFEE == 136);
    func_80019194(250, 65530, 0, 4); assert(r->flagsFEE == 136 && channel_calls == 64);
    lookup = 0xFFFFFFFFU; func_80019194(250, 65530, 0, 0);
    assert(channel_calls == 64);
}

static void test_sequence_service(void)
{
    unsigned int i;
    reset();
    for (i = 0; i < 8; i++) { D_80043EB8[i].activeFC0 = 1; D_80043EB8[i].channelFC4 = i + 40; }
    func_800198C8(); assert(visit_mask == 0);
    D_8004F808 = 1; D_80043EB8[6].activeFC0 = 0;
    func_800198C8(); assert(visit_mask == 0xBF);
    for (i = 0; i < 8; i++) {
        if (i >= 1 && i <= 3) assert(D_80043EB8[i].activeFC0 && !D_80043EB8[i].inactiveFC1);
        else if (i != 6) assert(!D_80043EB8[i].activeFC0 && D_80043EB8[i].inactiveFC1);
    }
    assert(!D_80043EB8[6].inactiveFC1 && D_8004BE80 == &D_80043EB8[7]);
}

int main(void)
{
    test_pending_list(); test_timing(); test_stop_and_retire();
    test_channel_dispatch(); test_sequence_service();
    return 0;
}
