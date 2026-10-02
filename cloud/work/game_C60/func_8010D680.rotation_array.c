/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef signed char s8;
typedef short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef struct Vec3 {float x,y,z;} Vec3;
typedef struct Matrix {float row[3][3];} Matrix;
typedef struct Object {
    u8 opaque0[4],flags,opaque5[7];
    s32 model;
    u8 opaque16[4];
    Matrix orientation;
    Vec3 position;
    u8 opaque68[12];
    s16 kind;
    u8 opaque82[10];
    s8 player;
    u8 opaque93[15];
    Vec3 *velocity;
    float *delta;
} Object;
typedef struct Node {struct Node *next;u16 tag;s16 handle,index;u8 opaque10[2];Object *object;u8 opaque16[4];s32 state;} Node;
typedef struct Resource {u32 flags;u8 opaque4[64];} Resource;
typedef struct Player {u8 opaque0[900];s8 kind;u8 opaque901[51];} Player;
extern s32 D_801170FC;
extern s16 D_8014A108;
extern Resource D_8012E700[];
extern Player D_80152818[];
extern float D_8002EB94;
extern void entity_transform_apply(Node *,s32);
extern void sound_position_set(Vec3 *,Matrix *);
extern void model_transform_setup(s32,s32,u32);
extern void model_data_load(s32,s32,u32);
void func_8010D680(Node *node,s16 update) {
    Object *object;
    float rotation[3];
    s32 kind,allowed,i,model;
    Vec3 *velocity;
    float *delta;
    if(!update) {
        entity_transform_apply(node,1);
        return;
    }
    if(D_801170FC) return;
    object=node->object;
    if(object->flags&2) {
        velocity=object->velocity;
        delta=&D_8002EB94;
        rotation[0]=velocity->x*(*delta);
        rotation[1]=velocity->y*(*delta);
        rotation[2]=velocity->z*(*delta);
        sound_position_set((Vec3 *)rotation,&object->orientation);
        return;
    }
    model=object->model;
    if(D_8012E700[(s16)model].flags&0x80000000) {
        switch(object->kind) {
        case 350:kind=0;break;
        case 351:kind=1;break;
        case 352:kind=2;break;
        case 355:kind=3;break;
        case 356:kind=4;break;
        case 357:kind=5;break;
        case 358:kind=6;break;
        case 360:kind=7;break;
        default:kind=-1;break;
        }
        if(kind!=-1) {
            allowed=1;
            for(i=0;i<D_8014A108;i++) {
                if(D_80152818[i].kind==kind) allowed=0;
            }
            if(allowed) {
                model_transform_setup(model,0,15);
                object->flags|=2;
            }
        }
    } else model_data_load(model,0,15);
}
