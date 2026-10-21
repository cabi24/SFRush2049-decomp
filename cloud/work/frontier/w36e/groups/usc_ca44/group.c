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
extern s8 D_8011157C[5][13];      /* gTireRims */
extern s32 D_80143F68[];
extern f32 D_8011418C[3][3],D_801106C4[];   /* unitmatrix, FSCLS[3] */
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
extern void sfx_volume_set(s16,s16,s16);
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

void CreateSelectCar(s16 slot, s16 player, s32 cam)
{
    music_seq_load(slot, cam, 0);
    model_transform_setup(D_80139320[slot].handle, 1, cam);
    model_data_load(D_80139320[slot].parts[3], 1, cam);
    func_80390A28(slot, player);
}

/* UpdateSelectCar: direct typed port of MAT3 selectcarload.c UpdateSelectCar (GC 0x8003bf00), N64 9-argument
 * variant (r, g, b colour instead of PaintJob; SetupCarColor = sound_bank_unload). */
void func_80390DCC(s16 slot, s16 player, u8 r, u8 g, u8 b, s16 sheenIdx, f32 *pos, f32 (*uvs)[3], u8 alpha)
{
    f32 newuvs[3][3];
    s16 idx;
    s16 cam;
    s32 padA;
    Color alphaCol = D_803B65B4;
    s32 wheelTex;
    u8 padB[44];
    idx = slot % 13;
    if (D_80151AD0 < 3) {
        if (player == 2) player = 0;
        if (player == 3) player = 1;
    }
    switch (player) {
    case 0: cam = 1; break;
    case 1: cam = 2; break;
    case 2: cam = 4; break;
    case 3: cam = 8; break;
    }
    if (alpha == 0) {
        if (!D_803BA2F0[slot].hidden) {
            D_803BA2F0[slot].hidden = 1;
            model_data_load(D_80139320[slot].handle, 0, cam);
        } else if (D_803BA2F0[slot].resource != -1 && player_state_get(D_803BA2F0[slot].resource) && D_803BA2F0[slot].created) {
            func_80390D2C(slot);
            D_803BA2F0[slot].created = 0;
            D_803BA2F0[slot].parts_ready = 0;
            D_803BA2F0[slot].resource = -1;
            D_803BA2F0[slot].load_started = 0;
            D_803BA2F0[slot].appearance[0] = -1;
            D_803BA2F0[slot].appearance[1] = -1;
            D_803BA2F0[slot].appearance[2] = -1;
            D_803BA2F0[slot].texture = -1;
            D_803BA2F0[slot].front_scale = D_803BA2F0[slot].rear_scale = -1.0f;
            sound_call_simple(D_80139320[slot].handle, 1);
            D_80139320[slot].handle = 0;
            D_80139320[slot].parts[0] = 0;
            D_80139320[slot].parts[1] = 0;
            D_80139320[slot].parts[2] = 0;
            D_80139320[slot].parts[3] = 0;
            D_80139320[slot].parts[4] = 0;
            D_80139320[slot].joints[0] = 0;
            D_80139320[slot].joints[1] = 0;
            D_80139320[slot].joints[2] = 0;
            D_80139320[slot].joints[3] = 0;
        }
    } else {
        if (!D_803BA2F0[slot].load_started) {
            D_803BA2F0[slot].load_started = func_80390BC0(slot, idx, &D_803BA2F0[slot].resource);
            return;
        }
        if (!player_state_get(D_803BA2F0[slot].resource))
            return;
        if (!D_803BA2F0[slot].parts_ready) {
            D_803BA2F0[slot].parts_ready = 1;
            sfx_volume_set(slot, idx, D_80142B08[slot]);
        }
        if (!D_803BA2F0[slot].created) {
            D_803BA2F0[slot].created = 1;
            D_803BA2F0[slot].hidden = 0;
            CreateSelectCar(slot, player, cam);
            sfx_stop(idx, player, sheenIdx);
        }
        if (D_803BA2F0[slot].hidden) {
            model_transform_setup(D_80139320[slot].handle, 0, cam);
            D_803BA2F0[slot].hidden = 0;
        }
        if (D_803BA2F0[slot].appearance[0] != r || D_803BA2F0[slot].appearance[1] != g || D_803BA2F0[slot].appearance[2] != b) {
            sound_bank_unload(slot, idx, r, g, b);
            D_803BA2F0[slot].appearance[0] = r;
            D_803BA2F0[slot].appearance[1] = g;
            D_803BA2F0[slot].appearance[2] = b;
        }
        alphaCol.channel[3] = alpha;
        func_80092FE0(slot, (s32 *)&alphaCol);
        math_utility(uvs, newuvs);
        if (D_801613AB) {
            newuvs[0][1] *= D_801106C4[D_801613AB - 1];
            newuvs[1][1] *= D_801106C4[D_801613AB - 1];
            newuvs[2][1] *= D_801106C4[D_801613AB - 1];
        }
        func_8008D6FC(D_80139320[slot].handle, pos, newuvs);
        if (D_803BA2F0[slot].texture != D_8011157C[player + 1][idx]) {
            wheelTex = D_80143F68[D_8011157C[player + 1][idx] + 3];
            D_803BA2F0[slot].texture = D_8011157C[player + 1][idx];
            if (!D_80156994 && D_8014978C >= 0 && D_8014978C < 6)
                func_8008D870(D_80139320[slot].joints[0], D_80143F68[16], -1);
            else
                func_8008D870(D_80139320[slot].joints[0], wheelTex, -1);
        }
        if (!D_80156994 && D_8014978C >= 0 && D_8014978C < 6)
            func_8008D870(D_80139320[slot].joints[0], D_80143F68[16], -1);
        if (D_803BA2F0[slot].front_scale != D_801112DC[player + 1][idx] || D_803BA2F0[slot].rear_scale != D_801113E0[player + 1][idx]) {
            D_803BA2F0[slot].front_scale = D_801112DC[player + 1][idx];
            D_803BA2F0[slot].rear_scale = D_801113E0[player + 1][idx];
            func_80390A28(slot, player);
        }
        D_8012E700[D_80139320[slot].handle].flags &= ~16;
        D_8012E700[D_80139320[slot].handle].flags &= ~32;
        D_8012E700[D_80139320[slot].handle].word4 = 0;
    }
}
