/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* func_8001EF8C - lib_1f5b0 (IDO 7.1). Set a voice's priority (MusyX voiceSetPriority shape, control flow
 * from the GameCube MusyX decomp; Rush's build has no hwSetPriority call): if the voice (index =
 * identifier60 & 255) is already linked at this priority return, otherwise unlink it (func_8001EE9C =
 * voiceRemovePriority); push it on the head of the per-priority voice list D_80050548[channel]; if that list
 * was empty, insert the priority into the sorted priority root list D_80050648 (head D_80050A48, 0xFFFF = end).
 * w21b (from w20b best, true=310): the last test of the sorted insert reads the just-stored link back,
 * `if (D_80050648[channel].next != 65535)`, instead of testing `current`: uopt forwards the u16 store, so
 * retail's `andi t8,v1,0xffff` before the compare and the v0/v1 roles in the loop (current in v1, the compare
 * copy in v0) both follow (true=310 -> 0). Semantically identical (the field was just set to current).
 * w20b levers kept: `& 255` index into a u8 (copy-then-narrow, spill slot 39), `state->channel2E == channel`,
 * the assignment-in-test of link->next, the `break` loop; locals current/previous/link/index in that order.
 * No devices, no unused locals. Types: the TU's VoiceState (its later prototype `func_8001EF8C(VoiceState *, u8)`)
 * and SequenceNode, and VoicePrefix_8001EE9C / ChannelLink_8001EE9C / GroupLink_8001EE9C (as the locked
 * func_8001EE9C uses them; the call casts like func_8001F6EC). */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct SequenceNode { struct SequenceNode *next; struct SequenceNode *previous; u32 key; int value; } SequenceNode;
#pragma pack(0)
#pragma pack(1)
typedef struct VoiceState { u32 command00; u8 unknown04[12]; u32 next_identifier; u32 parent_identifier; SequenceNode *entry18; u8 unknown1C[8]; u32 flags24; u32 value28; u8 unknown2C[2]; u8 channel2E; u8 unknown2F[29]; u8 external4C; u8 unknown4D[19]; u32 identifier60; u8 unknown64[89]; u8 activeBD; u8 unknownBE[226]; } VoiceState;
#pragma pack(0)
#pragma pack(1)
typedef struct VoicePrefix_8001EE9C { u8 unknown00[46]; u8 channel2E; u8 unknown2F[49]; u32 identifier60; } VoicePrefix_8001EE9C;
#pragma pack(0)
typedef struct ChannelLink_8001EE9C { u8 previous, next; u16 active; } ChannelLink_8001EE9C;
typedef struct GroupLink_8001EE9C { u16 next, previous; } GroupLink_8001EE9C;
extern ChannelLink_8001EE9C D_800504C8[32];
extern u8 D_80050548[256];
extern GroupLink_8001EE9C D_80050648[256];
extern u16 D_80050A48;
extern void func_8001EE9C(VoicePrefix_8001EE9C *);
void func_8001EF8C(VoiceState *state, u8 channel)
{
    u16 current;
    u16 previous;
    ChannelLink_8001EE9C *link;
    u8 index;
    index = state->identifier60 & 255;
    link = &D_800504C8[index];
    if (link->active == 1) {
        if (state->channel2E == channel) return;
        func_8001EE9C((VoicePrefix_8001EE9C *)state);
    }
    link->active = 1;
    link->previous = 255;
    if ((link->next = D_80050548[channel]) != 255) {
        D_800504C8[D_80050548[channel]].previous = index;
    } else {
        if (D_80050A48 != 65535) {
            if (channel >= D_80050A48) {
                for (current = D_80050A48; current != 65535; current = D_80050648[current].next) {
                    if (channel < current) break;
                    previous = current;
                }
                D_80050648[previous].next = channel;
                D_80050648[channel].previous = previous;
                D_80050648[channel].next = current;
                if (D_80050648[channel].next != 65535) D_80050648[current].previous = channel;
            } else {
                D_80050648[channel].next = D_80050A48;
                D_80050648[channel].previous = 65535;
                D_80050648[D_80050A48].previous = channel;
                D_80050A48 = channel;
            }
        } else {
            D_80050648[channel].next = 65535;
            D_80050648[channel].previous = 65535;
            D_80050A48 = channel;
        }
    }
    D_80050548[channel] = index;
    state->channel2E = channel;
}
