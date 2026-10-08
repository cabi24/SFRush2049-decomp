/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Production-compatible adaptation of func_8001F954; sole 124-byte claim.
 * Standalone function: no inlined helpers, callers, or deleted-static stubs.
 * Reuses the accepted SequenceNode/VoiceState declarations without changing
 * their field contracts. The helper prototype is visible before this body.
 * Release/free the selected voice and clear its blocked byte.
 * MusyX family: synthvoice.c:voiceUnblock, AxioDL/musyx 78d2e16 (CC0).
 * N64 runtime reconstruction; no arcade equivalent. Native ABI/layout and
 * helper behavior remain authoritative over the newer public donor.
 */
typedef unsigned char u8;
typedef unsigned int u32;
#pragma pack(1)
typedef struct SequenceNode { struct SequenceNode *next; struct SequenceNode *previous; u32 key; int value; } SequenceNode;
typedef struct VoiceState { u32 command00; u8 unknown04[12]; u32 next_identifier; u32 parent_identifier; SequenceNode *entry18; u8 unknown1C[8]; u32 flags24; u32 value28; u8 unknown2C[32]; u8 external4C; u8 unknown4D[19]; u32 identifier60; u8 unknown64[89]; u8 activeBD; u8 unknownBE[226]; } VoiceState;
#pragma pack(0)
extern VoiceState D_8004BEB8[];
extern u8 func_8001467C(int);
extern void func_80014AF0(int);
extern void func_8001F6EC(VoiceState *state);

void func_8001F954(u32 index)
{
    if (index == 0xFFFFFFFFU) {
        return;
    }
    if (func_8001467C(index)) {
        func_80014AF0(index);
    }
    D_8004BEB8[index].identifier60 = index;
    func_8001F6EC(&D_8004BEB8[index]);
    D_8004BEB8[index].activeBD = 0;
}
