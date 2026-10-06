/* Unchanged production candidate, with a separately defined real helper model. */
#include <stddef.h>
#include <string.h>
#ifndef CANDIDATE_PATH
#error CANDIDATE_PATH required
#endif
#include CANDIDATE_PATH
s8 D_80146130;
Player D_80152818[4];
s8 D_80395ED0;
Event D_80395E70[4];
s16 D_80151AD0;
s32 D_80115F28[4][4][2];
s32 D_803940D0[4][4][2];
s8 D_80149428[4][4];
int call_count;
int call_args[3];
u8 call_ring[96];
s8 call_index, call_guard, call_remaining, call_score;
s16 call_selector;

typedef char player_size[(sizeof(Player) == 952) ? 1 : -1];
typedef char remaining_offset[(offsetof(Player, remaining) == 931) ? 1 : -1];
typedef char event_size[(sizeof(Event) == 24) ? 1 : -1];
typedef char event_fields[(offsetof(Event, active) == 0 &&
    offsetof(Event, age) == 1 && offsetof(Event, column) == 2 &&
    offsetof(Event, row) == 3 && offsetof(Event, from_x) == 4 &&
    offsetof(Event, from_y) == 8 && offsetof(Event, to_x) == 12 &&
    offsetof(Event, to_y) == 16 && offsetof(Event, delta) == 20) ? 1 : -1];

void func_800F7E30(s8 row, s8 column, s8 delta) {
    call_count++;
    call_args[0] = row;
    call_args[1] = column;
    call_args[2] = delta;
    memcpy(call_ring, D_80395E70, sizeof(call_ring));
    call_index = D_80395ED0;
    call_guard = D_80146130;
    call_remaining = D_80152818[row].remaining;
    call_score = D_80149428[row][column];
    call_selector = D_80151AD0;
    D_80149428[row][column] += delta;
}
