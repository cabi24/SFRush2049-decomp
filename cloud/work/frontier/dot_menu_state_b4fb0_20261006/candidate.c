/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef signed int s32;typedef unsigned int u32;typedef float f32;
typedef struct Sprite Sprite;
s32 audio_channel_reset(Sprite *);
void *ambient_sound_set(s32,s32,s32,s32,s32,s32,s32,s32);
void func_800B4E68(void);
s16 object_bytes_sum_global(void);
s32 object_manager_update(u8 *,s16);
s32 osRecvMesg(void *,void *,s32);
s32 osJamMesg(void *,void *,s32);
s32 slot_state_setup(s32);
void *sound_control(s16,s16,void *,s16);
void voice_stop(void);
extern u8 D_80117100[],D_80117280[],D_80117284[],D_801172A8[],D_801172CC[],D_801172F0[],D_80117314[];
extern void *D_80117338,*D_8011733C,*D_80117340,*D_80117344,*D_80117348;
extern u8 D_801461D0[],countdown_state[];
#define NULL ((void *)0)
#define M2C_FIELD(p,t,o) (*(t)((u8 *)(p)+(o)))
extern s32 D_801170FC;
extern s8 D_80117354;
extern s16 active_player_count;
void func_800B4FB0(s32 arg0) {
    s32 x,y,width,half,i,offset,measured;
    s16 count;
    D_801170FC = arg0;
    func_800B4E68();
    switch(D_801170FC) {
    case 1:
        if(D_80117338 == 0) D_80117338=sound_control(0,0,D_80117284,1);
        voice_stop();
        osRecvMesg(D_801461D0,NULL,1);
        slot_state_setup(13);
        osJamMesg(D_801461D0,NULL,0);
        x=M2C_FIELD(D_80117280,s16 *,0);
        y=object_bytes_sum_global()*2+M2C_FIELD(D_80117280,s16 *,2);
        osRecvMesg(D_801461D0,NULL,1);
        slot_state_setup(10);
        osJamMesg(D_801461D0,NULL,0);
        width=0;
        offset=0;
        do {
            measured=object_manager_update(M2C_FIELD((M2C_FIELD(countdown_state,s32 *,0x10)+(M2C_FIELD(M2C_FIELD(countdown_state,void **,0xC),u16 *,0x70)*4)+offset),u8 **,4),-1);
            offset+=4;
            if(width<measured) width=measured;
        } while(offset!=0x20);
        half=width/2;
        ambient_sound_set((x-half)-8,y-4,half+x+8,object_bytes_sum_global()*8+y+4,0xB0,0,0,0);
        D_8011733C=sound_control(0,0,D_801172A8,1);
        D_80117340=sound_control(0,0,D_801172CC,1);
        return;
    case 6:
        D_80117354=0;
        voice_stop();
        osRecvMesg(D_801461D0,NULL,1);
        slot_state_setup(10);
        osJamMesg(D_801461D0,NULL,0);
        x=M2C_FIELD(D_80117100,s32 *,0x28);
        y=M2C_FIELD(D_80117100,s32 *,0x2C)-object_bytes_sum_global()/2;
        width=object_manager_update(M2C_FIELD(M2C_FIELD(countdown_state,void **,4),u8 **,0x388),-1);
        half=width/2;
        ambient_sound_set((x-half)-8,y-4,half+x+8,object_bytes_sum_global()+y+4,0xB0,(s32)audio_channel_reset,0,0);
        osRecvMesg(D_801461D0,NULL,1);
        slot_state_setup(11);
        osJamMesg(D_801461D0,NULL,0);
        width=object_manager_update(M2C_FIELD(M2C_FIELD(countdown_state,void **,4),u8 **,0x2AC),-1);
        if(object_manager_update(M2C_FIELD(M2C_FIELD(countdown_state,void **,4),u8 **,0x2A8),-1)>=(u32)width)
            width=object_manager_update(M2C_FIELD(M2C_FIELD(countdown_state,void **,4),u8 **,0x2A8),-1);
        count=active_player_count;
        i=0;
        offset=0;
        if(count>0) {
            half=width/2;
            do {
                x=M2C_FIELD((D_80117100+count*0x60+offset),s32 *,-0x50);
                y=M2C_FIELD((D_80117100+active_player_count*0x60+offset),s32 *,-0x4C)-object_bytes_sum_global();
                ambient_sound_set((x-half)-8,y-4,half+x+8,object_bytes_sum_global()*2+y+4,0xB0,0,0,0);
                i++;
                offset+=0x18;
            } while(i<active_player_count);
        }
        D_8011733C=sound_control(0,0,D_801172A8,1);
        D_80117348=sound_control(0,0,D_80117314,1);
        return;
    case 7:
    case 8:
        D_80117354=0;
        voice_stop();
        osRecvMesg(D_801461D0,NULL,1);
        slot_state_setup(13);
        osJamMesg(D_801461D0,NULL,0);
        x=M2C_FIELD(D_80117280,s16 *,0);
        y=object_bytes_sum_global()*2+M2C_FIELD(D_80117280,s16 *,2);
        osRecvMesg(D_801461D0,NULL,1);
        slot_state_setup(10);
        osJamMesg(D_801461D0,NULL,0);
        width=object_manager_update(M2C_FIELD(M2C_FIELD(countdown_state,void **,4),u8 **,0x360),-1);
        half=width/2;
        ambient_sound_set((x-half)-8,y-4,half+x+8,object_bytes_sum_global()*2+y+4,0xB0,0,0,0);
        D_8011733C=sound_control(0,0,D_801172A8,1);
        D_80117344=sound_control(0,0,D_801172F0,1);
        return;
    }
}
