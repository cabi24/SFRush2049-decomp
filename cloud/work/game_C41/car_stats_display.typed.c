/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
#ifndef _STDARG_H
#define _STDARG_H

typedef char *va_list;
#define _FP 1
#define _INT 0
#define _STRUCT 2

#define _VA_FP_SAVE_AREA 0x10
#define _VA_ALIGN(p, a) (((unsigned int)(((char *)p) + ((a) > 4 ? (a) : 4) - 1)) & -((a) > 4 ? (a) : 4))
#define va_start(vp, parmN) (vp = ((va_list)&parmN + sizeof(parmN)))

#define __va_stack_arg(list, mode)                          \
    (                                                       \
        ((list) = (char *)_VA_ALIGN(list, __builtin_alignof(mode)) + \
                  _VA_ALIGN(sizeof(mode), 4)),              \
        (((char *)list) - (_VA_ALIGN(sizeof(mode), 4) - sizeof(mode))))

#define __va_double_arg(list, mode)                                                                    \
    (                                                                                                  \
        (((long)list & 0x1) /* 1 byte aligned? */                                                      \
             ? (list = (char *)((long)list + 7), (char *)((long)list - 6 - _VA_FP_SAVE_AREA))          \
             : (((long)list & 0x2) /* 2 byte aligned? */                                               \
                    ? (list = (char *)((long)list + 10), (char *)((long)list - 24 - _VA_FP_SAVE_AREA)) \
                    : __va_stack_arg(list, mode))))

#define va_arg(list, mode) ((mode *)(((__builtin_classof(mode) == _FP &&          \
                                       __builtin_alignof(mode) == sizeof(double)) \
                                          ? __va_double_arg(list, mode)           \
                                          : __va_stack_arg(list, mode))))[-1]
#define va_end(__list)

#endif /* STDARG_H */

typedef struct Object { u32 link0, link4, handle; u8 pad12, active, field14, pad15; f32 start, value; u8 field24, pad25[3]; u32 args[4]; u32 argument3, argument2; u8 kind, pad53[3]; s32 end; } Object;
extern u8 D_80142728[];
extern f32 D_80152748;
typedef struct List { u32 field0, field4, tail; } List;
extern List D_80149860;
extern s32 osRecvMesg(void *, void *, s32);
extern s32 osJamMesg(void *, void *, s32);
extern Object *func_800D18D8(void);
extern void func_80091FBC(void *, Object *, u32);
u32 car_stats_display(s32 kind, f32 value, u32 arg2, u32 arg3, s32 count, ...) {
    va_list args;
    Object *obj;
    s32 i;
    u32 result;
    osRecvMesg(D_80142728, 0, 1);
    obj = func_800D18D8();
    obj->kind = kind;
    obj->start = D_80152748;
    obj->value = value;
    obj->argument2 = arg2;
    obj->argument3 = arg3;
    obj->field14 = 0;
    obj->field24 = 0;
    va_start(args, count);
    for (i = 0; i < count; i++) obj->args[i] = va_arg(args, u32);
    for (; i < 4; i++) obj->args[i] = -1;
    va_end(args);
    obj->end = -1;
    func_80091FBC(&D_80149860, obj, D_80149860.tail);
    obj->active = 1;
    result = obj->handle;
    osJamMesg(D_80142728, 0, 0);
    return result;
}
