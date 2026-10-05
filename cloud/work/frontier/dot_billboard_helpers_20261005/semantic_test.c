/* Host behavior fixture only. These callbacks never enter a matching build. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stddef.h>
#include "group/func_800F207C.c"
InputRecord input_rec0[4];
s16 D_80144010[4], D_80144C40[4];
u8 D_80143A50[4][13];
s8 D_80143F18[4];
s32 D_801148D4[4], D_801424F0[4];
s16 D_80142528;
s32 D_80151690[12][3][5];
s8 D_80151AC0[3][5];
static u8 alphabet[54];
u8 *D_80117420 = alphabet;
static int timeout, sound_count, resource_count, filtered, cached;
static u32 last_sound;
void audio_distance_atten(u32 buttons) { sound_count++; last_sound = buttons; }
void audio_doppler(u32 buttons) { sound_count++; last_sound = buttons; }
void resource_type_select(u32 buttons) { resource_count++; (void)buttons; }
s32 viDeadlinePassed(void) { return timeout; }
u8 func_800F084C(s32 name) { filtered++; (void)name; return 0; }
s16 func_800F1D04(u8 *name) { cached++; assert(name == D_80143A50[2]); return 7; }
static void reset(void)
{
    memset(input_rec0, 0, sizeof(input_rec0));
    memset(D_80143A50, 0, sizeof(D_80143A50));
    memset(D_80144C40, 0, sizeof(D_80144C40));
    memset(D_80143F18, 1, sizeof(D_80143F18));
    memset(D_801148D4, -1, sizeof(D_801148D4));
    memset(D_801424F0, -1, sizeof(D_801424F0));
    memset(D_80151690, -1, sizeof(D_80151690));
    memset(D_80151AC0, -1, sizeof(D_80151AC0));
    timeout = sound_count = resource_count = filtered = cached = 0;
    last_sound = 0;
    D_80142528 = 8;
    input_rec0[2].player_id = 3;
}
static int moved(int key, int position)
{
    if (position < 0) {
        static const int special[4][3] = {
            {52,49,46}, {7,4,1}, {-2,-3,-1}, {-3,-1,-2}
        };
        return special[key][-position - 1];
    }
    if (key == 0) return position < 9 ? -3 + position / 3 : position - 9;
    if (key == 1) return position >= 45 ? -3 + (position - 45) / 3 : position + 9;
    if (key == 2) return position / 9 * 9 + (position + 8) % 9;
    return position / 9 * 9 + (position + 1) % 9;
}
int main(void)
{
    int key, position, row, place, i, cases;
    const u32 buttons[4] = {0x400,0x800,0x1000,0x2000};
    assert(sizeof(InputRecord) == 76);
    assert(offsetof(InputRecord, player_id) == 1);
    assert(offsetof(InputRecord, pressed) == 4);
    assert(offsetof(InputRecord, repeated) == 12);
    for (i = 0; i < 54; i++) alphabet[i] = (u8)('A' + i % 26);
    cases = 0;
    for (key = 0; key < 4; key++) {
        for (position = -3; position < 54; position++) {
            reset(); D_80144010[2] = (s16)position;
            input_rec0[2].repeated = buttons[key];
            func_800F207C(2);
            assert(D_80144010[2] == moved(key, position));
            assert(sound_count == 1 && last_sound == buttons[key]);
            assert(resource_count == 0 && filtered == 0 && cached == 0);
            cases++;
        }
    }
    reset(); D_80144010[2] = 20; input_rec0[2].repeated = 0x3C00;
    func_800F207C(2); assert(D_80144010[2] == 11 && sound_count == 1); cases++;
    reset(); D_80144010[2] = 10; input_rec0[2].pressed = 2;
    func_800F207C(2); assert(D_80143A50[2][0] == alphabet[10]);
    assert(D_80144C40[2] == 1 && D_80143A50[2][1] == 0); cases++;
    reset(); D_80144010[2] = -3; input_rec0[2].pressed = 2;
    func_800F207C(2); assert(D_80144C40[2] == 0); cases++;
    D_80143A50[2][0] = 'A'; D_80144C40[2] = 1;
    func_800F207C(2); assert(D_80143A50[2][1] == ' ' && D_80144C40[2] == 2); cases++;
    reset(); D_80144010[2] = 8; input_rec0[2].pressed = 2;
    memset(D_80143A50[2], 'A', 11); D_80144C40[2] = 11;
    func_800F207C(2); assert(D_80144C40[2] == 12 && D_80144010[2] == -1);
    assert(D_80143A50[2][11] == alphabet[8] && D_80143A50[2][12] == 0); cases++;
    for (key = 0; key < 2; key++) {
        reset(); D_80144010[2] = key ? 15 : -2;
        input_rec0[2].pressed = key ? 4 : 2;
        func_800F207C(2); assert(D_80144C40[2] == 0); cases++;
        strcpy((char *)D_80143A50[2], "ABC"); D_80144C40[2] = 3;
        func_800F207C(2); assert(strcmp((char *)D_80143A50[2], "AB") == 0);
        assert(D_80144C40[2] == 2); cases++;
    }
    for (key = 0; key < 3; key++) {
        reset(); D_80144010[2] = key == 0 ? -1 : 12;
        input_rec0[2].pressed = key == 0 ? 2 : (key == 1 ? 1 : 0);
        timeout = key == 2;
        strcpy((char *)D_80143A50[2], "ABC  "); D_80144C40[2] = 5;
        D_80151AC0[0][1] = D_80151AC0[2][4] = 2; D_80151AC0[1][3] = 1;
        func_800F207C(2);
        assert(strcmp((char *)D_80143A50[2], "ABC") == 0);
        assert(D_80144C40[2] == 3 && D_80143F18[2] == 0);
        assert(filtered == 1 && cached == 1 && resource_count == 1);
        assert(D_801148D4[3] == 7 && D_801424F0[2] == 7);
        for (row = 0; row < 3; row++) for (place = 0; place < 5; place++) {
            assert(D_80151690[8][row][place] == (D_80151AC0[row][place] == 2 ? 7 : -1));
            assert(D_80151690[7][row][place] == -1);
        }
        cases++;
    }
    printf("%d behavior cases passed\n", cases);
    return 0;
}
