/* Unchanged-source host harness, with explicit external-call contracts. */
#include <assert.h>
#include <stddef.h>
#include <string.h>
#include <stdint.h>
#include "candidate.c"
struct OSThread { u8 opaque[16]; };
OSThread D_80034840;
ModelData D_8014A250[6];
CameraData D_80150B70[4];
Vec3 D_801526A8[4];
f32 D_80110688,D_8011068C,D_80110690,D_80110694,D_80110698,D_8011069C,D_801106A0;
static CarData car_storage;
static unsigned *events;
static unsigned event_count;
static int mutation;
static float threshold;
static const void *local_in,*local_out;
static unsigned bts(float x) {unsigned u;memcpy(&u,&x,4);return u;}
static unsigned pointer_id(const void *p) {
    uintptr_t x=(uintptr_t)p,b;
    if(!p)return 0;
    if(p==local_in)return 0x60004cU;
    if(p==local_out)return 0x600040U;
    b=(uintptr_t)&car_storage;
    if(x>=b && x<b+sizeof car_storage)return 0x100000U+(unsigned)(x-b);
    b=(uintptr_t)D_8014A250;if(x>=b && x<b+sizeof D_8014A250)return 0x200000U+(unsigned)(x-b);
    b=(uintptr_t)D_80150B70;if(x>=b && x<b+sizeof D_80150B70)return 0x300000U+(unsigned)(x-b);
    b=(uintptr_t)D_801526A8;if(x>=b && x<b+sizeof D_801526A8)return 0x400000U+(unsigned)(x-b);
    if(p==&D_80034840)return 0x500000U;
    assert(0 && "unrecognized host pointer"); return 0;
}
static void word(unsigned x){assert(event_count<1023);events[++event_count]=x;}
static void event(unsigned id,const void *a,const void *b,const void *c,unsigned d){word(id);word(pointer_id(a));word(pointer_id(b));word(pointer_id(c));word(d);}
static void vector(const float *p,int n){int i;for(i=0;i<n;i++)word(bts(p[i]));}
void osPfsChecker_full(OSThread *t){assert(t==&D_80034840);word(1);}
void osStartThread(OSThread *t){assert(t==&D_80034840);word(2);}
void math_utility(void *src,void *dst){int i;float *a=src,*b=dst;event(3,src,dst,0,0);vector(a,9);for(i=0;i<9;i++)b[i]=a[i];}
void func_8009E820(float *in,float *out,Mat3 *basis){int i;float *m=(float *)basis;local_in=in;local_out=out;event(4,in,out,basis,0);vector(in,3);vector(m,9);for(i=0;i<3;i++)out[i]=(in[0]*m[i]+in[1]*m[i+3])+in[2]*m[i+6];}
void func_800E8CB8(CarData *car,float *p,Mat3 *m){event(5,car,p,m,0);vector(p,3);vector((float *)m,9);}
float func_800EAFDC(ModelData *m){float a,b,t;event(6,m,0,0,0);memcpy(&a,(u8 *)m+1884,4);memcpy(&b,(u8 *)m+1888,4);t=(a+b)*0.5f;return threshold<t?t-threshold:0.0f;}
static void camera_hook(unsigned id,CarData *car,float *p,Mat3 *m,float *snapshot,unsigned mode){
    int i;CameraData *c=&D_80150B70[car->view_slot];float scale=(float)id*0.0625f;
    if(snapshot)local_in=snapshot;
    event(id,car,p,m,snapshot?pointer_id(snapshot):mode);vector(p,3);if(m)vector((float *)m,9);if(snapshot)vector(snapshot,3);
    for(i=0;i<3;i++)c->follow_position[i]=(p[i]+scale)+(float)i*0.5f;
    for(i=0;i<9;i++)((float *)c->follow_basis)[i]=((float *)car->object_basis)[i]*0.125f+scale;
    if(mutation&1)D_8014A250[car->model_index].collision_state=-D_8014A250[car->model_index].collision_state;
    if(mutation&2)for(i=0;i<3;i++)car->position[i]+=2.0f+(float)i;
}
void func_800EA3F4(CarData *c,float *p){camera_hook(7,c,p,0,0,0);}
void func_800EA2DC(CarData *c,float *p,Mat3 *m,float *q){camera_hook(8,c,p,m,q,0);}
void func_800EA108(CarData *c,float *p,Mat3 *m){camera_hook(9,c,p,m,0,0);}
void func_800E9E2C(CarData *c,float *p,Mat3 *m){camera_hook(10,c,p,m,0,0);}
void func_800E9C70(s16 mode,CarData *c,float *p,Mat3 *m){camera_hook(11,c,p,m,0,(unsigned)mode);}
void func_800E95DC(s16 mode,CarData *c,float *p,Mat3 *m){camera_hook(12,c,p,m,0,(unsigned)mode);}
void func_800E8F10(s16 mode,CarData *c,float *p,Mat3 *m){camera_hook(13,c,p,m,0,(unsigned)mode);}
s32 entity_iterate(float *a,float *b){int i;if(b!=(float *)car_storage.position)local_in=b;event(14,a,b,0,0);vector(a,3);vector(b,3);for(i=0;i<3;i++)a[i]+=b[i]*0.25f;return 0;}
void host_run(void *car,void *models,void *cameras,void *accel,float *globals,int mutate,unsigned *trace){
    assert(sizeof(CarData)==952 && sizeof(ModelData)==2056 && sizeof(CameraData)==152);
    assert(offsetof(CarData,model_index)==859 && offsetof(CarData,view_slot)==860 && offsetof(CarData,view)==861);
    assert(offsetof(CarData,world_basis)==44 && offsetof(CarData,object_basis)==80);
    assert(offsetof(ModelData,peak_body_force)==328 && offsetof(ModelData,peak_center_force)==352 && offsetof(ModelData,collision_state)==1732);
    assert(offsetof(CameraData,position)==36 && offsetof(CameraData,follow_basis)==96 && offsetof(CameraData,follow_position)==132 && offsetof(CameraData,spring)==144);
    memcpy(&car_storage,car,952);memcpy(D_8014A250,models,sizeof D_8014A250);memcpy(D_80150B70,cameras,sizeof D_80150B70);memcpy(D_801526A8,accel,sizeof D_801526A8);
    D_80110688=globals[0];D_8011068C=globals[1];D_80110690=globals[2];D_80110694=globals[3];D_80110698=globals[4];D_8011069C=globals[5];D_801106A0=globals[6];threshold=globals[7];
    events=trace;event_count=0;mutation=mutate;local_in=0;local_out=0;
    func_800EB028(&car_storage);
    trace[0]=event_count;
    memcpy(car,&car_storage,952);memcpy(models,D_8014A250,sizeof D_8014A250);memcpy(cameras,D_80150B70,sizeof D_80150B70);memcpy(accel,D_801526A8,sizeof D_801526A8);
}
