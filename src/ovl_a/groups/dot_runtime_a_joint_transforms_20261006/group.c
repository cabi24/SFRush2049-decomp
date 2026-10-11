/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* A:80390A28 local group candidate, 100/100 words, 400 bytes.
 * Complete real 90DCC caller is unclaimed context. */
typedef signed char s8;typedef unsigned char u8;
typedef short s16;typedef unsigned short u16;
typedef int s32;typedef unsigned int u32;typedef float f32;
typedef union Color {u32 word;u8 channel[4];} Color;
typedef struct ModelState {
    s32 handle,parts[5];
    u8 unaccessed24[4];
    s32 joints[4];
    u8 unaccessed44[20];
} ModelState;
typedef struct LoadState {
    s8 created,hidden,parts_ready,load_started;
    s32 resource;
    s8 appearance[3],unaccessed11;
    s32 texture;
    f32 front_scale,rear_scale;
} LoadState;
typedef struct Transform {f32 matrix[3][3],position[3];} Transform;
typedef struct ModelDescription {u8 unaccessed[112];f32 joint_position[4][3];} ModelDescription;
typedef struct ObjectFlags {u32 flags,word4;u8 other[60];} ObjectFlags;
extern ModelState D_80139320[];
extern LoadState D_803BA2F0[];
extern ModelDescription *D_80110D08[13];
extern f32 D_801112DC[][13],D_801113E0[][13];
extern s8 D_80111589[][13];
extern s32 D_80143F68[];
extern f32 D_8011418C[3][3],D_801106C0[];
extern s16 D_80151AD0,D_80142B08[];
extern s8 D_801613AB,D_80156994,D_8014978C;
extern Color D_803B65B4;
extern ObjectFlags D_8012E700[];
extern s32 func_8008B3A0(s16);
extern void func_8008B32C(f32 (*)[3],f32 (*)[3],f32);
extern void math_utility(void *,void *);
extern void model_data_load(s32,s32,u32);
extern void model_transform_setup(s32,s32,u32);
extern s32 player_state_get(s32);
extern void func_80390D2C(s16);
extern s32 func_80390BC0(s16,s16,s32 *);
extern void sound_call_simple(s16,s32);
extern void sfx_volume_set(s16,s16,s8);
extern void music_seq_load(s16,s16,s32);
extern void sfx_stop(s16,s16,s16);
extern void sound_bank_unload(s16,s16,u8,u8,u8);
extern void func_80092FE0(s16,s32 *);
extern void func_8008D6FC(s16,void *,void *);
extern void func_8008D870(s16,s32,s32);

void func_80390A28(s16 id,s16 player)
{
    s16 model = id % 13;
    s16 i;
    Transform *transform;
    ModelDescription *description;
    description = D_80110D08[model]; for (i = 0; i < 4; i++) {
        transform = (Transform *)func_8008B3A0((s16)D_80139320[id].joints[i]);
        transform->position[0] = description->joint_position[i][0];
        transform->position[1] = description->joint_position[i][1] + 1.25f;
        transform->position[2] = description->joint_position[i][2];
        if (i >= 2) {
            if (D_801113E0[player + 1][model] != 1.0f)
                func_8008B32C(D_8011418C,transform->matrix,D_801113E0[player + 1][model]);
            else math_utility(D_8011418C,transform->matrix);
        } else {
            if (D_801112DC[player + 1][model] != 1.0f)
                func_8008B32C(D_8011418C,transform->matrix,D_801112DC[player + 1][model]);
            else math_utility(D_8011418C,transform->matrix);
        }
    }
}

void func_80390DCC(s16 id,s16 player,u8 appearance0,u8 appearance1,
    u8 appearance2,s16 part_texture,f32 *position,f32 (*matrix)[3],u8 alpha)
{
    f32 local_matrix[3][3];
    s16 view_mask;
    Color color = D_803B65B4;
    LoadState *state;
    ModelState *model_state;
    s16 model;
    s32 texture;
    s32 material;
    f32 scale;
    ObjectFlags *object;
    if (D_80151AD0 < 3) {
        if (player == 2) player = 0;
        if (player == 3) player = 1;
    }
    switch (player) {
    case 0:view_mask=1;break;
    case 1:view_mask=2;break;
    case 2:view_mask=4;break;
    case 3:view_mask=8;break;
    }
    state = &D_803BA2F0[id];
    if (!alpha) {
        if (!state->hidden) {
            state->hidden = 1;
            model_data_load(D_80139320[id].handle,0,view_mask);
            return;
        }
        if (state->resource != -1 && player_state_get(state->resource) && state->created) {
            func_80390D2C(id);
            state->created = 0;
            state->parts_ready = 0;
            state->resource = -1;
            state->load_started = 0;
            state->appearance[0] = -1;
            state->appearance[1] = -1;
            state->appearance[2] = -1;
            state->texture = -1;
            state->front_scale = -1.0f;
            state->rear_scale = -1.0f;
            model_state = &D_80139320[id];
            sound_call_simple((s16)model_state->handle,1);
            model_state->handle = 0;
            model_state->parts[0] = 0;
            model_state->parts[1] = 0;
            model_state->parts[2] = 0;
            model_state->parts[3] = 0;
            model_state->parts[4] = 0;
            model_state->joints[0] = 0;
            model_state->joints[1] = 0;
            model_state->joints[2] = 0;
            model_state->joints[3] = 0;
        }
        return;
    }
    if (!state->load_started) {
        state->load_started = func_80390BC0(id,(s16)(id % 13),&state->resource);
        return;
    }
    if (!player_state_get(state->resource)) return;
    if (!state->parts_ready) {
        state->parts_ready = 1;
        sfx_volume_set(id,(s16)(id % 13),D_80142B08[id]);
    }
    model = id % 13;
    if (!state->created) {
        state->created = 1;
        state->hidden = 0;
        music_seq_load(id,view_mask,0);
        model_state = &D_80139320[id];
        model_transform_setup(model_state->handle,1,view_mask);
        model_data_load(model_state->parts[3],1,view_mask);
        func_80390A28(id,player);
        sfx_stop(model,player,part_texture);
    }
    model_state = &D_80139320[id];
    if (state->hidden) {
        model_transform_setup(model_state->handle,0,view_mask);
        state->hidden = 0;
    }
    if (appearance0 != state->appearance[0] || appearance1 != state->appearance[1]
        || appearance2 != state->appearance[2]) {
        sound_bank_unload(id,model,appearance0,appearance1,appearance2);
        state->appearance[0] = appearance0;
        state->appearance[1] = appearance1;
        state->appearance[2] = appearance2;
    }
    color.channel[3] = alpha;
    func_80092FE0(id,(s32 *)&color);
    math_utility(matrix,local_matrix);
    if (D_801613AB) {
        scale = D_801106C0[D_801613AB];
        local_matrix[0][1] *= scale;
        local_matrix[1][1] *= scale;
        local_matrix[2][1] *= scale;
    }
    func_8008D6FC((s16)model_state->handle,position,local_matrix);
    material = D_80111589[player][model];
    if (state->texture != material) {
        texture = D_80143F68[material + 3];
        state->texture = material;
        if (!D_80156994 && D_8014978C >= 0 && D_8014978C < 6)
            func_8008D870((s16)model_state->joints[0],D_80143F68[16],-1);
        else
            func_8008D870((s16)model_state->joints[0],texture,-1);
    }
    if (!D_80156994 && D_8014978C >= 0 && D_8014978C < 6)
        func_8008D870((s16)model_state->joints[0],D_80143F68[16],-1);
    if (state->front_scale != D_801112DC[player + 1][model]
        || state->rear_scale != D_801113E0[player + 1][model]) {
        state->front_scale = D_801112DC[player + 1][model];
        state->rear_scale = D_801113E0[player + 1][model];
        func_80390A28(id,player);
    }
    object = &D_8012E700[model_state->handle];
    object->word4 = 0;
    object->flags &= ~0x10;
    object->flags &= ~0x20;
}
