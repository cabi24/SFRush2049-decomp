/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct MenuText { u8 pad0[0x52]; u16 first; u16 value; } MenuText;
typedef struct MenuCtx { u8 pad0[0xC]; MenuText *text; char **strings; } MenuCtx;
typedef struct Box { s32 x, w, h, x2; } Box;

extern u8 D_801461D0[];
extern s8 D_8011AD40[4];
extern MenuCtx D_8017A4E0;
extern Box D_8011AD70;

extern s32 osRecvMesg(void *, void *, s32);
extern s32 osSendMesg(void *, void *, s32);
extern s32 slot_state_setup(s32);
extern u32 object_manager_update(char *, s32);
extern s16 object_bytes_sum_global(void);
extern void fcvt_wrapper(char *buf, char *fmt, ...);

void vr_font_lock(void) {
    osRecvMesg(D_801461D0, 0, 1);
}

void vr_font_unlock(void) {
    osSendMesg(D_801461D0, 0, 0);
}

s32 vr_font_set(s32 sel) {
    s32 old;
    vr_font_lock();
    old = slot_state_setup(sel);
    vr_font_unlock();
    return old;
}

#define MAX(a, b) ((a) < (b) ? (b) : (a))

s8 video_validOption(s32);

void particle_render(void) {
    u32 rows;
    u32 labelw;
    u32 valuew;
    s32 i;
    char buf[40];

    vr_font_set(11);
    rows = 0;
    labelw = 0;
    valuew = 0;
    for (i = 0; i < 4; i++) {
        if (video_validOption(i) == 1) {
            rows++;
            labelw = MAX(labelw, object_manager_update(D_8017A4E0.strings[D_8017A4E0.text->first + i], -1));
            if (i == 2) {
                fcvt_wrapper(buf, "%s  %s", (&D_8017A4E0.strings[D_8017A4E0.text->value])[0], (&D_8017A4E0.strings[D_8017A4E0.text->value])[1]);
                valuew = MAX(valuew, object_manager_update(buf, -1));
            } else {
                valuew = MAX(valuew, object_manager_update("000", -1));
            }
        }
    }
    D_8011AD70.w = labelw + valuew + 10;
    D_8011AD70.h = object_bytes_sum_global() * rows;
    D_8011AD70.x = 160 - D_8011AD70.w / 2;
    D_8011AD70.x2 = D_8011AD70.x + labelw + 5;
}
