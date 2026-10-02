#include <assert.h>
#include <limits.h>
#include <stdio.h>
#include "../../matches/func_800A5158.c"
s8 D_80118EE4;
s16 D_8013FEC8;
u8 D_8002E860, D_80140A10;
s32 D_80140AF0;
static int calls, expected_input, result;
s32 audio_frame_sync(s32 a, s32 b, s32 c, s32 d, void *e)
{
    assert(calls++ == 0);
    assert(D_80118EE4 == 1 && D_8013FEC8 == 0);
    assert(D_80140A10 == expected_input);
    assert(a == expected_input && b == 0 && c == 0 && d == 0 && e == 0);
    assert(D_80140AF0 == 123);
    D_8002E860 ^= 255;
    return result;
}
void display_list_alloc(s32 id)
{
    assert(calls++ == 1);
    assert(id == result && D_80140AF0 == result);
}
void func_800A4E58(void) { assert(calls++ == 2); }
void func_800A510C(void) { assert(calls++ == 3); }
int main(void)
{
    static const int results[] = { INT_MIN, -1, 0, 1, INT_MAX };
    int guard, input, k;
    unsigned cases = 0;
    for (guard = -128; guard <= 127; guard++) {
        for (input = 0; input < 256; input++) {
            for (k = 0; k < 5; k++) {
                calls = 0; expected_input = input; result = results[k];
                D_80118EE4 = (s8)guard;
                D_8013FEC8 = -123;
                D_8002E860 = (u8)input;
                D_80140A10 = 77;
                D_80140AF0 = 123;
                func_800A5158();
                if (guard == 0) {
                    assert(calls == 4 && D_80118EE4 == 1);
                    assert(D_8013FEC8 == 0 && D_80140A10 == input);
                    assert(D_80140AF0 == result && D_8002E860 == (input ^ 255));
                } else {
                    assert(calls == 0 && D_80118EE4 == guard);
                    assert(D_8013FEC8 == -123 && D_80140A10 == 77);
                    assert(D_80140AF0 == 123 && D_8002E860 == input);
                }
                func_800A5158();
                assert(calls == (guard == 0 ? 4 : 0));
                cases++;
            }
        }
    }
    printf("PASS %u exhaustive guard/input/result cases plus repeated calls\n", cases);
    return 0;
}
