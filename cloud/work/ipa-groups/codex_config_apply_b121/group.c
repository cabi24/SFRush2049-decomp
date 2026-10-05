/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef signed int s32;
typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
#define NULL ((void *)0)
#define M2C_FIELD(p,t,o) (*(t)((char *)(p)+(o)))
extern s32 D_80142728,D_801427A8;
extern void *func_80091B00(void);
extern s32 osRecvMesg(OSMesgQueue *,void *,s32);
extern s32 osJamMesg(OSMesgQueue *,void *,s32);
extern f32 D_80124280,D_801247EC;
extern s8 D_80146115,D_8010FFC0;
void speed_set(f32 blend,f32 amount,s8 first,s8 second) {
    f32 var_f0;
    void *temp_v0;

    osRecvMesg((OSMesgQueue *) &D_80142728, NULL, 1);
    temp_v0 = func_80091B00();
    M2C_FIELD(temp_v0, s8 *, 2) = 0xA;
    if (blend < 0.0f) {
        M2C_FIELD(temp_v0, f32 *, 4) = 0.0f;
    } else {
        if (blend > 1.0f) {
            var_f0 = 1.0f;
        } else {
            var_f0 = blend;
        }
        M2C_FIELD(temp_v0, f32 *, 4) = var_f0;
    }
    if (amount < 0.0f) {
        M2C_FIELD(temp_v0, f32 *, 8) = 0.0f;
    } else {
        M2C_FIELD(temp_v0, f32 *, 8) = (f32) amount;
    }
    M2C_FIELD(temp_v0, s8 *, 0xC) = (s8) first;
    M2C_FIELD(temp_v0, s8 *, 0xD) = (s8) second;
    osJamMesg((OSMesgQueue *) &D_80142728, NULL, 0);
    osJamMesg((OSMesgQueue *) &D_801427A8, temp_v0, 0);
}

void speed_mode0_wrapper(f32 blend,f32 amount) { speed_set(blend,amount,1,0); }
void speed_mode1_wrapper(f32 blend,f32 amount) { speed_set(blend,amount,0,1); }
void continue_prompt(void) { speed_set(0.0f,D_80124280,1,0); }
void vsync_wait(s32 flag) {
    f32 blend;
    D_8010FFC0=flag;
    if(flag) blend=0.0f;
    else blend=(f32)D_80146115/10.0f;
    speed_set(blend,D_801247EC,0,1);
}

typedef signed short s16;
typedef struct Config76 {
    s8 index;
    s8 state;
    unsigned char opaque[64];
    s8 active;
    unsigned char reserved;
    s32 handle;
    unsigned char tail[4];
} Config76;
extern Config76 D_8014A118[4];
extern s8 D_80146108[41];
extern s8 D_80146180[4];
extern f32 D_80124308,D_8012430C;
extern s16 D_801164A4[];
extern s16 D_80151AD0,D_8014A108;
extern void drone_target_update(s32);
extern s32 func_800CCB40(s32);
extern void func_800D6E00(s32);
extern s16 D_80152734;
extern s16 D_80142724;
extern s8 D_80156BDC;
extern s8 D_801543C8;
extern s8 D_80156CE8;
extern s8 D_8015723C;
extern s8 D_8015B24C;
extern s8 D_8015B25C;
extern s8 D_8015F72C;
extern s8 D_8015F734;
extern s8 D_8016137C;
extern s8 D_80161394;
extern s8 D_80152030;
extern s8 D_801426EC;
extern s8 D_80150EFC;
extern s8 D_80152570;
extern s8 D_80140A04;
extern s8 D_8013F1D9;
extern s8 D_8013F1D8;
extern s8 D_80142726;
extern s8 D_801613AA;
extern s8 D_80142760;
extern s8 D_801613A8;
extern s8 D_801613A9;
extern s16 D_8013FEC8;
extern s8 D_801613A0;
extern s32 D_801407BC;
extern s32 D_801407DC;
extern s32 D_80140804;
extern s32 D_80140A00;
extern s32 D_80140AD8;
extern s32 D_80140B08;
extern s32 D_80140BD8;
extern s32 D_80142510;
extern s8 D_8017A4D8;
extern s8 D_801427A0;
extern s8 D_8013FECC;
extern s8 D_8013FECD;
extern s8 D_80151AD8;
extern s8 D_801403D0;
extern s8 D_80142B00;
extern s8 D_80140418;
extern s8 D_8014061A;
extern s8 D_8013F2FC;
extern s8 D_80142DB0;
extern s8 D_80143F1C;
extern s8 D_8017A634;
extern s8 D_801427A1;
extern s8 D_801407B0;
extern s8 D_801613AB;
extern s8 D_801407D0;

void func_800DE45C(void)
{
    int i;
    s16 *state;
    D_8014A118[0].state=5;
    D_8014A118[0].active=0;
    D_8014A118[0].handle=-1;
    D_8014A118[0].index=0;
    D_8014A118[1].state=5;
    D_8014A118[1].active=0;
    D_8014A118[1].handle=-1;
    D_8014A118[1].index=1;
    D_8014A118[2].state=5;
    D_8014A118[2].handle=-1;
    D_8014A118[2].active=0;
    D_8014A118[2].index=2;
    D_8014A118[3].state=5;
    D_8014A118[3].active=0;
    D_8014A118[3].handle=-1;
    D_8014A118[3].index=3;
    drone_target_update(1);
    for(i=0;i<41;i++)D_80146108[i]=func_800CCB40(i);
    speed_set((f32)D_80146108[13]/10.0f,D_80124308,0,1);
    speed_set((f32)D_80146108[12]/10.0f,D_8012430C,1,0);
    func_800D6E00(D_80146108[15]);
    D_80152734=D_80146108[21];
    state=D_801164A4;
    D_80142724=D_80146108[22];
    D_80156BDC=D_80146108[0];
    D_801543C8=D_80146108[23];
    D_80156CE8=D_80146108[1];
    D_8015723C=D_80146108[2];
    D_8015B24C=D_80146108[3];
    D_8015B25C=D_80146108[4];
    D_8015F72C=D_80146108[5];
    D_8015F734=D_80146108[6];
    D_8016137C=D_80146108[7];
    D_80161394=D_80146108[8];
    D_80152030=D_80146108[24];
    D_801426EC=D_80146108[25];
    D_80150EFC=D_80146108[26];
    D_80152570=D_80146108[27];
    D_80140A04=D_80146108[28]>0;
    D_8013F1D9=D_80146108[28]>=2;
    D_8013F1D8=D_80146108[29];
    D_80142726=D_80146108[30];
    D_801613AA=D_80146108[9];
    D_80142760=D_80146108[31];
    D_801613A8=D_80146108[10];
    D_801613A9=D_80146108[11];
    D_8013FEC8=D_80146108[16];
    D_801613A0=D_80146108[18];
    D_801407BC=D_80146108[32];
    D_801407DC=D_80146108[33];
    D_80140804=D_80146108[35];
    D_80140A00=D_80146108[34];
    D_80140AD8=D_80146108[36];
    D_80140B08=D_80146108[37];
    D_80140BD8=D_80146108[39];
    D_80142510=D_80146108[38];
    D_8017A4D8=0;
    D_801427A0=0;
    D_8013FECC=0;
    D_8013FECD=0;
    state[3]=0;
    D_80151AD8=0;
    D_801403D0=0;
    D_80142B00=0;
    D_80140418=0;
    D_8014061A=0;
    D_8013F2FC=0;
    D_80142DB0=0;
    D_80143F1C=0;
    D_8017A634=0;
    state[13]=state[14]=state[15]=state[16]=0;
    D_801427A1=0;
    D_801407B0=0;
    D_801613AB=0;
    D_801407D0=0;
    D_80146180[1]=0;
    D_80146180[2]=0;
    D_80146180[3]=0;
    D_80146180[0]=0;
    D_80151AD0=1;
    D_8014A108=1;
}
