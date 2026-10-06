#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#ifndef CANDIDATE
#define CANDIDATE "func_8001F6EC.c"
#endif
#include CANDIDATE

FreeLink D_80050440[32];
u8 D_800504C0, D_800504C1, D_800504C2, D_800504C3;
typedef char voice_layout[sizeof(VoiceState) == 416 &&
    offsetof(VoiceState, channel2E) == 46 &&
    offsetof(VoiceState, external4C) == 76 &&
    offsetof(VoiceState, identifier60) == 96 ? 1 : -1];
typedef char link_layout[sizeof(FreeLink) == 4 && offsetof(FreeLink, active) == 2 ? 1 : -1];
#pragma pack(1)
typedef union Fixture {
    struct { u8 pre[4]; VoiceState state; u8 post[4]; } a;
    struct { u8 pre[5]; VoiceState state; u8 post[3]; } b;
    struct { u8 pre[6]; VoiceState state; u8 post[2]; } c;
    struct { u8 pre[7]; VoiceState state; u8 post[1]; } d;
} Fixture;
#pragma pack(0)
typedef char fixture_layout[sizeof(Fixture) == 424 ? 1 : -1];
static VoiceState *expected_pointer;
static u8 entry_state[416], entry_links[128];
static int slot, active, external, mutation, calls;

void func_8001EE9C(VoiceState *state)
{
    int after_slot;
    assert(state == expected_pointer && calls++ == 0);
    assert(memcmp(state, entry_state, sizeof(entry_state)) == 0);
    assert(memcmp(D_80050440, entry_links, sizeof(entry_links)) == 0);
    after_slot = mutation ? (slot + 11) % 32 : slot;
    if (mutation) {
        state->identifier60 = 0xDAAB0000U | (u32)after_slot;
        state->external4C = (u8)(external ? 0 : 255);
        state->command00 = 99;
        state->channel2E = 77;
        ((u8 *)state)[17] ^= 0x55;
        ((u8 *)D_80050440)[127] ^= 0x37;
        if (after_slot == 31) D_80050440[31].active = (u16)active;
    }
}

int main(void)
{
    static const int actives[3] = {0, 1, 65535};
    static const int externals[3] = {0, 1, 255};
    static const int counts[3] = {0, 1, 255};
    Fixture box;
    VoiceState *state;
    u8 *memory;
    u8 output[132];
    int a, nonempty, e, c, alignment, count, seed, i, after_slot;
    for (slot = 0; slot < 32; slot++)
    for (a = 0; a < 3; a++)
    for (nonempty = 0; nonempty < 2; nonempty++)
    for (e = 0; e < 3; e++)
    for (c = 0; c < 3; c++)
    for (alignment = 0; alignment < 4; alignment++)
    for (mutation = 0; mutation < 2; mutation++) {
        active = actives[a]; external = externals[e]; count = counts[c];
        seed = slot * 3 + external + count + mutation * 17;
        if (alignment == 0) { state = &box.a.state; memory = (u8 *)&box.a; }
        else if (alignment == 1) { state = &box.b.state; memory = (u8 *)&box.b; }
        else if (alignment == 2) { state = &box.c.state; memory = (u8 *)&box.c; }
        else { state = &box.d.state; memory = (u8 *)&box.d; }
        for (i = 0; i < 424; i++) memory[i] = (u8)(i * 13 + seed);
        state->identifier60 = 0xACDEF000U | (u32)slot;
        state->external4C = (u8)external;
        for (i = 0; i < 128; i++) ((u8 *)D_80050440)[i] = (u8)(i * 7 + seed);
        after_slot = mutation ? (slot + 11) % 32 : slot;
        D_80050440[after_slot].active = (u16)active;
        D_800504C0 = (u8)(nonempty ? 3 : 255);
        D_800504C1 = (u8)(mutation ? after_slot : (after_slot + 13) % 32);
        D_800504C2 = D_800504C3 = (u8)count;
        expected_pointer = state; calls = 0;
        memcpy(entry_state, state, sizeof(entry_state));
        memcpy(entry_links, D_80050440, sizeof(entry_links));
        func_8001F6EC(state);
        assert(calls == 1);
        memcpy(output, D_80050440, 128);
        output[after_slot * 4 + 2] = (u8)(D_80050440[after_slot].active >> 8);
        output[after_slot * 4 + 3] = (u8)D_80050440[after_slot].active;
        output[128] = D_800504C0; output[129] = D_800504C1;
        output[130] = D_800504C2; output[131] = D_800504C3;
        assert(fwrite(memory, 1, 424, stdout) == 424);
        assert(fwrite(output, 1, 132, stdout) == 132);
    }
    return 0;
}
