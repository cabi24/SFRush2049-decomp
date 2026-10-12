/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Image B callback initializer. Native-led research; source names provisional.
 * AF5E0 returns the scene handle left by its E26C call, although its accepted
 * source presently declares void. The external return contract is audited.
 */
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned long address_word;
typedef unsigned char u8;
typedef float f32;
typedef struct Callback {
    s32 unknown00;
    s16 state, handle, player;
    u8 unknown0a[6];
    f32 time;
    void (*callback)(void *);
} Callback;
typedef struct Player { u8 unknown000[752]; Callback callback; u8 unknown308[176]; } Player;
typedef struct Control { s32 parent; u8 unknown04[20];s32 handle;u8 unknown1c[36];} Control;
typedef struct Vehicle {u8 unknown00[8];u8 model;u8 unknown09[2047];} Vehicle;
typedef struct Vec {f32 x,y,z;} Vec;
extern Player D_80152818[];
extern Control D_80139320[];
extern Vehicle D_8014A250[];
extern Vec D_803943A4[];
extern f32 D_8002EB90[];
extern u16 D_80142972;
extern void func_8038AA8C(void *);
extern s32 func_800AF5E0(s32,s32,s32,s32,s32,s32,s32);
extern void func_8008D6FC(s16,void *,void *);
void func_8038C910(s16 player)
{
    Callback *callback;
    Control *control;
    if (D_80152818[player].callback.handle < 0) {
        callback = &D_80152818[player].callback;
        callback->state = 0;
        callback->callback = func_8038AA8C;
        callback->time = D_8002EB90[0];
        callback->player = player;
        control = &D_80139320[player];
        control->handle = func_800AF5E0(D_80142972,15,control->parent,0x2000,player,-1,1);
        callback->handle = control->handle;
        func_8008D6FC(callback->handle,&D_803943A4[D_8014A250[player].model] + 13,0);
    }
}
