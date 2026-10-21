/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Runtime image A: resource-handle cache lookup and load.
 * N64-specific adaptation; no whole-function arcade equivalent identified.
 * Complete extent [0x80390BC0,0x80390D2C), 364 bytes.
 * Record layout is independently witnessed by the accepted initializer
 * A:8039156C and release path A:80390D2C.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef struct ResourceCache {
    s32 handle;
    s8 loaded;
    s8 id;
    u8 reserved[6];
} ResourceCache;
extern ResourceCache D_803BA230[];
extern s16 D_80142B08[];
s32 audio_frame_sync(s32 kind, s32 skip, s32 async, s32 flag, void *buffer);
void func_800BB02C(s32 slot, s32 kind, void *data);

s32 func_80390BC0(s16 id, s16 kind, s32 *result)
{
    s32 i;
    s32 count = 16;
    ResourceCache *record;

    for (i = 0; i < count; i++) {
        record = &D_803BA230[i];
        if (record->id == id && record->loaded != 0) {
            *result = record->handle;
            return 1;
        }
    }
    for (i = 0; i != count; i++) {
        if (D_803BA230[i].id == id) {
            break;
        }
    }
    if (i == count) {
        for (i = 0; i != count; i++) {
            if (D_803BA230[i].id < 0) {
                break;
            }
        }
        if (i == count) {
            return 0;
        }
        D_803BA230[i].id = id;
    }
    record = &D_803BA230[i];
    record->handle = audio_frame_sync(kind + 88, 1, 1, 0, 0);
    *result = record->handle;
    record->loaded = 1;
    D_80142B08[id] = record->handle;
    func_800BB02C(id, kind, 0);
    return 1;
}
