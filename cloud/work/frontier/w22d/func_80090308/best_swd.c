/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed char s8;typedef short s16;typedef unsigned short u16;typedef unsigned int u32;typedef int s32;typedef float f32;
typedef struct Node {struct Node *next;s16 count,id,state;u8 other10[2];void *object;f32 value;void (*callback)(struct Node *,s16);} Node;
typedef union Color {u32 word;struct {u8 r,g,b,a;} c;} Color;
typedef struct Blast {s32 handle;f32 matrix[3][3];f32 pos[3];u8 alpha,flag,other54[2];f32 time;} Blast;
typedef struct Car952 {u8 other0[8];f32 pos[3];u8 other20[932];} Car952;
typedef struct Object68 {u8 other0[8];void *data;f32 scale,scale2;u16 texture;u8 other22[38];Color color;u8 other64[4];} Object68;
extern Node *D_801391F0;
extern Blast D_80154FD8[];
extern Car952 player_array[];
extern Object68 D_8012E700[];
extern f32 *D_8013C238[];
extern s16 D_8013C094;
extern u16 D_80142906,D_80142908;
extern f32 D_8011418C[3][3];
extern f32 D_801543CC;
extern Color D_8011B554;
extern s16 active_player_count;
extern s8 D_80156994,D_8014978C;
extern s8 D_8010FFC0;
extern u8 D_801141B0[];
Node *func_80090284(void);
void func_80090308(s16 car);
void entity_physics_update(Node *node,s16 update);
void func_80090B68(Node *,s16);
void entity_spawn_callback(s16 idx,s32 freeChildren,s32 freeSiblings);
void math_utility(void *,void *);
s32 func_8008E26C(s32,void *,s16,s32);
s32 camera_target_track(void *arg0, s32 arg1, f32 arg2, f32 arg3, f32 arg4, f32 arg5, s32 arg6, s32 arg7, s32 arg8, s32 arg9);

static s32 play_at(f32 *pos, f32 volume, s32 sound, s32 id)
{
    if (D_8010FFC0 == 0) {
        return -1;
    }
    if (sound == -1) {
        return -1;
    }
    return camera_target_track(pos, (s32)D_801141B0, volume, 0.0f, 1.0f, 0.0f, sound, id, 0, 0x80);
}

void save_write_data(void *where, s32 on_car, f32 size, s32 sound_on)
{
    Color color;
    s16 slot;
    Node *node;
    Blast *b;
    f32 *m;
    s32 sound;

    if (!D_80156994 && D_8014978C >= 0 && D_8014978C < 6) {
        func_80090308(*(s16 *)where);
        return;
    }
    if (on_car && active_player_count < 4) {
        color = D_8011B554;
        slot = *(s16 *)where;
        node = func_80090284();
        if (node) {
            node->callback = func_80090B68;
            node->value = D_801543CC;
            node->state = slot;
            b = &D_80154FD8[slot];
            math_utility(D_8011418C, b->matrix);
            b->pos[0] = player_array[slot].pos[0];
            b->pos[1] = player_array[slot].pos[1];
            b->pos[2] = player_array[slot].pos[2];
            if (b->handle != -1) entity_spawn_callback((s16)b->handle, 0, 0);
            b->handle = func_8008E26C(D_80142906, b->matrix, -1, 0x42000);
            b->alpha = 240;
            b->flag = 0;
            D_8012E700[b->handle].color = color;
            b->time = 0.0333333015f;
            node->next = D_801391F0;
            D_801391F0 = node;
        }
    }
    node = func_80090284();
    if (node) {
        m = D_8013C238[D_8013C094];
        math_utility(D_8011418C, m);
        node->callback = entity_physics_update;
        node->value = 0.0333333015f;
        if (on_car) {
            node->state = *(s16 *)where;
            m[9] = player_array[node->state].pos[0];
            m[10] = player_array[node->state].pos[1];
            m[11] = player_array[node->state].pos[2];
        } else {
            node->state = -1;
            m[9] = ((f32 *)where)[0];
            m[10] = ((f32 *)where)[1];
            m[11] = ((f32 *)where)[2];
        }
        node->count = 0;
        node->id = func_8008E26C(D_80142908, m, -1, 0x52000);
        node->next = D_801391F0;
        D_801391F0 = node;
        D_8012E700[node->id].scale = size;
        D_8013C094++;
        if (D_8013C094 >= 50) D_8013C094 = 0;
        if (sound_on) {
            if (size >= 1.0f) sound = 45;
            else if (size >= 0.5f) sound = 69;
            else sound = 47;
            play_at(&m[9], 400.0f, sound, node->id);
        }
    }
}
