typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef void *OSMesg;
typedef struct { s32 opaque[6]; } OSMesgQueue;

typedef struct {
    u8 pad000[0xFC];
    void *unkFC;
} CountdownDetail;
typedef struct {
    u8 pad00[0x5A];
    u16 unk5A;
} CountdownIndex;
typedef struct {
    void *unk0;
    CountdownDetail *unk4;
    void *unk8;
    CountdownIndex *unkC;
    void **unk10;
} CountdownState;

extern CountdownState countdown_state;
extern s32 D_80138870;
extern OSMesgQueue D_801461D0;
extern u8 D_80114724[2];
extern s8 D_80149DA0;
extern s32 D_80149780;
extern s32 D_801497A4;
extern s32 D_80114740;

s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
void sound_handles_clear(s32 arg0);
s32 object_manager_update(void *obj, s32 arg1);
s16 object_bytes_sum_global(void);
void *ambient_sound_set(s32 x1, s32 y1, s32 x2, s32 y2, s32 style, s32 word40, s32 word44, s32 own);
s32 func_800F84B0(s32 slot);
s32 func_80097694(s32 a, s32 b);
s32 audio_frame_sync(s32 a, s32 b, s32 c, s32 d, s32 e);
void display_list_alloc(s32 a);
void sound_update_channel(s32 a);
s8 object_byte9_set(s8 value);

/* context: IPA callee taking its argument in $s2 (cloud/work/s20261004/E draft) */
s32 slot_state_setup(s32 selection)
{
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

void func_800F857C(s32 mode)
{
    s32 width;
    s32 x;
    s32 y;
    s32 h;
    s32 count;
    s32 i;
    s32 old;

    if (mode == D_80138870) {
        return;
    }
    if (D_80138870 == 1) {
        sound_handles_clear(0);
    }
    D_80138870 = mode;
    if (D_80138870 == 1) {
        osRecvMesg(&D_801461D0, 0, 1);
        old = slot_state_setup(13);
        osJamMesg(&D_801461D0, 0, 0);
        width = object_manager_update(countdown_state.unk4->unkFC, -1);
        x = D_80114724[0];
        y = D_80114724[1];
        h = object_bytes_sum_global();
        ambient_sound_set(x - width / 2 - 8, y - 4, width / 2 + x + 8, h + y + 4, 176, 0, 0, 0);
        osRecvMesg(&D_801461D0, 0, 1);
        old = slot_state_setup(10);
        osJamMesg(&D_801461D0, 0, 0);
        width = 0;
        count = 0;
        for (i = 0; i < 7; i++) {
            if (func_800F84B0(i) == 1) {
                y = object_manager_update(countdown_state.unk10[countdown_state.unkC->unk5A + i], -1);
                count++;
                if (width < y) {
                    width = y;
                }
            }
        }
        h = object_bytes_sum_global();
        ambient_sound_set(x - width / 2 - 8, 114, width / 2 + x + 8, h * count + 122, 176, 0, 0, 0);
    }
}

void __standin_a(void) { func_800F857C(1); }
void __standin_b(void) { func_800F857C(2); }
void __standin_c(void) { slot_state_setup(-1); }
void __standin_d(void) { slot_state_setup(0); }
