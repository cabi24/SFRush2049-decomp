/* Host scenario fixture only: callbacks here are never matching context. */
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include "group/func_800F1930.c"
s32 D_80144DA0;
s8 D_801460C0, D_801460FC, D_80146131, D_801460F0, D_80146148;
s32 D_801461C0[4];
u32 D_80156944, D_8015694C;
void *D_8014A160, *D_801461A8;
static int saves, save_result, save_force, save_slot, transitions, next_state;
static int status_calls, status_slot, mutate_selection;
s32 func_800CC040(s32 slot, void *data, void *output, s32 force)
{
    assert(data == D_8014A160 && output == D_801461A8);
    saves++; save_force = force; save_slot = slot;
    if (mutate_selection) D_80144DA0 = 3;
    return save_result;
}
void func_800F1210(s32 state) { transitions++; next_state = state; }
void func_800F0F44(s32 slot) { status_calls++; status_slot = slot; }
static void reset(void)
{
    static int data, output;
    memset(D_801461C0, 0, sizeof(D_801461C0));
    D_80144DA0 = 0; D_801460C0 = 2;
    D_801460FC = D_80146131 = D_801460F0 = D_80146148 = 0;
    D_80156944 = D_8015694C = 0;
    D_8014A160 = &data; D_801461A8 = &output;
    saves = transitions = status_calls = save_result = mutate_selection = 0;
    save_force = save_slot = next_state = status_slot = -1;
}
static int selectable(int row, int status) { return row != 1 && (row != 2 || status == 0 || status == 1); }
static int next_row(int row, int status, int step)
{
    do { row = (row + step + 4) % 4; } while (!selectable(row, status));
    return row;
}
static void check_status(void) { assert(status_calls == 1 && status_slot == D_801460C0); }
int main(void)
{
    int row, status, direction, expected, i, cases;
    cases = 0;
    for (row = 0; row < 4; row++) for (status = -4; status <= 2; status++) {
        for (direction = -1; direction <= 1; direction++) {
            reset(); D_80144DA0 = row; D_801461C0[2] = status;
            expected = selectable(row,status) ? row : next_row(row,status,-1);
            if (direction) {
                D_8015694C = direction < 0 ? 0x400 : 0x800;
                expected = next_row(expected,status,direction);
            }
            func_800F1930();
            assert(D_80144DA0 == expected && saves == 0 && transitions == 0);
            check_status(); cases++;
        }
    }
    for (i = 0; i < 4; i++) for (direction = -1; direction <= 1; direction += 2) {
        reset(); D_801460C0 = (s8)i; D_80156944 = 0x3000;
        D_8015694C = direction < 0 ? 0x1000 : 0x2000;
        func_800F1930(); assert(D_801460C0 == (i + direction + 4) % 4); check_status(); cases++;
    }
    reset(); D_801460FC = 1; D_80156944 = D_8015694C = 0x3000;
    func_800F1930(); assert(D_80146131 == 1 && D_801460C0 == 2); check_status(); cases++;
    reset(); D_801460FC = 1; D_80156944 = 0x3000; D_8015694C = 2;
    func_800F1930(); assert(D_801460FC == 1 && saves == 0); check_status(); cases++;
    reset(); D_801460F0 = 1; D_8015694C = 2;
    func_800F1930(); assert(D_801460F0 == 0 && saves == 0 && transitions == 0); check_status(); cases++;
    for (i = 0; i < 2; i++) {
        reset(); D_801460FC = D_80146131 = 1; D_8015694C = 2; save_result = i;
        func_800F1930(); assert(saves == 1 && save_force == 0 && save_slot == 2);
        assert(D_80146148 == i && D_801460FC == 0 && transitions == 0); check_status(); cases++;
    }
    reset(); D_80146148 = 1; D_8015694C = 1;
    func_800F1930(); assert(D_80146148 == 0 && transitions == 1 && next_state == 5); check_status(); cases++;
    for (i = 0; i < 2; i++) {
        reset(); D_80144DA0 = 2; D_8015694C = 2; save_result = i;
        func_800F1930(); assert(saves == 1 && save_force == 0 && D_80146148 == i);
        assert(transitions == 0); check_status(); cases++;
    }
    reset(); D_80144DA0 = 2; D_801461C0[2] = 1; D_8015694C = 2;
    func_800F1930(); assert(D_801460FC == 1 && D_80146131 == 0 && D_80146148 == 0);
    assert(saves == 0 && transitions == 0); check_status(); cases++;
    reset(); D_80144DA0 = 3; D_8015694C = 1;
    func_800F1930(); assert(saves == 1 && save_force == 1 && D_801461A8 == 0);
    assert(transitions == 1 && next_state == 5); check_status(); cases++;
    reset(); D_80144DA0 = 2; D_8015694C = 2; mutate_selection = 1;
    func_800F1930(); assert(saves == 2 && save_force == 1 && D_801461A8 == 0);
    assert(transitions == 1); check_status(); cases++;
    printf("%d save-input scenarios passed\n", cases);
    return 0;
}
