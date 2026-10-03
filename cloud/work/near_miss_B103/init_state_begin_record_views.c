/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned int u32;
/* External object views only; no local instance or sizeof-based stride. */
typedef struct VehicleView { u8 before1785[1785]; u8 option1785,other1786,option1787; } VehicleView;
typedef struct VehicleLink { VehicleView *vehicle; } VehicleLink;
typedef struct ModelView { u8 before44[44]; VehicleLink *link; } ModelView;
typedef struct SlotView { ModelView *model; } SlotView;
typedef struct Input76 { u8 before72[72]; SlotView *slot; } Input76;
typedef struct Settings40 {s8 value[40];} Settings40;
typedef struct Rule5 { u8 field[5]; } Rule5;
extern Input76 D_8014A118[];
extern s16 D_8014A108,D_80152734,D_80142724;
extern u32 D_801174B4;
extern int D_8014A110;
extern Settings40 D_80146108;
extern s8 D_80154628;
extern u8 D_801543D4;
extern Rule5 D_801543D8[];
extern void audio_bus_mix(SlotView *,u8,s8);
extern s8 D_80152570,D_80140A04,D_8013F1D9,D_8013F1D8,D_80142726,D_801543C8,D_80152030,D_80150EFC,D_80142760,D_8015F734,D_801613A9,D_801426EC,D_80156BDC,D_80156CE8,D_8015723C,D_8015B24C,D_8015B25C,D_8015F72C,D_8016137C,D_80161394,D_801613AA,D_801613A8,D_801613A0;
extern int D_801407BC,D_801407DC,D_80140A00,D_80140804,D_80140AD8,D_80140B08,D_80142510,D_80140BD8;
void init_state_begin(void)
{
    int player,channel;
    u32 flags;
    s8 *settings;
    Rule5 *rule;
    SlotView *slot;
    s16 *half_setting;
    flags=D_801174B4;
    if(!(flags&8)) {
        for(player=0;player<D_8014A108;player++) {
            if(D_8014A118[player].slot) {
                for(channel=0;channel<21;channel++) {
                    audio_bus_mix(D_8014A118[player].slot,(u8)channel,D_80146108.value[channel]);
                }
            }
        }
        flags=D_801174B4;
    }
    settings=(s8 *)(u32)&D_80146108;
    half_setting=&D_80142724;
    D_80156BDC=settings[0];
    D_80156CE8=settings[1];
    D_8015723C=settings[2];
    D_8015B24C=settings[3];
    D_8015B25C=settings[4];
    D_8015F72C=settings[5];
    D_8016137C=settings[7];
    D_80161394=settings[8];
    D_801613AA=settings[9];
    D_801613A8=settings[10];
    D_801613A0=settings[18];
    if(flags&8) {
        D_80152734=3; *half_setting=6; D_80152570=0; D_80140A04=0;
        D_8013F1D9=0; D_8013F1D8=0; D_80142726=0;
        D_801543C8=1; D_80152030=2; D_80150EFC=2; D_80142760=0;
    } else {
        switch(D_8014A110) {
        case 0:
            D_80152734=settings[21]; *half_setting=settings[22];
            if(D_8014A108>=3) *half_setting=0;
            D_80152570=settings[27]; D_80140A04=settings[28]>0; D_8013F1D9=settings[28]>=2;
            D_8013F1D8=settings[29]; D_80142726=settings[30]; D_801543C8=settings[23];
            D_80152030=settings[24]; D_80150EFC=settings[26]; D_80142760=settings[31];
            D_8015F734=settings[6]; D_801613A9=settings[11];
            break;
        case 1:
            D_80152734=1; *half_setting=0;
            D_80152570=settings[27]; D_80140A04=settings[28]>0; D_8013F1D9=settings[28]>=2;
            D_8013F1D8=settings[29]; D_80142726=settings[30]; D_801543C8=settings[23];
            D_80152030=settings[24]; D_80150EFC=settings[26]; D_80142760=0;
            D_8015F734=settings[6]; D_801613A9=settings[11];
            break;
        case 2:
            D_80152734=3; *half_setting=0; D_80152570=settings[27];
            D_80140A04=0; D_8013F1D9=0; D_8013F1D8=settings[29]; D_80142726=0;
            D_801543C8=1; D_80152030=2; D_801426EC=settings[25]; D_80150EFC=0; D_80142760=0;
            D_8015F734=settings[6]; D_801613A9=settings[11];
            break;
        case 3:
            D_80152734=3; *half_setting=5;
            rule=&D_801543D8[D_80154628];
            slot=D_8014A118[D_801543D4].slot;
            D_80152570=rule->field[1]; D_80140A04=rule->field[2]; D_8013F1D9=0;
            D_8013F1D8=rule->field[3]; D_80142726=rule->field[4]; D_801543C8=1;
            D_80152030=slot->model->link->vehicle->option1787; D_80150EFC=2;
            D_80142760=slot->model->link->vehicle->option1785;
            D_8015F734=settings[6]; D_801613A9=settings[11];
            break;
        case 4:
            D_80152734=1; *half_setting=0; D_80152570=0;
            D_80140A04=settings[28]>0; D_8013F1D9=settings[28]>=2; D_8013F1D8=0;
            D_80142726=settings[30]; D_801543C8=settings[23]; D_80152030=0; D_80150EFC=0; D_80142760=0;
            D_801407BC=settings[32]; D_801407DC=settings[33]; D_80140A00=settings[34];
            D_80140804=settings[35]; D_80140AD8=settings[36]; D_80156BDC=0; D_8015F734=0;
            D_801613A9=settings[11];
            break;
        case 5:
            D_80152734=1; *half_setting=0; D_80152570=0;
            D_80140A04=settings[28]>0; D_8013F1D9=settings[28]>=2; D_8013F1D8=0;
            D_80142726=0; D_801543C8=settings[23]; D_80152030=0; D_80150EFC=0; D_80142760=0;
            D_80156BDC=0; D_8015F734=0; D_801613A9=0;
            break;
        case 6:
            D_80152734=1; *half_setting=0; D_80152570=0;
            D_80140A04=settings[28]>0; D_8013F1D9=settings[28]>=2; D_8013F1D8=0;
            D_80142726=settings[30]; D_801543C8=settings[23]; D_80152030=0; D_80150EFC=0; D_80142760=0;
            D_80140B08=settings[37]; D_80142510=settings[38]; D_80140BD8=settings[39];
            D_8015F72C=0; D_80156BDC=0; D_8015F734=0; D_801613A9=0;
            break;
        }
    }
    if(flags&0x7c03fffe) {
        D_80140A04=0; D_8013F1D9=0;
    }
}
