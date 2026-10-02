/* Research-only NONMATCH. N64-specific pooled-object construction.
 * No direct arcade equivalent established. Field names are offset hypotheses.
 * flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed int s32;
typedef unsigned int u32;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned char u8;
typedef struct PoolObject {
    u32 owner;
    u8 unknown04[8];
    u16 resource;
    s16 x;
    s16 y;
    u8 unknown12[7];
    u8 field19;
    u8 field1a;
    u8 active;
    u8 unknown1c[12];
    s32 field28;
    s32 field2c;
    s32 field30;
    s16 handle;
    u8 unknown36[2];
    s32 field38;
    s32 field3c;
} PoolObject;
extern s32 D_80149788;
extern PoolObject *D_80149450[];
extern void func_800B362C(PoolObject *);
extern s32 func_800A79F4(u16, s32, s32, s32, s32, s32, s32);
extern void func_80094EC8(PoolObject *);
PoolObject *func_800B3704(u32 owner, s32 x, s32 y)
{
    PoolObject *object;
    if (D_80149788 >= 200) return (PoolObject *)0;
    object = D_80149450[D_80149788++];
    object->owner = owner;
    object->x = x;
    object->y = y;
    object->field1a = 0;
    object->field19 = 0;
    object->active = 1;
    object->field3c = 0;
    object->field28 = 0;
    object->field2c = -1;
    object->field30 = -1;
    object->field38 = 0;
    func_800B362C(object);
    object->handle = func_800A79F4(object->resource, 0, 0, x, y, -1, -1);
    func_80094EC8(object);
    return object;
}
