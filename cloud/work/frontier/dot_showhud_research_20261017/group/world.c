typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef struct OSMesgQueue OSMesgQueue;
extern OSMesgQueue D_801461D0;
extern s32 osRecvMesg(OSMesgQueue *, void **, s32);
extern s32 osSendMesg(OSMesgQueue *, void *, s32);
extern s32 slot_state_setup(s32 selection);

extern s32 D_80149B48;
extern f32 D_80114748;
extern s16 D_80149D92, D_80149D9E;
extern s8 D_80149DA0, D_80149B60, D_80149B70;
extern s32 D_80149B88;
extern s32 D_8002AFC4;
extern s8 D_8011474C, D_8011473C;

void func_800B6748(u8 *rgba,f32 *amount,s16 *x,s16 *y,s8 *bank,s8 *mode,s8 *variant) {
 rgba[0]=rgba[1]=rgba[2]=230;
 rgba[3]=255;
 *amount=-1.0f;
 *x=0;
 *y=0;
 *bank=-1;
 *mode=-1;
 *variant=-1;
}


void world_trigger_check(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
    func_800B6748((u8 *)&D_80149B48, &D_80114748, &D_80149D92, &D_80149D9E, &D_80149DA0, &D_80149B60, &D_80149B70);
    slot_state_setup(D_80149DA0);
    D_80149B88 = 0;
    if (D_8002AFC4 > 220) {
        D_80149B88 |= 0x8000;
    }
    D_8011474C = 0;
    D_8011473C = 0;
    osSendMesg(&D_801461D0, (void *)0, 0);
}

