/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001DC08 - lib_1cf90 (IDO 7.1). Starts pending spatial voices: for each emitter group
 * D_8004FDA0[0..D_8004FF20) walks the pending (large) list; while an active (small) list remains, a
 * pending entry quieter than the active head by at most D_8002D908 is skipped, one within D_8002D904 is
 * skipped until its counter reaches 20, a louder one resets the counter. Otherwise the emitter's sound
 * is started (func_8001B1D0(sound_id, 127, 64)); on failure the emitter is marked 0x40000 (unless flag 2),
 * on success it gets fade 0, flag 0x100000, its mix (func_8001CCDC) and the active head advances; 0x20000
 * is cleared either way.
 * w20b levers (w18c dc1 = true=140, f20/f22 swapped): (1) line layout: `group = ...; lower = ...;` and the
 * `do {` on ONE source line (as1 then schedules the preheader as retail: lwc1 upper, lui/ori/addiu, lwc1
 * lower; on separate lines true=60..150); (2) SHAPING DEVICE (disclosed): a compiled-out read
 * `if (lower) {}` on that line gives `lower` the colour tie (f20; retail colours lower f20, upper f22).
 * Types: the TU's EmitGroup / LargeNode / SmallNode (lib_1cf90, func_8001D998 block) and
 * boot_tail_sample_contract.h's StateNode (flags08, identifier34 = voice handle, sound_id, counter, fade).
 * No unused locals. */
#include "boot_tail_sample_contract.h"
typedef struct SmallNode { struct SmallNode *next; float distance; StateNode *object; } SmallNode;
typedef struct LargeNode { struct LargeNode *next; float distance, first, second, third, fourth; StateNode *object; } LargeNode;
typedef struct EmitGroup { unsigned int identifier; LargeNode *large; SmallNode *small; } EmitGroup;
extern EmitGroup D_8004FDA0[32];
extern unsigned char D_8004FF20;
extern float D_8002D904, D_8002D908;
extern unsigned int func_8001B1D0(unsigned short, unsigned char, unsigned char);
extern void func_8001CCDC(StateNode *em, float vol, float xPan, float yPan, float zPan, float doppler);

void func_8001DC08(void)
{
    int i;
    EmitGroup *group;
    LargeNode *entry;
    StateNode *state;
    float difference, upper, lower;
    i = 0;
    if (D_8004FF20 > 0) {
        upper = D_8002D904;
        group = D_8004FDA0; lower = D_8002D908; if (lower) {} do {
            for (entry = group->large; entry != 0; entry = entry->next) {
                if (group->small != 0) {
                    difference = entry->distance - group->small->distance;
                    if (difference <= lower) continue;
                    if (difference <= upper) {
                        entry->object->counter++;
                        if (entry->object->counter < 20) continue;
                    } else entry->object->counter = 0;
                }
                state = entry->object;
                state->identifier34 = func_8001B1D0(state->sound_id, 127, 64);
                if (state->identifier34 == 0xFFFFFFFFU) {
                    if (!(state->flags08 & 2)) {
                        state->flags08 |= 0x40000;
                        state->flags08 &= ~0x20000U;
                    }
                } else {
                    state->fade = 0.0f;
                    state->flags08 |= 0x100000;
                    func_8001CCDC(state, entry->distance, entry->first, entry->second, entry->third, entry->fourth);
                    state->flags08 &= ~0x20000U;
                    if (group->small != 0) group->small = group->small->next;
                }
            }
            i++; group++;
        } while (i < D_8004FF20);
    }
}
