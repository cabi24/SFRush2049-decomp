/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B [0x8038D798,0x8038DA78): apply a distance-weighted effect.
 * Native-led matching reconstruction using the CURRENT ACCEPTED shared-count
 * view: extern volatile s16 D_801543CA, present in EC914/108DA8/108F40/109A60/
 * world_physics_tick production sources. count_contract.py binds those current
 * sources and their image-gate records, game/B symbol identity, and EB90C's
 * independent call/store-free continuing-loop count read. This is reuse of an
 * accepted project contract, not a claim to recover original qualification or
 * prove an asynchronous writer. Original source spelling remains unknown.
 *
 * For each enabled, unblocked, non-disabled player within the strict squared
 * radius, apply trunc(((radius-distance)/radius)^2 * integer magnitude).
 * Kind 5 first transforms endpoint-origin into target-local space and applies
 * the native binary32 0.35 multiplier if the native angle magnitude is <1.35.
 * The three vectors are real three-float values. Moving the two existing
 * pointer declarations recovers direction/transformed/delta homes at
 * sp+152/+140/+124. Original declaration order and arcade ancestry are unknown.
 * No buffer change, fake formal, artificial caller, extra local or assembly.
 *
 * Ordinary O32: two vector pointers, signed source index, binary32 squared
 * radius in a3 bits, signed integer magnitude in the first stack-argument slot.
 * Valid backed player/model/vector storage and indices, finite in-range float
 * conversions, and genuine helper contracts are assumed. Origin/endpoint may
 * alias each other. Arbitrary partial overlaps, exceptional FCSR behavior,
 * helper implementation, concurrency and full gameplay remain unproved.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Player {
    u8 unknown000[8]; f32 position[3];
    u8 unknown014[756]; s8 active;
    u8 unknown309[80]; s8 blocked;
    u8 unknown35A; s8 index;
    u8 unknown35C[40]; s8 kind;
    u8 unknown385[51];
} Player;
typedef struct Vehicle { u8 unknown000[1600]; s8 disabled; u8 unknown641[455]; } Vehicle;
extern Player D_80152818[];
extern Vehicle D_8014A250[];
extern volatile s16 D_801543CA;
extern void func_800A61B0(f32 *,f32 *,f32 *);
extern f32 func_8008C768(f32,f32);
extern void func_8038D3A4(Player *,Player *,s32);
extern f32 fabsf(f32);
#pragma intrinsic(fabsf)

void func_8038D798(f32 *origin, f32 *endpoint, s32 source_index, f32 radius_squared, s32 magnitude)
{
    Player *target;
    f32 direction[3];
    f32 transformed[3];
    Player *source;
    f32 delta[3];
    f32 distance_squared;
    for (target = D_80152818; target < D_80152818 + D_801543CA; target++) {
        if (!D_8014A250[target->index].disabled && target->active && !target->blocked) {
            delta[0] = origin[0] - target->position[0];
            delta[1] = origin[1] - target->position[1];
            delta[2] = origin[2] - target->position[2];
            distance_squared = delta[0]*delta[0] + delta[1]*delta[1] + delta[2]*delta[2];
            if (distance_squared < radius_squared) {
                distance_squared = (radius_squared - distance_squared) / radius_squared;
                source = &D_80152818[source_index];
                if (target->kind == 5) {
                    direction[0] = endpoint[0] - origin[0];
                    direction[1] = endpoint[1] - origin[1];
                    direction[2] = endpoint[2] - origin[2];
                    func_800A61B0(direction, transformed, (f32 *)((u8 *)target + 0x2c));
                    if (fabsf(func_8008C768(transformed[0], transformed[2])) < 1.35f) {
                        func_8038D3A4(source, target, distance_squared*distance_squared*(f32)magnitude*0.35f);
                    } else {
                        func_8038D3A4(source, target, distance_squared*distance_squared*(f32)magnitude);
                    }
                } else {
                    func_8038D3A4(source, target, distance_squared*distance_squared*(f32)magnitude);
                }
            }
        }
    }
}
