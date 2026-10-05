/* Host-only semantic/layout checks. Never use this translation unit for
 * compiler matching: only the three external callees below are test doubles.
 * The genuine priority helper and mixer are included without substitutions.
 * This is a C/oracle comparison, not native execution or a matching claim.
 */
#include <assert.h>
#include <float.h>
#include <limits.h>
#include <stddef.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include "group.c"

enum {
    STATE_BYTES = 812, HEADER_BYTES = 12, RECORD_BYTES = 80,
    INDEX_OFFSET = 34, LENGTH_OFFSET = 76, ROUTES = 15,
    POINTS = 16, POINT_BYTES = 6, MAX_CALLS = 60, PROFILES = 8
};

RouteSet D_801407F0;
TrackerSet D_80151CE8;
f32 D_80152800;
f32 D_801543AC;

/* The oracle does not use Tracker, TrackerSet, RoutePoint, their member
 * accesses, or offsetof-derived tracker offsets. memcpy gives byte-addressed
 * loads/stores without alignment or strict-aliasing assumptions. Native byte
 * offsets are tested on host-endian values, not serialized N64 byte order.
 */
static s16 load16(const unsigned char *p, size_t at)
{
    s16 value;
    memcpy(&value, p + at, 2);
    return value;
}

static s32 load32(const unsigned char *p, size_t at)
{
    s32 value;
    memcpy(&value, p + at, 4);
    return value;
}

static f32 loadf(const unsigned char *p, size_t at)
{
    f32 value;
    memcpy(&value, p + at, 4);
    return value;
}

static void store16(unsigned char *p, size_t at, s16 value)
{
    memcpy(p + at, &value, 2);
}

static void store32(unsigned char *p, size_t at, s32 value)
{
    memcpy(p + at, &value, 4);
}

static void storef(unsigned char *p, size_t at, f32 value)
{
    memcpy(p + at, &value, 4);
}

static size_t record_at(int who)
{
    assert(who >= 0 && who < 10);
    return HEADER_BYTES + (size_t)who * RECORD_BYTES;
}

/* Canonical route inputs have a native 12-byte scalar prefix and flat 6-byte
 * point records. Host Route pointers are supplied separately, since a 64-bit
 * host does not have the N64 pointer ABI. */
static unsigned char route_bytes[ROUTES][12];
static unsigned char point_bytes[ROUTES][POINTS * POINT_BYTES];
static Route host_routes[ROUTES];
static RoutePoint host_points[ROUTES][POINTS];
static Route saved_routes[ROUTES];
static RoutePoint saved_points[ROUTES][POINTS];
static RouteSet saved_route_set;
static unsigned int route_count;
static int mock_profile;

enum { PRIMARY_CALL = 1, PATH_CALL = 2, LENGTH_CALL = 3 };
typedef struct {
    int kind;
    int a;
    int b;
} Call;
static Call actual_calls[MAX_CALLS];
static Call expected_calls[MAX_CALLS];
static int actual_count;
static int expected_count;
static unsigned long mixer_cases;
static unsigned long priority_cases;
static int negative_control;
static unsigned int boundary_witnesses;

static void record_call(Call *calls, int *count, int kind, int a, int b)
{
    assert(*count < MAX_CALLS);
    calls[*count].kind = kind;
    calls[*count].a = a;
    calls[*count].b = b;
    ++*count;
}

/* These are deliberately bounded deterministic contracts, not substitutes
 * for the accepted real callee bodies used by the separate compiler proof.
 * Include the -1 sentinel and large positive s16 indices. */
static s16 primary_value(int who)
{
    static const s16 values[10] = {-1, 0, 1, 7, 63, 255, 1023, 2047, 32766, 32767};
    return values[(who + mock_profile) % 10];
}

static s16 path_value(int who, int path)
{
    return (s16)((who * 193 + path * 17 + mock_profile * 13) % 1025 - 1);
}

static f32 length_value(int from, int to)
{
    if (from < 0 || to < 0) return -1.0f;
    return (f32)((from * 3 + to * 5 + mock_profile) % 257) * 0.25f;
}

s16 func_800BA61C(s16 who)
{
    record_call(actual_calls, &actual_count, PRIMARY_CALL, who, 0);
    return primary_value(who);
}

s16 func_800BA2B8(s16 who, s16 path)
{
    record_call(actual_calls, &actual_count, PATH_CALL, who, path);
    return path_value(who, path);
}

f32 audio_channel_alloc(s32 from, s32 to)
{
    record_call(actual_calls, &actual_count, LENGTH_CALL, from, to);
    return length_value(from, to);
}

static s16 oracle_priority(const unsigned char *state, int who, int route)
{
    size_t at;
    int i, anchor, side, num;
    f32 x, z, dx, dz, distance, dot;
    if ((unsigned int)route >= route_count) return -1;
    if (route_bytes[route][0] == 2) return who == 0 ? 0 : -1;
    at = record_at(who);
    anchor = 0;
    num = load16(route_bytes[route], 10);
    for (i = 0; i < num; ++i) {
        x = (f32)load16(point_bytes[route], (size_t)i * 6) - loadf(state, at);
        z = (f32)load16(point_bytes[route], (size_t)i * 6 + 4) - loadf(state, at + 8);
        dx = loadf(state, at + 12);
        dz = loadf(state, at + 20);
        distance = x * x + z * z;
        if (distance > (f32)load32(state, at + 24)) continue;
        dot = x * dx + z * dz;
        side = dot < 0.0f ? -1 : 1;
        if (anchor == 0) anchor = side;
        else if (side != anchor) return (s16)i;
    }
    return -1;
}

static void oracle_mixer(unsigned char *state, int mode, f32 *a, f32 *b)
{
    int count, first, last, who, slot, successor, from, to;
    size_t at;
    f32 length;
    count = load16(state, 8);
    first = load16(state, 2);
    last = load16(state, 4);
    for (who = 0; who < count; ++who) {
        at = record_at(who);
        if (mode <= 0) {
            record_call(expected_calls, &expected_count, PRIMARY_CALL, who, 0);
            store16(state, at + INDEX_OFFSET, primary_value(who));
        }
        for (slot = 1; slot <= 4; ++slot) {
            if (mode < 0 || mode == slot) {
                record_call(expected_calls, &expected_count, PATH_CALL, who, slot - 1);
                store16(state, at + INDEX_OFFSET + (size_t)slot * 2,
                        path_value(who, slot - 1));
            }
        }
        for (slot = 5; slot < 20; ++slot) {
            if (mode < 0 || (mode == 0 && (unsigned int)(slot - 5) < route_count))
                store16(state, at + INDEX_OFFSET + (size_t)slot * 2,
                        oracle_priority(state, who, slot - 5));
        }
    }
    *a = 0.0f;
    *b = 0.0f;
    for (who = 0; who < count; ++who) {
        successor = who == count - 1 ? first : who + 1;
        from = load16(state, record_at(who) + INDEX_OFFSET);
        to = load16(state, record_at(successor) + INDEX_OFFSET);
        record_call(expected_calls, &expected_count, LENGTH_CALL, from, to);
        length = length_value(from, to);
        storef(state, record_at(who) + LENGTH_OFFSET, length);
        /* Independent closed-form selection instead of the caller's done
         * flag: last == 0 includes all links; a positive last is excluded. */
        if (last == 0 || who < last) *a += length;
        if (who >= first) *b += length;
    }
}

static void validate_layout(void)
{
    assert(CHAR_BIT == 8 && sizeof(s16) == 2 && sizeof(s32) == 4);
    assert(sizeof(f32) == 4 && FLT_RADIX == 2 && FLT_MANT_DIG == 24);
    assert(sizeof(Tracker) == 80 && sizeof(TrackerSet) == 812);
    assert(offsetof(TrackerSet, limit) == 0);
    assert(offsetof(TrackerSet, first) == 2);
    assert(offsetof(TrackerSet, last) == 4);
    assert(offsetof(TrackerSet, selected_c) == 6);
    assert(offsetof(TrackerSet, count) == 8);
    assert(offsetof(TrackerSet, reserved) == 10);
    assert(offsetof(TrackerSet, tracks) == 12);
    assert(offsetof(Tracker, x) == 0 && offsetof(Tracker, y) == 4);
    assert(offsetof(Tracker, z) == 8 && offsetof(Tracker, dx) == 12);
    assert(offsetof(Tracker, dy) == 16 && offsetof(Tracker, dz) == 20);
    assert(offsetof(Tracker, range) == 24 && offsetof(Tracker, flags) == 28);
    assert(offsetof(Tracker, timer_a) == 30 && offsetof(Tracker, timer_b) == 32);
    assert(offsetof(Tracker, idx) == 34 && sizeof(((Tracker *)0)->idx) == 40);
    assert(offsetof(Tracker, unknown4A) == 74 && offsetof(Tracker, len) == 76);
    assert(sizeof(RoutePoint) == 6 && offsetof(RoutePoint, x) == 0);
    assert(offsetof(RoutePoint, y) == 2 && offsetof(RoutePoint, z) == 4);
    assert(offsetof(Route, type) == 0 && offsetof(Route, num) == 10);
    assert(offsetof(RouteSet, count) == 8);
    if (sizeof(void *) == 4) {
        assert(sizeof(Route) == 16 && offsetof(Route, pts) == 12);
        assert(sizeof(RouteSet) == 16 && offsetof(RouteSet, routes) == 12);
    }
}

static void install_routes(void)
{
    int route;
    memset(host_routes, 0xA6, sizeof host_routes);
    memset(&D_801407F0, 0x6B, sizeof D_801407F0);
    for (route = 0; route < ROUTES; ++route) {
        host_routes[route].type = route_bytes[route][0];
        host_routes[route].num = (u16)load16(route_bytes[route], 10);
        memcpy(host_points[route], point_bytes[route], sizeof host_points[route]);
        host_routes[route].pts = host_points[route];
    }
    D_801407F0.count = (u8)route_count;
    D_801407F0.routes = host_routes;
    memcpy(saved_routes, host_routes, sizeof saved_routes);
    memcpy(saved_points, host_points, sizeof saved_points);
    memcpy(&saved_route_set, &D_801407F0, sizeof saved_route_set);
}

static void check_routes_unchanged(void)
{
    assert(memcmp(saved_routes, host_routes, sizeof saved_routes) == 0);
    assert(memcmp(saved_points, host_points, sizeof saved_points) == 0);
    assert(memcmp(&saved_route_set, &D_801407F0, sizeof saved_route_set) == 0);
}

static void make_state(unsigned char *state, int count, int first, int last, int profile)
{
    static const f32 positions[8][3] = {
        {0.0f, -32768.0f, 0.0f}, {1.0f, 32767.0f, -1.0f},
        {-2.0f, -13.5f, 3.0f}, {0.5f, 0.25f, -0.5f},
        {0.0f, 7.0f, 0.0f}, {16384.0f, -0.5f, -16384.0f},
        {-32768.0f, 17.0f, 32767.0f}, {7.0f, 33.0f, -4.0f}
    };
    static const f32 directions[8][3] = {
        {1.0f, 0.0f, 0.0f}, {0.0f, 100.0f, -1.0f},
        {-1.0f, -999.0f, 2.0f}, {0.5f, 1.0f, -0.25f},
        {0.0f, 0.0f, 0.0f}, {1.0f, 32767.0f, 1.0f},
        {-2.0f, -32768.0f, 0.5f}, {3.0f, 42.0f, -2.0f}
    };
    static const s32 ranges[8] = {-2147483647 - 1, -1, 0, 1, 4, 25, 65536, 2147483647};
    int who, axis, slot, p;
    size_t byte, at;
    for (byte = 0; byte < STATE_BYTES; ++byte)
        state[byte] = (unsigned char)(byte * 37 + (size_t)profile * 13 + 0x59);
    store16(state, 2, (s16)first);
    store16(state, 4, (s16)last);
    store16(state, 8, (s16)count);
    for (who = 0; who < 10; ++who) {
        at = record_at(who);
        p = (who + profile) % PROFILES;
        for (axis = 0; axis < 3; ++axis) {
            storef(state, at + (size_t)axis * 4, positions[p][axis]);
            storef(state, at + 12 + (size_t)axis * 4, directions[p][axis]);
        }
        store32(state, at + 24, ranges[(who * 3 + profile) % 8]);
        for (slot = 0; slot < 20; ++slot)
            store16(state, at + INDEX_OFFSET + (size_t)slot * 2,
                    (s16)((who * 61 + slot * 43 + profile * 17) % 2049 - 1));
        storef(state, at + LENGTH_OFFSET, (f32)(who * 7 - profile) * 0.5f);
    }
}

static void make_routes(int profile)
{
    static const s16 coordinates[16][2] = {
        {-3, 0}, {-2, 0}, {-1, 0}, {0, 0}, {1, 0}, {2, 0}, {3, 0},
        {0, -2}, {0, 2}, {-1, 1}, {1, -1}, {-32768, 32767},
        {32767, -32768}, {16384, -16384}, {7, -4}, {-7, 4}
    };
    static const int lengths[10] = {0, 1, 2, 3, 4, 5, 6, 7, 8, 16};
    int route, point, selection;
    memset(route_bytes, 0xC3, sizeof route_bytes);
    for (route = 0; route < ROUTES; ++route) {
        route_bytes[route][0] = (unsigned char)((route + profile) % 5 == 0 ? 2 :
                                               (route % 2 == 0 ? 0 : 255));
        store16(route_bytes[route], 10, (s16)lengths[(route + profile) % 10]);
        for (point = 0; point < POINTS; ++point) {
            selection = (point + route * 3 + profile * 5) % 16;
            store16(point_bytes[route], (size_t)point * 6, coordinates[selection][0]);
            store16(point_bytes[route], (size_t)point * 6 + 2,
                    (s16)(point % 2 == 0 ? -32768 : 32767));
            store16(point_bytes[route], (size_t)point * 6 + 4, coordinates[selection][1]);
        }
    }
    install_routes();
}

static void check_calls(void)
{
    int i;
    assert(actual_count == expected_count);
    for (i = 0; i < expected_count; ++i) {
        assert(actual_calls[i].kind == expected_calls[i].kind);
        assert(actual_calls[i].a == expected_calls[i].a);
        assert(actual_calls[i].b == expected_calls[i].b);
    }
}

static void check_mixer(int count, int first, int last, int mode, int profile)
{
    unsigned char expected[STATE_BYTES];
    f32 total_a, total_b;
    size_t byte;
    make_state(expected, count, first, last, profile);
    memcpy(&D_80151CE8, expected, STATE_BYTES);
    D_80152800 = 123.5f;
    D_801543AC = -777.25f;
    mock_profile = profile;
    actual_count = expected_count = 0;
    oracle_mixer(expected, mode, &total_a, &total_b);
    audio_mixer_main((s16)mode);
    /* Fault injection is entirely in this host harness, after the real
     * function returns. These controls prove the same comparisons used by
     * positive cases reject an index-19 write error and a totals error. */
    if (negative_control == 1) {
        unsigned char *observed;
        size_t index19;
        observed = (unsigned char *)&D_80151CE8;
        index19 = record_at(9) + INDEX_OFFSET + 19 * 2;
        store16(observed, index19, (s16)(load16(observed, index19) == -1 ? 0 : -1));
    } else if (negative_control == 2) {
        D_801543AC += 0.25f;
    }
    if (memcmp(expected, &D_80151CE8, STATE_BYTES) != 0) {
        for (byte = 0; byte < STATE_BYTES; ++byte) {
            if (expected[byte] != ((unsigned char *)&D_80151CE8)[byte]) {
                fprintf(stderr, "state mismatch: count=%d first=%d last=%d routes=%u mode=%d profile=%d byte=%lu\n",
                        count, first, last, route_count, mode, profile, (unsigned long)byte);
                exit(EXIT_FAILURE);
            }
        }
    }
    if (memcmp(&D_80152800, &total_a, sizeof total_a) != 0 ||
        memcmp(&D_801543AC, &total_b, sizeof total_b) != 0) {
        fputs("totals mismatch\n", stderr);
        exit(EXIT_FAILURE);
    }
    check_calls();
    check_routes_unchanged();
    if (count == 10 && route_count == 15 && mode == -1 &&
        (first == 0 || first == 9) && (last == 0 || last == 9)) {
        /* Explicit witnesses for all four header-edge combinations and the
         * final legal tracker/index element, not just an aggregate count. */
        assert(load16((const unsigned char *)&D_80151CE8, 804) ==
               oracle_priority(expected, 9, 14));
        boundary_witnesses |= 1U << ((first == 9 ? 2 : 0) + (last == 9 ? 1 : 0));
    }
    ++mixer_cases;
}

static void check_priority(const unsigned char *state, int who, int route, int known)
{
    s16 expected;
    s32 actual;
    f32 a, b;
    a = 123.5f;
    b = -777.25f;
    memcpy(&D_80151CE8, state, STATE_BYTES);
    D_80152800 = a;
    D_801543AC = b;
    actual_count = 0;
    expected = oracle_priority(state, who, route);
    if (known != -2) assert(expected == known);
    actual = audio_priority_find((s16)who, (s16)route);
    assert(actual == expected);
    assert(actual_count == 0);
    assert(memcmp(state, &D_80151CE8, STATE_BYTES) == 0);
    assert(memcmp(&D_80152800, &a, sizeof a) == 0);
    assert(memcmp(&D_801543AC, &b, sizeof b) == 0);
    check_routes_unchanged();
    ++priority_cases;
}

static void priority_matrix(void)
{
    unsigned char state[STATE_BYTES];
    int profile, who, query;
    for (profile = 0; profile < PROFILES; ++profile) {
        make_state(state, 10, 0, 9, profile);
        for (route_count = 0; route_count <= ROUTES; ++route_count) {
            make_routes(profile);
            for (who = 0; who < 10; ++who) {
                for (query = 0; query <= ROUTES + 1; ++query)
                    check_priority(state, who, query, -2);
                check_priority(state, who, 32767, -2);
            }
        }
    }
}

static void priority_known_cases(void)
{
    /* Columns: point count, range, dir-x, dir-z, type, who, expected.
     * All unused Y values alternate s16 extrema and must have no effect. */
    static const int specs[][7] = {
        {0, 4, 1, 0, 0, 0, -1}, {1, 4, 1, 0, 0, 0, -1},
        {2, 4, 1, 0, 0, 0, 1}, {2, 3, 1, 0, 0, 0, -1},
        {2, -1, 1, 0, 0, 0, -1}, {2, 4, 0, 0, 0, 0, -1},
        {2, 4, -1, 0, 0, 0, 1}, {2, 4, 0, 1, 0, 0, -1},
        {3, 4, 1, 0, 0, 0, 2}, {3, 4, 1, 0, 0, 0, 1},
        {3, 4, 1, 0, 0, 0, 2}, {2, 4, 1, 0, 0, 0, 1},
        {2, 4, 1, 0, 0, 0, -1}, {2, 4, 0, 1, 0, 0, 1},
        {0, -1, 0, 0, 2, 0, 0}, {0, -1, 0, 0, 2, 9, -1},
        {2, 4, 1, 0, 255, 0, 1}, {2, 0, 1, 0, 0, 0, -1}
    };
    static const s16 xs[][3] = {
        {-2, 2, 0}, {-2, 2, 0}, {-2, 2, 0}, {-2, 2, 0},
        {-2, 2, 0}, {-2, 2, 0}, {-2, 2, 0}, {-2, 2, 0},
        {-2, 100, 2}, {-2, 2, -2}, {-100, -2, 2}, {-1, 0, 0},
        {-2, -1, 0}, {0, 0, 0}, {-2, 2, 0}, {-2, 2, 0},
        {-2, 2, 0}, {0, 0, 0}
    };
    unsigned char state[STATE_BYTES];
    size_t test, at;
    int point, who;
    route_count = 1;
    for (test = 0; test < sizeof specs / sizeof specs[0]; ++test) {
        make_state(state, 10, 0, 9, 0);
        who = specs[test][5];
        at = record_at(who);
        storef(state, at, 0.0f);
        storef(state, at + 8, 0.0f);
        storef(state, at + 12, (f32)specs[test][2]);
        storef(state, at + 20, (f32)specs[test][3]);
        store32(state, at + 24, specs[test][1]);
        memset(route_bytes, 0, sizeof route_bytes);
        memset(point_bytes, 0, sizeof point_bytes);
        route_bytes[0][0] = (unsigned char)specs[test][4];
        store16(route_bytes[0], 10, (s16)specs[test][0]);
        for (point = 0; point < 3; ++point) {
            store16(point_bytes[0], (size_t)point * 6, xs[test][point]);
            store16(point_bytes[0], (size_t)point * 6 + 2,
                    (s16)(point == 0 ? -32768 : 32767));
            store16(point_bytes[0], (size_t)point * 6 + 4,
                    (s16)(test == 13 ? (point == 0 ? -2 : 2) : 0));
        }
        install_routes();
        check_priority(state, who, 0, specs[test][6]);
        check_priority(state, who, 1, -1);
        check_priority(state, who, 32767, -1);
    }
}

static void mixer_matrix(void)
{
    static const int modes[] = {-32768, -7, -1, 0, 1, 2, 3, 4};
    int profile, count, first, last, bound;
    size_t mode;
    for (profile = 0; profile < PROFILES; ++profile) {
        for (route_count = 0; route_count <= ROUTES; ++route_count) {
            make_routes(profile);
            for (count = 0; count <= 10; ++count) {
                /* The empty case uses first=last=0; no record is read. */
                bound = count == 0 ? 1 : count;
                for (first = 0; first < bound; ++first)
                    for (last = 0; last < bound; ++last)
                        for (mode = 0; mode < sizeof modes / sizeof modes[0]; ++mode)
                            check_mixer(count, first, last, modes[mode], profile);
            }
        }
    }
}

int main(int argc, char **argv)
{
    validate_layout();
    if (argc == 2) {
        if (strcmp(argv[1], "--negative-index19") == 0) negative_control = 1;
        else if (strcmp(argv[1], "--negative-total") == 0) negative_control = 2;
        else return EXIT_FAILURE;
        route_count = 15;
        make_routes(0);
        check_mixer(10, 9, 9, -1, 0);
        fputs("negative control unexpectedly passed\n", stderr);
        return EXIT_SUCCESS;
    }
    assert(argc == 1);
    priority_known_cases();
    priority_matrix();
    mixer_matrix();
    assert(mixer_cases == 395264UL);
    assert(priority_cases == 23094UL);
    assert(boundary_witnesses == 15U);
    printf("PASS: %lu mixer cases; %lu priority cases; 812-byte state, two totals, external call order/arguments, read-only routes\n",
           mixer_cases, priority_cases);
    printf("LAYOUT: tracker=80 set=812 idx=34 idx_count=20 length=76; host pointers=%lu bytes\n",
           (unsigned long)sizeof(void *));
    puts("BOUNDARIES: tracker9/index19 byte804 and all first/last 0-or-9 pairs witnessed");
    puts("LIMITS: host C versus oracle, not native execution or compiler proof; only three external callees mocked; finite inputs; no invalid negative route/who/count/mode or overflowing point counter");
    return 0;
}
