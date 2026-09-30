typedef unsigned char u8;
typedef signed char s8;
typedef int s32;
typedef struct { u8 pad[16]; u8 idx; } ObjInfo;
typedef struct { ObjInfo *info; } Obj;
typedef struct { void *pad[2]; Obj *obj; } Ctx;
typedef struct { Ctx *ctx; } Arg;
typedef struct { s8 flag; s8 pad[771]; } Slot;
extern Slot D_80144031[];
s32 func_800A1A60(void **arg0);
s32 func_800CC848(Arg *a0, s32 a1) {
    Obj *o = a0->ctx->obj;
    if (!o) return 1;
    if (!D_80144031[o->info->idx].flag) return 0;
    if (!a1) return 1;
    return func_800A1A60((void **)o);
}
