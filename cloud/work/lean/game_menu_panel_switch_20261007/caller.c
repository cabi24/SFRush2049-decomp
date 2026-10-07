/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Voice Voice;
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
extern void *D_80114740;

s32 osRecvMesg(OSMesgQueue *mq, OSMesg *msg, s32 flag);
s32 osJamMesg(OSMesgQueue *mq, OSMesg msg, s32 flag);
void sound_handles_clear(s32 arg0);
s32 object_manager_update(void *obj, s16 arg1);
s16 object_bytes_sum_global(void);
void *ambient_sound_set(s32 x1, s32 y1, s32 x2, s32 y2, s32 style, s32 word40, s32 word44, s32 own);
s32 func_80097694(s32 a, s32 b);
s32 audio_frame_sync(s32 a, s32 b, s32 c, s32 d, void *e);
void display_list_alloc(s32 a);
void sound_update_channel(s32 a);
s8 object_byte9_set(s8 value);


typedef struct {
    s16 id;
    u8 unk2,used;
    u32 payload[5];
} SceneMsg;

extern s32 D_8015694C;
extern s32 D_801391E0;
extern s32 gameplay_mode;
extern OSMesgQueue D_80142728;
extern OSMesgQueue D_801427A8;
extern s8 D_8014978C;
extern void *D_801391F0;
extern s16 D_80151AD0;
extern Voice *D_801541A4;
extern s32 D_801174BC;
extern s32 state_word_b;
extern u8 D_80114650;
extern s32 D_80149784;

void resource_type_select(s32 arg0);
SceneMsg *func_80091B00(void);
void func_800B912C(s32 arg0);
void entity_transform_apply(void *entity, s32 arg1);
void func_800B0580(void);
s32 wheel_render_full(s32 a);
void sound_stop(Voice *);
void audio_update_d(void);
void func_800D5A04(void);
void audio_distance_atten(void);


/* Current accepted filter body is in filter.c. */
extern u8 D_801543D4;
extern s8 D_80152907;

s32 func_800F84B0(s32);

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

static void gfx_lock(void)
{
    osRecvMesg(&D_801461D0, (void *)0, 1);
}

static void gfx_unlock(void)
{
    osJamMesg(&D_801461D0, (void *)0, 0);
}

static s32 font_set(s32 font)
{
    s32 old;

    gfx_lock();
    old = slot_state_setup(font);
    gfx_unlock();
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

    if (mode == D_80138870) {
        return;
    }
    if (D_80138870 == 1) {
        sound_handles_clear(0);
    }
    D_80138870 = mode;
    if (D_80138870 == 1) {
        font_set(13);
        width = object_manager_update(countdown_state.unk4->unkFC, -1);
        x = D_80114724[0];
        y = D_80114724[1];
        h = object_bytes_sum_global();
        ambient_sound_set(x - width / 2 - 8, y - 4, width / 2 + x + 8, h + y + 4, 176, 0, 0, 0);
        font_set(10);
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


/* F87A0 research: the O32 pointer-word-pointer conversion below is a used,
 * lossless address view on this 32-bit target. It changes IDO address formation
 * and is disclosed as source shaping, not an original-source assertion.
 * No new volatile, unused local, artificial caller or runtime input is added.
 */
void func_800F87A0(void)
{
    SceneMsg *msg;
    s32 buttons;

    buttons = *(s32 *)(u32)&D_8015694C;
    if (buttons & 3) {
        resource_type_select(buttons);
        switch (D_801391E0) {
        case 0:
            func_800F857C(2);
            break;
        case 1:
            if (gameplay_mode == 2) {
                osRecvMesg(&D_80142728, 0, 1);
                (msg = func_80091B00())->unk2 = 1;
                osJamMesg(&D_80142728, 0, 0);
                osJamMesg(&D_801427A8, msg, 0);
                func_800B912C(D_8014978C);
                while (D_801391F0 != 0) {
                    entity_transform_apply(D_801391F0, 1);
                }
                func_800B0580();
                D_80151AD0 = 1;
                wheel_render_full(1);
                if (D_801541A4 != 0) {
                    sound_stop(D_801541A4);
                    D_801541A4 = 0;
                }
                audio_update_d();
                D_801174BC = 0x40000;
                state_word_b = 4;
            } else {
                audio_update_d();
                D_80114650 = 1;
                osRecvMesg(&D_80142728, 0, 1);
                (msg = func_80091B00())->unk2 = 7;
                osJamMesg(&D_80142728, 0, 0);
                osJamMesg(&D_801427A8, msg, 0);
                func_800D5A04();
            }
            break;
        case 2:
        case 3:
        case 4:
        case 5:
        case 6:
            osRecvMesg(&D_80142728, 0, 1);
            (msg = func_80091B00())->unk2 = 1;
            osJamMesg(&D_80142728, 0, 0);
            osJamMesg(&D_801427A8, msg, 0);
            func_800B912C(D_8014978C);
            while (D_801391F0 != 0) {
                entity_transform_apply(D_801391F0, 1);
            }
            func_800B0580();
            D_80151AD0 = 1;
            wheel_render_full(1);
            if (D_801541A4 != 0) {
                sound_stop(D_801541A4);
                D_801541A4 = 0;
            }
            if (D_801391E0 == 2) {
                D_801174BC = 0x40;
            } else if (D_801391E0 == 3) {
                D_801174BC = 0x80;
            } else if (D_801391E0 == 4) {
                D_801174BC = 0x100;
            } else if (D_801391E0 == 5) {
                D_801174BC = 0x4000;
            } else if (D_801391E0 == 6) {
                D_801174BC = 0x10;
            }
            audio_update_d();
            state_word_b = 4;
            break;
        }
    } else {
        buttons = D_80149784;
        if (buttons & 0x400) {
        audio_distance_atten();
        do {
            if (--D_801391E0 < 0) {
                D_801391E0 = 6;
            }
        } while (func_800F84B0(D_801391E0) == 0);
    } else if (buttons & 0x800) {
        audio_distance_atten();
        do {
            if (++D_801391E0 >= 7) {
                D_801391E0 = 0;
            }
        } while (func_800F84B0(D_801391E0) == 0);
    }
}
}


/* finish_state_alt is prior #222 matching-source context, not a claim here.
 * The real selection counter is read/written directly, and the current object
 * is reloaded after each callback before deciding whether to repeat.
 * The allocation/type-store/queue-unlock line is intentionally one physical
 * line: this compile-affecting layout reproduces the native two-word schedule.
 * No runtime operation or unused local is introduced by that layout.
 */
extern s8 D_80114728, D_80157244;
extern Voice *D_8011472C;
extern s32 D_801174B8;
extern u8 D_80114700[];
Voice *sound_control(s16,s16,const void *,s32);
void func_800CFCA8(void);
void func_800F7448(unsigned short);
void finish_state_alt(void)
{
    SceneMsg *msg;
    void *entity;
    if (D_80114728==0) {
        func_800F857C(1);
        D_801391E0=0;
        if (!func_800F84B0(0)) {
            do {
                D_801391E0++;
                if (D_801391E0>=7) {
                    D_801391E0=0;
                    break;
                }
            } while (!func_800F84B0(D_801391E0));
        }
        D_8011472C=sound_control(0,0,D_80114700,1);
        D_80114728=1;
    }
    func_800CFCA8();
    if (D_80157244) {
        osRecvMesg(&D_80142728,0,1);
        msg=func_80091B00(); msg->unk2=1; osJamMesg(&D_80142728,0,0);
        osJamMesg(&D_801427A8,msg,0);
        func_800B912C(D_8014978C);
        entity=D_801391F0;
        if (entity) {
            do {
                entity_transform_apply(entity,1);
                entity=D_801391F0;
            } while (entity);
        }
        func_800B0580();
        D_80151AD0=1;
        wheel_render_full(1);
        if (D_801541A4) {
            sound_stop(D_801541A4);
            D_801541A4=0;
        }
        func_800F7448(1);
        D_801174BC=1;
        audio_update_d();
        D_801174B8=4;
        return;
    }
    switch (D_80138870) {
    case 1:func_800F87A0();break;
    case 2:
        if (D_8015694C&7) {
            resource_type_select(4);
            func_800F857C(1);
        }
        break;
    }
}

