#ifndef DOT_MENU_OPTIONS_CONTEXT_H
#define DOT_MENU_OPTIONS_CONTEXT_H
/* Research-only declarations, scoped to the native menu root. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef union Color4 { struct { u8 r, g, b, a; } channels; u32 rgba; } Color4;
typedef struct ResourceHeader { u8 prefix[32]; u16 string_offset; } ResourceHeader;
typedef struct MenuAssets {
    void *first;
    void **labels;
    void *third;
    ResourceHeader *header;
    void **values;
} MenuAssets;
extern Color4 D_801146BC, D_801146C0;
extern s32 D_80116D0C;
extern s32 D_80116D14[];
extern s8 D_80116DA8;
extern u32 D_801174B4;
extern s32 D_8015698C;
extern char D_80121018[];
extern s16 D_80149DA2, D_80149B84, D_8013FEC8;
extern s8 D_80146108[];
extern void *D_801164D0[];
extern MenuAssets D_8017A4E0;
extern u8 D_801461D0[];
extern void render_helper(f32);
extern s32 osRecvMesg(void *, void *, s32);
extern s32 osJamMesg(void *, void *, s32);
extern s32 slot_state_setup(s32);
extern void dispatch_handler(s32);
extern void fcvt_wrapper(char *, char *, ...);
extern u32 object_manager_update(void *, s16);
extern void state_utility(s16, s16, void *);
extern s32 object_bytes_sum_global(void);
extern void func_8010A7A4(s32, s32, s32, void *, void *);
extern void func_8010A8D0(void);
extern void func_800ED66C(f32);
extern void func_800BEA3C(Color4, Color4);
#endif
