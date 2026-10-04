typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { u32 opaque[6]; } OSMesgQueue;   /* 24-byte libultra queue */
extern OSMesgQueue D_801461D0;
s32 osRecvMesg(OSMesgQueue *, void **, s32);
s32 osJamMesg(OSMesgQueue *, void *, s32);
s32 slot_state_setup(s32 selection);
void sound_update_channel(s32);
s8 object_byte9_set(s8);
s32 func_80097694(s32, s32);
extern s32 D_80149780;
extern s32 D_801497A4;
extern s8 D_80149DA0;
extern void *D_80114740;
s32 audio_frame_sync(s32, s32, s32, s32, void *);
void display_list_alloc(s32);
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
s32 object_create(s32 sel)
{
    s32 old;
    osRecvMesg(&D_801461D0, (void*)0, 1);
    old = slot_state_setup(sel);
    osJamMesg(&D_801461D0, (void*)0, 0);
    return old;
}

typedef struct { u32 first, second; } Pair;
extern Pair D_80118E20;
void func_800B669C(u32 first, u32 second) { Pair *p = &D_80118E20; p->first = first; p->second = second; }
