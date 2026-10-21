/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * drone_target_update (0x800D7E88, 496 B) = SetControls(int resetToDefault) of controls.c (MAT3 GameCube controls.c
 * SetControls + SetPlayerControls; w36n identity "lead", confirmed here by structure and globals).
 * For every player: the control bytes come from gDefaultControls (D_8013C228) or gNewControls[controller]
 * (D_8013C068, 10 bytes per controller on N64, read in place: no local copy, no rumble byte); if the player's
 * controller set changed (gControlsSet = D_80139300) it is cleared and the controls are saved to the player's
 * profile (SavePlayerControls = func_800CCE5C) and, without a profile or when resetting, to gJustPlayers
 * (D_80146150); then SetPlayerControls maps each byte through its gControlTable (D_801118E4) into
 * gPlayers[p].controls[] (gPlayers = D_8014A118, 76-byte records).
 * Shaping: SetPlayerControls is written in place as a block (GC has it as a separate function). As an inlined
 * static helper its parameter copy of p stays a coloured web (an s3 save with no code) and pushes the hoisted
 * &gDefaultControls (retail s8) out of the callee-saved set (111/124 words).
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct Player {
    u8 pad00;
    u8 controller;        /* 0x01 */
    u8 pad02[0x18 - 2];
    u32 controls[10];     /* 0x18 */
    u8 pad40[0x48 - 0x40];
    void **save;          /* 0x48 */
} Player;

extern s16 D_8014A108;              /* gNumPlayers */
extern Player D_8014A118[];         /* gPlayers */
extern s8 D_8013C228[10];           /* gDefaultControls */
extern s8 D_8013C068[][10];         /* gNewControls */
extern s8 D_80139300[];             /* gControlsSet */
extern void *D_80146150[];          /* gJustPlayers */
extern u32 D_801118E4[];            /* SetPlayerControls' gControlTable */
void func_800CCE5C(void **playerh, s8 *controls);  /* SavePlayerControls */

void drone_target_update(s32 resetToDefault)
{
    s32 p;
    s8 *controls;
    for (p = 0; p < D_8014A108; p++) {
        if (resetToDefault) {
            controls = D_8013C228;
        } else {
            controls = D_8013C068[D_8014A118[p].controller];
        }
        if (D_8014A118[p].controller != 5 && D_80139300[D_8014A118[p].controller]) {
            D_80139300[D_8014A118[p].controller] = 0;
            if (D_8014A118[p].save) func_800CCE5C(D_8014A118[p].save, controls);
            if (!D_8014A118[p].save || resetToDefault) func_800CCE5C(&D_80146150[D_8014A118[p].controller], controls);
        }
        {
            s32 i;
            for (i = 0; i < 10; i++) D_8014A118[p].controls[i] = D_801118E4[controls[i]];
        }
    }
}
