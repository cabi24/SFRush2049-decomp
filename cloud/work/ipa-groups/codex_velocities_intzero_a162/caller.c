/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef unsigned char u8;typedef signed char s8;typedef short s16;typedef float f32;
typedef struct ModelView {
    u8 other0[40];f32 A[3],AA[3],V[3],W[3];
    u8 other88[676];f32 up;u8 other768[208];f32 brake,throttle;
    u8 other984[24];f32 magvel;u8 other1012[504];f32 contact[4];
    u8 other1532[56];f32 dt;u8 other1592[8];s8 sliding;
    u8 other1601[15];f32 damping;u8 other1620[370];s16 node;
} ModelView;
typedef struct Car952 {u8 other0[857];s8 mode;u8 other858[94];} Car952;
extern Car952 D_80152818[];
extern f32 D_801243B4,D_801243B8,D_801243BC,D_80114184,D_80114188;
extern f32 fabsf(f32),func_8008B3C8(f32 *);
#pragma intrinsic (fabsf)
void func_800E114C(ModelView *m)
{
    f32 temp[3],factor,speed;
    temp[0]=m->A[0]*m->dt;
    temp[1]=m->A[1]*m->dt;
    temp[2]=m->A[2]*m->dt;
    m->V[0]=temp[0]+m->V[0];
    m->V[1]=temp[1]+m->V[1];
    m->V[2]=temp[2]+m->V[2];
    if(m->brake<D_801243B4) {
        if(fabsf(m->V[0])<D_801243B8)m->V[0]=0;
        if(fabsf(m->V[2])<D_801243B8)m->V[2]=0;
    }
    if(D_80152818[m->node].mode!=2)m->magvel=func_8008B3C8(m->V);
    if(m->magvel>300) {
        factor=300/m->magvel;
        m->V[0]*=factor;
        m->V[1]*=factor;
        m->V[2]*=factor;
        return;
    }
    temp[0]=m->AA[0]*m->dt;
    temp[1]=m->AA[1]*m->dt;
    temp[2]=m->AA[2]*m->dt;
    if(m->damping!=0) {
        temp[0]-=m->W[0]*D_801243BC;
        temp[1]-=m->W[1]*D_801243BC;
        temp[2]-=m->W[2]*D_801243BC;
    }
    m->W[0]=temp[0]+m->W[0];
    m->W[1]=temp[1]+m->W[1];
    m->W[2]=temp[2]+m->W[2];
    if((m->contact[0]>0 && m->contact[1]>0 && m->contact[2]>0 && m->contact[3]>0) || m->sliding) {
        speed=func_8008B3C8(m->W);
        factor=speed*speed*(m->sliding?D_80114188:D_80114184);
        m->W[0]-=m->W[0]*factor;
        m->W[1]-=m->W[1]*factor;
        m->W[2]-=m->W[2]*factor;
    }
    if(m->magvel<3.0f && (m->throttle>0.5f || m->up<0)) {
        m->V[0]=0;
        m->V[2]=0;
    }
    if(m->magvel<3.0f && m->up< -0.75f) {
        if(func_8008B3C8(m->W)<0.5f) {
            m->W[0]=0;
            m->W[1]=0;
            m->W[2]=0;
        }
    }
}
