/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed int s32;
typedef float f32;
typedef f32 Vec3[3];
typedef f32 Mat3[3][3];
typedef struct Object {u8 other0[248];s16 speed;u8 other250[574];s16 path_index;u8 other826[2];s16 track;} Object;
typedef struct Point8 {s16 x,y,z;u16 other6;} Point8;
typedef struct Path8 {u16 count,other2;Point8 *points;} Path8;
typedef struct Position16 {s8 flag;u8 other1[3];Vec3 position;} Position16;
extern Path8 D_8012E5E8[];
extern Position16 *D_801108B0[];
extern s8 D_8014978C;
extern f32 D_80124558;
extern u32 D_8011735C;
extern s16 func_800B9338(s16,s16);
extern f32 sqrtf(f32);
extern int entity_iterate(f32 *,f32 *);
extern void func_800E9C70(s16,Object *,f32 *,Mat3 *);
void world_object_destroy(Object *object)
{
    s16 advances=(object->speed>>6)+4;
    s16 index=object->path_index;
    Position16 *entry;
    Mat3 matrix;
    Vec3 position,delta;
    while(advances>=0) {
        index=func_800B9338(index,0);
        advances--;
    }
    position[0]=D_8012E5E8[object->track].points[index].x;
    position[1]=D_8012E5E8[object->track].points[index].y;
    position[2]=D_8012E5E8[object->track].points[index].z;
    entry=D_801108B0[D_8014978C];
    if(entry!=0) {
        while(entry->position[1]!=D_80124558) {
            delta[0]=entry->position[0]-position[0];
            delta[1]=entry->position[1]-position[1];
            delta[2]=entry->position[2]-position[2];
            if(!(200.0f<sqrtf(delta[0]*delta[0]+delta[2]*delta[2]))) {
                if(entry->flag!=0)func_800E9C70(1,object,entry->position,&matrix);
                else func_800E9C70(0,object,entry->position,&matrix);
                return;
            }
            entry++;
        }
    }
    D_8011735C=D_8011735C*1103515245+12345;
    delta[0]=position[0];
    delta[1]=(((s32)D_8011735C>>16)&32767)*10.0f/32768.0f+(position[1]+10.0f);
    delta[2]=position[2];
    entity_iterate(delta,position);
    func_800E9C70(0,object,delta,&matrix);
}
