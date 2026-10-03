typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef int s32; typedef float f32;
typedef struct { u8 pad[72]; void *owner; } Player;
extern s16 D_8014A108;
extern Player D_8014A118[];

s32 func_80395E24(void *owner, s32 count, s32 skip)
{
    s8 i;
    s32 found = 0;

    count = D_8014A108;
    if (owner != 0) {
        for (i = 0; i < count; i++) {
            if (i != skip && owner == D_8014A118[i].owner) {
                found = 1;
            }
        }
    }
    return found;
}
