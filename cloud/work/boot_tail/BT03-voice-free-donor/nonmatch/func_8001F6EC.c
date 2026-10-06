/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/* COMPLETE-NONMATCH: 28/64 words at the pinned IDO O2 recipe.
 * N64 runtime voiceFree reconstruction; no arcade equivalent is claimed.
 * Donor topology: AxioDL/musyx synthvoice.c:voiceFree, commit
 * 78d2e16e4905fc675952162d331c24d5198b2687 (CC0-1.0).
 * Retain the native one-helper call and 416-byte packed N64 state layout.
 * Newer donor macMakeInactive and platform/debug calls are absent in native.
 * Unknown spans describe actual object storage, never stack padding.
 */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
#pragma pack(1)
typedef struct VoiceState {
    u32 command00;
    u8 unknown04[12];
    u32 next10;
    u8 unknown14[16];
    u32 flags24;
    u8 unknown28[3];
    u8 channel2B;
    u8 unknown2C[2];
    u8 channel2E;
    u8 unknown2F[28];
    u8 channel4B;
    u8 external4C;
    u8 unknown4D[8];
    u8 channel55;
    u8 unknown56[10];
    u32 identifier60;
    u8 unknown64[316];
} VoiceState;
#pragma pack(0)
typedef struct FreeLink { u8 previous, next; u16 active; } FreeLink;
extern FreeLink D_80050440[32];
extern u8 D_800504C0, D_800504C1, D_800504C2, D_800504C3;
extern void func_8001EE9C(VoiceState *);
void func_8001F6EC(VoiceState *state)
{
    u32 index;
    FreeLink *link;
    func_8001EE9C(state);
    state->command00 = 0;
    state->channel2E = 0;
    link = &D_80050440[(index = state->identifier60 & 255)];
    if (link->active == 0) {
        link->active = 1;
        if (D_800504C0 != 255) {
            link->next = 255;
            link->previous = D_800504C1;
            D_80050440[D_800504C1].next = index;
        } else {
            link->next = 255;
            link->previous = 255;
            D_800504C0 = index;
        }
        D_800504C1 = index;
        if (state->external4C) --D_800504C2;
        else --D_800504C3;
    }
    state->identifier60 = 0xFFFFFFFFU;
}
