/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef short s16;
typedef unsigned int u32;
typedef int s32;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Object {u8 opaque0[4],flags,opaque5[11];s16 kind;u8 opaque18[50],field68[16],field84[8],player;} Object;
typedef int (*Callback)(void *,void *,void *,int);
typedef struct Format {u8 opaque0[8];void (*on_hit)(Object *);u8 opaque12[4];s16 kind;u8 opaque18[30];} Format;
typedef struct Node {struct Node *next;Object *object;} Node;
typedef struct Pool {u8 doubly,opaque1[15];Node *active,*free;} Pool;
typedef struct Player {u8 opaque0[8];float position[3];u8 opaque20[932];} Player;
typedef struct Tree {u8 opaque0[3],flags;float minx,maxx,minz,maxz;u16 count,offset,child2,child3;} Tree;
extern s32 state_word_a,D_8014A110;
extern s8 D_80156994;
extern Player D_80152818[];
extern Pool D_80151AA8;
extern Node *D_80151AB8;
extern Format D_80117530[];
extern Callback D_80117518[];
extern Tree *D_80149770;
extern Object **D_801497C0;
extern void func_800AFA84(Pool *,Node *);
void race_setup_2(s16 player) {
    Player *row;
    Node *node,*next;
    Object *object;
    Format *format;
    float position[3];
    Tree *tree,*split;
    s32 index;
    s32 i;
    s32 kind;
    void *arg0,*arg1,*arg2;
    row=&D_80152818[player];
    if((state_word_a&8)&&!D_80156994) return;
    if(D_8014A110==6) {
        node=D_80151AB8;
        while(node) {
            object=node->object;
            next=node->next;
            format=&D_80117530[object->kind];
            if(D_80117518[format->kind](&player,object->field68,object->field84,0)) {
                object->flags|=4;
                object->player=player;
                if(format->on_hit) format->on_hit(object);
                func_800AFA84(&D_80151AA8,node);
            }
            node=next;
        }
    }
    for(index=0;index<3;index++) position[index]=row->position[index];
    index=0;
    for(;;) {
        tree=&D_80149770[index];
        if(!(tree->minx<=position[0]&&position[0]<tree->maxx&&tree->minz<=position[2]&&position[2]<tree->maxz)) return;
        if(!(tree->flags&1)) break;
        split=&D_80149770[tree->child2];
        if(split->maxz<=position[2]) {
            if(position[0]<split->maxx) index=tree->count;
            else index=tree->offset;
        } else {
            if(position[0]<split->maxx) index=tree->child2;
            else index=tree->child3;
        }
    }
    for(i=0;i<tree->count;i++) {
        object=D_801497C0[tree->offset+i];
        if(object&&(object->flags&2)&&(object->flags&8)) {
            format=&D_80117530[object->kind];
            kind=format->kind;
            switch(kind) {
            case 0: case 1: case 2:
                arg0=&player;arg1=object->field68;arg2=object->field84;
                break;
            case 3: case 4: case 5:
                arg0=object;arg1=&player;arg2=0;
                break;
            default: continue;
            }
            if(D_80117518[kind](arg0,arg1,arg2,0)) {
                object->flags|=4;
                object->player=player;
                if(format->on_hit) format->on_hit(object);
            }
        }
    }
}
