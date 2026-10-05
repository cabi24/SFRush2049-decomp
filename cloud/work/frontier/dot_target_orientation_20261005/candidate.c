/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research only: complete NONMATCH, six stack-frame/home words differ.
 * Arcade StartKnockdown/StartCone use a full MATRIX and a consumed car pointer.
 * The MATRIX layout comes from rushtherock LIB/fmath.h, not frame padding.
 * N64 record views and callback-visible reloads come from the native body.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef signed short s16;
typedef unsigned int u32;
typedef float f32;

typedef struct Basis { f32 m[9]; } Basis;
typedef struct Matrix3 { f32 uvs[3][3]; f32 pos[3]; } Matrix3;
typedef struct Matrix4 { f32 uvs[4][3]; } Matrix4;
typedef struct MatrixVectors { f32 xuv[3], yuv[3], zuv[3], pos[3]; } MatrixVectors;
typedef union Matrix {
    Matrix3 mat3;
    Matrix4 mat4;
    MatrixVectors matv;
    f32 uvs[4][3];
} Matrix;

typedef struct Actor96 {
    u8 before_flags[4];
    u8 flags;
    u8 before_index[11];
    s16 index;
    u8 before_basis[2];
    Basis basis;
    f32 position[3];
    u8 before_state[22];
    s16 state;
    s8 player;
    u8 tail[3];
} Actor96;

typedef struct Model952 {
    u8 before_direction[20];
    f32 direction[3];
    u8 tail[920];
} Model952;

typedef struct Node24 {
    struct Node24 *next;
    s16 state;
    u8 before_owner[6];
    Actor96 *owner;
    f32 time;
    u32 resource;
} Node24;

typedef struct Row48 {
    u32 resource;
    u8 before_event[12];
    u32 event;
    u8 tail[28];
} Row48;

extern Row48 D_8011753C[];
extern f32 D_801249D0;
extern Node24 *D_801391F0;
extern Model952 player_array[];
extern Node24 *func_80090284(void);
extern void vector_normalize_length(f32 *, f32 (*)[3]);
extern void math_utility(void *, void *);
extern int stat_lap_split(int, int, f32 *, u8);

void func_8010E72C(Actor96 *actor)
{
    Node24 *node;
    Matrix matrix;
    Model952 *car;

    node = func_80090284();
    if (node != 0) {
        node->state = 0;
        node->resource = D_8011753C[actor->index].resource;
        node->owner = actor;
        node->time = D_801249D0;
        actor->state = 4;
        actor->flags &= ~6;
        car = &player_array[actor->player];
        vector_normalize_length(car->direction, matrix.mat3.uvs);
        math_utility(matrix.mat3.uvs, &actor->basis);
        node->next = D_801391F0;
        D_801391F0 = node;
        stat_lap_split((int)D_8011753C[actor->index].event,
                       actor->player, actor->position, 2);
    }
}
