/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete image-B three-body research closure. Children are genuine static
 * bodies; the externally visible no-input root contains their private ABI. */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;

typedef struct EffectObject {
    u8 unknown00[4];
    u8 flags;
    u8 unknown05[7];
    s32 scene;
    u8 unknown10[0x50 - 0x10];
    s16 animation;
} EffectObject;
typedef struct EffectEntry {
    EffectObject *object;
    s16 state;
    s16 eligibility;
} EffectEntry;
typedef struct EffectControl {
    s8 count;
    s8 selected_fast;
    s8 selected_second;
    s8 selected_first;
    f32 timer_fast;
    f32 timer_first;
    f32 timer_second;
    EffectEntry *entries;
} EffectControl;
typedef struct EffectPlayer {
    u8 unknown00[0x38C];
    u32 status;
    u8 unknown390[0x3A0 - 0x390];
    s8 countdown;
    u8 alpha;
    s8 phase;
    u8 unknown3A3[5];
    f32 timer;
    u8 unknown3AC[0x3B8 - 0x3AC];
} EffectPlayer;

extern EffectControl D_80399AE0;
extern EffectPlayer D_80152818[];
extern s16 D_8014A108;
extern f32 D_8002EB94;
extern u16 D_80142A82, D_80142A84, D_80142A8E;
extern f32 func_8008B2E4(f32 range);
extern void func_80090770(s16 index, u16 value);
extern void func_8008B0D8(s32 index, s32 mode, u32 viewports);

static void func_80390B10(void)
{
    s32 index;
    if (D_80399AE0.timer_second == -2.0f) {
        if (!(D_80399AE0.entries[D_80399AE0.selected_second].object->flags & 2)) {
            D_80399AE0.timer_second = func_8008B2E4(60.0f) + 90.0f;
            for (index = 0; index < D_80399AE0.count; index++) {
                if (D_80399AE0.entries[index].state == 3)
                    D_80399AE0.entries[index].state = 1;
            }
            D_80399AE0.entries[D_80399AE0.selected_second].state = 3;
            D_80399AE0.entries[D_80399AE0.selected_second].eligibility = 1;
        }
    } else {
        D_80399AE0.timer_second -= D_8002EB94;
        if (D_80399AE0.timer_second < 0.0f) {
            index = func_8008B2E4((f32) D_80399AE0.count);
            while (D_80399AE0.entries[index].eligibility != 1 ||
                   D_80399AE0.entries[index].state == 3) {
                index++;
                if (index >= D_80399AE0.count) index = 0;
            }
            D_80399AE0.selected_second = index;
            D_80399AE0.entries[index].state = 1;
            D_80399AE0.entries[index].eligibility = 3;
            D_80399AE0.entries[index].object->flags |= 2;
            D_80399AE0.entries[index].object->animation = 359;
            func_80090770((s16) D_80399AE0.entries[index].object->scene, D_80142A8E);
            func_8008B0D8(D_80399AE0.entries[index].object->scene, 0, 15);
            D_80399AE0.timer_second = -2.0f;
        }
    }
}

static void func_80390D38(void)
{
    s32 index;
    if (D_80399AE0.timer_fast == -2.0f) {
        if (!(D_80399AE0.entries[D_80399AE0.selected_fast].object->flags & 2)) {
            D_80399AE0.timer_fast = func_8008B2E4(10.0f) + 5.0f;
            for (index = 0; index < D_80399AE0.count; index++) {
                if (D_80399AE0.entries[index].state == 4)
                    D_80399AE0.entries[index].state = 1;
            }
            D_80399AE0.entries[D_80399AE0.selected_fast].state = 4;
            D_80399AE0.entries[D_80399AE0.selected_fast].eligibility = 1;
        }
    } else {
        D_80399AE0.timer_fast -= D_8002EB94;
        if (D_80399AE0.timer_fast < 0.0f) {
            index = func_8008B2E4((f32) D_80399AE0.count);
            while (D_80399AE0.entries[index].eligibility != 1 ||
                   D_80399AE0.entries[index].state == 4) {
                index++;
                if (index >= D_80399AE0.count) index = 0;
            }
            D_80399AE0.selected_fast = index;
            D_80399AE0.entries[index].state = 1;
            D_80399AE0.entries[index].eligibility = 4;
            D_80399AE0.entries[index].object->flags |= 2;
            D_80399AE0.entries[index].object->animation = 353;
            func_80090770((s16) D_80399AE0.entries[index].object->scene, D_80142A82);
            func_8008B0D8(D_80399AE0.entries[index].object->scene, 0, 15);
            D_80399AE0.timer_fast = -2.0f;
        }
    }
}

void func_80390F60(void)
{
    s32 index;
    s32 alpha;
    EffectPlayer *player;
    u8 alpha_steps[8] = { 16, 32, 64, 96, 112, 144, 176, 176 };

    func_80390D38();
    func_80390B10();
    for (index = 0, player = D_80152818; index < D_8014A108; index++, player++) {
        if (player->status & 1) {
            if (player->phase == 3) {
                player->alpha = alpha_steps[player->countdown];
            } else if (player->phase == 1) {
                player->countdown = 0;
                if (player->timer > 0.0f) {
                    player->alpha -= 8;
                    if (player->alpha <= 16) {
                        player->alpha = 16;
                        player->phase = 3;
                    }
                    player->timer -= D_8002EB94;
                } else {
                    player->phase = 3;
                }
            } else {
                player->countdown = 0;
                if (player->timer > 0.0f) {
                    alpha = player->alpha + 8;
                    if (alpha >= 256) player->alpha = 255;
                    else player->alpha = alpha;
                    player->timer -= D_8002EB94;
                } else {
                    player->phase = 0;
                    player->status &= ~1;
                }
            }
        }
        if (player->countdown > 0) player->countdown--;
    }
    if (D_80399AE0.timer_first == -2.0f) {
        if (!(D_80399AE0.entries[D_80399AE0.selected_first].object->flags & 2)) {
            D_80399AE0.timer_first = func_8008B2E4(60.0f) + 90.0f;
            for (index = 0; index < D_80399AE0.count; index++) {
                if (D_80399AE0.entries[index].state == 2)
                    D_80399AE0.entries[index].state = 1;
            }
            D_80399AE0.entries[D_80399AE0.selected_first].state = 2;
            D_80399AE0.entries[D_80399AE0.selected_first].eligibility = 1;
        }
    } else {
        D_80399AE0.timer_first -= D_8002EB94;
        if (D_80399AE0.timer_first < 0.0f) {
            index = func_8008B2E4((f32) D_80399AE0.count);
            while (D_80399AE0.entries[index].eligibility != 1 ||
                   D_80399AE0.entries[index].state == 2) {
                index++;
                if (index >= D_80399AE0.count) index = 0;
            }
            D_80399AE0.selected_first = index;
            D_80399AE0.entries[index].state = 1;
            D_80399AE0.entries[index].eligibility = 2;
            D_80399AE0.entries[index].object->flags |= 2;
            D_80399AE0.entries[index].object->animation = 354;
            func_80090770((s16) D_80399AE0.entries[index].object->scene, D_80142A84);
            func_8008B0D8(D_80399AE0.entries[index].object->scene, 0, 15);
            D_80399AE0.timer_first = -2.0f;
        }
    }
}
