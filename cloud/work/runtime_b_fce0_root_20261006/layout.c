/* Native data-layout evidence only. Does not add source context to root.c. */
#include "root.c"
#define OFFSET(type, member) ((u32) &(((type *) 0)->member))
u32 fce0_layout[] = {
    sizeof(BRecord), sizeof(BDebris), sizeof(BPlayer), sizeof(BVehicle),
    OFFSET(BRecord, attached_effect), OFFSET(BRecord, primary),
    OFFSET(BPlayer, active), OFFSET(BPlayer, blocked), OFFSET(BPlayer, owner),
    OFFSET(BPlayer, input), OFFSET(BPlayer, kind), OFFSET(BPlayer, ammo),
    OFFSET(BPlayer, status), OFFSET(BPlayer, action), OFFSET(BPlayer, latched),
    OFFSET(BPlayer, cooldown), OFFSET(BPlayer, pitch), OFFSET(BPlayer, yaw),
    OFFSET(BVehicle, extent_fc), OFFSET(BVehicle, extent_108),
    OFFSET(BVehicle, blocked), OFFSET(BVehicle, radius),
    OFFSET(BInput, reset_mask), OFFSET(BInput, fire_mask), OFFSET(BPool, head)
};
