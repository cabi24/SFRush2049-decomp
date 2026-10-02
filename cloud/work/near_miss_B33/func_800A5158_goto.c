/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
extern s8 D_80118EE4;
extern s16 D_8013FEC8;
extern u8 D_8002E860,D_80140A10;
extern s32 D_80140AF0;
extern s32 audio_frame_sync(s32,s32,s32,s32,s32);
extern void display_list_alloc(s32),func_800A4E58(void),func_800A510C(void);
void func_800A5158(void) {
    if (D_80118EE4!=0) goto end;
    {
        D_80118EE4=1;
        D_8013FEC8=0;
        D_80140A10=D_8002E860;
        D_80140AF0=audio_frame_sync(D_8002E860,0,0,0,0);
        display_list_alloc(D_80140AF0);
        func_800A4E58();
        func_800A510C();
    }
end:;
}
