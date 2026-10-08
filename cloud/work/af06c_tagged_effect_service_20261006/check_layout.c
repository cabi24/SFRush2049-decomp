/* Compiler-only ILP32 layout test; no linked code and no target tuning. */
#include "candidate.c"
#define OFF(t,m) __builtin_offsetof(t,m)
#define CHECK(n,e) typedef char layout_##n[(e) ? 1 : -1]
CHECK(pointer,sizeof(void *) == 4);
CHECK(node,sizeof(EffectNode) == 24);
CHECK(node_scene,OFF(EffectNode,scene) == 6);
CHECK(node_index,OFF(EffectNode,index) == 8);
CHECK(node_timer,OFF(EffectNode,timer) == 16);
CHECK(node_callback,OFF(EffectNode,callback) == 20);
CHECK(transform,sizeof(Transform) == 48);
CHECK(position,OFF(Transform,position) == 36);
CHECK(player,sizeof(Player) == 952);
CHECK(player_position,OFF(Player,position) == 8);
CHECK(extra,sizeof(ExtraEffect) == 60);
CHECK(extra_alpha,OFF(ExtraEffect,alpha) == 52);
CHECK(extra_timer,OFF(ExtraEffect,timer) == 56);
CHECK(debris,sizeof(Debris) == 84);
CHECK(debris_scale,OFF(Debris,scale) == 52);
CHECK(debris_speed,OFF(Debris,speed) == 56);
CHECK(debris_velocity,OFF(Debris,velocity) == 60);
CHECK(debris_angle,OFF(Debris,angle) == 72);
CHECK(debris_alpha,OFF(Debris,alpha) == 76);
CHECK(debris_lifetime,OFF(Debris,lifetime) == 77);
CHECK(debris_timer,OFF(Debris,timer) == 80);
CHECK(group,sizeof(EffectGroup) == 400);
CHECK(group_extra,OFF(EffectGroup,extra) == 336);
CHECK(group_timer,OFF(EffectGroup,timer) == 396);
CHECK(scene,sizeof(Scene) == 68);
CHECK(scene_scale,OFF(Scene,scale) == 12);
CHECK(scene_rate,OFF(Scene,rate) == 16);
CHECK(scene_color,OFF(Scene,color) == 60);
