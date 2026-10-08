/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Controller {
    u8 pad0[5];
    s8 saved;
    u8 pad1[766];
} Controller;
extern Controller D_80144030[];
extern void *memset(void *, s32, u32);

void func_800A3724(u8 port) {
    Controller *controller = &D_80144030[port];
    s8 saved = controller->saved;
    memset(controller, 0, sizeof(Controller));
    controller->saved = saved;
}
