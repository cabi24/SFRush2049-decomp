/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef signed char s8;
typedef float f32;
typedef struct Object {
    u8 other0[10];
    s8 disabled;
    u8 other11[117];
    f32 left;
    u8 other132[8];
    f32 right;
    u8 other144[812];
    f32 output_left,output_right;
    u8 other964[76];
    f32 drive;
    u8 other1044[4];
    f32 auxiliary,average;
    u8 other1056[220];
    f32 auxiliary_left;
    u8 other1280[48];
    f32 wheel_left;
    u8 other1332[36];
    f32 auxiliary_right;
    u8 other1372[48];
    f32 wheel_right;
    u8 other1424[24];
    s8 coupled;
} Object;
extern void func_800E31D4(Object *),func_800E2F00(Object *),func_800E2C70(Object *),func_800E2AC4(Object *);
void func_800E32CC(Object *object)
{
    f32 left,right,sum,force;
    if(object->disabled!=0)func_800E31D4(object);
    func_800E2F00(object);
    func_800E2C70(object);
    object->average=(object->wheel_right+object->wheel_left)*0.5f;
    func_800E2AC4(object);
    right=object->right;
    left=object->left;
    sum=right+left;
    if(object->coupled==0 || sum<500.0f) {
        force=object->drive*0.5f;
        object->output_left+=force;
        object->output_right+=force;
    } else if(left<=0.0f) {
        object->output_left=0.0f;
        object->output_right+=object->drive;
    } else if(right<=0.0f) {
        object->output_right=0.0f;
        object->output_left+=object->drive;
    } else {
        force=object->drive;
        object->output_left+=(force*left)/sum;
        object->output_right+=(force*right)/sum;
    }
    force=object->auxiliary*2.0f;
    object->auxiliary_left=force;
    object->auxiliary_right=force;
}
