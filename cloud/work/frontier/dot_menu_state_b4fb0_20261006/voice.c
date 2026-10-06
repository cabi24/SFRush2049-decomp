/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* B438C menu heading layout. The real text destination starts at native
 * sp+72 in a 152-byte frame. The available 80-byte span is an inferred
 * capacity, not original-source proof. No dimension sweep or filler locals.
 */
typedef signed char s8;typedef unsigned char u8;typedef signed short s16;typedef unsigned short u16;typedef signed int s32;
#define F(p,t,o) (*(t *)((u8 *)(p)+(o)))
extern u8 D_801461D0[],D_80117280[],D_8014A160[],countdown_state[];
extern char D_801210B0[],D_801210C4[];
extern s32 D_801170FC,D_8015698C;
extern s8 D_80117350;
extern void *D_8011734C;
s32 osRecvMesg(void *,void *,s32);s32 osJamMesg(void *,void *,s32);
s32 slot_state_setup(s32);
void fcvt_wrapper(char *,const char *,...);
s32 object_manager_update(u8 *,s16);
s16 object_bytes_sum_global(void);
void *ambient_sound_set(s32,s32,s32,s32,s32,s32,s32,s32);
void crowd_cheer_play(void *,s32,s32,s32,s32);
void voice_stop(void)
{
    char text[80];
    s32 width,half;
    s16 x,y;
    osRecvMesg(D_801461D0,0,1);
    slot_state_setup(13);
    osJamMesg(D_801461D0,0,0);
    if(D_80117350) {
        fcvt_wrapper(text,D_801210B0,D_8015698C+1,F(F(countdown_state,void *,4),char *,932));
        width=object_manager_update((u8 *)text,-1);
    } else if(D_801170FC==7 || D_801170FC==8) {
        width=object_manager_update(F(countdown_state,u8 **,16)[F(F(countdown_state,void *,12),u16,112)+D_801170FC],-1);
    } else if(D_801170FC==6) {
        width=0;
    } else {
        fcvt_wrapper(text,D_801210C4,(u8 *)*F(D_8014A160+D_8015698C*76,void **,0)+20,F(countdown_state,char **,16)[F(F(countdown_state,void *,12),u16,112)]);
        width=object_manager_update((u8 *)text,-1);
    }
    x=F(D_80117280,s16,0);
    y=F(D_80117280,s16,2);
    if(!D_8011734C) D_8011734C=ambient_sound_set(-20,0,-10,10,176,0,0,0);
    if(!width) {
        crowd_cheer_play(D_8011734C,-20,0,-10,10);
        return;
    }
    half=width/2;
    crowd_cheer_play(D_8011734C,x-half-8,y-4,half+x+8,object_bytes_sum_global()+y+4);
}
