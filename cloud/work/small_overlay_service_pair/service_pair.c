/* B6FEC4-only behavioral reconstruction; NONMATCH, no coverage claim.
 * Full local branches are represented; external helper bodies are not replaced.
 * Arithmetic assumes native 32-bit int, IEEE binary32 and in-range float->int.
 * Read README.md for residency, alias, return-value and transitive limits.
 */
#include "service_pair.h"

void small_8038D3A4(SmallPlayer *source, SmallPlayer *target, SmallS32 amount)
{
    SmallS32 target_index;
    SmallS32 source_index;
    SmallU32 remaining_bits;
    SmallS32 old_remaining;
    float scaled;

    target_index = target->car_index;
    if (small_models[target_index].excluded != 0) return;
    source_index = source->car_index;
    if (source_index == target_index) return;
    if (small_teams[target_index] == small_teams[source_index]) return;
    if (target->flags & 4) {
        scaled = (float)amount * 0.2f;
        amount = (SmallS32)scaled;
    }
    old_remaining = target->remaining;
    target->last_amount = (SmallS16)(SmallU16)amount;
    /* Native subu followed by sh/lh, NOT an unbounded saturating subtract. */
    remaining_bits = (SmallU32)old_remaining - (SmallU32)amount;
    target->remaining = (SmallS16)(SmallU16)remaining_bits;
    if (target->remaining <= 0) {
        target->remaining = 0;
        func_800C55E4(source->car_index, target->car_index, 1);
        /* Callback may alter target->car_index. Do not reuse target_index. */
        small_models[target->car_index].excluded = 1;
    }
}

void small_8038D798(float *origin, float *endpoint, SmallS32 source_index,
                    float radius_squared, SmallS32 magnitude)
{
    SmallPlayer *target;
    SmallPlayer *source;
    SmallS32 index;
    float dx, dy, dz;
    float xx, yy, zz, xy, distance_squared;
    float weight, weight_squared, amount_float;
    float magnitude_float;
    float direction[3];
    float transformed[3];
    float angle;

    index = 0;
    /* Signed count checked at entry and freshly after EVERY iteration. The
     * native unsigned-address comparison is equivalent on documented valid
     * player storage/counts, not arbitrary overflowing pointer arithmetic. */
    while (index < small_player_count) {
        target = &small_players[index];
        if (small_models[target->car_index].excluded == 0 &&
            target->enabled != 0 && target->excluded == 0) {
            dx = origin[0] - target->position[0];
            dy = origin[1] - target->position[1];
            dz = origin[2] - target->position[2];
            xx = dx * dx;
            yy = dy * dy;
            zz = dz * dz;
            xy = xx + yy;
            distance_squared = zz + xy;
            if (distance_squared < radius_squared) {
                weight = (radius_squared - distance_squared) / radius_squared;
                magnitude_float = (float)magnitude;
                source = &small_players[source_index];
                if (target->kind == 5) {
                    direction[0] = endpoint[0] - origin[0];
                    direction[1] = endpoint[1] - origin[1];
                    direction[2] = endpoint[2] - origin[2];
                    func_800A61B0(direction, transformed,
                        (void *)((unsigned char *)target + 0x2c));
                    angle = func_8008C768(transformed[0], transformed[2]);
                    /* Native abs.s, including clearing the sign of -0/NaN.
                     * Comparison result is identical for all IEEE operands. */
                    if (angle < 0.0f) angle = -angle;
                    weight_squared = weight * weight;
                    amount_float = weight_squared * magnitude_float;
                    if (angle < 1.35f) amount_float = amount_float * 0.35f;
                } else {
                    weight_squared = weight * weight;
                    amount_float = weight_squared * magnitude_float;
                }
                small_8038D3A4(source, target, (SmallS32)amount_float);
            }
        }
        ++index;
    }
}
