/* Complete old-state cleanup and new-state entry flow from retail F1210. */
typedef signed char s8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef struct Rect { s16 x,y,w,h; s16 unknown[2]; } Rect;
typedef struct TextInfo { char unknown[54]; u16 index; } TextInfo;
typedef struct TextState { s32 unknown[3]; TextInfo *info; char **strings; } TextState;
extern TextState countdown_state;
extern s32 D_80140B20, D_801148CC, D_8014A110, D_80144DA0;
extern s16 active_player_count, D_80151AD0;
extern s8 *D_80149418[], *D_80146208, *D_80149798[3], *D_80149778[2];
extern s8 D_801460C0, D_801148D0;
extern Rect D_801148E8[][4];
extern char D_801461D0[], D_80120600[], D_801148A8[];
extern void sound_stop(s32);
extern void sound_handles_array_clear(s8 *);
extern void func_800F1114(void);
extern void *ambient_sound_set(s32,s32,s32,s32,s32,s32,s32,s32);
extern void viScheduleTick(float);
extern s32 osRecvMesg(void *,void *,s32), osJamMesg(void *,void *,s32);
extern s32 slot_state_setup(s32);
extern s32 object_manager_update(char *,s32), object_bytes_sum_global(void);
extern void func_800F0F44(s32), func_800F0914(void);
extern s32 sound_control(s32,s32,void *,s32);

void func_800F1210(s32 mode)
{
    s32 i;
    s32 previous;
    s32 width, height;
    if (mode == D_80140B20) return;
    if (D_801148CC != 0) {
        sound_stop(D_801148CC);
        D_801148CC = 0;
    }
    switch (D_80140B20) {
    case 1:
        for (i = 0; i < active_player_count; i++) {
            if (D_80149418[i] != 0) {
                sound_handles_array_clear(D_80149418[i]); D_80149418[i] = 0;
            }
        }
        break;
    case 2:
        if (D_80146208 != 0) { sound_handles_array_clear(D_80146208); D_80146208 = 0; }
        break;
    case 3:
        for (i = 0; i < active_player_count; i++) {
            if (D_80149418[i] != 0) {
                sound_handles_array_clear(D_80149418[i]); D_80149418[i] = 0;
            }
        }
        break;
    case 4:
        for (i = 0; i < 3; i++) {
            if (D_80149798[i] != 0) { sound_handles_array_clear(D_80149798[i]); D_80149798[i] = 0; }
        }
        break;
    case 5:
        for (i = 0; i < 2; i++) {
            if (D_80149778[i] != 0) { sound_handles_array_clear(D_80149778[i]); D_80149778[i] = 0; }
        }
        break;
    }
    D_80140B20 = mode;
    switch (mode) {
    case 0:
        func_800F1114();
        break;
    case 1:
        for (i = 0; i < active_player_count; i++) {
            if (D_80149418[i] == 0) {
                D_80149418[i] = ambient_sound_set(D_801148E8[D_80151AD0-1][i].x,
                    D_801148E8[D_80151AD0-1][i].y,
                    D_801148E8[D_80151AD0-1][i].x + D_801148E8[D_80151AD0-1][i].w,
                    D_801148E8[D_80151AD0-1][i].y + D_801148E8[D_80151AD0-1][i].h,176,0,0,0);
            }
        }
        viScheduleTick(600.0f);
        break;
    case 2:
        D_80146208 = ambient_sound_set(83,70,237,136,192,0,0,0);
        viScheduleTick(600.0f);
        break;
    case 3:
        for (i = 0; i < active_player_count; i++) {
            if (D_80149418[i] == 0) {
                D_80149418[i] = ambient_sound_set(D_801148E8[D_80151AD0-1][i].x,
                    D_801148E8[D_80151AD0-1][i].y,
                    D_801148E8[D_80151AD0-1][i].x + D_801148E8[D_80151AD0-1][i].w,
                    D_801148E8[D_80151AD0-1][i].y + D_801148E8[D_80151AD0-1][i].h,176,0,0,0);
            }
        }
        viScheduleTick(600.0f);
        break;
    case 4:
        if (D_80149798[0] == 0) {
            osRecvMesg(D_801461D0,0,1); previous = slot_state_setup(13); osJamMesg(D_801461D0,0,0);
            width = object_manager_update(D_80120600,-1); height = object_bytes_sum_global();
            D_80149798[0] = ambient_sound_set(152-width/2,6,168+width/2,14+height,176,0,0,0);
        }
        if (D_80149798[1] == 0) {
            osRecvMesg(D_801461D0,0,1); previous = slot_state_setup(10); osJamMesg(D_801461D0,0,0);
            height = object_bytes_sum_global();
            D_80149798[1] = ambient_sound_set(91,51,229,59+height*3,176,0,0,0);
        }
        if (D_80149798[2] == 0) {
            osRecvMesg(D_801461D0,0,1); previous = slot_state_setup(10); osJamMesg(D_801461D0,0,0);
            height = object_bytes_sum_global();
            D_80149798[2] = ambient_sound_set(91,106,229,114+height*5,176,0,0,0);
        }
        viScheduleTick(600.0f);
        D_80144DA0 = 0;
        D_801460C0 = 0;
        for (i = 0; i < 4; i++) func_800F0F44(i);
        break;
    case 5:
        if (D_8014A110 == 2) D_801148D0 = 2; else D_801148D0 = 0;
        func_800F0914();
        if (D_80149778[0] == 0) {
            osRecvMesg(D_801461D0,0,1); previous = slot_state_setup(13); osJamMesg(D_801461D0,0,0);
            width = object_manager_update(countdown_state.strings[countdown_state.info->index+D_801148D0],-1);
            height = object_bytes_sum_global();
            D_80149778[0] = ambient_sound_set(152-width/2,6,168+width/2,14+height,176,0,0,0);
        }
        if (D_80149778[1] == 0) {
            D_80149778[1] = ambient_sound_set(8,75,312,200,176,0,0,0);
        }
        if (D_8014A110 == 2) viScheduleTick(30.0f); else viScheduleTick(15.0f);
        break;
    }
    if (D_80140B20 != 0) D_801148CC = sound_control(0,0,D_801148A8,1);
}
