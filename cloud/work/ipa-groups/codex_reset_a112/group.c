/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef short s16;
typedef float f32;
typedef struct Vec3 {f32 x,y,z;} Vec3;
typedef struct Sample12 {f32 speed;u8 opaque[8];} Sample12;
typedef struct Wheel92 {f32 inertia;u8 opaque[68];f32 ratio;u8 tail[16];} Wheel92;
typedef struct Object {
 u8 other0[72];f32 speed;u8 other76[80];Sample12 sample[4];
 u8 other204[808];s16 old_gear,new_gear;u8 other1016[56];Wheel92 wheel[4];
 u8 other1440[192];Vec3 position;u8 other1644[12];f32 basis[9];
 u8 other1692[32];int initial_speed;u8 other1728[4];s16 state;u8 other1734[14];
 Vec3 saved_position;f32 saved_basis[9];u8 other1796[24];s16 reset;u8 other1822[18];
 u8 gear;u8 other1841[149];s16 player;u8 tail[64];
} Object;
extern Object D_8014A250[];
extern void math_utility(f32 *,f32 *);
extern void func_800D11BC(Object *);
void func_800D1248(Object *input)
{
    Object *object=input;
    s16 i;
    if(object->state==-2) {
        object->reset=0;
        object->state=-1;
        object->saved_position.x=object->position.x;
        object->saved_position.y=object->position.y;
        object->saved_position.z=object->position.z;
        math_utility(object->basis,object->saved_basis);
        func_800D11BC(object);
        if((f32)object->initial_speed>=44.0f) {
            object->gear=2;
            object->new_gear=2;
            object->old_gear=2;
        } else {
            object->gear=1;
            object->new_gear=1;
            object->old_gear=1;
        }
        object->speed=(f32)object->initial_speed;
        for(i=0;i<4;i++) {
            object->sample[i].speed=(f32)object->initial_speed;
            object->wheel[i].ratio=object->sample[i].speed/object->wheel[i].inertia;
        }
        D_8014A250[object->player].reset=1;
    }
}

void math_utility(f32 *source,f32 *destination)
{
    destination[0]=source[0];
    destination[1]=source[1];
    destination[2]=source[2];
    destination[3]=source[3];
    destination[4]=source[4];
    destination[5]=source[5];
    destination[6]=source[6];
    destination[7]=source[7];
    destination[8]=source[8];
}
