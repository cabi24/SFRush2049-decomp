typedef signed char s8;typedef unsigned char u8;typedef signed int s32;typedef unsigned int u32;
typedef struct List16 {s8 type,enabled;u8 pad2[2];void *word4,*first,*word12;} List16;
typedef struct Slot24 {u8 pad0[3];s8 used;u8 pad4[20];} Slot24;
typedef struct Record68 {u32 pad0[2];List16 *owner;s32 index;u8 pad16[52];} Record68;
typedef struct Record60 {u32 pad0[2];s32 index;u8 active,unused;u8 pad14[46];} Record60;
typedef struct Policy12 {void (*callback)(void);u32 word4,word8;} Policy12;
typedef struct OSMesgQueue OSMesgQueue;typedef struct OSThread OSThread;
extern s8 D_8011023C,D_80110274;
extern s32 D_80146200,D_80149410,D_80110264,D_8011024C,D_801461F8,D_80146204;
extern Record68 *D_80110244;extern void *D_80110248;extern u32 D_80110250;
extern Record60 *D_80110270;
extern Slot24 D_80142DD8[];extern u8 D_801439D8[];
extern OSMesgQueue D_80142728;extern void *D_80142768;
extern OSThread D_80034F00;extern u8 D_800339F0[];
extern List16 D_80143FF8,D_80144020,D_80144C50,D_80149808,D_80149860;
extern Policy12 D_80143F58;
void func_800C8918(void);
void osCreateMesgQueue(OSMesgQueue *,void **,s32);
s32 osJamMesg(OSMesgQueue *,void *,s32);
void osCreateThread(OSThread *,s32,void (*)(void *),void *,void *,s32);
void osStartThread(OSThread *);
void entity_ai_pathfind(void *);
void draw_sprites(void);
void car_shadow_render(u8,u8,u8);
void *audio_dma_sync(s32,s32);
void audio_loop_control(void *,s32);
void func_80091FBC(List16 *,void *,void *);
s32 func_800C8738(s32);
void hud_speed_display(s32,s32,s32,s32,s32);
#define CLEAR_LIST(list) do{(list).enabled=1;(list).type=0;(list).first=0;(list).word12=0;(list).word4=0;}while(0)
void hud_setup(s32 mode,u8 voices,u8 music,s32 config,s32 additional,s32 parameter5,s32 parameter6,s32 parameter7) {
    s32 old=0,index,power,limit;
    u32 offset;
    Slot24 *slot;
    if(D_8011023C){
        old=1;
        if(D_80146200==voices && D_80149410==music)return;
        func_800C8918();
    }
    D_80146200=voices;D_80149410=music;
    slot=D_80142DD8;
    do{slot->used=0;slot++;}while(slot<(Slot24 *)D_801439D8);
    if(!D_80110274){
        D_80110274=1;
        osCreateMesgQueue(&D_80142728,&D_80142768,1);
        osJamMesg(&D_80142728,0,0);
    }
    car_shadow_render((u8)mode,voices,music);
    if(!old){
        osCreateThread(&D_80034F00,10,entity_ai_pathfind,0,D_800339F0,10);
        osStartThread(&D_80034F00);
    }
    limit=D_80146200;
    D_80110264=limit*2<64?limit*2:64;
    limit+=additional;
    D_80110264=limit<D_80110264?D_80110264:limit;
    D_80110244=audio_dma_sync(0,D_80110264*68);
    audio_loop_control(D_80110244,0);
    limit=D_80110264;
    CLEAR_LIST(D_80143FF8);CLEAR_LIST(D_80144020);CLEAR_LIST(D_80144C50);
    D_80110248=audio_dma_sync(0,limit*12);
    audio_loop_control(D_80110248,0);
    D_80143F58.callback=draw_sprites;D_80143F58.word8=0;D_80143F58.word4=0;
    D_80110250=0;D_8011024C=0;
    for(index=0,offset=0;index<D_80110264;index++,offset+=68){
        ((Record68 *)((u8 *)D_80110244+offset))->index=index;
        func_80091FBC(&D_80143FF8,(u8 *)D_80110244+offset,D_80143FF8.first);
        ((Record68 *)((u8 *)D_80110244+offset))->owner=&D_80143FF8;
    }
    power=func_800C8738(D_80110264);
    limit=1<<power;
    D_801461F8=power;
    if(limit<D_80110264){D_801461F8=power+1;limit=1<<(power+1);}
    D_80146204=limit-1;
    D_80110270=audio_dma_sync(0,360);
    audio_loop_control(D_80110270,0);
    CLEAR_LIST(D_80149808);CLEAR_LIST(D_80149860);
    for(index=0,offset=0;index!=6;index++,offset+=60){
        ((Record60 *)((u8 *)D_80110270+offset))->index=index;
        ((Record60 *)((u8 *)D_80110270+offset))->active=1;
        ((Record60 *)((u8 *)D_80110270+offset))->unused=0;
        func_80091FBC(&D_80149808,(u8 *)D_80110270+offset,D_80149808.first);
    }
    hud_speed_display(config,additional,parameter5,parameter6,parameter7);
    D_8011023C=1;
}
