typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;

typedef struct Object {
    char pad0[4];
    u8 flags;
    char pad5[11];
    s16 type;
    char pad12[50];
    f32 unk44[4];
    f32 unk54[2];
    s8 player;
} Object;

typedef s32 (*HitFunc)(void *, void *, void *, s32);

typedef struct ObjType {
    char pad0[8];
    void (*onHit)(Object *);
    char padC[4];
    s16 shape;
    char pad12[30];
} ObjType;

typedef struct Node {
    struct Node *next;
    Object *obj;
} Node;

typedef struct Pool {
    u8 doubly;
    char pad1[15];
    Node *active;
    Node *free;
} Pool;

typedef struct Car {
    char pad0[8];
    f32 pos[3];
    char pad14[0x3B8 - 0x14];
} Car;

typedef struct Tree {
    char pad0[3];
    u8 flags;
    f32 minx;
    f32 maxx;
    f32 minz;
    f32 maxz;
    u16 count;
    u16 first;
    u16 child2;
    u16 child3;
} Tree;

extern s32 D_801174B4;
extern s32 D_8014A110;
extern s8 D_80156994;
extern Car D_80152818[];
extern Pool D_80151AA8;
extern ObjType D_80117530[];
extern HitFunc D_80117518[];
extern Tree *D_80149770;
extern Object **D_801497C0;
extern void func_800AFA84(Pool *, Node *);

void race_setup_2(s16 player) {
    DECLS

    car = &D_80152818[player];
    if ((D_801174B4 & 8) && D_80156994 == 0) {
        return;
    }
    if (D_8014A110 == 6) {
        for (node = D_80151AA8.active; node != 0; node = next) {
            obj = node->obj;
            next = node->next;
            type = &D_80117530[obj->type];
            if (D_80117518[type->shape](&player, obj->unk44, obj->unk54, 0)) {
                obj->flags |= 4;
                obj->player = player;
                if (type->onHit != 0) {
                    type->onHit(obj);
                }
                func_800AFA84(&D_80151AA8, node);
            }
        }
    }
    pos[0] = car->pos[0];
    pos[1] = car->pos[1];
    pos[2] = car->pos[2];
    idx = 0;
    for (;;) {
        tree = &D_80149770[idx];
        if (!(tree->minx <= pos[0] && pos[0] < tree->maxx && tree->minz <= pos[2] && pos[2] < tree->maxz)) {
            return;
        }
        if (!(tree->flags & 1)) {
            break;
        }
        sub = &D_80149770[tree->child2];
        z = sub->maxz;
        x = sub->maxx;
        if (z <= pos[2]) {
            if (pos[0] < x) {
                idx = tree->count;
            } else {
                idx = tree->first;
            }
        } else {
            if (pos[0] < x) {
                idx = tree->child2;
            } else {
                idx = tree->child3;
            }
        }
    }
    for (i = 0; i < tree->count; i++) {
        obj = D_801497C0[tree->first + i];
        if (obj != 0 && (obj->flags & 2) && (obj->flags & 8)) {
            type = &D_80117530[obj->type];
            switch (type->shape) {
                case 0:
                case 1:
                case 2:
                    hit = D_80117518[type->shape](&player, obj->unk44, obj->unk54, 0);
                    break;
                case 3:
                case 4:
                case 5:
                    hit = D_80117518[type->shape](obj, &player, 0, 0);
                    break;
                default:
                    continue;
            }
            if (hit) {
                obj->flags |= 4;
                obj->player = player;
                if (type->onHit != 0) {
                    type->onHit(obj);
                }
            }
        }
    }
}
