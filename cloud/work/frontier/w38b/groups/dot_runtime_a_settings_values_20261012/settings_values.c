/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image A:803AECE4..803AF744, complete projected settings-label/value callback.
 * (w38b, wave 38) This callback is MAT3 trackscr.c animText; its body is animButtonText, now the non-kept
 * helper func_803AECDC (inlined; its deleted procedure is the retail stub 0x803AECDC right before animText).
 * umerge's 8-byte area for that inlined call sits exactly where the native x/y homes were (retail never
 * touches sp+208..215; i is the memory-resident counter at sp+204), so the helper declares i first and x, y
 * last, and the native `slot` carrier is gone (D_803B6B14[index] directly). N64 animText has no
 * `else drawOptions()` (a second inlined call would add 8 more bytes). Frame 216, buffer capacities unchanged. Native expressions
 * evaluate the selected table base before the language offset; three existing
 * label assignments preserve text evaluation before argument narrowing.
 * Reordered existing vector writes and a split Y subtraction preserve the
 * native named value and floating temporary sequence. No padding, artificial
 * read, volatile, fake caller, extra parameter or compiler patch is used.
 * Color4 is the word-aligned native view of the two four-byte by-value colors.
 * Six natural angle literals and the 21-entry switch table own 108 verified
 * bytes at 803B99E0..803B9A4C. No exact arcade donor is claimed. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Color4 { u32 word; } Color4;
typedef struct Language { s32 unknown0; char **text; s32 unknown8; u16 *indices; char **indexed; } Language;
typedef struct Slot { f32 x,y,angle; u8 unknown0C[36]; f32 position[3]; u8 alpha,unknown3D[3]; } Slot;
typedef struct Camera { f32 uv[3][3],position[3]; u8 unknown30[104]; } Camera;
typedef struct Settings { u8 unknown00[28]; s8 mode; u8 unknown1D[3]; s8 a,b,c,d,e,f,g,h,i; } Settings;
typedef struct CourseFlag { u8 flag,unknown01[4]; } CourseFlag;
extern s32 D_803B7714,D_8014A110;
extern f32 D_803B75C4[3],D_803B7720,D_803B771C;
extern s8 D_80156994,D_8014978C,D_801426EC,D_80152570,D_80154628,D_80142760;
extern s16 D_80152734,D_80142724,D_803BA898,D_803BA8B0[];
extern Slot D_803B6B14[];
extern Camera D_80150B70[];
extern Language countdown_state;
extern Color4 D_801146BC,D_801146C0;
extern char *D_8011A8D8[];
extern Settings D_80146108;
extern CourseFlag D_801543DA[];
extern char D_803B9244[],D_803B9248[],D_803B924C[],D_803B9250[],D_803B9254[],D_803B9258[],D_803B925C[];
extern void render_helper(f32);
extern s32 object_create(s32);
extern void dispatch_handler(s32);
extern void func_800B669C(u32,u32);
extern void brake_light_update(s32,f32 *,Camera *,void *,s16 *);
extern s16 sound_pitch_diff_halved(void *,s16);
extern void state_utility(s16,s16,void *);
extern void fcvt_wrapper(char *,char *,...);
extern int sprintf(char *,const char *,...);
extern void func_800ED66C(f32);
extern void func_800BEA3C(Color4,Color4);

void func_803AECDC(void);
s32 func_803AECE4(void *callback_context)
{
    if (D_803B7714 == 1) func_803AECDC();
    return 1;
}

/* animButtonText@trackscr.c (MAT3): inlined into animText func_803AECE4; deleted stub 0x803AECDC (w38b) */
void func_803AECDC(void)
{
    s32 i;
    char text[80];
    char *label;
    s16 point[2];
    f32 position[3];
    s32 index;
    s32 x;
    s32 y;
    render_helper(0.0f);
    func_800B669C(0, 1);
    brake_light_update(0, D_803B75C4, &D_80150B70[0], 0, point);
    x = point[0];
    y = point[1];
    object_create(13);
    dispatch_handler(1);
    fcvt_wrapper(text, countdown_state.text[45]);
    state_utility(sound_pitch_diff_halved(text, x), y, text);
    if (!D_80156994 && D_8014978C == 5) {
        position[2] = D_803B6B14[0].position[2];
        position[0] = 0.0f;
        position[1] = D_803B6B14[0].position[1];
        position[1] -= D_803B7720 * D_803B771C;
        object_create(11);
        brake_light_update(0, position, &D_80150B70[0], 0, point);
        x = point[0];
        y = point[1];
        label = countdown_state.text[232];
        dispatch_handler(22);
        state_utility(sound_pitch_diff_halved(label, x), y, label);
    }
    object_create(11);
    for (i = 0; i < D_803BA898; i++) {
        index = D_803BA8B0[i];
        if ((D_803B6B14[index].angle <= 0.6981316804885864f || D_803B6B14[index].angle >= 2.4434609413146973f) && D_803B6B14[index].alpha) {
            func_800ED66C(D_803B6B14[index].alpha);
            brake_light_update(0, D_803B6B14[index].position, &D_80150B70[0], 0, point);
            x = point[0] - 35.0f * D_803B771C;
            y = point[1];
            if (D_803B6B14[index].angle < 1.5707963705062866f) {
                func_800BEA3C(D_801146BC, D_801146C0);
                dispatch_handler(1);
            } else {
                func_800BEA3C(D_801146BC, D_801146C0);
                dispatch_handler(22);
            }
            label = (countdown_state.indexed + D_803BA8B0[i])[countdown_state.indices[1]];
            state_utility(x, y, label);
        }
        if ((D_803B6B14[index + 21].angle <= 0.6981316804885864f || D_803B6B14[index + 21].angle >= 2.4434609413146973f) && D_803B6B14[index + 21].alpha) {
            func_800ED66C(D_803B6B14[index + 21].alpha);
            brake_light_update(0, D_803B6B14[index + 21].position, &D_80150B70[0], 0, point);
            x = point[0] - 35.0f * D_803B771C;
            y = point[1];
            if (D_803B6B14[index + 21].angle < 1.5707963705062866f) {
                func_800BEA3C(D_801146BC, D_801146C0);
                dispatch_handler(1);
            } else {
                func_800BEA3C(D_801146BC, D_801146C0);
                dispatch_handler(22);
            }
            switch (index) {
            case 0:
                label = D_8011A8D8[D_8014978C];
                state_utility(x, y, label);
                break;
            case 1:
                label = countdown_state.indexed[countdown_state.indices[35] + D_801426EC];
                state_utility(x, y, label);
                break;
            case 2:
                sprintf(text, D_803B9244, D_80152734);
                state_utility(x, y, text);
                break;
            case 3:
                if (D_80146108.a == 0) label = countdown_state.text[208];
                else if (D_80146108.a == 1) label = countdown_state.text[130];
                else label = countdown_state.text[12];
                state_utility(x, y, label);
                break;
            case 4:
                state_utility(x, y, (countdown_state.indexed + (D_80146108.b == 0 ? 0 : 1))[countdown_state.indices[36]]);
                break;
            case 5:
                sprintf(text, D_803B9248, D_80146108.c);
                state_utility(x, y, text);
                break;
            case 6:
                sprintf(text, D_803B924C, D_80146108.d);
                state_utility(x, y, text);
                break;
            case 7:
                sprintf(text, D_803B9250, D_80146108.e);
                state_utility(x, y, text);
                break;
            case 8:
                state_utility(x, y, D_80146108.f == 1 ? countdown_state.text[208] : countdown_state.text[130]);
                break;
            case 9:
                sprintf(text, D_803B9254, D_80146108.g);
                state_utility(x, y, text);
                break;
            case 10:
                sprintf(text, D_803B9258, D_80146108.h);
                state_utility(x, y, text);
                break;
            case 11:
                state_utility(x, y, (countdown_state.indexed + (D_80146108.i == 0 ? 0 : 1))[countdown_state.indices[16]]);
                break;
            case 12:
                state_utility(x, y, (countdown_state.indexed + (D_80152570 == 0 ? 0 : 1))[countdown_state.indices[16]]);
                break;
            case 13:
                if (D_8014A110 == 3) {
                    state_utility(x, y, (countdown_state.indexed + (D_801543DA[D_80154628].flag == 0 ? 0 : 1))[countdown_state.indices[16]]);
                } else if (D_80146108.mode == 2) {
                    state_utility(x, y, countdown_state.indexed[countdown_state.indices[18] + 2]);
                } else {
                    state_utility(x, y, (countdown_state.indexed + (D_80146108.mode == 0 ? 0 : 1))[countdown_state.indices[16]]);
                }
                break;
            case 17:
                sprintf(text, D_803B925C, D_80142724);
                state_utility(x, y, text);
                break;
            case 20:
                state_utility(x, y, (countdown_state.indexed + (D_80142760 == 0 ? 0 : 1))[countdown_state.indices[16]]);
                break;
            }
        }
    }
    func_800B669C(0, 3);
    func_800ED66C(-1.0f);
    render_helper(-1.0f);
}


