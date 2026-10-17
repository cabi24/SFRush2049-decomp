/* Genuine TB_DrawText: no automatic locals. Native unscaled SetTextPos. */
typedef signed short s16;typedef unsigned int u32;
typedef enum { J_LEFT=0,J_CENTER=1,J_RIGHT=2,J_BOTTOM=3 } JustEnum;
typedef struct {JustEnum h,v;} Just;
extern Just D_80118E20;
u32 object_manager_update(unsigned char *,s16);
s16 object_bytes23_sum(void);
void func_800B70EC(s16,s16);
void menu_input_process(void *,s16);
s16 sound_pitch_diff_calc(void *text,s16 x){return x-object_manager_update(text,-1);}
s16 sound_pitch_diff_halved(void *text,s16 x){return x-(object_manager_update(text,-1)>>1);}
void state_utility(s16 x,s16 y,void *text)
{
 if(D_80118E20.h==1)x=sound_pitch_diff_halved(text,x);
 else if(D_80118E20.h==2)x=sound_pitch_diff_calc(text,x);
 if(D_80118E20.v==1)y-=object_bytes23_sum()/2;
 else if(D_80118E20.v==4)y-=object_bytes23_sum();
 func_800B70EC(x,y);
 menu_input_process(text,-1);
}
