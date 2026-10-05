/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Direct ancestor: rushtherock/game/visuals.c:AnimateTire.
 * The N64 wheel geometry, bounce ownership, model selection, scale tables,
 * object flags and real helper interfaces follow the native body. */
#ifndef VISUAL_TIRE_TYPES_H
#define VISUAL_TIRE_TYPES_H
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef f32 Vec3[3];
typedef f32 Matrix3[3][3];
typedef struct Visual {
    u8 prefix[4];
    s16 tire,object,slot,reserved;
    f32 angle,time;
    void (*callback)(void);
} Visual;
typedef struct Tire92 {
    u8 prefix[64];
    f32 angular_velocity;
    u8 tail[24];
} Tire92;
typedef struct Model2056 {
    u8 prefix[8];
    u8 car_type;
    u8 to_body_type[6];
    s8 body_type;
    u8 to_steering[928];
    f32 steering;
    u8 to_tires[132];
    Tire92 tires[4];
    u8 to_suspension[36];
    f32 suspension[4];
    u8 to_visual_code[64];
    u16 visual_code[4];
    u8 to_player[418];
    s16 player;
    u8 to_mode[4];
    s8 mode;
    u8 tail[59];
} Model2056;
typedef struct Car952 {
    u8 prefix[232];
    u32 appearance;
    u8 to_mph[12];
    s16 mph;
    u8 to_controller[610];
    s8 controller,render_state,reserved,hidden;
    u8 tail[88];
} Car952;
typedef struct CarConfig {
    u8 prefix[96];
    f32 *radius[4];
    Vec3 tire_position[4];
} CarConfig;
typedef struct Control152 {
    u8 prefix[144];
    f32 spring_save;
    u8 tail[4];
} Control152;
typedef struct Object68 {
    u32 flags;
    u8 tail[64];
} Object68;
extern Model2056 D_8014A250[];
extern Car952 player_array[];
extern CarConfig *D_80110D08[];
extern Control152 D_80150B70[];
extern Object68 D_8012E700[];
extern s8 D_80140418,D_80156994,D_8015978C;
extern u8 D_80123264[];
extern s32 D_80143F68[];
extern f32 D_801112DC[][13],D_801113E0[][13];
extern f32 D_801543CC;
extern f32 D_8012393C,D_80123940,D_80123944,D_80123948;
extern s32 func_8008B2B4(void);
extern void model_data_load(s16,s32,s32);
extern void model_transform_setup(s16,s32,s32);
extern s32 model_bounds_calc(s32,Visual *);
extern s32 matrix_scale_apply(Visual *,s32,s32);
extern void func_8008D870(s16,s32,s32);
extern void euler_to_matrix(Matrix3,Vec3);
extern void func_8008B32C(Matrix3,Matrix3,f32);
extern void func_8008D6FC(s16,Vec3,Matrix3);
#endif

#define Random(range) ((f32)func_8008B2B4() * (range) / 32768.0f)
void anim_state_update(Visual *v,s16 op)
{
    f32 pos[3],mat[3][3],rpy[3],spring,spring2,t,vel,lastpos,scale;
    Car952 *car;
    Model2056 *m;
    CarConfig *config;
    s16 tire,slot,mph,row,index;
    s32 controller;
    s32 lucent,show,appearance;
    if(op==0) {
        model_data_load(v->object,1,15);
        v->callback=0;
        v->object=-1;
        return;
    }
    slot=v->slot;
    tire=v->tire;
    m=&D_8014A250[slot];
    spring=m->suspension[tire];
    lucent=D_80140418!=0 || (player_array[slot].appearance&8)!=0;
    car=&player_array[slot];
    mph=car->mph>>2;
    controller=car->controller;
    appearance=car->appearance;
    if(m->mode==2) row=m->player+1;
    else row=0;
    config=D_80110D08[m->car_type];
    show=(appearance&16)==0 && car->hidden==0;
    if(!model_bounds_calc(show,v)) return;
    matrix_scale_apply(v,car->render_state>=2,controller);
    if(D_8014A250[slot].visual_code[tire]==1) {
        t=(mph>>2)*.05f;
        if(t>.6f) t=.6f;
        spring+=Random(t);
    }
    if(spring<0) spring=0;
    else if(spring>0.5f) spring=0.5f;
    pos[0]=config->tire_position[tire][0];
    pos[1]=*config->radius[tire]+config->tire_position[tire][1]+spring;
    pos[2]=config->tire_position[tire][2];
    if(controller>=0 && car->render_state==1 && tire==0) {
        t=(mph>>2)*.002f;
        if(D_8014A250[slot].visual_code[tire]==1) spring2=(Random(2.0f)-1.0f)*t;
        else if(D_8014A250[slot].visual_code[tire]==8) spring2=0;
        else spring2=(Random(2.0f)-1.0f)*t*0.25f;
        D_80150B70[controller].spring_save=
            D_80150B70[controller].spring_save*0.75f+spring2;
    }
    vel=m->tires[tire].angular_velocity;
    lastpos=v->angle;
    if(vel>64.0f) lastpos+=3.7f;
    else lastpos+=vel*(D_801543CC-v->time);
    v->angle=lastpos;
    v->time=D_801543CC;
    rpy[0]=-lastpos;
    rpy[1]=(tire<2)?-m->steering:0;
    rpy[2]=0;
    if(tire==0) {
        index=m->body_type;
        if(vel>64.0f) {
            if(!D_80156994 && D_8015978C>=0 && D_8015978C<6) index=0;
            else index=D_80123264[index];
        } else {
            index+=3;
            if(!D_80156994 && D_8015978C>=0 && D_8015978C<6) index=16;
        }
        func_8008D870(v->object,D_80143F68[index],-1);
    }
    euler_to_matrix(mat,rpy);
    if(tire>=2) scale=D_801113E0[row][m->car_type];
    else scale=D_801112DC[row][m->car_type];
    if(scale!=1.0f) func_8008B32C(mat,mat,scale);
    func_8008D6FC(v->object,pos,mat);
    if(lucent) D_8012E700[v->object].flags^=0x80000000U;
    else D_8012E700[v->object].flags&=0x7FFFFFFFU;
}
