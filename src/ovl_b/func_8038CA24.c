/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
/*
 * Runtime image B only: 0x8038CA24..0x8038CB10, 236 bytes.
 * Reset per-player battle/HUD state, group handle/timer, and five slot handles.
 * Reconstructed from the protected B-image body and its game callers; no
 * known arcade analogue or original N64 declarations are claimed.
 * Field names are descriptive hypotheses; widths/offsets/strides are native.
 * Caller supplies a signed 16-bit player index backed by every named array.
 * Tests establish whole-object behavior for fixture indices 0..3; neither
 * broader caller bounds nor negative C-array indexing are asserted valid.
 * Unknown record bytes preserve observed layout; they are not stack padding.
 */
typedef signed char s8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;

typedef struct PlayerHudState {
    unsigned char unknown000[0x384];
    s8 mode;
    s8 selection;
    unsigned char unknown386[6];
    s32 counter;
    unsigned char unknown390[0x10];
    s8 state;
    unsigned char unknown3a1;
    s8 flags;
    unsigned char unknown3a3;
    s32 timer;
    unsigned char unknown3a8[0x10];
} PlayerHudState;

typedef struct HudGroup {
    unsigned char unknown000[0x104];
    s32 handle;
    unsigned char unknown108[0x30];
    s8 selection;
    unsigned char unknown139[0xB];
    f32 timer;
} HudGroup;

typedef struct HudSlots {
    s32 handle[5];
    unsigned char unknown014[0xF4];
    f32 timer;
} HudSlots;

extern PlayerHudState D_80152818[];
extern s8 D_80399118[];
extern HudGroup D_80399550[];
extern HudSlots D_80399120[];

void func_8038CA24(s16 player)
{
    PlayerHudState *state;
    s32 i;

    state = &D_80152818[player];
    state->counter = 0;
    state->flags = 0;
    state->state = 0;
    state->timer = 0;
    state->mode = 8;
    state->selection = -1;
    D_80399118[player] = 9;
    D_80399550[player].handle = -1;
    D_80399550[player].selection = -1;
    D_80399550[player].timer = 0.0f;
    D_80399120[player].timer = 0.0f;
    for (i = 0; i < 5; i++) {
        D_80399120[player].handle[i] = -1;
    }
}
