#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include <string.h>
#include "children.c"

struct BPool { int marker; };
BPool D_80394F70;
f32 D_8011418C[3][3];
s32 D_80399B18[17];
static BRecord record;
static BObject objects[3];
static int copy_calls, pitch_calls, alloc_calls, create_calls, delete_calls;
static int mutation, active_kind, resource_index, transform_mode;
static s32 expected_parent, expected_result;
static u32 expected_flags;
static BObject *released[2];
static s16 deleted[2];

void math_utility(f32 src[3][3], f32 dst[3][3])
{
    int i, j;
    copy_calls++;
    assert(dst == objects[0].uv);
    for (i = 0; i < 3; i++) for (j = 0; j < 3; j++) dst[i][j] = src[i][j];
    if (mutation) {
        record.kind = 0;
        record.flags = (s8) 0x92;
        record.primary = &objects[2];
    }
}
void func_80090F44(f32 angle, f32 uv[3][3])
{
    assert(angle == 0.18f && uv == objects[0].uv);
    pitch_calls++;
}
BObject *func_8008E3C0(BPool *pool)
{
    assert(pool == &D_80394F70);
    alloc_calls++;
    if (mutation) D_80399B18[resource_index] += 701;
    return &objects[0];
}
s32 func_8008E398(s32 resource, f32 transform[3][3], s32 parent, u32 flags)
{
    assert(alloc_calls == 1 && create_calls == 0);
    create_calls++;
    assert(resource == resource_index * 1009 - 811 + (mutation ? 701 : 0));
    assert(transform == (transform_mode ? 0 : objects[0].uv));
    assert(parent == expected_parent && flags == expected_flags);
    return expected_result;
}
void sound_call_minimal(s16 index)
{
    assert(delete_calls < 2);
    deleted[delete_calls] = index;
}
void func_800AFA84(BPool *pool, BObject *object)
{
    assert(pool == &D_80394F70 && delete_calls < 2);
    released[delete_calls++] = object;
    if (mutation && active_kind == 3 && delete_calls == 1) {
        record.primary = &objects[2];
        record.kind = 5;
    }
}
static void reset(void)
{
    int i, j;
    memset(&record, 0, sizeof(record));
    memset(objects, 0, sizeof(objects));
    copy_calls = pitch_calls = alloc_calls = create_calls = delete_calls = 0;
    record.primary = &objects[0]; record.secondary = &objects[1];
    for (i = 0; i < 3; i++) {
        record.position[i] = (f32)(i * 3 - 8) * 0.125f;
        for (j = 0; j < 3; j++) {
            objects[0].uv[i][j] = (f32)(i * 9 + j * 2 - 40) * 0.125f;
            record.uv[i][j] = (f32)(i * 9 + j * 2 - 80) * 0.125f;
            D_8011418C[i][j] = (f32)(i * 9 + j * 2 - 120) * 0.125f;
        }
    }
    for (i = 0; i < 17; i++) D_80399B18[i] = i * 1009 - 811;
}
int main(void)
{
    static const s32 modes[] = {INT_MIN, -1, 0, 1, 2, 3};
    static const s32 parents[] = {-1, 0, 32767, -32768};
    static const u32 flags_set[] = {0, 0x2000, 0x2080, 0x800000};
    static const s32 results[] = {-32768, -1, 0, 32767};
    unsigned long tests = 0;
    int kind, flags, m, mu, i, j, a, b, c, transformed, effective_flags, effective_kind;
    f32 expected[3][3], scale;
    for (mu = 0; mu < 2; mu++) for (kind = 0; kind < 256; kind++)
    for (flags = 0; flags < 256; flags++) for (m = 0; m < 6; m++) {
        reset(); mutation = mu; record.kind = (s8)kind; record.flags = (s8)flags;
        transformed = modes[m] != 2;
        effective_flags = (mu && transformed) ? 0x92 : flags;
        effective_kind = (mu && transformed) ? 0 : kind;
        for (i = 0; i < 3; i++) for (j = 0; j < 3; j++)
            expected[i][j] = modes[m] == 1 ? D_8011418C[i][j] :
                modes[m] == 2 ? objects[0].uv[i][j] : record.uv[i][j];
        scale = 1.0f;
        if (effective_flags & 0x70) {
            if (effective_kind == 0)
                scale = (effective_flags & 0x80) ? 0.2f : 0.4f;
            else if ((effective_kind == 1 || effective_kind == 8) &&
                     (effective_flags & 0x90) == 0x90) scale = 0.2f;
        }
        for (j = 0; j < 3; j++) expected[2][j] *= scale;
        func_8038D200(&record, modes[m]);
        assert(copy_calls == transformed && pitch_calls == !!(effective_flags & 2));
        assert(memcmp(expected, objects[0].uv, sizeof(expected)) == 0);
        assert(memcmp(record.position, objects[0].position, sizeof(record.position)) == 0);
        tests++;
    }
    for (mu = 0; mu < 2; mu++) for (resource_index = 0; resource_index < 17; resource_index++)
    for (transform_mode = 0; transform_mode < 2; transform_mode++)
    for (a = 0; a < 4; a++) for (b = 0; b < 4; b++) for (c = 0; c < 4; c++) {
        reset(); mutation = mu; expected_parent = parents[a]; expected_flags = flags_set[b];
        expected_result = results[c];
        assert(func_8038D328(resource_index, expected_parent, expected_flags,
                            transform_mode ? -7 : 0) == &objects[0]);
        assert(alloc_calls == 1 && create_calls == 1);
        assert(objects[0].scene_index == expected_result);
        tests++;
    }
    for (mu = 0; mu < 2; mu++) for (kind = 0; kind < 256; kind++) {
        reset(); mutation = mu; active_kind = kind; record.kind = (s8)kind;
        objects[0].scene_index = 0x12348000; objects[1].scene_index = 0x7654FFFF;
        objects[2].scene_index = 0x24687FFF;
        func_8038E088(&record);
        if (kind == 3) {
            assert(delete_calls == 2 && released[0] == &objects[1] && deleted[0] == -1);
            assert(released[1] == &objects[mu ? 2 : 0]);
            assert(deleted[1] == (mu ? 32767 : -32768));
        } else if (kind < 9 && kind != 5) {
            assert(delete_calls == 1 && released[0] == &objects[0] && deleted[0] == -32768);
        } else assert(delete_calls == 0);
        assert(record.primary == &objects[(mu && kind == 3) ? 2 : 0]);
        assert(record.secondary == &objects[1]);
        tests++;
    }
    printf("%lu bounded host fixtures passed\n", tests);
    return 0;
}
