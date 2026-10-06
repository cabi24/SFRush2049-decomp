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
    s32 sp48;
    s16 temp_s5;
    s16 temp_s5_4;
    s16 temp_v1_2;
    s32 temp_s2;
    s32 temp_s2_2;
    s32 temp_s2_3;
    s32 temp_s4;
    s32 temp_s4_2;
    s32 temp_s4_3;
    s32 temp_s4_4;
    s32 temp_s5_2;
    s32 temp_s5_3;
    s32 temp_v0;
    s32 temp_v1;
    s32 var_s0;
    s32 var_s0_2;
    s32 var_s1;
    s32 var_s2;
    s32 var_t0;
    D_801170FC = arg0;
    func_800B4E68();
    switch(D_801170FC) {
    case 1:
        if(D_80117338 == 0) D_80117338=sound_control(0,0,D_80117284,1);
        voice_stop();
        osRecvMesg(D_801461D0,NULL,1);
        slot_state_setup(13);
        osJamMesg(D_801461D0,NULL,0);
        temp_s5=M2C_FIELD(D_80117280,s16 *,0);
        temp_s4=object_bytes_sum_global()*2+M2C_FIELD(D_80117280,s16 *,2);
        osRecvMesg(D_801461D0,NULL,1);
        slot_state_setup(10);
        osJamMesg(D_801461D0,NULL,0);
        var_s2=0;
        var_s0=0;
        do {
            temp_v0=object_manager_update(M2C_FIELD((M2C_FIELD(countdown_state,s32 *,0x10)+(M2C_FIELD(M2C_FIELD(countdown_state,void **,0xC),u16 *,0x70)*4)+var_s0),u8 **,4),-1);
            var_s0+=4;
            if(var_s2<temp_v0) var_s2=temp_v0;
        } while(var_s0!=0x20);
        temp_v1=var_s2/2;
        ambient_sound_set((temp_s5-temp_v1)-8,temp_s4-4,temp_v1+temp_s5+8,object_bytes_sum_global()*8+temp_s4+4,0xB0,0,0,0);
        D_8011733C=sound_control(0,0,D_801172A8,1);
        D_80117340=sound_control(0,0,D_801172CC,1);
        return;
    case 6:
        D_80117354=0;
        voice_stop();
        osRecvMesg(D_801461D0,NULL,1);
        slot_state_setup(10);
        osJamMesg(D_801461D0,NULL,0);
        temp_s5_2=M2C_FIELD(D_80117100,s32 *,0x28);
        temp_s4_2=M2C_FIELD(D_80117100,s32 *,0x2C)-object_bytes_sum_global()/2;
        sp48=object_manager_update(M2C_FIELD(M2C_FIELD(countdown_state,void **,4),u8 **,0x388),-1);
        temp_s2=sp48/2;
        ambient_sound_set((temp_s5_2-temp_s2)-8,temp_s4_2-4,temp_s2+temp_s5_2+8,object_bytes_sum_global()+temp_s4_2+4,0xB0,(s32)audio_channel_reset,0,0);
        osRecvMesg(D_801461D0,NULL,1);
        slot_state_setup(11);
        osJamMesg(D_801461D0,NULL,0);
        sp48=object_manager_update(M2C_FIELD(M2C_FIELD(countdown_state,void **,4),u8 **,0x2AC),-1);
        var_t0=sp48;
        if(object_manager_update(M2C_FIELD(M2C_FIELD(countdown_state,void **,4),u8 **,0x2A8),-1)>=(u32)var_t0)
            var_t0=object_manager_update(M2C_FIELD(M2C_FIELD(countdown_state,void **,4),u8 **,0x2A8),-1);
        temp_v1_2=active_player_count;
        var_s1=0;
        var_s0_2=0;
        if(temp_v1_2>0) {
            temp_s2_2=var_t0/2;
            do {
                temp_s5_3=M2C_FIELD((D_80117100+temp_v1_2*0x60+var_s0_2),s32 *,-0x50);
                temp_s4_3=M2C_FIELD((D_80117100+active_player_count*0x60+var_s0_2),s32 *,-0x4C)-object_bytes_sum_global();
                ambient_sound_set((temp_s5_3-temp_s2_2)-8,temp_s4_3-4,temp_s2_2+temp_s5_3+8,object_bytes_sum_global()*2+temp_s4_3+4,0xB0,0,0,0);
                var_s1++;
                var_s0_2+=0x18;
            } while(var_s1<active_player_count);
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
        temp_s5_4=M2C_FIELD(D_80117280,s16 *,0);
        temp_s4_4=object_bytes_sum_global()*2+M2C_FIELD(D_80117280,s16 *,2);
        osRecvMesg(D_801461D0,NULL,1);
        slot_state_setup(10);
        osJamMesg(D_801461D0,NULL,0);
        sp48=object_manager_update(M2C_FIELD(M2C_FIELD(countdown_state,void **,4),u8 **,0x360),-1);
        temp_s2_3=sp48/2;
        ambient_sound_set((temp_s5_4-temp_s2_3)-8,temp_s4_4-4,temp_s2_3+temp_s5_4+8,object_bytes_sum_global()*2+temp_s4_4+4,0xB0,0,0,0);
        D_8011733C=sound_control(0,0,D_801172A8,1);
        D_80117344=sound_control(0,0,D_801172F0,1);
        return;
    }
}
