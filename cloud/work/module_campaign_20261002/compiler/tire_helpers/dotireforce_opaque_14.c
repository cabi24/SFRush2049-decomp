/* Complete ordinary fourteen-argument dotireforce ABI, arcade tires.c ancestor.
 * Opposite tire velocity was removed on N64; original airfact remains formal14.
 * This trial uses actual native record field offsets, no local padding. */
typedef float f32;typedef int s32;typedef unsigned char u8;typedef signed char s8;typedef short s16;
typedef struct Tire92 {
 f32 tradius,springK,rubdamp,unk12,Cfmax,invmi,unk24,Afmax,k1,k2,k3;
 u8 native44[24];f32 patchy,angvel,sliptorque,sideforce,traction;s8 slipflag;
} Tire92;
typedef struct Tuning {u8 native0[24];f32 traction_gain,lateral_gain;} Tuning;
typedef struct Model2056 {
 void *parameters;Tuning *tuning;u8 byte8;s8 kind;u8 byte10;s8 coupling,car_class;
 u8 to_difficulty[999];s16 difficulty;u8 to_tires[58];Tire92 tires[4];
 u8 to_lateral[16];f32 lateral_gain,slip;u8 native1464[4];f32 weight,mass;
 u8 to_idt[116];f32 idt;u8 to_steering[228];f32 steering;u8 tail[228];
} Model2056;
extern void camera_dolly(void *,f32 *,f32,f32,void *,f32 *,f32 *);
extern void func_8009E820(f32 *,f32 *,void *);
#define FIELD(e,t,o) (*(t *)((u8 *)(e)+(o)))
extern f32 D_80111130[],D_80110F80[];
extern f32 D_80123E10,D_80123E14,D_80123E18,D_80123E1C,D_80123E20,D_80123E24;
extern s8 D_80142DB0;
f32 fabsf(f32);
#pragma intrinsic(fabsf)

void func_8009E820(f32 *,f32 *,void *);
void camera_follow_target(void *model,f32 *tirev,void *tireuv,void *tire,f32 torque,f32 *forcevec,f32 suscomp,f32 otsuscomp,f32 springrate,f32 arspringrate,f32 cdamping,f32 rdamping,s32 poortract,f32 airfact) {
    f32 arforce,damping,normal,sideforce,traction,factor;
    f32 tireforcevec[3];
    f32 rate;
    if(suscomp>0.0f && otsuscomp>0.0f)arforce=(suscomp-otsuscomp)*arspringrate;
    else arforce=0.0f;
    if(tirev[1]<0.0f)damping=cdamping;else damping=rdamping;
    if(suscomp>10.0f){
        if(tirev[1]<1.0f)tireforcevec[1]=(1.0f-tirev[1])*FIELD(model,f32,1472)*-0.25f*FIELD(model,f32,1592);
        else tireforcevec[1]=arforce+suscomp*springrate-tirev[1]*damping;
    }else if(suscomp>0.0f)tireforcevec[1]=arforce+suscomp*springrate-tirev[1]*damping;
    else tireforcevec[1]=0.0f;
    rate=FIELD(model,f32,1592);
    normal=tireforcevec[1]*(rate*(rate*D_80123E10-D_80123E14));
    tireforcevec[1]=normal;
    if(normal<0.0f){normal=0.0f;tireforcevec[1]=0.0f;}
    else if(normal>FIELD(model,f32,1468))normal=FIELD(model,f32,1468);
    camera_dolly(model,tirev,normal,torque,tire,&sideforce,&traction);
    if(tire==(u8 *)model+1256 || tire==(u8 *)model+1348){
        traction*=D_80111130[FIELD(model,s8,12)*3+FIELD(model,s8,11)];
        traction*=1.0f+(D_80110F80[FIELD(model,s8,9)*6+FIELD(model,s16,1012)]-1.0f)*(1.0f-fabsf(FIELD(model,f32,1824)));
        traction*=FIELD(FIELD(model,void *,4),f32,24);
        sideforce*=FIELD(model,f32,1456)*(1.0f-FIELD(FIELD(model,void *,4),f32,28)*D_80123E18);
        if(poortract){
            if(D_80142DB0==2)factor=D_80123E1C;
            else factor=FIELD(model,f32,1460)*D_80123E20+D_80123E24;
            sideforce*=factor;
        }
    }
    tireforcevec[2]=traction;tireforcevec[0]=sideforce;
    func_8009E820(tireforcevec,forcevec,tireuv);
    FIELD(tire,f32,80)=sideforce;FIELD(tire,f32,84)=traction;
}
