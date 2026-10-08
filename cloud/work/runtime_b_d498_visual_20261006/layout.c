#define func_8038D498 layout_excluded_body
#include "visual.c"
#define OFF(t,m) ((u32)&((t *)0)->m)
u32 layout_facts[] = {
    sizeof(BRecord), sizeof(BObject), sizeof(BPlayer), sizeof(BVehicle), sizeof(BScene),
    OFF(BRecord,owner), OFF(BRecord,hit_mask), OFF(BRecord,primary),
    OFF(BObject,scene_index), OFF(BObject,position),
    OFF(BPlayer,position), OFF(BPlayer,uv), OFF(BPlayer,active), OFF(BPlayer,owner),
    OFF(BVehicle,impulse), OFF(BVehicle,horizontal_impulse), OFF(BVehicle,state),
    OFF(BScene,scale)
};
