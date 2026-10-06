/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete C104 semantic context; declarations of the shared queue are
 * harmonized with candidate.c. The empty hook has no synthetic blocker.
 * This context remains nonmatching and is not proposed as accepted source.
 */
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

extern u8 D_801461D0[];
extern s8 D_80149DA0; extern void *D_80114740;
s32 osRecvMesg(void*,void*,s32); s32 osJamMesg(void*,void*,s32);
s32 func_80097694(s32,s8); s32 audio_frame_sync(s32,s32,s32,s32,void*); void display_list_alloc(s32); s8 object_byte9_set(s8);
void func_80096288(s32 a,s32 b,s32 c) {}
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

s32 slot_state_setup(s32 selection) {
    s32 temp_v0;
    s32 temp_v0_2;
    s32 temp_v0_3;
    s32 temp_v0_4;
    s8 temp_s3;

    temp_s3 = D_80149DA0;
    D_80149DA0 = selection;
    if (selection != -1) {
        temp_v0 = func_80097694(D_80149DA0 + 0x26, -1);
        D_80149780 = temp_v0;
        if (temp_v0 < 0) {
            temp_v0_2 = audio_frame_sync(D_80149DA0 + 0x26, 0, 0, 1, 0);
            D_80149780 = temp_v0_2;
            display_list_alloc(temp_v0_2);
        }
        temp_v0_3 = func_80097694(D_80149DA0 + 0x16, -1);
        D_801497A4 = temp_v0_3;
        if (temp_v0_3 < 0) {
            temp_v0_4 = audio_frame_sync(D_80149DA0 + 0x16, 0, 0, 0, D_80114740);
            D_801497A4 = temp_v0_4;
            display_list_alloc(temp_v0_4);
        }
        sound_update_channel(temp_s3 != selection);
    }
    if (selection == 0) {
        object_byte9_set(1);
    }
    return (s32) temp_s3;
}
s32 object_create(s32 selection) {
 s32 old;
 osRecvMesg(D_801461D0,0,1);
 old=slot_state_setup(selection);
 osJamMesg(D_801461D0,0,0);
 return old;
}

extern u32 D_80149B48; extern f32 D_80114748; extern s16 D_80149D92,D_80149D9E; extern s32 D_80149B88,D_8002AFC4; extern s8 D_8011474C,D_8011473C;
void func_800B6748(u8*,f32*,s16*,s16*,s8*,s8*,s8*);
void world_trigger_check(void) {
 osRecvMesg(D_801461D0,0,1);
 func_800B6748((u8*)&D_80149B48,&D_80114748,&D_80149D92,&D_80149D9E,&D_80149DA0,(s8*)&D_80149B60,(s8*)&D_80149B70);
 slot_state_setup(D_80149DA0);
 D_80149B88=0; if(D_8002AFC4>=0xDD)D_80149B88=0x8000;
 D_8011474C=0;D_8011473C=0;
 osJamMesg(D_801461D0,0,0);
}

s8 object_byte9_set(s8 value) {
 s8 old;
 sound_update_channel(0);
 old = D_801497F0->f9;
 D_801497F0->f9 = value;
 return old;
}
