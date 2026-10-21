/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Color4 { u32 word; } Color4;
typedef struct FileInfo FileInfo;
typedef struct Profile Profile;
struct Profile { Profile **next, **previous; FileInfo **file; u32 id0, id1; u8 name[24]; u8 **stats; };
typedef struct Player { u32 held, pressed, unknown08, repeated; u8 unknown10[50]; u8 paint, unknown43[5]; Profile **owner; } Player;
typedef struct Record Record;
struct Record { Record **next; FileInfo **file; s8 track; u8 mode, name[14]; u32 id0, id1; f32 time; u32 unknown24; void **active; };
typedef struct Language { s32 unknown0; u8 **text; s32 unknown8; u16 *indices; u8 **indexed; } Language;
typedef struct Camera { f32 uv[3][3], position[3]; u8 unknown30[104]; } Camera;
typedef struct Slot { s32 kind, handle; f32 angle, matrix[3][3], position[3]; u8 alpha, unknown3D[3]; } Slot;
typedef struct Blit Blit;

extern Language countdown_state;
extern s16 D_8014A108, D_80151AD0;
extern s32 D_8014A110;
extern Player D_8014A118[];
extern Camera D_80150B70[];
extern Slot D_803AF9A8[][44];
extern f32 D_803B25AC, D_803B25C0;
extern s8 D_803BA028[], D_803BA00C, D_803B9FD0[], D_8012E67C[];
extern s32 D_803B9B90[], D_803BA050[], D_803BA060[][19], D_803AF980, D_801146AC[];
extern s16 D_803B28B0[];
extern Record **D_80152698[4];
extern Color4 D_801146BC, D_801146C0;
extern u8 *D_80110D70[];
extern s8 D_80110E78[][13], D_8011103C[][13], D_80111080[][13], D_8011119C[][13], D_80111230[][13], D_8011128C[][13];
extern s8 D_8011157C[][13], D_801115C0[][13];
extern void render_helper(f32);
extern void *object_create(s32);
extern void dispatch_handler(s32);
extern void func_800B669C(s32, s32);
extern void brake_light_update(s32, f32 *, Camera *, void *, s16 *);
extern s16 sound_pitch_diff_halved(void *, s16);
extern s16 object_bytes_sum_global(void);
extern void state_utility(s16, s16, void *);
extern void fcvt_wrapper(u8 *, char *, ...);
extern void music_fade(s16, s16, s16, void *);
extern void func_800ED66C(f32);
extern void func_800F6928(f32);
extern void func_800BEA3C(Color4, Color4);
extern u8 *func_800BE6A4(u8 *, u8 *);

static void animButtonText(s32 p);
static void drawOptions(s32 p);

s32 func_803A44FC(Blit *blt)
{
    s32 i;
    for (i = 0; i < D_8014A108; i++) {
        switch (D_803B9B90[i]) {
        case 1: animButtonText(i); break;
        default: drawOptions(i); break;
        }
    }
    return 1;
}

static void animButtonText(s32 p)
{
    s32 i;
    s32 x;
    s32 y;
    s32 w;
    u8 *str;
    u8 str1[64];
    s16 screenPos[2];
    f32 gTextPosScale = D_803B25AC;
    if (D_80151AD0 > 1) gTextPosScale *= 0.5f;
    render_helper(0.0f);
    func_800B669C(1, 1);
    if (D_80151AD0 == 1) { w = 140; object_create(13); }
    else { w = 69; object_create(12); }
    brake_light_update(p, D_803AF9A8[p][38].position, &D_80150B70[p], 0, screenPos);
    x = screenPos[0];
    y = screenPos[1];
    str = (*D_8014A118[p].owner)->name;
    dispatch_handler(1);
    music_fade(x, y, w, str);
    func_800B669C(0, 1);
    if (D_8014A110 == 2) {
        s32 ghostIdx;
        for (ghostIdx = 1; ghostIdx < D_803BA00C + 1; ghostIdx++) {
            f32 pos[3];
            brake_light_update(ghostIdx, D_803AF9A8[0][38].position, &D_80150B70[ghostIdx], 0, screenPos);
            x = screenPos[0];
            y = screenPos[1];
            fcvt_wrapper(str1, "GHOST %d", ghostIdx);
            dispatch_handler(1);
            state_utility(sound_pitch_diff_halved(str1, x), y, str1);
            pos[0] = D_803AF9A8[0][38].position[0];
            pos[1] = D_803AF9A8[0][38].position[1];
            pos[2] = D_803AF9A8[0][38].position[2];
            pos[1] = -pos[1] / 2.0f;
            brake_light_update(ghostIdx, pos, &D_80150B70[ghostIdx], 0, screenPos);
            x = screenPos[0];
            y = screenPos[1];
            if (!D_80152698[ghostIdx]) {
                str = !D_803AF980 ? countdown_state.text[225] : countdown_state.text[224];
                dispatch_handler(1);
                state_utility(sound_pitch_diff_halved(str, x), y, str);
            } else {
                str = (*D_80152698[ghostIdx])->name;
                dispatch_handler(D_801146AC[ghostIdx]);
                state_utility(sound_pitch_diff_halved(str, x), y, str);
                y += object_bytes_sum_global();
                fcvt_wrapper(str1, "TIME %01d:%02d.%03d", (s32)(*D_80152698[ghostIdx])->time / 60,
                             (s32)(*D_80152698[ghostIdx])->time % 60, (s32)(1000.0f * (*D_80152698[ghostIdx])->time) % 1000);
                dispatch_handler(1);
                state_utility(sound_pitch_diff_halved(str1, x), y, str1);
            }
        }
    }
    if (D_803BA028[p]) {
        s32 notReadyCount;
        s32 notReadyIdx;
        s32 pIdx;
        f32 msgPos[3] = {0.0f, -120.0f, 300.0f};
        brake_light_update(p, msgPos, &D_80150B70[p], 0, screenPos);
        x = screenPos[0];
        y = screenPos[1] - object_bytes_sum_global();
        notReadyCount = 0;
        for (pIdx = 0; pIdx < D_8014A108; pIdx++) {
            if (p != pIdx && !D_803BA028[pIdx]) { notReadyIdx = pIdx; notReadyCount++; }
        }
        if (!notReadyCount) {
            y += object_bytes_sum_global();
            str = (u8 *)"GET READY TO RACE";
            dispatch_handler(1);
        } else {
            str = countdown_state.text[151];
            dispatch_handler(1);
            state_utility(sound_pitch_diff_halved(str, x), y, str);
            y += object_bytes_sum_global();
            if (notReadyCount == 1) {
                dispatch_handler(D_801146AC[notReadyIdx]);
                str = (*D_8014A118[notReadyIdx].owner)->name;
            } else str = countdown_state.text[31];
        }
        state_utility(sound_pitch_diff_halved(str, x), y, str);
        func_800B669C(0, 3);
        func_800ED66C(-1.0f);
        return;
    }
    if (D_80151AD0 == 1) object_create(11); else object_create(12);
    for (i = 0; i < D_803BA050[p]; i++) {
        s32 item = D_803BA060[p][i];
        if ((D_803AF9A8[p][item].angle > 0.6981317f && D_803AF9A8[p][item].angle < 2.443461f) || !D_803AF9A8[p][item].alpha) continue;
        func_800ED66C(D_803AF9A8[p][item].alpha);
        if (D_803AF9A8[p][item].angle < 1.5707964f) {
            func_800BEA3C(D_801146BC, D_801146C0);
            dispatch_handler(1);
        } else {
            func_800BEA3C(D_801146BC, D_801146C0);
            dispatch_handler(22);
        }
        func_800F6928(D_803AF9A8[p][item].position[2] - 10.0f);
        brake_light_update(p, D_803AF9A8[p][item].position, &D_80150B70[p], 0, screenPos);
        x = screenPos[0] - 45.0f * gTextPosScale * 300.0f / D_803B25C0;
        y = screenPos[1];
        str = D_80151AD0 == 1 ? countdown_state.indexed[countdown_state.indices[2] + item]
                              : countdown_state.indexed[countdown_state.indices[3] + item];
        state_utility(x, y, str);
        switch (item) {
        case 0: str = D_80110D70[D_803B9FD0[p]]; break;
        case 1: str = countdown_state.indexed[countdown_state.indices[4] + D_80110E78[p+1][D_803B9FD0[p]]]; break;
        case 2: str = countdown_state.indexed[countdown_state.indices[5] + D_8011103C[p+1][D_803B9FD0[p]]]; break;
        case 3: str = countdown_state.indexed[countdown_state.indices[6] + D_80111080[p+1][D_803B9FD0[p]]]; break;
        case 4: str = countdown_state.indexed[countdown_state.indices[7] + D_8011119C[p+1][D_803B9FD0[p]]]; break;
        case 5: str = countdown_state.indexed[countdown_state.indices[8] + D_80111230[p+1][D_803B9FD0[p]]]; break;
        case 6: str = countdown_state.indexed[countdown_state.indices[9] + D_8011128C[p+1][D_803B9FD0[p]]]; break;
        case 10: str = countdown_state.indexed[countdown_state.indices[12] + D_8012E67C[p]]; break;
        case 11: str = countdown_state.indexed[countdown_state.indices[10] + D_8011157C[p+1][D_803B9FD0[p]]]; break;
        case 12: str = countdown_state.indexed[countdown_state.indices[11] + D_801115C0[p+1][D_803B9FD0[p]]]; break;
        case 15: fcvt_wrapper(str1, "%d", D_803B28B0[p] + 1); str = str1; break;
        case 16:
            if (D_80152698[1]) func_800BE6A4(str1, (*D_80152698[1])->name);
            else func_800BE6A4(str1, countdown_state.indexed[countdown_state.indices[15]]);
            str = str1;
            break;
        case 17:
            if (D_80152698[2]) func_800BE6A4(str1, (*D_80152698[2])->name);
            else func_800BE6A4(str1, countdown_state.indexed[countdown_state.indices[15]]);
            str = str1;
            break;
        case 18:
            if (D_80152698[3]) func_800BE6A4(str1, (*D_80152698[3])->name);
            else func_800BE6A4(str1, countdown_state.indexed[countdown_state.indices[15]]);
            str = str1;
            break;
        default: str = 0; break;
        }
        if (str) {
            brake_light_update(p, D_803AF9A8[p][item+19].position, &D_80150B70[p], 0, screenPos);
            x = screenPos[0] - 50.0f * gTextPosScale * 300.0f / D_803B25C0;
            y = screenPos[1];
            state_utility(x, y, str);
        }
    }
    func_800B669C(0, 3);
    func_800ED66C(-1.0f);
    render_helper(-1.0f);
}

static void drawOptions(s32 p)
{
}

