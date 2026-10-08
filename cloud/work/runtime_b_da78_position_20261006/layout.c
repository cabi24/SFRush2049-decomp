#include "position.c"
#define OFF(t,m) ((u32)&(((t *)0)->m))
u32 layout_facts[] = {
    sizeof(BRecord), sizeof(BPlayer), sizeof(BVehicle),
    OFF(BRecord,owner), OFF(BRecord,kind), OFF(BRecord,flags),
    OFF(BRecord,radius), OFF(BRecord,position), OFF(BRecord,previous_position),
    OFF(BPlayer,position), OFF(BPlayer,uv), OFF(BPlayer,active),
    OFF(BPlayer,owner), OFF(BPlayer,kind),
    OFF(BVehicle,blocked), OFF(BVehicle,state)
};
