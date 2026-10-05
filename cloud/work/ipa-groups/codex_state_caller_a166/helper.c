/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned int u32;
typedef struct ModelSlot {u8 other[1990];s16 handle;u8 rest[64];} ModelSlot;
typedef struct InputSlot {s8 index;u8 other[75];} InputSlot;
extern s8 D_8011415C,D_80114154,D_80152744,D_8014978C;
extern s16 D_80114158,D_80151AD0;
extern u32 state_word_a;
extern int gameplay_mode;
extern ModelSlot D_8014A250[];
extern InputSlot input_rec0[];
extern void *D_801391F0;
void func_800EC190(s16);
int display_list_flush(int,int);
void camera_race_setup(void);
void race_init_helper(void);
void func_800FBE60(void);
void render_viewport_init(void);
void func_800D5828(s16);
void players_frame_update(void);
void func_800D5374(void);
void records_screen(void);
void entity_transform_apply(void *,int);
void func_800B0580(void);
void func_800B1F30(s16);
void func_800B912C(s8);
void debug_stats(int start)
{
    s16 i;
    if (start) {
        if (!D_8011415C) {
            D_8011415C=1;
            D_80151AD0=1;
            if (!(state_word_a & 0x02000000)) {
                gameplay_mode=0;
                func_800EC190(D_80114158);
            }
        }
        if (!D_80114154 && display_list_flush(state_word_a & 8,0)) {
            camera_race_setup();
            race_init_helper();
            func_800FBE60();
            D_80114154=1;
        } else if (D_80114154) {
            render_viewport_init();
        }
    } else {
        D_8011415C=0;
        D_80114154=0;
        for (i=0;i<D_80152744;i++) func_800D5828(D_8014A250[i].handle);
        players_frame_update();
        func_800D5374();
        records_screen();
        while (D_801391F0) entity_transform_apply(D_801391F0,1);
        func_800B0580();
        for (i=0;i<D_80152744;i++) func_800B1F30(i);
        for (i=0;i<4;i++) input_rec0[i].index=i;
        if (!(state_word_a & 0x02000000)) func_800B912C(D_8014978C);
    }
}
