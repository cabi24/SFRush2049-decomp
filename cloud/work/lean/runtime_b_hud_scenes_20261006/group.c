/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Player { u8 unknown000[0x384]; s8 kind, lives; u8 unknown386[6]; s32 flags; u8 unknown390[0x28]; } Player;
typedef struct Status { s16 kind, handle; f32 uv[3][3], position[3]; } Status;
typedef struct Handle { s16 handle, unknown2; f32 uv[3][3], position[3]; } Handle;
typedef struct Camera { f32 uv[3][3], position[3]; u8 unknown30[104]; } Camera;
typedef struct Scene { u8 unknown00[16]; f32 scale; u8 unknown14[48]; } Scene;
typedef struct Point { s16 x, y; } Point;
typedef union Color { u32 word; struct { u8 r,g,b,a; } rgba; } Color;
typedef struct NameQuad { char *name[4]; } NameQuad;
typedef struct ScoreState {
    s16 active, lock;
    f32 started;
    s32 unknown08, score, shown_score, total_score, working;
    u8 unknown1C[4];
    s16 source_counts[10], counts[10];
    u8 unknown48[29];
    s8 message_kind;
    u8 unknown66;
    s8 message_index;
    u8 unknown68[4];
    s32 multiplier, pending, state;
} ScoreState;
typedef struct Effect { s16 state, handle; f32 uv[3][3], position[3], alpha; s16 x,y,from_x,from_y,to_x,to_y; f32 timer; } Effect;
extern s16 D_80151AD0, D_8014A108;
extern s32 D_8014A110, D_80393B74[];
extern Player D_80152818[];
extern Status D_80395C00[], D_80395CD0[];
extern Handle D_80395DA0[];
extern Camera D_80150B70[];
extern Scene D_8012E700[];
extern char *D_80393B68[], *D_80393B40[];
extern volatile u8 D_80140BDC;
extern f32 D_80116178, D_801543CC, D_8002EB94;
extern Point D_80394210[4][4], D_803941D0[4][4];
extern s32 D_803940D0[4][4][2], D_80393FD0[4][4][2], D_80115F28[4][4][2];
extern Color D_803942A4, D_80394270[], D_803942A0;
extern NameQuad D_803942A8;
extern s8 D_8012E67C[];
extern u16 D_801427C0[];
extern ScoreState D_80152038[];
extern Effect D_803950C0[4][10];
extern char D_80394AA8[];
extern u32 state_word_a;
extern s32 D_801170FC;
extern s32 string_copy_format(char *,s8,s8,s8);
extern s32 func_8008E398(s32,f32[3][3],s32,u32);
extern void sound_call_minimal(s16);
extern void model_data_load(s32,s32,u32);
extern void model_transform_setup(s32,s32,u32);
extern void func_8008B32C(f32[3][3],f32[3][3],f32);
extern void func_80090E9C(f32,f32[3][3]);
extern void func_80090F44(f32,f32[3][3]);
extern void car_lights_render(s32,s16 *,Camera *,f32,f32 *);
extern void func_8008E06C(s16,u32 *);
extern void func_80090770(s16,u16);
extern void fcvt_wrapper(char *,char *,...);

s32 func_80391650(void *unused)
{
    s32 i, kind;
    Point point;
    Status *status;
    for (i = 0; i < D_80151AD0; i++) {
        status = &D_80395CD0[i];
        if (D_80152818[i].flags) {
            if (D_80152818[i].flags & 1) kind = 1;
            else if (D_80152818[i].flags & 2) kind = 2;
        } else {
            kind = 0;
        }
        if (kind != status->kind) {
            if (status->handle != -1) {
                sound_call_minimal(status->handle);
                status->handle = -1;
            }
            status->kind = kind;
        }
        if (status->kind && status->handle == -1) {
            status->handle = func_8008E398(
                string_copy_format(D_80393B68[status->kind], 0, D_80140BDC - 1, 1),
                status->uv, -1, 0);
            model_data_load(status->handle, 1, 15);
            model_transform_setup(status->handle, 0, D_80393B74[i]);
            D_8012E700[status->handle].scale = 1.5f;
        }
        func_8008B32C(D_80150B70[i].uv, status->uv, 0.025f);
        func_80090E9C(D_80116178, status->uv);
        point.x = D_80394210[D_80151AD0 - 1][i].x;
        point.y = D_80394210[D_80151AD0 - 1][i].y;
        car_lights_render(i, &point.x, &D_80150B70[i], 2.0f, status->position);
    }
    return 1;
}
s32 func_80391864(void *unused)
{
    s32 i, resource;
    Point point;
    Color color;
    Status *status;
    for (i = 0; i < D_80151AD0; i++) {
        status = &D_80395C00[i];
        if (status->kind != D_80152818[i].kind) {
            if (status->handle != -1) {
                sound_call_minimal(status->handle);
                status->handle = -1;
            }
            status->kind = D_80152818[i].kind;
        }
        if (status->kind != 8 && status->handle == -1) {
            color.word = D_803942A4.word;
            switch ((s32)status->kind) {
            case 0: resource = 216; break;
            case 1: resource = 217; break;
            case 2: resource = 218; break;
            case 3: resource = 219; break;
            case 4: resource = 220; break;
            case 5: resource = 221; break;
            case 6: resource = 222; break;
            case 7: resource = 223; break;
            }
            status->handle = func_8008E398(D_801427C0[resource], status->uv, -1, 0x42000);
            func_8008E06C(status->handle, &color.word);
            D_8012E700[status->handle].scale = 1.5f;
            model_data_load(status->handle, 1, 15);
            model_transform_setup(status->handle, 0, D_80393B74[i]);
        }
        if (status->kind == 7) func_8008B32C(D_80150B70[i].uv, status->uv, 0.175f);
        else func_8008B32C(D_80150B70[i].uv, status->uv, 0.069f);
        func_80090E9C(D_80116178, status->uv);
        point.x = D_803941D0[D_80151AD0 - 1][i].x;
        point.y = D_803941D0[D_80151AD0 - 1][i].y;
        if (status->kind == 7) car_lights_render(i, &point.x, &D_80150B70[i], 5.75f, status->position);
        else car_lights_render(i, &point.x, &D_80150B70[i], 2.0f, status->position);
    }
    return 1;
}
s32 func_80391650(void *);
s32 func_80391864(void *);
void func_80391B00(void)
{
    s32 i, j, kind, y_offset;
    Point point;
    NameQuad names;
    Color color;
    Color player_color;
    char name[32];
    Handle *handle;
    ScoreState *score;
    Effect *effect, *selected, *previous;
    f32 alpha, fraction;

    if (D_8014A110 == 6) {
        func_80391864(0);
        func_80391650(0);
        for (i = 0; i < D_80151AD0; i++) {
            handle = &D_80395DA0[i];
            if (handle->handle == -1) {
                names = D_803942A8;
                player_color.rgba.r = D_80394270[D_8012E67C[i]].rgba.r;
                player_color.rgba.g = D_80394270[D_8012E67C[i]].rgba.g;
                player_color.rgba.b = D_80394270[D_8012E67C[i]].rgba.b;
                player_color.rgba.a = 255;
                handle->handle = func_8008E398(
                    string_copy_format(names.name[D_8012E67C[i]], 0, D_80140BDC - 1, 1),
                    handle->uv, -1, 0);
                D_8012E700[handle->handle].scale = 1.5f;
                model_data_load(handle->handle, 1, 15);
                model_transform_setup(handle->handle, 0, D_80393B74[i]);
            }
            func_8008B32C(D_80150B70[i].uv, handle->uv, D_8014A108 >= 3 ? 0.012f : 0.009f);
            func_80090F44(-1.5707963267948966f, handle->uv);
            point.x = D_803940D0[D_80151AD0 - 1][i][0];
            point.y = D_803940D0[D_80151AD0 - 1][i][1];
            car_lights_render(i, &point.x, &D_80150B70[i], 2.0f, handle->position);
        }
    }
    if (D_8014A110 == 4) {
        color.word = D_803942A0.word;
        if ((state_word_a & 0x7C03FFFE) || D_801170FC) return;
        for (i = 0; i < D_8014A108; i++) {
            score = &D_80152038[i];
            if (score->lock) alpha = (1.0f - (D_801543CC - score->started) / 2.69f) * 255.0f;
            else alpha = 255.0f;
            if (alpha < 0.0f) alpha = 0.0f;
            color.rgba.a = (u8)alpha;
            if (score->active && (D_801543CC - score->started < 2.69f || score->working)) {
                for (kind = 0; kind < 10; kind++) {
                    if (!score->state) score->counts[kind] = score->source_counts[kind];
                    if (score->counts[kind]) {
                        for (j = 0; j < 10; j++) {
                            effect = &D_803950C0[i][j];
                            selected = effect;
                            if (effect->state == kind) break;
                            if (effect->state == 11) {
                                fcvt_wrapper(name, D_80394AA8, D_80393B40[kind]);
                                if (effect->handle != -1) {
                                    func_80090770(effect->handle, (u16)string_copy_format(name, 0, D_80140BDC - 1, 1));
                                } else {
                                    effect->handle = func_8008E398(
                                        string_copy_format(name, 0, D_80140BDC - 1, 1), effect->uv, -1, 0x42000);
                                }
                                D_8012E700[effect->handle].scale = 1.5f;
                                effect->state = kind;
                                effect->timer = 0.5f;
                                y_offset = D_80151AD0 == 1 ? -28 : D_80151AD0 == 2 ? -16 : -12;
                                selected->from_x = D_80115F28[D_80151AD0 - 1][i][0];
                                selected->from_y = D_80115F28[D_80151AD0 - 1][i][1] + y_offset;
                                selected->to_x = D_80393FD0[D_80151AD0 - 1][i][0];
                                selected->to_y = D_80393FD0[D_80151AD0 - 1][i][1] + y_offset;
                                for (j--; j >= 0; j--) {
                                    previous = &D_803950C0[i][j];
                                    previous->from_x = previous->x;
                                    previous->from_y = previous->y;
                                    previous->timer = 0.5f;
                                    y_offset -= D_80151AD0 == 1 ? 18 : 8;
                                    previous->to_x = D_80393FD0[D_80151AD0 - 1][i][0];
                                    previous->to_y = D_80393FD0[D_80151AD0 - 1][i][1] + y_offset;
                                }
                                break;
                            }
                        }
                        if (selected->handle != -1) {
                            selected->timer -= D_8002EB94;
                            if (selected->timer < 0.0f) selected->timer = 0.0f;
                            fraction = selected->timer / 0.5f;
                            selected->x = (selected->from_x - selected->to_x) * fraction + selected->to_x;
                            selected->y = (selected->from_y - selected->to_y) * fraction + selected->to_y;
                            car_lights_render(i, &selected->x, &D_80150B70[i], 2.0f, selected->position);
                            func_8008B32C(D_80150B70[i].uv, selected->uv,
                                D_80151AD0 == 1 ? 0.005f : D_80151AD0 == 2 ? 0.0035f : 0.005f);
                            selected->alpha = alpha;
                            color.rgba.a = (u8)alpha;
                            func_8008E06C(selected->handle, &color.word);
                            model_transform_setup(selected->handle, 0, D_80393B74[i]);
                        }
                    }
                }
            } else if (!score->working) {
                score->active = 0;
                score->lock = 0;
                score->multiplier = 1;
                score->message_index = 0;
                score->message_kind = 0;
                score->shown_score = 0;
                for (j = 0; j < 10; j++) {
                    score->counts[j] = 0;
                    selected = &D_803950C0[i][j];
                    if (selected->handle != -1) {
                        sound_call_minimal(selected->handle);
                        selected->handle = -1;
                    }
                    selected->state = 11;
                }
            }
        }
    }
}
