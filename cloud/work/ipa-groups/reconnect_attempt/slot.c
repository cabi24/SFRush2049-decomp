typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

extern s8 D_80149DA0;
extern s32 D_80149780;
extern s32 D_801497A4;
extern s32 D_80114740;
extern s32 func_80097694(s32, s32);
extern s32 audio_frame_sync(s32, s32, s32, s32, s32);
extern void display_list_alloc(s32);
extern void sound_update_channel(s32);
extern s8 object_byte9_set(s8);

s8 slot_state_setup(s32 slot)
{
    s8 old;

    old = D_80149DA0;
    D_80149DA0 = slot;
    if (slot != -1) {
        D_80149780 = func_80097694(D_80149DA0 + 38, -1);
        if (D_80149780 < 0) {
            D_80149780 = audio_frame_sync(D_80149DA0 + 38, 0, 0, 1, 0);
            display_list_alloc(D_80149780);
        }
        D_801497A4 = func_80097694(D_80149DA0 + 22, -1);
        if (D_801497A4 < 0) {
            D_801497A4 = audio_frame_sync(D_80149DA0 + 22, 0, 0, 0, D_80114740);
            display_list_alloc(D_801497A4);
        }
        sound_update_channel(old != slot);
    }
    if (slot == 0) {
        object_byte9_set(1);
    }
    return old;
}


