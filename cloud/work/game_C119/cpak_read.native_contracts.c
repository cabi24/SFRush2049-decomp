/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm */
#include "types.h"
typedef struct C119Callback C119Callback;
struct C119Callback {
    u8 native_prefix[20];
    void (*function)(C119Callback *, s32);
};
typedef struct {
    u8 native_prefix[0xe8];
    u32 flags;
    u8 before_callbacks[0x110 - 0xec];
    C119Callback callbacks[21];
    u8 native_tail[952 - 0x110 - 21 * 24];
} C119Car;
typedef struct {
    u8 native_prefix[1564];
    u16 effects[4];
    u8 native_tail[2056 - 1572];
} C119State;
extern C119Car player_array[];
extern C119State D_8014A250[];
extern u32 D_80124F70[];
extern f32 D_8002AFB4, D_80123C10, D_80123C14, D_80123C18;
extern s16 active_player_count;
extern s8 D_80143F14;
extern void entity_spawn_init(s16, s32, s32, s32);
extern void cpak_init(s32);
void cpak_read(s16 player)
{
    C119Car *car;
    C119Callback *callback;
    C119State *state;
    f32 elapsed;
    s16 i;

    car = &player_array[player];
    callback = car->callbacks;
    elapsed = (f32) (((u64) D_80124F70[0] * 64) / 3000);
    if (D_8002AFB4 == 50.0f) {
        D_80143F14 = elapsed < D_80123C10;
    } else if (active_player_count >= 2) {
        D_80143F14 = elapsed < D_80123C14;
    } else {
        D_80143F14 = elapsed < D_80123C18;
    }
    if (car->flags & 0x60) {
        entity_spawn_init(player, 4, 0, 0);
    }
    state = &D_8014A250[player];
    if (state->effects[0] == 2) {
        entity_spawn_init(player, 0, 1, 0);
    } else if (car->flags & 0x40000) {
        entity_spawn_init(player, 0, 0, 0);
    }
    if (state->effects[1] == 2) {
        entity_spawn_init(player, 1, 1, 0);
    } else if (car->flags & 0x80000) {
        entity_spawn_init(player, 1, 0, 0);
    }
    if (state->effects[2] == 2) {
        entity_spawn_init(player, 2, 1, 0);
    } else if (car->flags & 0x10000) {
        entity_spawn_init(player, 2, 0, 0);
    }
    if (state->effects[3] == 2) {
        entity_spawn_init(player, 3, 1, 0);
    } else if (car->flags & 0x20000) {
        entity_spawn_init(player, 3, 0, 0);
    }
    for (i = 0; i < 21; i++, callback++) {
        if (callback->function != 0) {
            callback->function(callback, 1);
        }
    }
    cpak_init(player);
}
