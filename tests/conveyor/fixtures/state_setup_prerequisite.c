#include <assert.h>
#include <stddef.h>
#include <string.h>
#include "layout.h"
s16 D_8014A108, D_8015274C, D_80152768, D_80153FD2;
s16 D_80153E84, D_80153F08, D_80153F24, D_80153F40;
s8 D_80152744;
SetupEntry76 D_8014A118[6]; SetupConfig8 D_80153E88[6];
SetupModel952 player_array[6]; SetupVehicle2056 D_8014A250[6];
s16 D_801527D8[16], D_80152808[16];
static int calls, continued, mutate; static int ids[16];
void music_tempo_set(s16 player, u8 mode, int apply) {
    assert(apply == 1); assert(mode == 20 + player);
    assert(player_array[player].flags_e8 == 0);
    assert(player_array[player].byte_359 == 0);
    assert(player_array[player].byte_ef == 0);
    assert(D_8014A250[player].word_7d4 == 0);
    ids[calls++] = player;
    if (mutate == 2 && calls == 1) D_8014A108 = 1;
    if (mutate == 3 && calls == 1) D_80152744 = 1;
    if (mutate == 1 && calls == 1) { D_80152768 = 3; D_8015274C = 2; }
}
void init_state_continue(void) {
    assert(D_80153E84 == D_8015274C);
    assert(D_80153F08 == D_8015274C);
    assert(D_80153F24 == -1 && D_80153F40 == 0);
    continued++;
}
static void execute_fragment(void) {
    int i; u8 player; SetupModel952 *model; SetupVehicle2056 *vehicle;
#include "fast_prepare.inc.c"
    assert(i == 0);
}
static void reset(int active, int total) {
    int i;
    memset(player_array, 0xA5, sizeof(player_array));
    memset(D_8014A250, 0xA5, sizeof(D_8014A250));
    memset(D_80153E88, 0xA5, sizeof(D_80153E88));
    memset(D_801527D8, 0xA5, sizeof(D_801527D8));
    memset(D_80152808, 0xA5, sizeof(D_80152808));
    for (i=0; i<6; i++) { D_8014A118[i].player=(u8)i; D_8014A250[i].mode=(u8)(20+i); }
    D_8014A108=(s16)active; D_80152744=(s8)total;
    D_80153FD2=12; calls=continued=mutate=0;
}
int main(void) {
    assert(sizeof(SetupModel952)==952 && sizeof(SetupVehicle2056)==2056);
    assert(sizeof(SetupEntry76)==76 && sizeof(SetupConfig8)==8);
    assert(offsetof(SetupModel952, byte_ef)==0xEF);
    assert(offsetof(SetupModel952, byte_359)==0x359);
    assert(offsetof(SetupModel952, word_380)==0x380);
    assert(offsetof(SetupVehicle2056, half_7ca)==0x7CA);
    assert(offsetof(SetupVehicle2056, word_7d4)==0x7D4);
    reset(2,4); D_8014A118[0].player=1; D_8014A118[1].player=0;
    D_80153E88[1].mode=3; execute_fragment();
    assert(calls==4 && continued==1 && D_80153FD2==0);
    assert(ids[0]==1 && ids[1]==0 && ids[2]==2 && ids[3]==3);
    assert(D_801527D8[0]==0 && D_80152808[1]==1);
    assert(D_80153E88[1].mode==253); /* unsigned-byte wrap */
    assert(player_array[0].word_380==0 && D_8014A250[0].half_7ca==1);
    assert(player_array[2].word_380==0xA5A5A5A5u);
    assert(D_8014A250[2].byte_a==0xA5 && D_8014A250[2].byte_7cc==0xA5);
    assert(player_array[4].flags_e8==0xA5A5A5A5u);
    reset(0,0); execute_fragment(); assert(calls==0 && continued==1);
    reset(-1,-1); execute_fragment(); assert(calls==0 && D_80153E84==0);
    reset(2,1); execute_fragment(); assert(calls==2 && D_80153E84==2);
    reset(2,-1); execute_fragment(); assert(calls==2 && D_80153E84==2);
    reset(1,1); mutate=1; execute_fragment();
    assert(D_801527D8[3]==0 && D_80152808[2]==0);
    assert(D_80152768==4 && D_8015274C==3);
    reset(3,4); mutate=2; execute_fragment();
    assert(calls==4 && ids[1]==1);
    assert(player_array[1].word_380==0xA5A5A5A5u);
    reset(0,4); mutate=3; execute_fragment();
    assert(calls==1 && D_80153E84==1);
    return 0;
}
