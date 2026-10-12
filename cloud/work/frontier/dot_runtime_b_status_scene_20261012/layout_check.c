/* Compile-only assertions under the actual native 32-bit IDO ABI. */
#include "group.c"
#define CHECK(name, condition) typedef char check_##name[(condition) ? 1 : -1]
#define OFFSET(type, field) ((u32)&((type *)0)->field)
CHECK(pointer, sizeof(void *) == 4);
CHECK(word, sizeof(s32) == 4 && sizeof(f32) == 4);
CHECK(player, sizeof(Player) == 952);
CHECK(flags, OFFSET(Player, flags) == 908);
CHECK(status, sizeof(Status) == 52);
CHECK(status_uv, OFFSET(Status, uv) == 4);
CHECK(status_position, OFFSET(Status, position) == 40);
CHECK(scene, sizeof(Scene) == 68);
CHECK(scene_scale, OFFSET(Scene, scale) == 16);
CHECK(camera, sizeof(Camera) == 152);
CHECK(point, sizeof(Point) == 4 && OFFSET(Point, y) == 2);
CHECK(effect, sizeof(Effect) == 72);
CHECK(score, sizeof(ScoreState) == 120);
