/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef f32 Mat3[3][3];
typedef struct Coin {
    s32 model;          /* 0 */
    Mat3 matrix;        /* 4 */
    f32 position[3];    /* 40 */
    u8 pad52[8];
    f32 spin[3];        /* 60 */
    u16 missing;        /* 72 */
    s16 number;         /* 74 */
} Coin;
typedef union Id { s32 handle; struct { s16 high, index; } part; } Id;
typedef struct Entity {
    u8 pad0[4];
    u8 flags;           /* 4 */
    u8 pad5[7];
    Id id;              /* 12 */
    u8 pad16[40];
    f32 position[3];    /* 56 */
    u8 pad68[40];
    Coin *coin;         /* 108 */
} Entity;
typedef struct EntityHdr { u8 pad0[2]; s16 handle; } EntityHdr;
typedef struct Node { u8 pad0[12]; Entity *entity; } Node;
typedef struct Resource { u8 pad0[20]; s16 object; u8 pad22[38]; u32 color; u8 pad64[4]; } Resource;
typedef struct Player { u8 pad0[860]; s8 slot; u8 pad861[91]; } Player;
extern s32 D_801170FC;
extern u32 D_80118E14, D_80118E18;
extern s32 D_80151610, D_80151964;
extern u8 D_80140BDC;
extern s16 D_8014A108;
extern char D_80121DB0[], D_80121DBC[], D_80121DCC[];
extern Mat3 D_8011418C;
extern Resource D_8012E700[];
extern Player D_80152818[];
extern volatile f32 D_8002EB94;
extern s32 D_80143FC8;
extern void entity_transform_apply(Node *, s32);
extern void entity_spawn_callback(s16, s32, s32);
extern void func_800AFA84(void *, void *);
extern s32 string_copy_format(char *, s8, s8, s8);
extern s32 func_8008E26C(s32, void *, s16, s32);
extern void math_utility(void *, void *);
extern u32 func_800F7A98(s32, s32);
extern void model_data_load(s32, s32, u32);
extern void sound_position_set(void *, void *);
static void resource_set_object(s16 index, s16 object)
{
    D_8012E700[index].object = object;
}

static void resource_set_color(s16 index, u32 color)
{
    D_8012E700[index].color = color;
}

void func_8010E0FC(Node *node, s16 update)
{
    Entity *entity;
    Coin *coin;
    f32 spin[3];
    s32 number, i, alone;
    Player *player;
    u32 gold, silver;
    s32 pad[35];
    f32 *rate;
    if (!update) {
        entity_transform_apply(node, 1);
        return;
    }
    if (D_801170FC) return;
    entity = node->entity;
    coin = entity->coin;
    if (entity->flags & 0x80) {
        entity_spawn_callback(((EntityHdr *)coin)->handle, 0, 0);
        entity_spawn_callback(entity->id.part.index, 0, 0);
        func_800AFA84(&D_80143FC8, entity);
        entity_transform_apply(node, 1);
        return;
    }
    if (!(entity->flags & 1)) {
        gold = D_80118E14;
        silver = D_80118E18;
        if (entity->flags & 0x40) {
            number = D_80151610 + 8;
            coin->number = number;
            D_80151610++;
        } else {
            number = D_80151964;
            coin->number = number;
            D_80151964++;
        }
        resource_set_object(entity->id.part.index, string_copy_format(D_80121DB0, 0, D_80140BDC - 1, 1));
        coin->spin[0] = 0.0f;
        coin->spin[2] = 0.0f;
        coin->spin[1] = 3.0f;
        math_utility(D_8011418C, coin->matrix);
        coin->position[0] = entity->position[0];
        coin->position[1] = entity->position[1];
        coin->position[2] = entity->position[2];
        if (entity->flags & 0x40) {
            coin->model = func_8008E26C(string_copy_format(D_80121DBC, 0, D_80140BDC - 1, 1), coin->matrix, -1, 0x40000);
            resource_set_color(entity->id.part.index, gold);
        } else {
            coin->model = func_8008E26C(string_copy_format(D_80121DCC, 0, D_80140BDC - 1, 1), coin->matrix, -1, 0x40000);
            resource_set_color(entity->id.part.index, silver);
        }
        coin->missing = 0;
        alone = 1;
        for (i = 0; i < D_8014A108; i++) {
            if (!func_800F7A98(i, number)) {
                alone = 0;
                coin->missing |= 1 << i;
            } else {
                player = &D_80152818[i];
                model_data_load(entity->id.handle, 1, 1 << player->slot);
                model_data_load(coin->model, 1, 1 << player->slot);
            }
        }
        if (alone) {
            entity_spawn_callback(((EntityHdr *)coin)->handle, 0, 0);
            entity_spawn_callback(entity->id.part.index, 0, 0);
            func_800AFA84(&D_80143FC8, entity);
            entity_transform_apply(node, 1);
            return;
        }
        entity->flags |= 1;
    }
    rate = entity->coin->spin;
    spin[0] = rate[0] * D_8002EB94;
    spin[1] = rate[1] * D_8002EB94;
    spin[2] = rate[2] * D_8002EB94;
    sound_position_set(spin, coin->matrix);
}
