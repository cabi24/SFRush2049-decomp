/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research: native pause state setter and its genuine state-creator helper.
 * MAT3 records max,width,i,x,y; N64 has eight menu rows and different text
 * indices. The record fields below are donor-backed and native-offset checked.
 * Eight remaining words swap s1/s2 for the first-loop bound and maximum.
 * No accepted-image credit; see the complete receipt and negative controls. */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef signed int s32;typedef unsigned int u32;typedef float f32;
typedef struct Sprite Sprite;
s32 audio_channel_reset(Sprite *);
void *ambient_sound_set(s32,s32,s32,s32,s32,s32,s32,s32);
void func_800B4E68(void);
s16 object_bytes_sum_global(void);
u32 object_manager_update(void *,s16);
s32 osRecvMesg(void *,void *,s32);
s32 osSendMesg(void *,void *,s32);
s32 slot_state_setup(s32);
void *sound_control(s16,s16,void *,s16);
void voice_stop(void);
extern u8 D_80117284[],D_801172A8[],D_801172CC[],D_801172F0[],D_80117314[];
extern void *D_80117338,*D_8011733C,*D_80117340,*D_80117344,*D_80117348;
extern u8 D_801461D0[];
typedef struct { s32 x,y,w,h,cx,cy; } PlayerRect;
typedef struct { s16 x,y; } TitlePos;
typedef struct { u16 numSingle; u8 **single; u16 numTab,numTabA; u16 *tab; u8 **taba; u16 numTab2,numTab2a,numTab2b; u16 *tab2,*tab2a; u8 **tab2b; } TextData;
extern PlayerRect D_80117100[4][4];
extern TitlePos D_80117280;
extern TextData countdown_state;
#define NULL ((void *)0)
#define M2C_FIELD(p,t,o) (*(t)((u8 *)(p)+(o)))
extern s32 D_801170FC;
extern s8 D_80117354;
extern s16 active_player_count;
static void pause_lock(void) { osRecvMesg(D_801461D0,0,1); } static void pause_unlock(void) { osSendMesg(D_801461D0,0,0); } static s32 pause_font(s32 font) { s32 old; pause_lock(); old=slot_state_setup(font); pause_unlock(); return old; }
static void pause_create(void) {
    s32 max,width,i,x,y;
    switch(D_801170FC) {
    case 1:
        if(D_80117338 == 0) D_80117338=sound_control(0,0,D_80117284,1);
        voice_stop();
        pause_font(13);
        x=D_80117280.x;
        y=object_bytes_sum_global()*2+D_80117280.y;
        pause_font(10);
        max=0;
        for (i=0;i<8;i++) {
            width=object_manager_update(countdown_state.taba[i+1+countdown_state.tab[56]],-1);
            if(max<width) max=width;
        }
        ambient_sound_set(x-max/2-8,y-4,max/2+x+8,object_bytes_sum_global()*8+y+4,176,0,0,0);
        D_8011733C=sound_control(0,0,D_801172A8,1);
        D_80117340=sound_control(0,0,D_801172CC,1);
        return;
    case 6:
        D_80117354=0;
        voice_stop();
        pause_font(10);
        x=D_80117100[0][1].cx;
        y=D_80117100[0][1].cy-object_bytes_sum_global()/2;
        width=object_manager_update(countdown_state.single[226],-1);
        ambient_sound_set((x-width/2)-8,y-4,width/2+x+8,object_bytes_sum_global()+y+4,0xB0,(s32)audio_channel_reset,0,0);
        pause_font(11);
        width=object_manager_update(countdown_state.single[171],-1);
        width = (u32)width > object_manager_update(countdown_state.single[170],-1) ? width : object_manager_update(countdown_state.single[170],-1);
        for (max=0;max<active_player_count;max++) {
            x=D_80117100[active_player_count-1][max].cx;
            y=D_80117100[active_player_count-1][max].cy-object_bytes_sum_global();
            ambient_sound_set(x-width/2-8,y-4,width/2+x+8,object_bytes_sum_global()*2+y+4,176,0,0,0);
        }
        D_8011733C=sound_control(0,0,D_801172A8,1);
        D_80117348=sound_control(0,0,D_80117314,1);
        return;
    case 7:
    case 8:
        D_80117354=0;
        voice_stop();
        pause_font(13);
        x=D_80117280.x;
        y=object_bytes_sum_global()*2+D_80117280.y;
        pause_font(10);
        width=object_manager_update(countdown_state.single[216],-1);
        ambient_sound_set((x-width/2)-8,y-4,width/2+x+8,object_bytes_sum_global()*2+y+4,0xB0,0,0,0);
        D_8011733C=sound_control(0,0,D_801172A8,1);
        D_80117344=sound_control(0,0,D_801172F0,1);
        return;
    }
}

void func_800B4FB0(s32 state) {
    D_801170FC = state;
    func_800B4E68();
    pause_create();
}
