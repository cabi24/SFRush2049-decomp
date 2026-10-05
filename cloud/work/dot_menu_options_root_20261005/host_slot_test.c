#include <assert.h>
#include <stdio.h>
#include "slot_state_setup.c"
s8 D_80149DA0;
s32 D_80149780, D_801497A4;
void *D_80114740;
static int log_ids[16], log_count, lookups, failures, sound_force, object_set;
static int mutate_selected;
static unsigned long cases;
static void event(int value) { assert(log_count < 16); log_ids[log_count++] = value; }
s32 func_80097694(s32 kind, s8 flag) {
    assert(flag == -1 && kind == D_80149DA0 + (lookups ? 22 : 38));
    event(lookups ? 4 : 1);
    if (mutate_selected) D_80149DA0 = (s8)(D_80149DA0 + 1);
    return (failures & (1 << lookups++)) ? -9 : 50 + lookups;
}
s32 audio_frame_sync(s32 kind, s32 skip, s32 async, s32 flag, void *buffer) {
    assert(skip == 0 && async == 0);
    assert(kind == D_80149DA0 + (lookups == 1 ? 38 : 22));
    assert(flag == (lookups == 1) && buffer == (lookups == 1 ? 0 : D_80114740));
    event(lookups == 1 ? 2 : 5); return 60 + lookups;
}
void display_list_alloc(s32 index) {
    assert(index == 60 + lookups);
    assert(index == (lookups == 1 ? D_80149780 : D_801497A4));
    event(lookups == 1 ? 3 : 6);
}
void sound_update_channel(s32 force) { sound_force = force; event(7); }
s8 object_byte9_set(s8 value) { assert(value == 1); object_set++; event(8); return -3; }
static void run(int old, int selected, int failure_mask, int mutate) {
    int expected[16], n = 0, i;
    D_80149DA0 = (s8)old; D_80149780 = 901; D_801497A4 = 902;
    log_count = lookups = object_set = 0; sound_force = -1;
    failures = failure_mask; mutate_selected = mutate;
    assert(slot_state_setup(selected) == old);
    if (selected != -1) {
        expected[n++] = 1;
        if (failure_mask & 1) { expected[n++] = 2; expected[n++] = 3; }
        expected[n++] = 4;
        if (failure_mask & 2) { expected[n++] = 5; expected[n++] = 6; }
        expected[n++] = 7;
        assert(lookups == 2 && sound_force == (old != selected));
        assert(D_80149780 == (failure_mask & 1 ? 61 : 51));
        assert(D_801497A4 == (failure_mask & 2 ? 62 : 52));
    } else {
        assert(!lookups && sound_force == -1 && D_80149780 == 901 && D_801497A4 == 902);
    }
    if (selected == 0) expected[n++] = 8;
    assert(object_set == (selected == 0));
    assert(D_80149DA0 == (s8)(selected + (mutate && selected != -1 ? 2 : 0)));
    assert(n == log_count);
    for (i = 0; i < n; i++) assert(expected[i] == log_ids[i]);
    cases++;
}
int main(void) {
    static int storage;
    const int selections[] = {-129,-128,-2,-1,0,1,11,13,127,128,255,256};
    int old, i, failures, mutation;
    D_80114740 = &storage;
    for (old = -128; old <= 127; old++) for (i = 0; i < 12; i++)
        for (failures = 0; failures < 4; failures++) for (mutation = 0; mutation < 2; mutation++)
            run(old, selections[i], failures, mutation);
    assert(cases == 24576UL); printf("slot host cases: %lu\n", cases); return 0;
}
