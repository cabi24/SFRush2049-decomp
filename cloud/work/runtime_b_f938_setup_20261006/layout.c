/* Layout evidence only; this unit is not an optimizer context for setup.c. */
#include "setup.c"
#define OFFSET(type, member) ((u32) &(((type *) 0)->member))
u32 f938_layout[] = {
    sizeof(BStatus), sizeof(BPlayer), sizeof(BVehicle), sizeof(BEffect), sizeof(BColor),
    OFFSET(BPlayer, position), OFFSET(BPlayer, uv), OFFSET(BPlayer, status),
    OFFSET(BPlayer, alpha), OFFSET(BPlayer, transition), OFFSET(BPlayer, transition_time),
    OFFSET(BVehicle, model), OFFSET(BVehicle, blocked),
    OFFSET(BEffect, uv), OFFSET(BEffect, position)
};
