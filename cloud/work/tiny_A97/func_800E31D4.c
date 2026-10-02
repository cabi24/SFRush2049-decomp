/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed short s16;
typedef float f32;
typedef struct Drive {u8 other0[172];f32 lower,upper;} Drive;
typedef struct Scale {u8 other0[20];f32 value;} Scale;
typedef struct Object {Drive *drive;Scale *scale;u8 other8[968];f32 brake;u8 other980[32];s16 gear_old,gear_current;u8 other1016[16];f32 speed;} Object;
extern void func_800E313C(Object *,f32,f32);
extern void func_800E30F4(Object *);
extern void func_800E30AC(Object *);
void func_800E31D4(Object *object)
{
    f32 lower,upper;
    s16 gear;
    {
        f32 factor=(object->brake+3.0f)*0.25f;
        lower=(object->scale->value*object->drive->lower)*factor;
        upper=(object->scale->value*object->drive->upper)*factor;
    }
    gear=object->gear_current;
    if(gear==0 || gear==-1)object->gear_old=gear;
    else {
        if(object->gear_old==0 || object->gear_old==-1)func_800E313C(object,lower,upper);
        if(lower<object->speed)func_800E30F4(object);
        if(object->speed<upper)func_800E30AC(object);
    }
}
