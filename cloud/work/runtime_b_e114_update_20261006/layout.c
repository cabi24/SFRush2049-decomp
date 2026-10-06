/* Independent native layout review; not a compilation context contribution. */
#include "update.c"
#define OFF(type,field) ((u32)&((type *)0)->field)
u32 layout_facts[] = { sizeof(BObject),sizeof(BEffect),sizeof(BRecord),sizeof(BPlayer),sizeof(BVehicle),sizeof(BScene),sizeof(BTexture),sizeof(BTextureBank),
OFF(BObject,scene_index),OFF(BObject,uv),OFF(BObject,position),
OFF(BRecord,owner),OFF(BRecord,kind),OFF(BRecord,flags),OFF(BRecord,lifetime),OFF(BRecord,timer),OFF(BRecord,collision_extent),OFF(BRecord,velocity),OFF(BRecord,position),OFF(BRecord,previous_position),OFF(BRecord,uv),OFF(BRecord,attached_effect),OFF(BRecord,primary),OFF(BRecord,secondary),
OFF(BPlayer,color),OFF(BPlayer,active),OFF(BPlayer,blocked),OFF(BPlayer,input),OFF(BPlayer,kind),OFF(BPlayer,ammo),OFF(BPlayer,status),OFF(BPlayer,action),OFF(BPlayer,alpha),OFF(BPlayer,latched),OFF(BPlayer,cooldown),
OFF(BVehicle,model),OFF(BVehicle,blocked),OFF(BScene,scale),OFF(BTextureBank,textures),OFF(BInput,pressed),OFF(BInput,fire_mask),OFF(BPool,head)};
