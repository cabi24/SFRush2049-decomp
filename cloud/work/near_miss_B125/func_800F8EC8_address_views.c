/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef int s32;
typedef float f32;
typedef struct { f32 x,y,z; } Vec3;
typedef struct {
    u8 prefix[4]; f32 time; Vec3 position;
    u8 to_blocked[238-20]; s8 blocked;
    u8 to_distance[264-239]; f32 distance;
    u8 to_crashed[856-268]; s8 crashed;
    u8 tail[952-857];
} Car952;
typedef struct {
    u8 prefix[1732]; s16 moving;
    u8 to_checkpoint[2020-1734]; s16 checkpoint;
    u8 pad[3]; s8 side; u8 tail[2056-2026];
} Vehicle2056;
typedef struct { Vec3 position, normal; s32 radius; u8 tail[52]; } Checkpoint80;
typedef struct { u8 prefix[7]; u8 type; } Player8;
#define D_80152818 ((Car952 *)0x80152818)
#define D_8014A250 ((Vehicle2056 *)0x8014A250)
#define D_80152218 ((Vec3 *)0x80152218)
#define D_80151CF4 ((Checkpoint80 *)0x80151CF4)
#define D_80153E88 ((Player8 *)0x80153E88)
#define D_801527E8 ((f32 *)0x801527E8)
#define D_801543CA (*(s16 *)0x801543CA)
#define D_8014A110 (*(s32 *)0x8014A110)
#define D_80152018 (*(f32 *)0x80152018)
#define D_80153FD2 (*(s16 *)0x80153FD2)
#define D_80152031 (*(s8 *)0x80152031)
#define D_8014A118 (*(u8 *)0x8014A118)
#define D_801174B4 (*(u32 *)0x801174B4)
#define D_8010FFC0 (*(s8 *)0x8010FFC0)
#define D_8002EB90 (*(f32 *)0x8002EB90)
#define D_8002EB94 (*(f32 *)0x8002EB94)
extern f32 func_8008B3C8(Vec3 *);
extern void race_countdown_display(Vehicle2056 *,f32);
extern void world_gravity_apply(void);
extern void func_800D1AB0(void);
extern void entity_flags_apply(s32,s32,s32,s32);
void func_800F8EC8(void)
{
    Vec3 movement,diff;
    s16 index;
    Car952 *car;
    Vec3 *previous;
    Vehicle2056 *vehicle;
    f32 plane;
    s32 side;
    for(index=0; index<D_801543CA; index++) {
        car=&D_80152818[index];
        previous=&D_80152218[index];
        movement.x=car->position.x-previous->x;
        movement.y=car->position.y-previous->y;
        movement.z=car->position.z-previous->z;
        car->distance+=func_8008B3C8(&movement);
        previous->x=car->position.x;
        previous->y=car->position.y;
        previous->z=car->position.z;
        if ((D_80153E88[index].type==0 || D_80153E88[index].type==6) && !car->crashed) {
            vehicle=&D_8014A250[index];
            if(vehicle->moving==-1) {
                diff.x=car->position.x-D_80151CF4[vehicle->checkpoint].position.x;
                diff.y=car->position.y-D_80151CF4[vehicle->checkpoint].position.y;
                diff.z=car->position.z-D_80151CF4[vehicle->checkpoint].position.z;
                side=1;
                plane=D_80151CF4[vehicle->checkpoint].normal.z*diff.z+
                    (diff.x*D_80151CF4[vehicle->checkpoint].normal.x+
                     diff.y*D_80151CF4[vehicle->checkpoint].normal.y);
                if(plane<0.0f) side=-1;
                if(side!=vehicle->side) {
                    vehicle->side=side;
                    if(diff.x*diff.x+diff.z*diff.z<(f32)D_80151CF4[vehicle->checkpoint].radius) {
                        race_countdown_display(vehicle,car->time-
                            D_8002EB94*(plane/(plane-D_801527E8[index])));
                    }
                }
                D_801527E8[index]=plane;
            }
        }
    }
    if(D_8014A110==1) return;
    world_gravity_apply();
    func_800D1AB0();
    if(D_80152018!=0.0f && D_8002EB90-D_80152018>3.0f) D_80152018=0.0f;
    if(D_80153FD2==1 && !D_80152818[D_8014A118].blocked && !(D_801174B4&8)) {
        if(!D_80152031 && D_80152018==0.0f) {
            D_80152018=D_8002EB90;
            if(D_8010FFC0) entity_flags_apply(16,D_8014A118,1,1);
        }
        D_80152031=1;
    } else D_80152031=0;
}
