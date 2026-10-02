/* Native dotireforce; arcade ancestry reference/repos/rushtherock/game/tires.c. */
typedef signed char s8;typedef signed short s16;typedef signed int s32;typedef unsigned char u8;typedef float f32;
#define FIELD(e,t,o) (*(t *)((u8 *)(e)+(o)))
extern f32 D_80111130[],D_80110F80[];
extern f32 D_80123E10,D_80123E14,D_80123E18,D_80123E1C,D_80123E20,D_80123E24;
extern s8 D_80142DB0;
f32 fabsf(f32);
#pragma intrinsic(fabsf)
void camera_dolly(void *,f32 *,f32,f32,void *,f32 *,f32 *);
void func_8009E820(f32 *,f32 *,void *);
void camera_follow_target(void *model,f32 *tirev,void *tireuv,void *tire,f32 torque,f32 *forcevec,f32 suscomp,f32 otsuscomp,f32 springrate,f32 arspringrate,f32 cdamping,f32 rdamping,s32 poortract) {
    f32 arforce,damping,normal,sideforce,traction;
    f32 tireforcevec[3];
    f32 rate;
    if(suscomp>0.0f && otsuscomp>0.0f)arforce=(suscomp-otsuscomp)*arspringrate;
    else arforce=0.0f;
    if(tirev[1]<0.0f)damping=cdamping;else damping=rdamping;
    if(suscomp>10.0f){
        if(tirev[1]<1.0f)tireforcevec[1]=(1.0f-tirev[1])*FIELD(model,f32,1472)*-0.25f*FIELD(model,f32,1592);
        else tireforcevec[1]=arforce+suscomp*springrate-damping*tirev[1];
    }else if(suscomp>0.0f)tireforcevec[1]=arforce+suscomp*springrate-damping*tirev[1];
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
            if(D_80142DB0==2)sideforce*=D_80123E1C;
            else sideforce*=FIELD(model,f32,1460)*D_80123E20+D_80123E24;
        }
    }
    tireforcevec[2]=traction;tireforcevec[0]=sideforce;
    func_8009E820(tireforcevec,forcevec,tireuv);
    FIELD(tire,f32,80)=sideforce;FIELD(tire,f32,84)=traction;
}
