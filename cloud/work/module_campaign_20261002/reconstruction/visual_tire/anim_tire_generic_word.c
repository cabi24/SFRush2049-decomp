/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
/* Direct ancestor: rushtherock/game/visuals.c:AnimateTire.
 * The N64 wheel geometry, bounce ownership, model selection, scale tables,
 * object flags and real helper interfaces follow the native body.
 * The wheel angle reuses a generic integer word through the exact float
 * interpretation from the donor; sibling callbacks use this word as flags. */
#include "types_generic_word.h"
#define Random(range) ((f32)func_8008B2B4() * (range) / 32768.0f)
void anim_state_update(Visual *v,s16 op)
{
    f32 pos[3],mat[3][3],rpy[3],spring,spring2,t,vel,lastpos,scale;
    Car952 *car;
    Model2056 *m;
    CarConfig *config;
    s16 tire,slot,mph,row,index;
    s8 controller;
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
    car=&player_array[slot];
    spring=m->suspension[tire];
    lucent=D_80140418!=0;
    if(!lucent) lucent=(car->appearance&8)!=0;
    mph=car->mph>>2;
    controller=car->controller;
    appearance=car->appearance;
    if(m->mode==2) row=m->player+1;
    else row=0;
    config=D_80110D08[m->car_type];
    show=(appearance&16)==0;
    if(show) show=car->hidden==0;
    if(!model_bounds_calc(show,v)) return;
    matrix_scale_apply(v,car->render_state>=2,controller);
    if(m->visual_code[tire]==1) {
        t=(mph>>2)*D_80123940;
        if(t>D_8012393C) t=D_8012393C;
        spring+=Random(t);
    }
    if(spring<0) spring=0;
    else if(spring>0.5f) spring=0.5f;
    pos[0]=config->tire_position[tire][0];
    pos[1]=*config->radius[tire]+config->tire_position[tire][1]+spring;
    pos[2]=config->tire_position[tire][2];
    if(controller>=0 && car->render_state==1 && tire==0) {
        t=(mph>>2)*D_80123944;
        if(m->visual_code[tire]==1) spring2=(Random(2.0f)-1.0f)*t;
        else if(m->visual_code[tire]==8) spring2=0;
        else spring2=(Random(2.0f)-1.0f)*t*0.25f;
        D_80150B70[controller].spring_save=
            D_80150B70[controller].spring_save*0.75f+spring2;
    }
    vel=m->tires[tire].angular_velocity;
    lastpos=(*(f32 *)&v->index);
    if(vel>64.0f) lastpos+=D_80123948;
    else lastpos+=vel*(D_801543CC-v->time);
    (*(f32 *)&v->index)=lastpos;
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
