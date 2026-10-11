/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001F6EC - lib_1f5b0 (IDO 7.1). Release a voice back to its channel's free list:
 * func_8001EE9C(state) unlinks it from the active list (locked, same TU); then the voice's
 * command/channel are cleared and, if its channel link (D_80050440[identifier & 255]) is not
 * already free, the link is marked free and appended at the free-list tail (D_800504C0 head,
 * D_800504C1 tail, 255 = none); the external/internal free counters D_800504C2/D_800504C3
 * are decremented; the voice identifier becomes 0xFFFFFFFF.
 * w18c: in the empty-list arm the two link stores come before the head store
 * (`next, previous, head`); D10's head-first order gives a 2-row schedule residual (true=60).
 * Types: the TU's packed VoiceState (lib_1f5b0, verbatim) with its byte at +0x2E named
 * channel2E (the TU has it inside unknown2C[32]; layout-neutral TU edit, RESULTS.md). func_8001EE9C keeps the
 * TU's own prototype (VoicePrefix_8001EE9C, typedef copied verbatim) and the argument is cast:
 * type view only. ChannelLink/D_80050440 are the TU's declarations from func_8001F7EC's block,
 * which sits AFTER this slot: integration hoists those two lines above this slot (see
 * RESULTS.md "TU edits"). No other shaping devices.
 * Prior: D10 BT03-high-larger (5.3, 30 words; 7.1 true=60 reloc_blind=2). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef struct SequenceNode { struct SequenceNode *next; struct SequenceNode *previous; u32 key; int value; } SequenceNode;
#pragma pack(1)
typedef struct VoiceState { u32 command00; u8 unknown04[12]; u32 next_identifier; u32 parent_identifier; SequenceNode *entry18; u8 unknown1C[8]; u32 flags24; u32 value28; u8 unknown2C[2]; u8 channel2E; u8 unknown2F[29]; u8 external4C; u8 unknown4D[19]; u32 identifier60; u8 unknown64[89]; u8 activeBD; u8 unknownBE[226]; } VoiceState;
#pragma pack(0)
#pragma pack(1)
typedef struct VoicePrefix_8001EE9C { u8 unknown00[46]; u8 channel2E; u8 unknown2F[49]; u32 identifier60; } VoicePrefix_8001EE9C;
#pragma pack(0)
typedef struct ChannelLink { u8 previous; u8 next; u16 flags; } ChannelLink;
extern ChannelLink D_80050440[32];
extern u8 D_800504C0;
extern u8 D_800504C1;
extern unsigned char D_800504C2, D_800504C3;
extern void func_8001EE9C(VoicePrefix_8001EE9C *);

void func_8001F6EC(VoiceState *state)
{
    u32 index;
    ChannelLink *link;
    func_8001EE9C((VoicePrefix_8001EE9C *)state);
    index = state->identifier60 & 255;
    link = &D_80050440[index];
    state->command00 = 0;
    state->channel2E = 0;
    if (link->flags == 0) {
        link->flags = 1;
        if (D_800504C0 != 255) {
            link->previous = D_800504C1;
            link->next = 255;
            D_80050440[D_800504C1].next = index;
        } else {
            link->next = 255;
            link->previous = 255;
            D_800504C0 = index;
        }
        D_800504C1 = index;
        if (state->external4C) {
            --D_800504C2;
        } else {
            --D_800504C3;
        }
    }
    state->identifier60 = 0xFFFFFFFFU;
}
