/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* Repair candidate: use the shared func_80023754 pointer contract already
 * accepted with func_80023818 in src/rom/lib_22300.c. The _80023818 suffix
 * identifies that existing declaration; it is not a distinct runtime object.
 * Derived from cloud/work/boot_tail_promotion/sources/func_8002394C.c.
 * Only type spellings change; control offset, mask and body behavior do not.
 */
/* Native audio-macro wrapper: select an embedded control record and flag mask.
 * Original opcode names and complete state layout remain unknown.
 * No authenticated arcade counterpart is available in this checkout.
 */
typedef unsigned char u8;
typedef unsigned int u32;
typedef struct MacroState_80023818 MacroState_80023818;
typedef struct MacroControl_80023818 MacroControl_80023818;
typedef struct MacroCommand_80023818 {
    u32 word[2];
} MacroCommand_80023818;

extern void func_80023754(MacroState_80023818 *state, MacroControl_80023818 *control,
                         MacroCommand_80023818 *command, u32 flag);

u8 func_8002394C(MacroState_80023818 *state, MacroCommand_80023818 *command)
{
    func_80023754(state, (MacroControl_80023818 *)((u8 *)state + 0xE8),
                  command, 0x10000000);
    return 0;
}
