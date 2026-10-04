/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Bank {
    /* 0x0 */ u8 pad0;
    /* 0x1 */ u8 mode;
    /* 0x2 */ u8 pad2[2];
    /* 0x4 */ u8 f4;
    /* 0x5 */ u8 pad5;
    /* 0x6 */ u8 f6;
    /* 0x7 */ u8 f7;
    /* 0x8 */ u8 f8;
    /* 0x9 */ s8 f9;
    /* 0xA */ u8 f10;
    /* 0xB */ u8 f11;
    /* 0xC */ u8 f12;
} Bank;

typedef struct { u8 *ptr; u8 id; u8 pad[7]; } Entry;
typedef struct { Bank *bank; } Hdr;
typedef struct { Hdr *hdr; s32 pad[4]; } Tbl;
typedef struct { s32 first; s32 second; } Pair;

extern Bank *D_801497F0;
extern s32 D_80149780;
extern s32 D_801497A4;
extern Tbl D_80156D44[];
extern Entry *D_80149800;
extern s16 D_80149878[256];
extern s32 D_80149820[];
extern Pair D_80151AE8[];
extern s8 D_80149B60;
extern u8 D_80149B70;
extern s32 D_80149B08;
extern s32 D_80149B28;

void func_80096288(s32 a,s32 b,s32 c) { if(0) { switch(a+b+c) {case 1: D_80149B08=1;break;case 2:D_80149B08=2;break;case 3:D_80149B08=3;break;} } if(c) {} }
extern s32 func_80097694(s32, s32);

void sound_update_channel(s32 force)
{
    Bank *b;
    u8 *p;
    s32 i;
    s16 *q;
    s32 *r;
    s32 v;

    func_80096288(D_80149780, 0, 0);
    b = D_80156D44[D_80149780].hdr->bank;
    if (force || b != D_801497F0) {
        D_801497F0 = b;
        D_80149800 = (Entry *)((u8 *)b + 16);
        p = (u8 *)b + 16 + D_801497F0->f12 * 12;
        if (D_801497F0->f10 != 0) {
            for (i = 0; i < D_801497F0->f12; i++) {
                D_80149800[i].ptr = p;
                p += D_801497F0->f12;
            }
        }
        for (q = D_80149878; q < &D_80149878[256]; q++) {
            *q = -1;
        }
        for (i = 0; i < D_801497F0->f12; i++) {
            D_80149878[D_80149800[i].id] = i;
        }
    }
    if (force) {
        v = D_80151AE8[D_801497A4].first;
        r = D_80149820;
        for (i = 0; i < D_801497F0->f11; i++) {
            *r++ = v;
            v += 36;
        }
        D_80149B60 = D_801497F0->f4;
        D_80149B70 = D_801497F0->f8;
    }
    D_80149B08 = (D_801497F0->mode == 1) ? 4 : 3;
    D_80149B28 = (D_801497F0->mode == 1) ? 0 : 1;
}


void mode_byte2_set(s16 a) {
 if (a < 0) { sound_update_channel(0); D_80149B60 = D_801497F0->f4; }
 else D_80149B60 = a;
}
u8 object_type_byte2_get(void) { sound_update_channel(0); return *((u8 *)D_801497F0 + 2); }
u8 object_type_byte3_get(void) { sound_update_channel(0); return *((u8 *)D_801497F0 + 3); }

void *slot_value_get(s32 arg) { func_80096288(arg, 0, 0); return D_80156D44[arg].hdr; }

void mode_byte_set(s16 a) { if (a < 0) { sound_update_channel(0); D_80149B70=D_801497F0->f8; } else D_80149B70=a; }

s8 object_byte9_set(s8 value) {
 s8 old;
 sound_update_channel(0);
 old = D_801497F0->f9;
 D_801497F0->f9 = value;
 return old;
}
s16 object_bytes23_sum(void) { u8 first; first=object_type_byte2_get(); return (s16)(first + object_type_byte3_get()); }
s16 object_bytes_sum_global(void) {
 u8 first;
 sound_update_channel(0);
 first = *((u8 *)D_801497F0 + 3);
 sound_update_channel(0);
 return (s16)(D_80149B60 + *((u8 *)D_801497F0 + 2) + first);
}
void func_800F68A4(u8 *out) {
 s32 offset;
 s32 i;
 sound_update_channel(0);
 for (i = 0, offset = 0; i < D_801497F0->f12; offset += 12, i++) {
  out[i] = *((u8 *)D_80149800 + offset + 4);
 }
 out[i] = 0;
}
s32 camera_shake_update(u16 id) {
 s16 index;
 sound_update_channel(0);
 if ((u8)D_801497F0->f9 != 0) return D_801497F0->f6;
 if (id == 32 || id >= 256) return D_801497F0->f7;
 if (id == 10) return 0;
 index = D_80149878[id];
 if (index < 0) return D_801497F0->f7;
 return D_80149800[index].pad[3] - D_80149800[index].pad[1] + 1;
}
typedef struct {u32 opaque[6];} Queue24;
extern Queue24 D_801461D0;
extern s8 D_80149DA0; extern void *D_80114740;
s32 osRecvMesg(Queue24*,void**,s32); s32 osJamMesg(Queue24*,void*,s32);
s32 audio_frame_sync(s32,s32,s32,s32,void*); void display_list_alloc(s32);
s32 slot_state_setup(s32 selection) {
    s8 old;
    old = D_80149DA0;
    D_80149DA0 = selection;
    if (selection != -1) {
        if ((D_80149780 = func_80097694(D_80149DA0 + 0x26, -1)) < 0) {
            display_list_alloc(D_80149780 = audio_frame_sync(D_80149DA0 + 0x26, 0, 0, 1, 0));
        }
        if ((D_801497A4 = func_80097694(D_80149DA0 + 0x16, -1)) < 0) {
            display_list_alloc(D_801497A4 = audio_frame_sync(D_80149DA0 + 0x16, 0, 0, 0, D_80114740));
        }
        sound_update_channel(old != selection);
    }
    if (selection == 0) {
        object_byte9_set(1);
    }
    return old;
}
extern char **countdown_object;
extern s32 state_word_a;
extern s32 D_8015698C;
typedef struct { s8 flag; s8 pad[15]; } Slot16;
extern Slot16 D_80156CF0[];
extern s8 D_80110634;
extern s8 D_8011064C;
extern s32 D_80110648;
extern s32 D_80110650[3];
extern char D_8011F6B0[];
extern s8 D_80116DA8;
extern s32 D_80116DA4;
extern char D_80121004[];
extern s32 D_80118E20;
extern s32 D_80118E24;
extern s8 D_801613E8;
extern char D_8012029C[];
void fcvt_wrapper(char *, char *, ...);
s32 object_manager_update(char *, s32);
void crowd_cheer_play(s32, s32, s32, s32, s32);
s32 ambient_sound_set(s32, s32, s32, s32, s32, s32, s32, s32);
void render_helper(f32);
void dispatch_handler(s32);
void camera_auto_follow(s32, s32, s32, s32, s32, s32, char *);

void credits_scroll(void)
{
    char buf[76];
    s32 w;
    s32 old;

    osRecvMesg(&D_801461D0, (void*)0, 1);
    old = slot_state_setup(13);
    osJamMesg(&D_801461D0, (void*)0, 0);
    if (D_8011064C) {
        fcvt_wrapper(buf, D_8011F6B0, D_8015698C + 1, countdown_object[233]);
        w = object_manager_update(buf, -1);
    } else {
        w = object_manager_update(countdown_object[52], -1);
    }
    if (D_80110648) {
        crowd_cheer_play(D_80110648, 152 - w / 2, 6, w / 2 + 168, object_bytes_sum_global() + 14);
    } else {
        D_80110648 = ambient_sound_set(152 - w / 2, 6, w / 2 + 168, object_bytes_sum_global() + 14, 176, 0, 0, 0);
    }
}

s32 game_timer_display(s32 arg0)
{
    s32 old1;
    s32 old2;
    s16 h;

    if (!D_80110634) {
        if (!(state_word_a & 0x7C03FFFE)) {
            osRecvMesg(&D_801461D0, (void*)0, 1);
            old1 = slot_state_setup(13);
            osJamMesg(&D_801461D0, (void*)0, 0);
            D_8011064C = !D_80156CF0[D_8015698C].flag;
            credits_scroll();
            h = object_bytes_sum_global() * 2 + 10;
            osRecvMesg(&D_801461D0, (void*)0, 1);
            old2 = slot_state_setup(10);
            osJamMesg(&D_801461D0, (void*)0, 0);
            ambient_sound_set(D_80110650[0] - 8, h - 4, D_80110650[0] + D_80110650[1] + 8,
                              h + D_80110650[2] + 4, 176, 0, 0, 0);
        }
        D_80110634 = 1;
    }
    return 1;
}

s32 func_80100B8C(s32 arg0)
{
    s32 old;

    render_helper(0.0f);
    D_80118E20 = 1;
    D_80118E24 = 1;
    if (D_801613E8 == 1) {
        osRecvMesg(&D_801461D0, (void*)0, 1);
        old = slot_state_setup(13);
        osJamMesg(&D_801461D0, (void*)0, 0);
        dispatch_handler(22);
        camera_auto_follow(160, 10, 320, 110, -1, 0, D_8012029C);
        camera_auto_follow(160, 190, 320, 110, -1, 0, countdown_object[33]);
    } else {
        osRecvMesg(&D_801461D0, (void*)0, 1);
        old = slot_state_setup(11);
        osJamMesg(&D_801461D0, (void*)0, 0);
        dispatch_handler(22);
        camera_auto_follow(160, 185, 320, 110, -1, 0, countdown_object[32]);
    }
    dispatch_handler(1);
    D_80118E24 = 3;
    D_80118E20 = 0;
    render_helper(-1.0f);
    return 1;
}
s32 object_create(s32 sel)
{
    s32 old;
    osRecvMesg(&D_801461D0, (void*)0, 1);
    old = slot_state_setup(sel);
    osJamMesg(&D_801461D0, (void*)0, 0);
    return old;
}
