/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800F7F3C (0x800F7F3C, 1,396 B) = MAT3 FindWinner@gamerecords.c (GC 0x80015E30, 1,340 B). Direct typed port.
 * N64 differences: no `ties = 0; tied[] = 0` prologue, and the last arm is a plain else (GC: gRace != kPractice).
 */
#define gNumWindows D_80151AD0
#define gNumPlayers D_8014A108
#define gRace D_8014A110
#define gRankedPlayer D_80143F54
#define model D_8014A250
#define gStuntPlayer D_80152038
#define gGameCar D_80152818
#define gTeamPoints D_8015256C
#define gCarTeam D_8012E67C
#define ties D_80150B60
#define tied D_80150B68

typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;

typedef struct tModelData { u8 pad000[0x7C6]; s16 slot; u8 pad7C8[0x808 - 0x7C8]; } tModelData;
typedef struct tStuntPlayer { u8 pad00[0x14]; s32 totalPoints; u8 pad18[0x78 - 0x18]; } tStuntPlayer;
typedef struct tCarData { u8 pad000[0xEE]; s8 place; u8 pad0EF[0x3A3 - 0xEF]; s8 Points; u8 pad3A4[952 - 0x3A4]; } tCarData;

extern s16 gNumWindows;
extern s16 gNumPlayers;
extern s32 gRace;
extern s8 gRankedPlayer[4];
extern tModelData model[];
extern tStuntPlayer gStuntPlayer[4];
extern tCarData gGameCar[];
extern s8 gTeamPoints[4];
extern s8 gCarTeam[4];
extern s32 ties;
extern s8 tied[4];

void func_800F7F3C(void)
{
    s32 i;
    s32 j;

    for (i = 0; i < gNumWindows; i++) gRankedPlayer[i] = i;
    if (gRace == 4) {
        for (i = 0; i < gNumPlayers - 1; i++) {
            for (j = 0; j < gNumPlayers - 1; j++) {
                if (gStuntPlayer[model[gRankedPlayer[j]].slot].totalPoints < gStuntPlayer[model[gRankedPlayer[j+1]].slot].totalPoints) {
                    s8 temp = gRankedPlayer[j+1];
                    gRankedPlayer[j+1] = gRankedPlayer[j];
                    gRankedPlayer[j] = temp;
                }
            }
        }
        for (i = 1; i < gNumPlayers; i++) {
            if (gStuntPlayer[model[gRankedPlayer[0]].slot].totalPoints == gStuntPlayer[model[gRankedPlayer[i]].slot].totalPoints) {
                ties++;
                tied[i] = 1;
            }
        }
    } else if (gRace == 6) {
        for (i = 0; i < gNumPlayers - 1; i++) {
            for (j = 0; j < gNumPlayers - 1; j++) {
                if (gGameCar[model[gRankedPlayer[j]].slot].Points < gGameCar[model[gRankedPlayer[j+1]].slot].Points) {
                    s8 temp = gRankedPlayer[j+1];
                    gRankedPlayer[j+1] = gRankedPlayer[j];
                    gRankedPlayer[j] = temp;
                }
            }
        }
        for (i = 0; i < gNumPlayers - 1; i++) {
            for (j = 0; j < gNumPlayers - 1; j++) {
                if (gTeamPoints[gCarTeam[model[gRankedPlayer[j]].slot]] < gTeamPoints[gCarTeam[model[gRankedPlayer[j+1]].slot]]) {
                    s8 temp = gRankedPlayer[j+1];
                    gRankedPlayer[j+1] = gRankedPlayer[j];
                    gRankedPlayer[j] = temp;
                }
            }
        }
        for (i = 1; i < gNumPlayers; i++) {
            if (gGameCar[model[gRankedPlayer[0]].slot].Points == gGameCar[model[gRankedPlayer[i]].slot].Points) {
                ties++;
                tied[i] = 1;
            }
        }
    } else {
        for (i = 0; i < gNumPlayers - 1; i++) {
            for (j = 0; j < gNumPlayers - 1; j++) {
                if (gGameCar[model[gRankedPlayer[j]].slot].place > gGameCar[model[gRankedPlayer[j+1]].slot].place) {
                    s8 temp = gRankedPlayer[j+1];
                    gRankedPlayer[j+1] = gRankedPlayer[j];
                    gRankedPlayer[j] = temp;
                }
            }
        }
        for (i = 1; i < gNumPlayers; i++) {
            if (gGameCar[model[gRankedPlayer[0]].slot].place == gGameCar[model[gRankedPlayer[i]].slot].place) {
                ties++;
                tied[i] = 1;
            }
        }
    }
}
