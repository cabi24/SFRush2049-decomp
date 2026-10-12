/* All values are synthetic; no ROM data or runtime is used. */
#include <assert.h>
#include <math.h>
#include <stdio.h>
#include "tools/recomp/game_rdram.h"

#define BASE UINT32_C(0x80000000)

static void bytes_and_words(void) {
    uint8_t data[64] = {0x44, 0x33, 0x22, 0x11, 0x88, 0x77, 0x66, 0x55};
    rush_rdram memory = {data, sizeof(data)};
    uint8_t b; uint16_t h; uint32_t w; uint64_t d;
    assert(rush_rdram_read_u8(&memory, BASE, &b) && b == 0x11);
    assert(rush_rdram_read_u16(&memory, BASE + 1, &h) && h == 0x2233);
    assert(rush_rdram_read_u16(&memory, BASE + 3, &h) && h == 0x4455);
    assert(rush_rdram_read_u32(&memory, BASE, &w) && w == UINT32_C(0x11223344));
    assert(rush_rdram_read_u32(&memory, BASE + 2, &w) && w == UINT32_C(0x33445566));
    assert(rush_rdram_read_u64(&memory, BASE, &d) && d == UINT64_C(0x1122334455667788));
    assert(rush_rdram_write_u64(&memory, BASE, UINT64_C(0x123456789abcdef0)));
    { const uint8_t expected[] = {0x78, 0x56, 0x34, 0x12, 0xf0, 0xde, 0xbc, 0x9a};
      assert(memcmp(data, expected, sizeof(expected)) == 0); }
}

/* Independent byte-stream oracle; cover every alignment and both ends. */
static void exhaustive_spans(void) {
    uint8_t storage[69], guest[64], before[64];
    /* Deliberately unaligned backing storage. */
    rush_rdram memory = {storage + 1, 64};
    size_t at, j, group;
    for (j = 0; j < 64; ++j) guest[j] = (uint8_t)(j * 73 + 19);
    for (group = 0; group < 64; group += 4) {
        memory.data[group] = guest[group + 3];
        memory.data[group + 1] = guest[group + 2];
        memory.data[group + 2] = guest[group + 1];
        memory.data[group + 3] = guest[group];
    }
    memcpy(before, memory.data, 64);
#define TEST_WIDTH(bits, type) \
    for (at = 0; at <= 64 - sizeof(type); ++at) { \
        type value = 0, expected = 0; \
        for (j = 0; j < sizeof(type); ++j) expected = (type)((expected << 8) | guest[at + j]); \
        assert(rush_rdram_read_u##bits(&memory, BASE + (rush_addr)at, &value)); \
        assert(value == expected); \
        assert(rush_rdram_write_u##bits(&memory, BASE + (rush_addr)at, (type)~expected)); \
        for (j = 0; j < 64; ++j) { \
            uint8_t want = (j >= at && j < at + sizeof(type)) ? (uint8_t)~guest[j] : guest[j]; \
            assert(rush_rdram_read_u8(&memory, BASE + (rush_addr)j, &storage[68])); \
            assert(storage[68] == want); \
        } \
        memcpy(memory.data, before, 64); \
    }
    TEST_WIDTH(8, uint8_t)
    TEST_WIDTH(16, uint16_t)
    TEST_WIDTH(32, uint32_t)
    TEST_WIDTH(64, uint64_t)
#undef TEST_WIDTH
}

static void signed_float_and_address(void) {
    uint8_t data[32] = {0};
    rush_rdram memory = {data, sizeof(data)};
#define SIGNED(bits, type, minv, maxv) \
    { type out = 0; \
      assert(rush_rdram_write_s##bits(&memory, BASE + 3, minv)); \
      assert(rush_rdram_read_s##bits(&memory, BASE + 3, &out) && out == minv); \
      assert(rush_rdram_write_s##bits(&memory, BASE + 1, maxv)); \
      assert(rush_rdram_read_s##bits(&memory, BASE + 1, &out) && out == maxv); \
      assert(rush_rdram_write_s##bits(&memory, BASE + 2, (type)-1)); \
      assert(rush_rdram_read_s##bits(&memory, BASE + 2, &out) && out == -1); }
    SIGNED(8, int8_t, INT8_MIN, INT8_MAX)
    SIGNED(16, int16_t, INT16_MIN, INT16_MAX)
    SIGNED(32, int32_t, INT32_MIN, INT32_MAX)
    SIGNED(64, int64_t, INT64_MIN, INT64_MAX)
#undef SIGNED
    { const uint32_t patterns[] = {0, 0x80000000, 0x3fa00000, 0x7f800000, 0xff800000, 0x7fc12345};
      size_t i; for (i = 0; i < sizeof(patterns) / sizeof(*patterns); ++i) {
          float value; uint32_t raw;
          assert(rush_rdram_write_u32(&memory, BASE + 3, patterns[i]));
          assert(rush_rdram_read_f32(&memory, BASE + 3, &value));
          memcpy(&raw, &value, sizeof(raw)); assert(raw == patterns[i]);
          assert(rush_rdram_write_f32(&memory, BASE + 7, value));
          assert(rush_rdram_read_u32(&memory, BASE + 7, &raw) && raw == patterns[i]);
      }
    }
    { const uint64_t patterns[] = {0, UINT64_C(0x8000000000000000), UINT64_C(0x3ff4000000000000),
          UINT64_C(0x7ff0000000000000), UINT64_C(0xfff0000000000000), UINT64_C(0x7ff8123456789abc)};
      size_t i; for (i = 0; i < sizeof(patterns) / sizeof(*patterns); ++i) {
          double value; uint64_t raw;
          assert(rush_rdram_write_u64(&memory, BASE + 3, patterns[i]));
          assert(rush_rdram_read_f64(&memory, BASE + 3, &value));
          memcpy(&raw, &value, sizeof(raw)); assert(raw == patterns[i]);
          assert(rush_rdram_write_f64(&memory, BASE + 15, value));
          assert(rush_rdram_read_u64(&memory, BASE + 15, &raw) && raw == patterns[i]);
      }
    }
    { rush_addr token = 7;
      assert(rush_rdram_write_addr(&memory, BASE + 1, BASE + 16));
      assert(rush_rdram_read_addr(&memory, BASE + 1, &token) && token == BASE + 16);
      assert(rush_rdram_write_addr(&memory, BASE + 1, 0));
      assert(rush_rdram_read_addr(&memory, BASE + 1, &token) && token == 0);
      assert(rush_addr_from_gpr(UINT64_C(0xffffffff80000010), &token) && token == BASE + 16);
      assert(rush_addr_from_gpr(BASE + 16, &token) && token == BASE + 16);
      assert(!rush_addr_from_gpr(UINT64_C(0x0000000180000010), &token) && token == BASE + 16);
      assert(!rush_addr_from_gpr(UINT64_C(0xffffffff00000010), &token));
      assert(!rush_addr_from_gpr(0, NULL));
      assert(!rush_addr_add(UINT32_MAX, 1, &token) && token == BASE + 16);
      assert(!rush_addr_add(1, SIZE_MAX, &token));
    }
}

static void failures_are_atomic(void) {
    uint8_t data[16], before[16];
    rush_rdram memory = {data, sizeof(data)}, bad;
    const rush_addr bad_addresses[] = {0, 1, 0x7fffffff, 0xa0000000, 0xb0000000, 0xffffffff};
    uint32_t value = 0x12345678; size_t i, offset = 99;
    memset(data, 0xa5, sizeof(data)); memcpy(before, data, sizeof(data));
    for (i = 0; i < sizeof(bad_addresses) / sizeof(*bad_addresses); ++i) {
        assert(!rush_rdram_read_u32(&memory, bad_addresses[i], &value) && value == 0x12345678);
        assert(!rush_rdram_write_u32(&memory, bad_addresses[i], 0));
    }
    assert(!rush_rdram_read_u32(&memory, BASE + 13, &value) && value == 0x12345678);
    assert(!rush_rdram_write_u32(&memory, BASE + 13, 0));
    assert(!rush_rdram_write_u64(&memory, BASE + 9, 0));
    assert(!rush_rdram_span(&memory, BASE, SIZE_MAX, &offset) && offset == 99);
    assert(!rush_rdram_read_u32(NULL, BASE, &value));
    assert(!rush_rdram_read_u32(&memory, BASE, NULL));
    bad = memory; bad.size = 15; assert(!rush_rdram_write_u32(&bad, BASE, 0));
    bad = memory; bad.size = 0; assert(!rush_rdram_write_u32(&bad, BASE, 0));
    bad = memory; bad.data = NULL; assert(!rush_rdram_write_u32(&bad, BASE, 0));
    bad = memory; bad.size = 0x20000004; assert(!rush_rdram_write_u32(&bad, BASE, 0));
    assert(memcmp(data, before, sizeof(data)) == 0);
}

/* A C replacement explicitly adapts pointer dereferences to typed views.
 * It does not copy a whole native struct into the guest allocation. */
static bool update_input(const rush_rdram *memory, rush_addr input) {
    rush_rdram_view view = {memory, input};
    uint8_t active; int32_t word; float f;
    if (!rush_InputRecord_read_active(view, &active) ||
        !rush_InputRecord_read_unk04(view, &word) ||
        !rush_InputRecord_read_unk10(view, &f)) return false;
    return rush_InputRecord_write_unk04(view, word + active) &&
           rush_InputRecord_write_unk10(view, f * 2.0f);
}
static void game_fields(void) {
    uint8_t data[0xa100], before[0xa100];
    rush_rdram memory = {data, sizeof(data)};
    rush_rdram_view input = {&memory, BASE + 3}, pad = {&memory, BASE + 0x100};
    rush_rdram_view target, unchanged = {NULL, 0};
    rush_addr next; int32_t word; float f; uint8_t byte; size_t i;
    memset(data, 0xa5, sizeof(data));
    assert(rush_InputRecord_write_active(input, 2));
    assert(rush_InputRecord_write_unk04(input, -7));
    assert(rush_InputRecord_write_unk10(input, 1.25f));
    memcpy(before, data, sizeof(data));
    assert(update_input(&memory, input.address));
    assert(rush_InputRecord_read_unk04(input, &word) && word == -5);
    assert(rush_InputRecord_read_unk10(input, &f) && f == 2.5f);
    for (i = 0; i < sizeof(data); ++i) {
        size_t logical = i ^ 3;
        if (!((logical >= 7 && logical < 11) || (logical >= 19 && logical < 23)))
            assert(data[i] == before[i]);
    }
    assert(rush_PadConfig_write_unk4(pad, BASE + 0x180));
    assert(rush_PadConfig_read_unk4(pad, &next) && next == BASE + 0x180);
    target.memory = &memory; target.address = next;
    assert(rush_PadConfig_write_unk14(target, 0x17));
    assert(rush_rdram_read_u8(&memory, BASE + 0x194, &byte) && byte == 0x17);
    assert(rush_SoundClearRecord_write_unk1C(pad, BASE + 0x40));
    assert(rush_SoundClearRecord_read_unk1C(pad, &next) && next == BASE + 0x40);
    assert(rush_SoundClearRecord_write_unk20(pad, -19));
    assert(rush_rdram_read_s32(&memory, pad.address + 0x20, &word) && word == -19);
    assert(rush_Track_Data_write_start_time(pad, 7, 3.5f));
    assert(rush_Track_Data_read_start_time(pad, 7, &f) && f == 3.5f);
    assert(!rush_Track_Data_read_start_time(pad, 8, &f) && f == 3.5f);
    assert(!rush_Track_Data_write_end_time(pad, SIZE_MAX, 0));
    assert(rush_SegmentHeader_view_unk9CC0(pad, &target) && target.address == BASE + 0x9dc0);
    pad.address = UINT32_MAX;
    assert(!rush_PadConfig_write_unk4(pad, 0));
    assert(!rush_rdram_subview(pad, 1, 1, &unchanged) && unchanged.memory == NULL && unchanged.address == 0);
    pad.address = BASE - 1;
    assert(!rush_InputRecord_write_active(pad, 0));
    pad.address = BASE + sizeof(data) - 1;
    assert(!rush_PadConfig_write_unk4(pad, 0));
}

int main(void) {
    bytes_and_words(); exhaustive_spans(); signed_float_and_address();
    failures_are_atomic(); game_fields();
    puts("RDRAM scalar, boundary, and game-view fixtures passed");
    return 0;
}
