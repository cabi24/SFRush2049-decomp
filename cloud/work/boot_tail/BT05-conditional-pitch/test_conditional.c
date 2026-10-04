/* Synthetic address-backed tests, never native table data or domain evidence.
 * Each plain-source call is preceded by validate(). Rejected cases do not run.
 * Linux/x86-64 host only: low mapping and absolute linker address token. */
#define _GNU_SOURCE 1
#include <assert.h>
#include <stddef.h>
#include <stdio.h>
#include <string.h>
#include <sys/mman.h>
#include "func_80022CD4_CONDITIONAL.c"

#define ANCHOR 0x8002D476U
#define MAP_BASE 0x8002D000U
#define MAP_BYTES 4096U
#define SENTINEL 0xA5

static u16 *backing;
static unsigned int calls;
static unsigned int rejected;
static const char *reason;

/* Read only within the explicit host fixture, and never mistake its bounds
 * or values for the missing native table object. */
static int validate(u32 sample, u32 command, u32 low, u32 high,
                    u16 *note, u8 *detune)
{
    u32 sr, cr, larger, smaller, q, f, n, address, index;
    u32 lower, upper, divisor, cents, key;
    sr = sample & 0xFFFFFFU;
    cr = (command >> 8) & 0xFFFFU;
    key = sample >> 24;
    if (sr == cr) {
        *note = (u16)key;
        *detune = 0;
        reason = "equal";
        return 1;
    }
    larger = sr > cr ? sr : cr;
    smaller = sr < cr ? sr : cr;
    if (!smaller) {
        reason = "zero rate divisor";
        return 0;
    }
    q = (u32)(((unsigned long)larger * 4096UL) & 0xFFFFFFFFUL) / smaller;
    n = 0;
    while (n < 11 && (q >> 12) >= (1U << (n + 1))) ++n;
    f = q / (1U << n);
    if (!f) {
        reason = "no u16 can be less than zero";
        return 0;
    }
    address = ANCHOR;
    index = 11;
    for (;;) {
        if (address < low || address > high || high - address < 2) {
            reason = "no terminating readable halfword";
            return 0;
        }
        lower = *(u16 *)(unsigned long)address;
        if (lower < f) break;
        if (address < 2) {
            reason = "address wrapping outside fixture";
            return 0;
        }
        address -= 2;
        --index;
    }
    if (address > high || high - address < 4) {
        reason = "unreadable adjacent endpoint";
        return 0;
    }
    upper = *(u16 *)(unsigned long)(address + 2);
    divisor = (upper - lower) & 0xFFFFFFFFU;
    if (!divisor) {
        reason = "zero endpoint divisor";
        return 0;
    }
    cents = (u32)(((unsigned long)(f - lower) * 100UL) & 0xFFFFFFFFUL) / divisor;
    if (sr < cr) {
        *note = (u16)(key + n * 12 + index);
        *detune = (u8)cents;
    } else {
        *note = (u16)(key - n * 12 - index);
        /* Native signed-byte cast then negation, modulo final byte. */
        *detune = (u8)(0U - (cents & 255U));
    }
    reason = "finite, readable and nonzero";
    return 1;
}

static void trial(u32 sample, u32 command, u32 low, u32 high, int expect)
{
    PitchState state, before;
    PitchCommand cmd;
    u16 note;
    u8 detune;
    unsigned int j;
    int valid;
    valid = validate(sample, command, low, high, &note, &detune);
    assert(valid == expect);
    if (!valid) {
        ++rejected;
        return;
    }
    memset(&state, SENTINEL, sizeof(state));
    state.sample5C = sample;
    state.flags24 = sample ^ command;
    before = state;
    cmd.word[0] = command;
    cmd.word[1] = 0xDEADBEEFU;
    assert(func_80022CD4(&state, &cmd) == 0);
    assert(state.note50 == note);
    assert((u8)state.detuneC0 == detune);
    assert(state.flags24 == (before.flags24 | 0x100));
    assert(cmd.word[0] == command && cmd.word[1] == 0xDEADBEEFU);
    for (j = 0; j < sizeof(state); ++j) {
        if ((j >= 0x24 && j < 0x28) || j == 0x50 || j == 0x51 || j == 0xC0) continue;
        assert(((u8 *)&state)[j] == ((u8 *)&before)[j]);
    }
    ++calls;
}

static void fill(u16 value)
{
    unsigned int j;
    for (j = 0; j < MAP_BYTES / 2; ++j) backing[j] = value;
}

int main(void)
{
    void *mapping;
    u16 *anchor;
    unsigned int k, key, j, mode;
    u32 samples[] = {22050, 44100, 0x100001, 0xFFFFFF, 1, 65535, 12345, 54321};
    u32 commands[] = {44100, 22050, 1, 65535, 65535, 1, 54321, 12345};
    u32 sr, cr, q, n, f, selected;
    assert(sizeof(u32) == 4 && sizeof(u16) == 2 && sizeof(u8) == 1);
    assert(offsetof(PitchState, flags24) == 0x24);
    assert(offsetof(PitchState, note50) == 0x50);
    assert(offsetof(PitchState, sample5C) == 0x5C);
    assert(offsetof(PitchState, detuneC0) == 0xC0);
    assert((unsigned long)&D_8002D476 == ANCHOR);
    mapping = mmap((void *)(unsigned long)MAP_BASE, MAP_BYTES,
                   PROT_READ | PROT_WRITE, MAP_PRIVATE | MAP_ANONYMOUS | MAP_FIXED_NOREPLACE, -1, 0);
    assert(mapping == (void *)(unsigned long)MAP_BASE);
    backing = (u16 *)mapping;
    anchor = (u16 *)(unsigned long)ANCHOR;
    fill(5000);
    for (key = 0; key < 256; ++key) {
        trial(key << 24, 0, ANCHOR, ANCHOR, 1);
        trial((key << 24) | 44100, 44100U << 8, ANCHOR, ANCHOR, 1);
    }
    trial(1, 0, MAP_BASE, MAP_BASE + MAP_BYTES, 0);
    trial(0, 1U << 8, MAP_BASE, MAP_BASE + MAP_BYTES, 0);
    trial(0x100000, 1U << 8, MAP_BASE, MAP_BASE + MAP_BYTES, 0);
    /* Donor lowest endpoint 4096, anchor at nominal index11: F4096 cannot
     * terminate within this donor-shaped fixture. Same counterexample reversed. */
    for (j = 0; j < 13; ++j) anchor[(int)j - 11] = (u16)(4096 + j * 200);
    trial(22050, 44100U << 8, ANCHOR - 22, ANCHOR + 4, 0);
    trial(44100, 22050U << 8, ANCHOR - 22, ANCHOR + 4, 0);
    fill(0);
    trial(22050, 44100U << 8, MAP_BASE, MAP_BASE + MAP_BYTES, 0);
    anchor[0] = 100;
    anchor[1] = 200;
    trial(22050, 44100U << 8, ANCHOR, ANCHOR + 2, 0);
    trial(22050, 44100U << 8, ANCHOR + 2, ANCHOR + 4, 0);
    for (j = 0; j < sizeof(samples) / sizeof(samples[0]); ++j) {
        sr = samples[j]; cr = commands[j];
        q = (u32)(((unsigned long)(sr > cr ? sr : cr) * 4096UL) & 0xFFFFFFFFUL) / (sr < cr ? sr : cr);
        n = 0;
        while (n < 11 && (q >> 12) >= (1U << (n + 1))) ++n;
        f = q / (1U << n);
        assert(f != 0);
        for (k = 0; k <= 20; ++k) {
            if (f > 65535 && k != 0) continue;
            for (mode = 0; mode < 3; ++mode) {
                fill((u16)(f > 65535 ? 65535 : f));
                selected = f > 65535 ? 65500 : f - 1 - mode * (f / 4);
                anchor[-(int)k] = (u16)selected;
                if (k == 0) anchor[1] = mode == 2 ? 0 : (u16)(selected + 1 + mode * 100);
                for (key = 0; key < 256; ++key) {
                    trial((key << 24) | sr, (cr << 8) | 0xA500005AU,
                          MAP_BASE, MAP_BASE + MAP_BYTES, 1);
                }
            }
        }
    }
    assert(rejected == 8);
    assert(munmap(mapping, MAP_BYTES) == 0);
    printf("PASS: %u validated actual-source calls; %u unsafe fixtures rejected before execution\n", calls, rejected);
    return 0;
}
