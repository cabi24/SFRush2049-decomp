/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Object effect activation with conditional half-turn and positional sound.
 * Full N64 body: 0x8010DBB8..0x8010DCFC (324 bytes).
 * The substantive dotprod body is from pinned arcade game/vecmath.c,
 * 845329d7b36f5a384c5625ed9a0aef584ab46139. Its natural O3 inlining
 * explains the native FP expression. A direct whole-function donor is not known.
 * Both allocator-result and working-node carriers occur in the archived B10
 * reconstruction and are consumed below; merging them shrinks the native frame.
 * Opaque record spans describe observed N64 layout, not stack padding.
 * Source/byte evidence only; image and ROM gates remain independent.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Actor {u8 opaque0[4];u8 flags;u8 opaque5[11];s16 index;u8 opaque18[2];f32 matrix[3][3];f32 position[3];u8 opaque68[22];s16 state;s8 player;} Actor;
typedef struct Player {u8 opaque0[20];f32 velocity[3];u8 opaque32[920];} Player;
typedef struct Node {struct Node *next;s16 field4; s16 field6; s16 field8; u8 padA[2];void *fieldC;f32 field10;u32 field14;} Node;
typedef struct Definition {u8 opaque0[12];s32 stat;u8 opaque16[12];s32 sound;u8 opaque32[16];} Definition;
extern Player player_array[];
extern Definition D_80117530[];
extern Node *D_801391F0;
extern Node *func_80090284(void);
extern void func_80090E9C(f32,f32 [][3]);
extern s32 stat_lap_split(s32,s32,f32 *,u8);
f32 dotprod(f32 a[3], f32 b[3])
{
    return(a[0]*b[0] + a[1]*b[1] + a[2]*b[2]);
}
void func_8010DBB8(Actor *actor)
{
    Node *node;
    Node *allocated;
    Definition *def;
    Player *car;
    def=&D_80117530[actor->index];
    actor->state=7;
    allocated=func_80090284();
    node=allocated;
    if (allocated!=0) {
        node->field4=0;
        node->field14=def->stat;
        node->fieldC=actor;
        node->field10=0.0333333f;
        car=&player_array[actor->player];
        if (dotprod(car->velocity,actor->matrix[2]) > 0.0f) {
            func_80090E9C(3.1415927f,actor->matrix);
        }
        actor->flags &= ~6;
        node->next=D_801391F0;
        D_801391F0=node;
        stat_lap_split(D_80117530[actor->index].sound,actor->player,actor->position,2);
    }
}
