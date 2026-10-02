/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef signed int s32;
typedef float f32;
typedef struct Wheel92 {u8 other0[72];f32 velocity;u8 other76[16];} Wheel92;
typedef struct Physics {
    u8 other0[10];
    s8 disabled;
    u8 other11[185];
    f32 vertices[12];
    u8 other244[48];
    f32 current[3];
    f32 previous[3];
    f32 angular[3];
    f32 maximum[3];
    f32 minimum[3];
    f32 current_maximum[3];
    f32 current_minimum[3];
    u8 other376[568];
    f32 base_output;
    f32 wheel_output[4];
    f32 response;
    u8 other968[4];
    f32 control;
    f32 brake;
    f32 friction;
    u8 other984[4];
    f32 wheel_strength[4];
    u8 other1004[8];
    s16 gear_old;
    s16 gear_current;
    u8 other1016[16];
    f32 speed;
    u8 other1036[36];
    Wheel92 wheel[4];
    u8 other1440[148];
    f32 mass_cached,inverse_mass;
    u8 other1596[4];
    s8 frozen;
    u8 other1601[27];
    s16 state;
    u8 other1630[178];
    s32 control_flags;
    u8 other1812[4];
    f32 mass;
    u8 other1820[4];
    f32 gain;
    f32 control_input;
    f32 friction_input;
    f32 brake_input;
    s8 gear;
    u8 other1841[149];
    s16 model;
    u8 other1992[4];
    s8 mode;
    u8 other1997[3];
    s16 speed_integer;
    u8 other2002[54];
} Physics;
typedef struct Model952 {u8 other0[239];s8 controller;u8 other240[617];s8 state;u8 other858[94];} Model952;
extern Model952 player_array[];
extern s8 D_80152718,D_8013FECB;
extern f32 D_801243F0,D_801243F4,D_801243F8,D_801243FC,D_80124400,D_80124404;
extern void func_800E32CC(Physics *),object_update_full(Physics *),func_800E0B20(Physics *);
void func_800E3430(Physics *object)
{
    Model952 *model=&player_array[object->model];
    int i;
    f32 factor,value,mass=object->mass,inverse=1.0f/mass;
    object->base_output=object->response*object->gain;
    object->mass_cached=mass;
    object->inverse_mass=inverse;
    object->control=object->control_input;
    if(model->controller==1) object->brake=0.0f;
    else object->brake=object->brake_input;
    if(D_80152718!=0 || model->controller!=0) {
        object->brake=0.0f;object->control=1.0f;object->friction=D_801243F0;
    } else if(D_8013FECB!=0) {
        object->brake=0.0f;object->control=1.0f;object->friction=D_801243F4;
    } else object->friction=object->friction_input;
    if(object->friction>D_801243F8) object->friction=1.0f;
    if(object->disabled==0) {object->gear_old=object->gear;object->gear_current=object->gear;}
    else object->gear_current=object->gear;
    if(object->control_flags<0) {object->gear_current=0;object->gear_old=0;object->control=1.0f;object->friction=1.0f;}
    for(i=0;i<4;i++) {
        value=object->wheel[i].velocity;
        if(value>0.0f) {
            if(value<10.0f) {
                if(value<2.0f) factor=0.0f;
                else factor=value*D_801243FC;
            } else factor=1.0f;
        } else {
            if(value>-10.0f) {
                if(value>-2.0f) factor=0.0f;
                else factor=value*D_801243FC;
            } else factor=-1.0f;
        }
        object->wheel_output[i]=-object->wheel_strength[i]*object->friction*factor;
    }
}

void func_800E3724(Physics *object)
{
    int i,j,absolute;
    f32 x,y,z;
    func_800E3430(object);
    if(object->frozen!=0 || player_array[object->model].state!=0) {
        object->base_output=0.0f;object->control=0.0f;object->friction=0.0f;object->brake=0.0f;
    }
    func_800E32CC(object);
    object_update_full(object);
    func_800E0B20(object);
    object->speed_integer=(s32)(object->speed*D_80124400*D_80124404);
    absolute=object->speed_integer;
    if(absolute<0) absolute=-absolute;
    if(object->disabled==0 && object->brake>0.5f && absolute<600) {
        if(object->state==0) object->state=1;
    } else if(object->state!=0) object->state=3;
    if(object->mode==2) {
        for(i=0;i<3;i++) {
            if(object->current_maximum[i]<object->current[i])object->current_maximum[i]=object->current[i];
            if(object->current[i]<object->current_minimum[i])object->current_minimum[i]=object->current[i];
            for(j=0;j<4;j++) {
                if(object->maximum[i]<object->vertices[j*3+i])object->maximum[i]=object->vertices[j*3+i];
                if(object->vertices[j*3+i]<object->minimum[i])object->minimum[i]=object->vertices[j*3+i];
            }
        }
    }
    x=object->current[0];y=object->current[1];z=object->current[2];
    object->current[2]=0.0f;object->current[1]=0.0f;object->current[0]=0.0f;
    object->angular[0]=0.0f;object->angular[1]=0.0f;object->angular[2]=0.0f;
    object->previous[0]=x;object->previous[1]=y;object->previous[2]=z;
}
