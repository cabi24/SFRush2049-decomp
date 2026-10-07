/* Behavioral checks supplement, and do not replace, the native-word proof. */
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include RECORDS_SOURCE

struct Voice { int marker; };
static Voice first_voice, second_voice;
Voice *D_80154198, *D_801541A0;
int D_80154360, D_80154350[4];
s16 D_80151AD0;
Row D_801541A8[4];
int D_8014A110;
typedef struct Event { int kind, value; } Event;
static Event actual[16], expected[16];
static int actual_count, expected_count, shrink_count;
static void append(Event *events, int *count, int kind, int value)
{
    if (*count >= 16) exit(1);
    events[*count].kind = kind;
    events[*count].value = value;
    ++*count;
}
void sound_handles_clear(int arg) { append(actual, &actual_count, 1, arg); }
void sound_stop(Voice *arg)
{
    if (arg != &first_voice && arg != &second_voice) exit(1);
    append(actual, &actual_count, 2, arg == &first_voice ? 765 : 432);
}
void entity_spawn_callback(s16 id, int a, int b)
{
    if (a || b) exit(1);
    append(actual, &actual_count, 3, id);
    if (shrink_count && D_80151AD0 > 0) --D_80151AD0;
}
void func_8039244C(void) { append(actual, &actual_count, 4, 0); }

int main(void)
{
    static const int counts[6] = {-1, 0, 1, 2, 3, 4};
    static const s16 ids[5] = {-32768, -1, 0, 123, 32767};
    static const int modes[5] = {-1, 0, 4, 6, 7};
    int seed, j, k, n, a, b, last, status[4], mode, shrink;
    Row rows[4];
    if (sizeof(Entry) != 52 || sizeof(Row) != 104) return 1;
    for (seed = 0; seed < 2400; ++seed) {
        actual_count = expected_count = 0;
        memset(actual, 0, sizeof(actual));
        memset(expected, 0, sizeof(expected));
        memset(D_801541A8, 0x5A, sizeof(D_801541A8));
        for (j = 0; j < 4; ++j) {
            D_801541A8[j].a.id = ids[(seed + j * 3) % 5];
            D_801541A8[j].b.id = ids[(seed / 5 + j * 2) % 5];
            D_80154350[j] = 0x1357 + j;
        }
        a = (seed & 1) ? 765 : 0;
        D_80154198 = a ? &first_voice : 0;
        b = (seed & 2) ? 432 : 0;
        D_801541A0 = b ? &second_voice : 0;
        D_80154360 = last = 987;
        D_80151AD0 = (s16)(n = counts[(seed / 4) % 6]);
        D_8014A110 = mode = modes[(seed / 24) % 5];
        shrink_count = shrink = (seed / 120) & 1;
        memcpy(rows, D_801541A8, sizeof(rows));
        memcpy(status, D_80154350, sizeof(status));
        if (a) {
            append(expected, &expected_count, 1, 1);
            append(expected, &expected_count, 2, a);
            a = 0;
            for (j = 0; j < n; ++j) {
                status[j] = 0;
                for (k = 0; k != 2; ++k) {
                    Entry *entry = k ? &rows[j].b : &rows[j].a;
                    if (entry->id != -1) {
                        append(expected, &expected_count, 3, entry->id);
                        if (shrink && n > 0) --n;
                        entry->id = -1;
                    }
                }
            }
            last = 0;
        }
        if (b) { append(expected, &expected_count, 2, b); b = 0; }
        if (mode == 4 || mode == 6) append(expected, &expected_count, 4, 0);
        records_screen();
        if ((a ? &first_voice : 0) != D_80154198 ||
            (b ? &second_voice : 0) != D_801541A0 || last != D_80154360 ||
            n != D_80151AD0 || mode != D_8014A110 ||
            memcmp(rows, D_801541A8, sizeof(rows)) ||
            memcmp(status, D_80154350, sizeof(status)) ||
            actual_count != expected_count ||
            memcmp(actual, expected, sizeof(actual))) {
            fprintf(stderr, "contract mismatch in case %d\n", seed);
            return 1;
        }
    }
    puts("PASS 2400 cleanup cases, including callback-driven count changes");
    return 0;
}
