/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * cpak_read (0x800B0180, 212 words): per-car visual-effects update. N64 descendant of arcade
 * game/visuals.c UpdateVisuals(slot) (the name is a historical label):
 *   - gGoodFrameRate (D_80143F14) = frame time in microseconds below a budget: 36000 us when the video
 *     rate D_8002AFB4 is 50 Hz, else 45000 us with 2+ players (active_player_count), else 30000 us.
 *     The time is OS_CYCLES_TO_USEC of the CPU-count delta D_80124F70 (count * 64 / 3000, u64).
 *   - car->appearance & 0x60 (smoke) -> StartSmoke(slot, 4, 0, 0) (= entity_spawn_init).
 *   - per tire: model[slot].sviscode[t] == 2 (water) -> StartSmoke(slot, t, 1, 0); else the tire's
 *     appearance smoke bit (0x40000, 0x80000, 0x10000, 0x20000) -> StartSmoke(slot, t, 0, 0).
 *   - the 21 Visual controllers at car+272 (24-byte records, func at +20): (*v->func)(v, 1).
 *   - cpak_init(slot) = the DoSkid driver (locked group frontier_skid_marks, defined `void cpak_init(s32)`).
 * Unlike the arcade the N64 does not gate the smoke on gGoodFrameRate and has no frames % 4 phase.
 * Devices / notes (disclosed):
 *   - D_80124F70 is `volatile` (bss count delta, read once). Retail addresses it `lui/addiu/lw 0(t9)`,
 *     which IDO emits only for a volatile access; non-volatile gives `lw %lo(t9)` (89 words off).
 *   - cpak_init is declared with its real definition's parameter type s32 (the locked group defines
 *     `void cpak_init(s32 slot)`); retail passes s1 without re-narrowing.
 *   - `D_8002AFB4 == 50.0f` operand order puts 50.0 in $f4 as retail; `50.0f == D` swaps them.
 *   - SHAPING DEVICE: the u64 time is written as explicit calls of the repo's names for the compiler
 *     runtime helpers, __floatundisf(__udivdi3(__muldi3(D_80124F70, 64), 3000)), with prototypes that
 *     match their locked definitions in src/rom (lib_e4f0.c / lib_a5f0). Retail's source almost certainly
 *     wrote the arithmetic `(u64)D_80124F70 * 64 / 3000` (OS_CYCLES_TO_USEC); IDO lowers that to the same
 *     three calls but under ultralib's names (__ll_mul/__ull_div/__ull_to_f), which the game link script
 *     does not provide. Same bytes either way. The natural form is cloud/work/frontier/w22d/cpak_read/natural.c
 *     and becomes the deliverable once the link provides those aliases.
 * STATE: score.py fn MATCH (own .rodata verified at 0x80123C10..0x80123C1C); unit EQUAL, 0 locked differ.
 */
typedef signed char s8; typedef unsigned char u8;
typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
typedef unsigned long long u64;
typedef struct Visual { struct Visual *next; s32 index; s32 objnum; u32 slot; u32 data; void (*func)(struct Visual *v, s16 op); } Visual;
typedef struct Car { u8 pad0[232]; u32 appearance; u8 pad236[36]; Visual visuals[21]; u8 pad776[176]; } Car;
typedef struct Model { u8 pad0[1564]; u16 sviscode[4]; u8 pad1572[484]; } Model;
extern Car D_80152818[];
extern Model D_8014A250[];
extern volatile u32 D_80124F70;
extern f32 D_8002AFB4;
extern s16 active_player_count;
extern u8 D_80143F14;
void entity_spawn_init(s16 slot, u32 type, s32 fast, f32 *offset);
void cpak_init(s32 slot);
u64 __muldi3(u64 a, u64 b);
u64 __udivdi3(u64 a, u64 b);
f32 __floatundisf(u64 a);

void cpak_read(s16 slot)
{
    Car *car = &D_80152818[slot];
    Visual *v = &car->visuals[0];
    s16 i;
    f32 usec;

    usec = __floatundisf(__udivdi3(__muldi3(D_80124F70, 64), 3000));
    if (D_8002AFB4 == 50.0f) {
        D_80143F14 = usec < 36000.0f;
    } else if (active_player_count >= 2) {
        D_80143F14 = usec < 45000.0f;
    } else {
        D_80143F14 = usec < 30000.0f;
    }

    if (car->appearance & 0x60)
        entity_spawn_init(slot, 4, 0, 0);

    if (D_8014A250[slot].sviscode[0] == 2)
        entity_spawn_init(slot, 0, 1, 0);
    else if (car->appearance & 0x40000)
        entity_spawn_init(slot, 0, 0, 0);
    if (D_8014A250[slot].sviscode[1] == 2)
        entity_spawn_init(slot, 1, 1, 0);
    else if (car->appearance & 0x80000)
        entity_spawn_init(slot, 1, 0, 0);
    if (D_8014A250[slot].sviscode[2] == 2)
        entity_spawn_init(slot, 2, 1, 0);
    else if (car->appearance & 0x10000)
        entity_spawn_init(slot, 2, 0, 0);
    if (D_8014A250[slot].sviscode[3] == 2)
        entity_spawn_init(slot, 3, 1, 0);
    else if (car->appearance & 0x20000)
        entity_spawn_init(slot, 3, 0, 0);

    for (i = 0; i < 21; ++i, ++v)
        if (v->func)
            (*v->func)(v, 1);

    cpak_init(slot);
}
