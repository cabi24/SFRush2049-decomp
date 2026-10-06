/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Runtime image B: allocate a four-position effect record. */
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Effect {
    struct Effect *next;
    u32 unknown04;
    u32 kind;
    f32 lifetime;
    f32 origin[3], previous[3], lower[3], upper[3];
} Effect;
typedef struct Pool Pool;
extern Pool D_80395EE8;
extern Effect *func_8008E3C0(Pool *);
Effect *func_8038D054(s32 mode, f32 *position, s32 unused)
{
    Effect *effect;
    f32 offset;
    effect = func_8008E3C0(&D_80395EE8);
    if (!effect) return 0;
    effect->lifetime = 0.1f;
    switch (mode) {
    case 0:
        offset = 0.75f;
        effect->kind = 1;
        break;
    case 1:
        offset = 0.5f;
        effect->kind = 2;
        break;
    case 2:
        offset = 0.35f;
        effect->kind = 4;
        break;
    case 3:
        effect->lifetime = 0.0f;
        offset = 0.025f;
        effect->kind = 8;
        break;
    }
    effect->origin[0] = position[0];
    effect->origin[1] = position[1];
    effect->origin[2] = position[2];
    effect->previous[0] = position[0];
    effect->previous[1] = position[1];
    effect->previous[2] = position[2];
    effect->lower[0] = position[0];
    effect->lower[1] = position[1];
    effect->lower[2] = position[2];
    effect->upper[0] = position[0];
    effect->upper[1] = position[1];
    effect->upper[2] = position[2];
    effect->lower[1] -= offset;
    effect->upper[1] += offset;
    return effect;
}
