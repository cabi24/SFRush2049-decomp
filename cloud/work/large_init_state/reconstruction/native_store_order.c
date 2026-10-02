/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Stats { u8 pad[1785]; u8 option_a; u8 pad1786; u8 option_b; } Stats;
typedef struct Profile { Stats *stats; } Profile;
typedef struct PlayerState { u8 pad[44]; Profile *profile; } PlayerState;
typedef struct Player { PlayerState *state; } Player;
typedef struct InputRecord { u8 pad[72]; Player *player; } InputRecord;
extern InputRecord input_rec0[];
extern s16 active_player_count;
extern u32 state_word_a;
extern s32 gameplay_mode;
extern s8 D_80146108[];
extern u8 D_801543D8[][5];
extern s8 D_80154628;
extern u8 D_801543D4;
void audio_bus_mix(Player *, u8, s8);
extern s8 D_80156BDC;
extern s8 D_80156CE8;
extern s8 D_8015723C;
extern s8 D_8015B24C;
extern s8 D_8015B25C;
extern s8 D_8015F72C;
extern s8 D_8016137C;
extern s8 D_80161394;
extern s8 D_801613AA;
extern s8 D_801613A8;
extern s8 D_801613A0;
extern s8 D_80152570;
extern s8 D_80140A04;
extern s8 D_8013F1D9;
extern s8 D_8013F1D8;
extern s8 D_80142726;
extern s8 D_801543C8;
extern s8 D_80152030;
extern s8 D_80150EFC;
extern s8 D_80142760;
extern s8 D_8015F734;
extern s8 D_801613A9;
extern s8 D_801426EC;
extern s16 D_80152734;
extern s16 D_80142724;
extern s32 D_801407BC;
extern s32 D_801407DC;
extern s32 D_80140A00;
extern s32 D_80140804;
extern s32 D_80140AD8;
extern s32 D_80140B08;
extern s32 D_80142510;
extern s32 D_80140BD8;
void init_state_begin(void)
{
    s32 i, setting;
    s8 configuration_mode;
    u8 *options;
    Player *player;
    if (!(state_word_a & 8)) {
        for (i = 0; i < active_player_count; i++) {
            if (input_rec0[i].player != 0) {
                for (setting = 0; setting < 21; setting++) {
                    audio_bus_mix(input_rec0[i].player, (u8)setting, D_80146108[setting]);
                }
            }
        }
    }
    D_80156BDC = D_80146108[0];
    D_80156CE8 = D_80146108[1];
    D_8015723C = D_80146108[2];
    D_8015B24C = D_80146108[3];
    D_8015B25C = D_80146108[4];
    D_8015F72C = D_80146108[5];
    D_8016137C = D_80146108[7];
    D_80161394 = D_80146108[8];
    D_801613AA = D_80146108[9];
    D_801613A8 = D_80146108[10];
    D_801613A0 = D_80146108[18];
    if (state_word_a & 8) {
        D_80152734 = 3;
        D_80142724 = 6;
        D_80152570 = 0;
        D_80140A04 = 0;
        D_8013F1D9 = 0;
        D_8013F1D8 = 0;
        D_80142726 = 0;
        D_801543C8 = 1;
        D_80152030 = 2;
        D_80150EFC = 2;
        D_80142760 = 0;
    } else {
        switch (gameplay_mode) {
        case 0:
            D_80152734 = D_80146108[21];
            D_80142724 = D_80146108[22];
            if (active_player_count >= 3) D_80142724 = 0;
            D_80152570 = D_80146108[27];
            configuration_mode = D_80146108[28];
            D_80140A04 = configuration_mode > 0;
            D_8013F1D9 = configuration_mode >= 2;
            D_8013F1D8 = D_80146108[29];
            D_80142726 = D_80146108[30];
            D_801543C8 = D_80146108[23];
            D_80152030 = D_80146108[24];
            D_80150EFC = D_80146108[26];
            D_80142760 = D_80146108[31];
            D_8015F734 = D_80146108[6];
            D_801613A9 = D_80146108[11];
            break;
        case 1:
            D_80152734 = 1;
            D_80142724 = 0;
            D_80152570 = D_80146108[27];
            configuration_mode = D_80146108[28];
            D_80140A04 = configuration_mode > 0;
            D_8013F1D9 = configuration_mode >= 2;
            D_8013F1D8 = D_80146108[29];
            D_80142726 = D_80146108[30];
            D_801543C8 = D_80146108[23];
            D_80152030 = D_80146108[24];
            D_80150EFC = D_80146108[26];
            D_80142760 = 0;
            D_8015F734 = D_80146108[6];
            D_801613A9 = D_80146108[11];
            break;
        case 2:
            D_80152734 = 3;
            D_80142724 = 0;
            D_80152570 = D_80146108[27];
            D_80140A04 = 0;
            D_8013F1D9 = 0;
            D_8013F1D8 = D_80146108[29];
            D_80142726 = 0;
            D_801543C8 = 1;
            D_80152030 = 2;
            D_801426EC = D_80146108[25];
            D_80150EFC = 0;
            D_80142760 = 0;
            D_8015F734 = D_80146108[6];
            D_801613A9 = D_80146108[11];
            break;
        case 3:
            D_80152734 = 3;
            D_80142724 = 5;
            options = D_801543D8[D_80154628];
            D_80152570 = options[1];
            D_80140A04 = options[2];
            D_8013F1D9 = 0;
            D_8013F1D8 = options[3];
            D_80142726 = options[4];
            D_801543C8 = 1;
            player = input_rec0[D_801543D4].player;
            D_80152030 = player->state->profile->stats->option_b;
            D_80150EFC = 2;
            D_80142760 = player->state->profile->stats->option_a;
            D_8015F734 = D_80146108[6];
            D_801613A9 = D_80146108[11];
            break;
        case 4:
            D_80152734 = 1;
            D_80142724 = 0;
            D_80152570 = 0;
            configuration_mode = D_80146108[28];
            D_80140A04 = configuration_mode > 0;
            D_8013F1D9 = configuration_mode >= 2;
            D_8013F1D8 = 0;
            D_80142726 = D_80146108[30];
            D_801543C8 = D_80146108[23];
            D_80152030 = 0;
            D_80150EFC = 0;
            D_80142760 = 0;
            D_801407BC = D_80146108[32];
            D_801407DC = D_80146108[33];
            D_80140A00 = D_80146108[34];
            D_80140804 = D_80146108[35];
            D_80140AD8 = D_80146108[36];
            D_80156BDC = 0;
            D_8015F734 = 0;
            D_801613A9 = D_80146108[11];
            break;
        case 5:
            D_80152734 = 1;
            D_80142724 = 0;
            D_80152570 = 0;
            configuration_mode = D_80146108[28];
            D_80140A04 = configuration_mode > 0;
            D_8013F1D9 = configuration_mode >= 2;
            D_8013F1D8 = 0;
            D_80142726 = 0;
            D_801543C8 = D_80146108[23];
            D_80152030 = 0;
            D_80150EFC = 0;
            D_80142760 = 0;
            D_80156BDC = 0;
            D_8015F734 = 0;
            D_801613A9 = 0;
            break;
        case 6:
            D_80152734 = 1;
            D_80142724 = 0;
            D_80152570 = 0;
            configuration_mode = D_80146108[28];
            D_80140A04 = configuration_mode > 0;
            D_8013F1D9 = configuration_mode >= 2;
            D_8013F1D8 = 0;
            D_80142726 = D_80146108[30];
            D_801543C8 = D_80146108[23];
            D_80152030 = 0;
            D_80150EFC = 0;
            D_80142760 = 0;
            D_8015F72C = 0;
            D_80140B08 = D_80146108[37];
            D_80142510 = D_80146108[38];
            D_80140BD8 = D_80146108[39];
            D_80156BDC = 0;
            D_8015F734 = 0;
            D_801613A9 = 0;
            break;
        }
    }
    if (state_word_a & 0x7C03FFFE) {
        D_80140A04 = 0;
        D_8013F1D9 = 0;
    }
}
