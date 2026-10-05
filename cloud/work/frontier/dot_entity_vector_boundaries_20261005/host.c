/* Test-only boundary fixture; the caller source is included unchanged. */
#include <stddef.h>
#include <string.h>
#include CANDIDATE_SOURCE
static f32 *first_pointer, *second_pointer;
static f32 replacement[3];
static int hit, mutate, event_count;
static unsigned int events[12];
extern f32 accepted_normalizer(f32 *);
f32 D_8012394C;
static void record(f32 *a, f32 *b)
{
    memcpy(events + event_count, a, 12);
    event_count += 3;
    if (b) {
        memcpy(events + event_count, b, 12);
        event_count += 3;
    }
}
void *input_deadzone_apply(f32 *start, f32 *end, Basis *basis,
                          f32 radius, int mode, int mask)
{
    int i;
    if (start != second_pointer || end != first_pointer ||
        radius != 0.5f || mode != 1 || mask != 6) __builtin_trap();
    record(end, start);
    for (i = 0; i < 9; ++i) basis->m[i] = (i + 1) * 0.125f;
    if (mutate) memcpy(end, replacement, sizeof replacement);
    return hit ? (void *)basis : (void *)0;
}
f32 func_8008E0B8(f32 *vector)
{
    record(vector, (f32 *)0);
    return accepted_normalizer(vector);
}
int run_case(const unsigned int *input, unsigned int *output,
             int hit_arg, int mutate_arg, int alias_arg, f32 threshold)
{
    f32 first[3], second[3];
    int result;
    memcpy(first, input, 12);
    memcpy(second, input + 3, 12);
    memcpy(replacement, input + 6, 12);
    first_pointer = first;
    second_pointer = alias_arg ? first : second;
    hit = hit_arg; mutate = mutate_arg; event_count = 0;
    memset(events, 0, sizeof events);
    D_8012394C = threshold;
    result = entity_iterate(first_pointer, second_pointer);
    memcpy(output, first, 12);
    memcpy(output + 3, second, 12);
    memcpy(output + 6, events, sizeof events);
    output[18] = event_count;
    return result;
}
