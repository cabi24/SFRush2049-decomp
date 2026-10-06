/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { f32 x, y, z; } Vec3;

typedef struct Ent {
    s32 flags;
    s32 w4;
    f32 *owner;
    f32 scaleA;
    f32 scaleB;
    u16 id;
    s16 link[3];
    s32 w28[9];
    s32 w64;
} Ent;

typedef struct Resource68 { u8 prefix[64]; u32 flags; } Resource68;

typedef struct Scene36 {
    char name[16];
    u32 flags;
    s16 count;
    s16 action;
    u32 field24;
    Resource68 *resources;
    s32 key;
} Scene36;

typedef struct Node {
    struct Node *next;
    u8 flags;
    s32 key;
    s32 handle;
    s16 metadata;
    f32 matrix[9];
    f32 pos[3];
    u8 xform[12];
    s16 f80;
    f32 f84;
    s16 texture;
    s16 f90;
    s8 f92;
    s32 f96;
    s8 f100;
    s8 index;
    f32 f104;
    void *state;
} Node;

typedef struct Metadata48 {
    char *name;
    char *file;
    void (*callback)(Node *);
    void *animation;
    s16 kind;
    u16 flags;
    s16 f20;
    s8 category;
    s8 element;
    f32 value;
    u8 tail[20];
} Metadata48;

typedef struct Place { char name[16]; f32 matrix[9]; f32 pos[3]; u32 flags; s32 w68; s32 w72; s32 key; } Place;
typedef struct Slot136 { s32 object[3]; u8 gap12[122]; s16 active; } Slot136;
typedef struct Matrix64 { u8 *record; f32 matrix[9]; f32 position[3]; f32 velocity[3]; } Matrix64;
typedef struct Animation24 {
    struct Animation24 *next;
    s16 state;
    u16 field6;
    u32 field8;
    Node *node;
    f32 time;
    void *data;
} Animation24;
typedef struct Pool { u8 b0; s32 count; s32 size; u8 *mem; Node *head; u8 *free; } Pool;

extern u32 D_801174B4;
extern s8 D_80156994;
extern Scene36 *D_801392D0;
extern s32 D_801392D4;
extern s32 D_8014A110;
extern s16 D_8014A108;
extern s8 D_80152570;
extern s8 D_80150DD0;
extern s8 D_80150E28[8];
extern u8 D_80117510[];
extern u8 *D_80150E68[8];
extern u8 *D_80150E98[8];
extern u8 *D_80118DDC[];
extern s32 D_80150F78;
extern s32 D_80150F80;
extern Matrix64 *D_80150F38;
extern s32 D_801497EC;
extern void **D_801497C0;
extern Pool D_80143FC8;
extern Pool D_80151AA8;
extern Metadata48 D_80117530[];
extern u16 D_801427C0[];
extern Ent D_8012E700[];
extern Animation24 *D_801391F0;
extern f32 D_8011418C[];
extern u8 D_80118C10[];
extern Vec3 D_80118DFC;
extern volatile u8 D_80140BDC;
extern s32 D_801515F0;
extern s32 D_80151688;

extern void *audio_dma_sync(s32, s32);
extern void func_800B2CB4(u32);
extern void listener_position_set(s32);
extern void math_utility(void *, void *);
extern Animation24 *func_80090284(void);
extern void func_800AB750(s32, f32 *, f32 *, void *);
extern void func_8039133C(s32);
extern int strlen(const char *);
extern int func_800950AC(const char *, const char *, unsigned int);
extern char *func_800A464C(char *, char *);
extern void *func_8008E3C0(Pool *);
extern s32 string_copy_format(char *, s8, s8, s8);
extern s32 func_8008E26C(s32, void *, s16, s32);
extern s32 func_800AB53C(f32 *);
extern f32 func_800ABB58(s32);
extern void transmission_shift(void *, s32);

void func_800AB7D0(Node *node,Metadata48 *metadata,s32 category);
void engine_torque_calc(Node *node)
{
    Metadata48 *metadata;
    s32 category;
    s32 resource;
    u16 value;
    Animation24 *animation;
    Vec3 transform;
    metadata=&D_80117530[node->metadata];
    category=metadata->category;
    func_800AB7D0(node,metadata,category);
    if(node->texture!=-1) {
        resource=(s16)node->handle;
        value=D_801427C0[node->texture];
        D_8012E700[resource].id=value;
    }
    if(metadata->flags&2) {
        animation=func_80090284();
        if(animation) {
            animation->state=0;
            animation->data=metadata->animation;
            animation->node=node;
            animation->time=0.0f;
            animation->next=D_801391F0;
            D_801391F0=animation;
            if((metadata->flags&4) && metadata->callback) metadata->callback(node);
        }
    }
    if(metadata->flags&0x4000) {
        if(!(node->flags&16)!=0) metadata->callback(node);
        else {
            transform=D_80118DFC;
            func_800AB750(node->index,(f32 *)&transform,(f32 *)node->xform,node->matrix);
        }
    }
}
void func_800AB7D0(Node *node,Metadata48 *metadata,s32 category)
{
    s32 index,j;
    Matrix64 *matrix;
    Scene36 *scene;
    Slot136 *slot;
    if(!(D_801174B4&0x400000)) {
        if(metadata->flags&8) {
            node->state=D_80150E98[category];
            if(!(node->flags&16) && category==4) {
                j=node->index;
                *(Scene36 **)node->state=&D_801392D0[j];
                if(D_801392D0) {}
                if((*(Scene36 **)node->state)->flags&16) node->flags&=~8;
            }
            D_80150E98[category]=D_80150E98[category]+D_80117510[category];
        } else if(category!=7) {
            node->state=D_80118DDC[category]+D_80117510[category]*metadata->element;
        }
        if(metadata->kind==4) {
            node->index=D_80150F80++;
            matrix=&D_80150F38[node->index];
            math_utility(D_8011418C,matrix->matrix);
            for(index=0;index<3;index++) {
                matrix->velocity[index]=0.0f;
                matrix->position[index]=0.0f;
            }
            index=(s32)D_80117530[node->metadata].value;
            matrix->record=D_80118C10+index*32;
        }
    } else if((metadata->flags&8) && metadata->category==5) {
        slot=(Slot136 *)D_80150E68[5];
        for(index=0;index<16;index++,slot++) {
            if(!slot->active && slot->object[0]==-1 && slot->object[1]==-1 && slot->object[2]==-1) break;
        }
        slot->active=1;
        node->state=D_80150E98[category]+D_80117510[category]*index;
    }
}
static u32 ent_flags(s16 id) {
    return D_8012E700[id].flags;
}

s32 transmission_ratio_get(Place *rec, s16 parent, f32 *pos, void *src, s8 index, s32 place, s32 other)
{
    s32 i;
    s32 pad152;
    s32 pad148;
    s32 res;
    f32 *owner;
    Node *node;
    u32 flags;
    s32 pad128;
    s32 pad124;
    s8 team;
    Metadata48 *md;
    u16 mflags;
    s16 shape;
    s32 tail[8];

    if (src != 0) {
        flags = rec->flags;
    } else {
        flags = 0;
    }
    for (i = 0; i < 122; i++) {
        md = &D_80117530[i];
        if (func_800950AC(rec->name, md->name, strlen(md->name)) != 0) {
            continue;
        }
        if ((D_801174B4 & 8) && !D_80156994) {
            return 1;
        }
        if (D_8014A110 == 2 && md->kind == 4 && other) {
            return 1;
        }
        if (!D_80156994 && !(md->flags & 0x20)) {
            return 1;
        }
        if (!(md->flags & ~4)) {
            return 1;
        }
        team = -1;
        if (func_800A464C(rec->name, "_BW") != 0) {
            team = 1;
        }
        if (func_800A464C(rec->name, "_FW") != 0) {
            team = 0;
        }
        if ((D_80152570 && team == 0) || (!D_80152570 && team == 1)) {
            return 1;
        }
        node = func_8008E3C0(&D_80143FC8);
        if (node == 0) {
            return 1;
        }
        mflags = md->flags;
        if (mflags & 8) {
            D_80150E28[md->category]++;
        }
        if (mflags & 0x8000) {
            D_80150F78++;
        }
        node->key = string_copy_format(md->file, 0, (s8)(D_80140BDC - 1), 1);
        node->metadata = i;
        shape = md->f20;
        node->f92 = -1;
        node->f80 = shape;
        node->texture = shape;
        node->index = index;
        node->f90 = 0;
        node->f96 = -1;
        node->f104 = 0.0f;
        if (mflags & 0x400) {
            node->state = rec;
        }
        if (mflags & 0x80) {
            flags |= 0x8000;
        }
        if (md->flags & 0x40) {
            flags |= 0x400000;
        }
        if (!(mflags & 4)) {
            if (md->category == 5) {
                node->flags = 0;
            } else {
                node->flags = 2;
            }
        } else {
            node->flags = 0;
        }
        if (md->category == 4 && src != 0) {
            node->flags |= 0x10;
        }
        if (md->category == 6) {
            if (func_800A464C(rec->name, "GOLD") != 0) {
                node->flags |= 0x40;
                D_801515F0++;
            } else {
                D_80151688++;
            }
        }
        if (place) {
            if (src != 0) {
                node->pos[0] = rec->pos[0];
                node->pos[1] = rec->pos[1];
                node->pos[2] = rec->pos[2];
                math_utility(rec->matrix, node->matrix);
            } else {
                node->pos[0] = pos[0];
                node->pos[1] = pos[1];
                node->pos[2] = pos[2];
                math_utility(D_8011418C, node->matrix);
            }
            if (mflags & 0x800) {
                flags |= 0x40000;
                res = -1;
            } else if (parent == -2) {
                res = func_800AB53C(pos);
            } else {
                res = parent;
            }
            if (md->category == 6) {
                res = -1;
                math_utility(D_8011418C, node->matrix);
                flags |= 0x52000;
            }
            if (D_8014A108 < 2 || !(mflags & 0x100)) {
                node->handle = func_8008E26C(node->key, node->matrix, res, flags);
            }
            if ((mflags & 0x300) && res >= 0) {
                owner = D_8012E700[res].owner;
                node->pos[0] -= owner[9];
                node->pos[1] -= owner[10];
                node->pos[2] -= owner[11];
            }
            if (md->category == 4 && func_800ABB58(node->key) < 50.0f) {
                D_8012E700[node->handle].flags = ent_flags(node->handle) | 0x20;
            }
            transmission_shift(node->xform, node->handle);
        }
        if (md->value == -1.0f) {
            node->f84 = func_800ABB58(node->key);
        } else {
            node->f84 = md->value;
        }
        if (md->kind != 6) {
            node->flags |= 8;
        }
        if (src != 0) {
            node->key = rec->key;
        } else if (md->category == 4) {
            node->key = D_801392D0[index].key;
        }
        if (D_801174B4 & 0x400000) {
            if (D_8014A110 == 6) {
                ((Node **)func_8008E3C0(&D_80151AA8))[1] = node;
            }
            engine_torque_calc(node);
        }
        return 1;
    }
    return 0;
}

void engine_sound_update(void)
{
    s32 i, j, offset, enabled, other;
    Node *node;
    Vec3 zero;
    void *key;
    Slot136 *slot;

    if ((D_801174B4 & 8) && !D_80156994) {
        return;
    }
    if (D_801392D4) {
        zero.z = zero.x = zero.y = 0.0f;
        for (i = 0, offset = 0; i < D_801392D4; i++, offset += 36) {
            if (D_8014A110 == 2 && ((Scene36 *)((u8 *)D_801392D0 + offset))->action > 0) {
                for (j = 0; j < ((Scene36 *)((u8 *)D_801392D0 + offset))->count; j++) {
                    if (((Scene36 *)((u8 *)D_801392D0 + offset))->resources[j].flags & 0x01000000) {
                        goto process;
                    }
                }
                if (((Scene36 *)((u8 *)D_801392D0 + offset))->action > 0) {
                    func_800B2CB4(((Scene36 *)((u8 *)D_801392D0 + offset))->action);
                }
                continue;
            }
        process:
            if (((((Scene36 *)((u8 *)D_801392D0 + offset))->flags & 4) && (((Scene36 *)((u8 *)D_801392D0 + offset))->flags & 8)) || (D_80152570 && (((Scene36 *)((u8 *)D_801392D0 + offset))->flags & 8)) ||
                (!D_80152570 && (((Scene36 *)((u8 *)D_801392D0 + offset))->flags & 4))) {
                enabled = (((Scene36 *)((u8 *)D_801392D0 + offset))->flags & 0x8000) ? 0 : 1;
                other = (((Scene36 *)((u8 *)D_801392D0 + offset))->flags & 0x10) ? 0 : 1;
                for (j = 0; j < ((Scene36 *)((u8 *)D_801392D0 + offset))->count; j++) {
                    if (D_80150DD0) {
                        ((Scene36 *)((u8 *)D_801392D0 + offset))->resources[j].flags &= ~0x1000;
                                }
                    if (((Scene36 *)((u8 *)D_801392D0 + offset))->resources[j].flags & 1) {
                        transmission_ratio_get((Place *)((Scene36 *)((u8 *)D_801392D0 + offset)), -1, &zero.x, 0, i, enabled, other);
                                }
                }
                if (D_80150DD0 && ((Scene36 *)((u8 *)D_801392D0 + offset))->action > 0) {
                    listener_position_set(((Scene36 *)((u8 *)D_801392D0 + offset))->action);
                }
            } else if (((Scene36 *)((u8 *)D_801392D0 + offset))->flags & 0x800) {
                func_800B2CB4(((Scene36 *)((u8 *)D_801392D0 + offset))->action);
            }
        }
    }
    if (!D_80150DD0) {
        for (i = 0; i < 8; i++) {
            if (D_80150E28[i]) {
                D_80150E68[i] = audio_dma_sync(0, D_80117510[i] * D_80150E28[i]);
            }
        }
        if (D_80150F78) {
            D_80150F38 = audio_dma_sync(0, D_80150F78 * 64);
        }
        for (i = 0; i < D_801497EC; i++) {
            key = D_801497C0[i];
            node = D_80143FC8.head;
            while (node != 0) {
                if ((node->flags & 8) && (void *)node->key == key) {
                    D_801497C0[i] = node;
                    break;
                }
                node = node->next;
            }
            if (key == D_801497C0[i]) {
                D_801497C0[i] = 0;
            }
        }
    }
    if (D_8014A110 == 6) {
        for (i = 0; i < 16; i++) {
            ((Slot136 *)D_80150E68[5])[i].active = 0;
            ((Slot136 *)D_80150E68[5])[i].object[0] = -1;
            ((Slot136 *)D_80150E68[5])[i].object[1] = -1;
            ((Slot136 *)D_80150E68[5])[i].object[2] = -1;
        }
    }
    D_80150F80 = 0;
    for (i = 0; i < 8; i++) {
        if (D_80150E28[i]) {
            D_80150E98[i] = D_80150E68[i];
        }
    }
    node = D_80143FC8.head;
    while (node != 0) {
        engine_torque_calc(node);
        node = node->next;
    }
    if (D_8014A110 == 6) {
        func_8039133C(D_80150DD0);
    }
}
