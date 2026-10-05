/* Host fixtures deliberately do not assert native storage or callback ABI. */
#include <assert.h>
#include <stdarg.h>
#include <stdio.h>
#include <string.h>
#define MENU_OPTIONS_TEST_FIXTURE 1
#define MENU_OPTIONS_HEADER_CAPACITY 128
#include "root.c"
Color4 D_801146BC, D_801146C0;
s32 D_80116D0C, D_80116D14[14], D_8015698C;
s8 D_80116DA8, D_80146108[19];
u32 D_801174B4;
char D_80121018[] = "CONTROLLER %d %s";
s16 D_80149DA2, D_80149B84, D_8013FEC8;
void *D_801164D0[4];
MenuAssets D_8017A4E0;
u8 D_801461D0[24];
static unsigned long cases;
static int events[128], event_count, locked, selected, row_count, formatted;
static int row_index[14], row_y[14], header_x, header_y, dynamic_header;
static int header_height, row_height, color_resets, alpha_resets;
static int mutate_after_first, height_calls;
static u32 width;
static char header_text[128];
static const int selector[14] = {-1,10,0,1,2,3,4,5,6,7,8,9,18,11};
static const int label_index[14] = {25,13,15,16,17,18,19,20,21,22,23,24,26,14};
static void event(int id) { assert(event_count < 128); events[event_count++] = id; }
void render_helper(f32 value) { assert(value == 0 || value == -1); event(value == 0 ? 1 : 12); }
s32 osRecvMesg(void *q, void *m, s32 f) {
    assert(q == D_801461D0 && !m && f == 1 && !locked); locked = 1; event(2); return -7;
}
s32 osJamMesg(void *q, void *m, s32 f) {
    assert(q == D_801461D0 && !m && f == 0 && locked); locked = 0; event(4); return -8;
}
s32 slot_state_setup(s32 n) {
    assert(locked && (n == 13 || n == 11)); selected = n; event(n == 13 ? 3 : 7); return -2;
}
void dispatch_handler(s32 mode) { assert(mode == 1 && selected == 13); event(5); }
void fcvt_wrapper(char *out, char *format, ...) {
    va_list args;
    assert(format == D_80121018 && selected == 13);
    va_start(args, format); vsprintf(out, format, args); va_end(args); formatted++;
}
u32 object_manager_update(void *text, s16 limit) {
    assert(limit == -1 && selected == 13);
    dynamic_header = text != D_8017A4E0.labels[50];
    if (dynamic_header) strcpy(header_text, text);
    return width;
}
void state_utility(s16 x, s16 y, void *text) {
    assert(selected == 13);
    assert(dynamic_header || text == D_8017A4E0.labels[50]);
    header_x = x; header_y = y; event(6);
}
s32 object_bytes_sum_global(void) {
    height_calls++; return selected == 13 ? header_height : row_height;
}
void func_8010A7A4(s32 index, s32 x, s32 y, void *label, void *value) {
    assert(selected == 11 && !locked && x == 160);
    assert(label == D_8017A4E0.labels[label_index[index]]);
    assert(value == (index == 0 ? D_801164D0[D_8013FEC8] :
        D_8017A4E0.values[D_80146108[selector[index]] + D_8017A4E0.header->string_offset]));
    row_index[row_count] = index; row_y[row_count++] = y; event(8);
    if (mutate_after_first && row_count == 1) {
        D_80116D14[1] = 0;
        D_80149DA2 = 3;
        D_80149B84 = 2;
    }
}
void func_8010A8D0(void) { event(13); }
void func_800ED66C(f32 alpha) { assert(alpha == -1.0f); alpha_resets++; event(10); }
void func_800BEA3C(Color4 a, Color4 b) {
    assert(a.rgba == D_801146BC.rgba && b.rgba == D_801146C0.rgba); color_resets++; event(11);
}
static void reset(void) {
    event_count = locked = selected = row_count = formatted = 0;
    color_resets = alpha_resets = dynamic_header = height_calls = 0;
    mutate_after_first = 0; header_height = 7; row_height = 11; width = 19;
    D_80116D0C = 0; D_801174B4 = 0; D_80116DA8 = 0;
    header_text[0] = '\0';
}
static void check_prefix_and_tail(void) {
    static const int prefix[] = {1,2,3,4,5,6,2,7,4};
    int i;
    for (i = 0; i < 9; i++) assert(events[i] == prefix[i]);
    assert(events[event_count-3] == 10 && events[event_count-2] == 11 && events[event_count-1] == 12);
    assert(!locked && color_resets == 1 && alpha_resets == 1);
    assert(header_x == (s16)(160 - (width >> 1)) && header_y == 10);
    assert(height_calls == row_count + 1);
}
int main(void) {
    static int storage[320];
    static void *labels[256], *values[64];
    static ResourceHeader resource;
    const int firsts[] = {-1,0,1,13,14};
    const int counts[] = {-1,0,1,5,14};
    const u32 widths[] = {0,1,319,320,321,65535,65536,0xffffffffU};
    int i, a, b, expected, first, max, bit, enabled, w;
    unsigned mask;
    for (i = 0; i < 256; i++) labels[i] = &storage[i];
    for (i = 0; i < 64; i++) values[i] = &storage[256+i];
    labels[233] = "OPTIONS";
    for (i = 0; i < 19; i++) D_80146108[i] = (s8)(i-8);
    resource.string_offset = 16;
    D_8017A4E0.labels = labels; D_8017A4E0.values = values; D_8017A4E0.header = &resource;
    D_8013FEC8 = 2; D_801164D0[2] = &storage[319];
    D_801146BC.rgba = 0x12345678; D_801146C0.rgba = 0x89abcdef;
    for (mask = 0; mask < (1U << 14); mask++) for (a = 0; a < 5; a++) for (b = 0; b < 5; b++) {
        reset(); first = firsts[a]; max = counts[b]; D_80149DA2 = first; D_80149B84 = max;
        for (i = 0; i < 14; i++) D_80116D14[i] = (mask & (1U << i)) ? (i%2 ? -1 : 2) : 0;
        assert(func_8010AEAC(0) == 1);
        expected = 0;
        for (i = 0; i < 14; i++) if ((mask & (1U << i)) && i >= first && expected < max) {
            assert(row_index[expected] == i && row_y[expected] == 24 + 11*expected); expected++;
        }
        assert(row_count == expected && !formatted); check_prefix_and_tail(); cases++;
    }
    for (bit = 0; bit < 32; bit++) for (enabled = -1; enabled <= 1; enabled++) for (w = 0; w < 8; w++) {
        reset(); D_801174B4 = 1U << bit; D_80116DA8 = enabled; D_8015698C = 2;
        D_80149B84 = 0; width = widths[w];
        assert(func_8010AEAC(0) == 1);
        expected = bit >= 18 && bit <= 22 && enabled != 0;
        assert(formatted == expected && dynamic_header == expected);
        if (expected) assert(strcmp(header_text, "CONTROLLER 3 OPTIONS") == 0);
        check_prefix_and_tail(); cases++;
    }
    reset(); D_80116D0C = 1;
    assert(func_8010AEAC(0) == 1 && event_count == 1 && events[0] == 13); cases++;
    reset(); for (i = 0; i < 14; i++) D_80116D14[i] = 1;
    D_80149DA2 = 0; D_80149B84 = 14; mutate_after_first = 1;
    assert(func_8010AEAC(0) == 1 && row_count == 2 && row_index[0] == 0 && row_index[1] == 3);
    check_prefix_and_tail(); cases++;
    assert(cases == 410370UL);
    printf("root host cases: %lu\n", cases);
    return 0;
}
