/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef f32 Vec3[3];
typedef f32 Mat3[3][3];
typedef union Id { s32 handle; struct { s16 high, index; } part; } Id;
typedef struct Puff {
    struct Puff *next;  /* 0 */
    Mat3 matrix;        /* 4 */
    Vec3 position;      /* 40 */
    Id id;              /* 52 */
    u8 pad56[4];
    f32 life;           /* 60 */
    f32 timer;          /* 64 */
    Vec3 velocity;      /* 68 */
    u32 flags;          /* 80 */
    s16 car;            /* 84 */
    s16 frame;          /* 86 */
} Puff;
typedef struct Player {
    u8 pad0[8];
    Vec3 position;      /* 8 */
    Vec3 direction;     /* 20 */
    u8 pad32[12];
    Mat3 matrix;        /* 44 */
    u8 pad80[36];
    Vec3 exhaust[4];    /* 116 */
    u8 pad164[84];
    s16 speed;          /* 248 */
    u8 pad250[610];
    s8 slot;            /* 860 */
    s8 view;            /* 861 */
    u8 pad862[90];
} Player;
typedef struct Car {
    u8 pad0[8];
    u8 model;           /* 8 */
    u8 pad9[1555];
    u16 exhaust[4];     /* 1564 */
    u8 pad1572[28];
    s8 enabled;         /* 1600 */
    u8 pad1601[455];
} Car;
typedef struct Resource {
    u8 pad0[12];
    f32 scale;          /* 12 */
    u8 pad16[4];
    s16 frame;          /* 20 */
    u8 pad22[41];
    u8 alpha;           /* 63 */
    u8 pad64[4];
} Resource;
extern Puff *D_8013F1F0;
extern s32 D_8013F1E0;
extern Player D_80152818[];
extern Car D_8014A250[];
extern u32 D_801392D8[];
extern s32 D_8011735C;
extern Resource D_8012E700[];
extern u16 D_80142948[2][9];
extern f32 D_8011744C[];
extern s8 D_80156994, D_8014978C;
extern volatile f32 D_8002EB94;
extern void model_data_load(s32, s32, u32);
extern void model_transform_setup(s32, s32, u32);
extern f32 func_8008E0B8(f32 *);
extern void vector_normalize_length(f32 *, f32 [3][3]);
extern void math_utility(void *, void *);
extern void entity_spawn_callback(s16, s32, s32);
extern void func_800AFA84(void *, void *);
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)

static s32 rand(void)
{
    D_8011735C = D_8011735C * 1103515245 + 12345;
    return (D_8011735C >> 16) & 0x7FFF;
}

static void resource_set_frame(s16 index, u16 frame)
{
    D_8012E700[index].frame = frame;
}

void hud_render(void)
{
    Puff *puff;
    s16 car;
    s16 type;
    s16 speed;
    s16 aspeed;
    f32 scale;
    s32 i, j;
    Player *player;
    Vec3 position;
    Vec3 direction;
    s32 pad1[2];
    Puff *next;
    s32 pad2[5];
    Mat3 matrix;
    s32 pad3[11];

    for (puff = D_8013F1F0; puff != 0; puff = next) {
        next = puff->next;
        car = puff->car;
        type = (puff->flags >> 17) & 0xF;
        if (puff->flags & 0x10) {
            player = &D_80152818[car];
            speed = player->speed >> 2;
            aspeed = fabsf((f32)speed);
            if (player->view < 2) {
                model_data_load(puff->id.handle, 1, 1 << player->slot);
            } else {
                model_transform_setup(puff->id.handle, 0, 1 << player->slot);
            }
            if (D_8014A250[car].exhaust[type] != 2 || aspeed < 10) {
                switch (type) {
                case 0:
                    D_801392D8[car] &= ~0x100;
                    break;
                case 1:
                    D_801392D8[car] &= ~0x200;
                    break;
                case 2:
                    D_801392D8[car] &= ~0x400;
                    break;
                case 3:
                    D_801392D8[car] &= ~0x800;
                    break;
                }
                goto kill;
            }
            direction[0] = player->direction[0];
            direction[1] = player->direction[1];
            direction[2] = player->direction[2];
            func_8008E0B8(direction);
            vector_normalize_length(direction, matrix);
            math_utility(matrix, puff->matrix);
            if (aspeed < 90) {
                if (speed < 0) {
                    scale = aspeed / 70.0f * 0.75f + 0.25f;
                } else {
                    scale = aspeed / 90.0f * 0.85f + 0.15f;
                }
                for (i = 0; i < 3; i++) {
                    for (j = 0; j < 3; j++) {
                        puff->matrix[i][j] *= scale;
                    }
                }
            }
            puff->position[0] = player->exhaust[type][0];
            puff->position[1] = player->exhaust[type][1];
            puff->position[2] = player->exhaust[type][2];
            puff->position[0] = puff->velocity[0] + puff->position[0];
            puff->position[1] = puff->velocity[1] + puff->position[1];
            puff->position[2] = puff->velocity[2] + puff->position[2];
            puff->position[1] += 1.25f;
            puff->timer -= D_8002EB94;
            if (puff->timer <= 0.0f) {
                resource_set_frame(puff->id.part.index, D_80142948[1][puff->frame]);
                puff->frame++;
                if (puff->frame >= 8) {
                    puff->frame = 0;
                }
                puff->timer = 0.0333333f;
            }
        } else if (type == 5) {
            if (!D_8014A250[car].enabled) {
                goto kill;
            }
            if (puff->frame == -1) {
                if (D_8012E700[puff->id.handle].scale < puff->life) {
                    D_8012E700[puff->id.handle].scale += 0.04f;
                } else {
                    puff->frame = 0;
                }
            } else {
                if (puff->frame < 8) {
                    D_8012E700[puff->id.handle].scale -= 0.03f;
                } else {
                    D_8012E700[puff->id.handle].scale += 0.03f;
                }
                if (puff->frame >= 14) {
                    puff->frame = 0;
                } else {
                    puff->frame++;
                }
            }
            player = &D_80152818[car];
            position[0] = player->position[0];
            position[1] = player->position[1];
            position[2] = player->position[2];
            scale = D_8011744C[D_8014A250[car].model];
            position[0] += puff->velocity[0] * player->matrix[0][0];
            position[1] += puff->velocity[0] * player->matrix[0][1];
            position[2] += puff->velocity[0] * player->matrix[0][2];
            position[0] += puff->velocity[2] * player->matrix[2][0];
            position[1] += puff->velocity[2] * player->matrix[2][1];
            position[2] += puff->velocity[2] * player->matrix[2][2];
            position[0] += scale * player->matrix[1][0];
            position[1] += scale * player->matrix[1][1];
            position[2] += scale * player->matrix[1][2];
            puff->position[0] = position[0];
            puff->position[1] = position[1];
            puff->position[2] = position[2];
        } else {
            puff->life -= D_8002EB94;
            if (puff->life <= 0.0f) {
kill:
                entity_spawn_callback(puff->id.part.index, 0, 0);
                func_800AFA84(&D_8013F1E0, puff);
                continue;
            }
            D_8002EB94;
            if (puff->life < 1.0f) {
                if (D_8012E700[puff->id.handle].alpha >= 9) {
                    D_8012E700[puff->id.handle].alpha -= 9;
                }
            }
            if (type == 4) {
                puff->position[0] += rand() * 0.3f / 32768.0f - 0.15f;
                puff->position[1] += rand() * 0.25f / 32768.0f;
                puff->position[2] += rand() * 0.15f / 32768.0f - 0.075f;
                D_8012E700[puff->id.handle].scale += 0.05f;
            }
            puff->timer -= D_8002EB94;
            if (puff->timer <= 0.0f) {
                if (D_80156994 || D_8014978C >= 6) {
                    resource_set_frame(puff->id.part.index, D_80142948[0][puff->frame]);
                    if (puff->flags & 1) {
                        puff->frame++;
                        if (puff->frame >= 8) {
                            puff->frame = 0;
                        }
                    } else {
                        puff->frame--;
                        if (puff->frame < 0) {
                            puff->frame = 7;
                        }
                    }
                }
                puff->timer = 0.0333333f;
            }
            if (type < 6) {
                D_8012E700[puff->id.handle].scale += 0.03f;
            } else {
                D_8012E700[puff->id.handle].scale += 0.05f;
            }
            puff->position[0] += puff->velocity[0] * D_8002EB94;
            puff->position[1] += puff->velocity[1] * D_8002EB94;
            puff->position[2] += puff->velocity[2] * D_8002EB94;
        }
    }
}
