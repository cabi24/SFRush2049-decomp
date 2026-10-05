/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
extern s16 D_80151AD0;
extern u8 D_80116DBC[];
extern s32 sound_control(s32,s32,void *,s32);
extern void display_settings(s32,s32,s32,s32,s32,s32);
extern void func_800A7428(s16 *,s16 *,u8 *,u8 *,u8 *,u8 *,s32);
extern void func_800A7480(s32,s32,u32,u32,u32,u32,s32);
extern s32 osPfsReAllocate(s32);
extern void game_mode_handler(void);
extern void func_800C813C(s32,s32);
extern void control_settings(s32,u32,u32,s8 *,s32 (*)(void));
extern void sound_stop(s32);
void save_settings(s32 menu,s32 mode,u32 abi2,u32 abi3,u32 input4,u32 input5,s32 (*poll)(void))
{
    s16 x[4],y[4];
    u8 red[4],green[4],blue[4],alpha[4];
    s32 sound,i;
    s8 done=0;
    sound=sound_control(0,0,D_80116DBC,1);
    display_settings(menu,mode,10,10,300,180);
    for(i=0;i<D_80151AD0;i++) {
        func_800A7428(&x[i],&y[i],&red[i],&green[i],&blue[i],&alpha[i],i);
        func_800A7480(1000,999,0,0,0,255,i);
    }
    while (!done) {
        if(osPfsReAllocate(0)==7) game_mode_handler();
        if(mode!=11 && mode!=12 && mode!=13 && mode!=10) func_800C813C(0,0);
        control_settings(menu,input4,input5,&done,poll);
    }
    sound_stop(sound);
    for(i=0;i<D_80151AD0;i++) {
        func_800A7480(x[i],y[i],red[i],green[i],blue[i],alpha[i],i);
    }
}
