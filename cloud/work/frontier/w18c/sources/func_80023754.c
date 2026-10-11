/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_80023754 - lib_22300 (IDO 7.1). Shared body of the eight "add controller route" macro
 * commands (func_80023818 ... , locked, same TU): if FLAG is not yet set in state->flags24, or
 * the command's combination byte (word1 & 0xFF) is 0, set FLAG and empty the route table
 * CONTROL; then, if fewer than 4 routes are in use, append one: controller =
 * func_80021548(word0 >> 8) (MIDI controller translation, lib_21210), the combination byte,
 * and scale = (s16)(word0 >> 16) * 256 / 100.
 * w18c: (1) `combination = word[1] & 0xFF` - retail loads the whole word and narrows it in its
 * register; a bare u8 assignment folds to `lbu 7(a2)` under 7.1 (shaping: mask, disclosed);
 * (2) the translated controller goes through a named local `controller` declared between
 * index and combination: its slot (0x1E) puts index at 0x1F and combination at 0x1D as in
 * retail (without it: index 0x1E, combination 0x1F, strict=2); the local is used, not a pad.
 * Types: the TU's MacroState_80023818/MacroControl_80023818/MacroCommand_80023818 and the
 * func_80023754 prototype (func_80023818's block, verbatim), which sit AFTER this slot:
 * integration hoists those 4 lines above this slot (RESULTS.md "TU edits"); this file completes
 * the two incomplete structs (fields as D10, packed).
 * Prior: D10 BT05-control-dispatch (5.3, 22 words; 7.1 true=600). */
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;
typedef struct MacroState_80023818 MacroState_80023818;
typedef struct MacroControl_80023818 MacroControl_80023818;
typedef struct MacroCommand_80023818 { u32 word[2]; } MacroCommand_80023818;
extern void func_80023754(MacroState_80023818 *state, MacroControl_80023818 *control, MacroCommand_80023818 *command, u32 flag);
#pragma pack(1)
struct MacroState_80023818 { u8 unknown00[0x24]; u32 flags24; };
typedef struct ControlEntry_80023754 { u8 controller; u8 combination; s16 scale; } ControlEntry_80023754;
struct MacroControl_80023818 { ControlEntry_80023754 entries[4]; u8 count; };
#pragma pack()
extern u8 func_80021548(u8);

void func_80023754(MacroState_80023818 *state, MacroControl_80023818 *control,
                  MacroCommand_80023818 *command, u32 flag)
{
    u8 index;
    u8 controller;
    u8 combination;
    combination = command->word[1] & 0xFF;
    if (!(state->flags24 & flag) || combination == 0) {
        state->flags24 |= flag;
        control->count = 0;
    }
    if (control->count < 4) {
        index = control->count++;
        controller = func_80021548(command->word[0] >> 8);
        control->entries[index].controller = controller;
        control->entries[index].combination = combination;
        control->entries[index].scale = ((s32)(s16)(command->word[0] >> 16) * 256) / 100;
    }
}
