/* Native-only C89 compile-time shape checks; not an accepted function body. */
#define offsetof(T, member) ((unsigned long)&((T *)0)->member)
#include "candidate.c"
#define CHECK(name, predicate) typedef char check_##name[(predicate) ? 1 : -1]
CHECK(pointer, sizeof(void *) == 4);
CHECK(short_width, sizeof(s16) == 2);
CHECK(int_width, sizeof(s32) == 4);
CHECK(effect_size, sizeof(HudEffect) == 72);
CHECK(effect_handle, offsetof(HudEffect, handle) == 2);
CHECK(effect_storage, offsetof(HudEffect, unknown04) == 4);
CHECK(group_size, sizeof(HudEffectGroup) == 720);
CHECK(status_size, sizeof(HudStatus) == 52);
CHECK(status_handle, offsetof(HudStatus, handle) == 2);
CHECK(status_storage, offsetof(HudStatus, unknown04) == 4);
CHECK(handle_size, sizeof(HudHandle) == 52);
CHECK(handle_storage, offsetof(HudHandle, unknown02) == 2);
