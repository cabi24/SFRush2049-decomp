typedef signed char s8; typedef unsigned char u8; typedef signed short s16; typedef unsigned short u16;
typedef signed int s32; typedef unsigned int u32; typedef float f32;
extern void *D_801391F0;
void entity_transform_apply(void *, s32);
void func_800B0580(void);
void func_800B0618(void) {
    void *p;
    while ((p = D_801391F0) != 0) {
        entity_transform_apply(p, 1);
    }
    func_800B0580();
}
