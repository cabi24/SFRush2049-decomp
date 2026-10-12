/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct Voice Voice;
typedef struct RecordSlot { s16 id; u8 pad02[50]; } RecordSlot;   /* 52 bytes */
typedef struct RecordRow { RecordSlot a, b; } RecordRow;              /* 104 bytes */
typedef struct MenuText { u16 first; } MenuText;
typedef struct MenuCtx { u8 pad0[0xC]; MenuText *text; char **strings; } MenuCtx;

extern Voice *D_80154198;
extern Voice *D_801541A0;
extern Voice *D_801541A4;
extern RecordRow D_801541A8[];
extern f32 D_80142740[4], D_80142770[4], D_80142AC8[4];
extern u32 D_801174B4;
extern s8 D_80156994;
extern s8 D_8014978C;
extern s16 D_80151AD0;
extern s32 gameplay_mode;
extern u8 D_80115A44[], D_801159FC[], D_80115A20[], D_80114C58[], D_801158DC[];
extern s16 D_80142D70;
extern s32 D_80138660;
extern s32 D_80154394;
extern f32 D_8015462C;
extern f32 D_80154644;
extern volatile f32 D_8002EB90;
extern int D_8011735C;
extern s32 D_801543A0;
extern u32 D_801543A8;
extern s32 D_801543B0;
extern char *D_80114A0C[][11];
extern MenuCtx D_8017A4E0;
extern u8 D_801461D0[];

Voice *sound_control(s16 arg0, s16 arg1, void *arg2, s16 arg3);
s32 *ambient_sound_set(s32 arg0, s32 arg1, s32 arg2, s32 arg3, s32 arg4, s32 arg5, s32 arg6, s32 arg7);
void sound_handles_clear(s32 arg0);
void viScheduleTick(f32);
f32 viGetTimeToDeadline(void);
void func_803925D0(void);
extern s32 osRecvMesg(void *, void *, s32);
extern s32 osSendMesg(void *, void *, s32);
extern s32 slot_state_setup(s32);
extern u32 object_manager_update(char *, s32);

int func_8008B2B4(void) {
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7fff;
}

static void font_lock(void) {
    osRecvMesg(D_801461D0, 0, 1);
}

static void font_unlock(void) {
    osSendMesg(D_801461D0, 0, 0);
}

static s32 font_select(s32 sel) {
    s32 old;
    font_lock();
    old = slot_state_setup(sel);
    font_unlock();
    return old;
}

void skid_mark_render(void) {
    s32 created;
    s32 i;
    s32 x;
    s32 y;

    created = 0;
    if (D_80154198 == 0) {
        for (i = 0; i < 4; i++) {
            D_80142740[i] = 0.0f;
            D_80142770[i] = 0.0f;
            D_80142AC8[i] = 0.0f;
            D_801541A8[i].a.id = -1;
            D_801541A8[i].b.id = -1;
        }
        if (D_801174B4 & 0x02000000) {
            D_80154198 = sound_control(0, 0, D_80115A44, 1);
        } else if (D_801174B4 & 8) {
            D_80154198 = sound_control(0, 0, D_80115A44, 1);
        } else {
            if (D_80156994 || D_8014978C >= 6) {
                if (D_80151AD0 == 2) {
                    D_801541A4 = sound_control(0, 0, D_801159FC, 1);
                } else if (D_80151AD0 == 3) {
                    D_801541A4 = sound_control(0, 0, D_80115A20, 1);
                }
            }
            if (gameplay_mode == 6 || gameplay_mode == 4) {
                func_803925D0();
            } else {
                D_801541A0 = sound_control(0, 0, D_80114C58, 89);
            }
            D_80154198 = sound_control(0, 0, D_801158DC, 8);
        }
        D_80142D70 = 0;
        created = 1;
    }
    if ((D_801174B4 & 8) && D_80138660) {
        if (created) {
            D_80154394 = -1;
            viScheduleTick(90.0f);
            D_8015462C = viGetTimeToDeadline() / 13.0f;
            D_80154644 = D_8002EB90;
        }
        if (D_80154644 <= D_8002EB90) {
            D_80154644 += D_8015462C;
            D_80154394++;
            D_801543A0 = func_8008B2B4() * 4.0f / 32768.0f;
            font_select(11);
            D_801543A8 = object_manager_update(D_8017A4E0.strings[D_8017A4E0.text->first + D_80154394], -1);
            D_801543B0 = 0;
            while (D_80114A0C[D_80154394][D_801543B0] != 0) {
                if (D_801543A8 < object_manager_update(D_80114A0C[D_80154394][D_801543B0], -1)) {
                    D_801543A8 = object_manager_update(D_80114A0C[D_80154394][D_801543B0], -1);
                }
                D_801543B0++;
            }
            switch (D_801543A0) {
            case 0:
                x = 20;
                y = 20;
                break;
            case 1:
                y = 20;
                x = 280 - D_801543A8;
                break;
            case 2:
                x = 20;
                y = 145 - D_801543B0 * 10;
                break;
            default:
                y = 145 - D_801543B0 * 10;
                x = 280 - D_801543A8;
                break;
            }
            sound_handles_clear(0);
            ambient_sound_set(x, y, x + D_801543A8 + 20, D_801543B0 * 10 + y + 32, 192, 0, 0, 0);
        }
    }
}
