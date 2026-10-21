/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* drone_target_update = SetControls@controls.c (MAT3 GameCube controls.c SetControls, with SetPlayerControls
 * inlined). N64: 10 control bytes, read in place (no local copy, no rumble byte). */
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
/*@{*/extern s8 D_8013C228[10];/*@| extern s8 D_8013C228[]; @| extern s8 D_8013C228[11]; @}*/           /* gDefaultControls */
extern s8 D_8013C068[][10];         /* gNewControls */
extern s8 D_80139300[];             /* gControlsSet */
extern void *D_80146150[];          /* gJustPlayers */
extern u32 D_801118E4[];            /* SetPlayerControls' gControlTable */
void func_800CCE5C(void **playerh, s8 *controls);  /* SavePlayerControls */

static void SetPlayerControls(s32 player, s8 *controls)
{
    s32 i;
    for (i = 0; i < 10; i++) D_8014A118[player].controls[i] = D_801118E4[controls[i]];
}

void drone_target_update(s32 resetToDefault)
{
    /*@{*/s32 p;
    s8 *controls;/*@| s8 *controls; s32 p; @}*/
    for (p = 0; p < D_8014A108; p++) {
        /*@{*/if (resetToDefault) {
            controls = D_8013C228;
        } else {
            controls = D_8013C068[D_8014A118[p].controller];
        }/*@| controls = resetToDefault ? D_8013C228 : D_8013C068[D_8014A118[p].controller]; @| controls = D_8013C228; if (!resetToDefault) controls = D_8013C068[D_8014A118[p].controller]; @| if (!resetToDefault) controls = D_8013C068[D_8014A118[p].controller]; else controls = D_8013C228; @| if (resetToDefault) controls = &D_8013C228[0]; else controls = &D_8013C068[D_8014A118[p].controller][0]; @}*/
        if (D_8014A118[p].controller != 5 && D_80139300[D_8014A118[p].controller]) {
            D_80139300[D_8014A118[p].controller] = 0;
            if (D_8014A118[p].save) func_800CCE5C(D_8014A118[p].save, controls);
            if (!D_8014A118[p].save || resetToDefault) func_800CCE5C(&D_80146150[D_8014A118[p].controller], controls);
        }
        SetPlayerControls(p, controls);
    }
}
