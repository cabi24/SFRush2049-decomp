/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
extern u8 D_8014A250[];
extern u32 D_80120EBC[4],D_80120ECC[4],D_80120EDC[4];
typedef struct { s32 level[5]; f32 first,second; } Curve28;
extern Curve28 D_80120EEC[4];
extern s8 D_8013FECB;
extern s32 D_8014A110;
f32 fabsf(f32);
#pragma intrinsic(fabsf)
void func_800E7FA0(s16 index)
{
    u8 *car;
    s32 blocked, alternate, i, sample, segment;
    f32 a,b,magnitude,scale;
    Curve28 *curve;
    car=D_8014A250+index*2056;
    blocked=FIELD(car,u16,1566)==8 || FIELD(car,u16,1568)==8;
    alternate=FIELD(car,u16,1566)==2 || FIELD(car,u16,1568)==2;
    for(i=0;i<4;i++) {
        if(FIELD(car,u16,1564+i*2)==1) FIELD(car,u32,2004)|=D_80120ECC[i];
        else FIELD(car,u32,2004)&=~(D_80120EDC[i]|D_80120ECC[i]);
    }
    if(blocked || alternate || D_8013FECB!=0) {
        FIELD(car,f32,2040)=0.0f;
        FIELD(car,u32,2004)&=~(D_80120EDC[0]|D_80120EBC[0]);
        FIELD(car,f32,2044)=0.0f;
        FIELD(car,u32,2004)&=~(D_80120EDC[1]|D_80120EBC[1]);
        FIELD(car,f32,2048)=0.0f;
        FIELD(car,u32,2004)&=~(D_80120EDC[2]|D_80120EBC[2]);
        FIELD(car,f32,2052)=0.0f;
        FIELD(car,u32,2004)&=~(D_80120EDC[3]|D_80120EBC[3]);
    } else {
        curve=D_80120EEC;
        for(i=0;i<4;i++,curve++) {
            switch(i) {
            case 0:
                a=fabsf(FIELD(car,f32,1244));
                b=fabsf(FIELD(car,f32,1428));
                if(b<a) sample=(s32)a; else sample=(s32)b;
                break;
            case 1:
                a=fabsf(FIELD(car,f32,1152));
                b=fabsf(FIELD(car,f32,1336));
                if(b<a) sample=(s32)a; else sample=(s32)b;
                break;
            case 2:
                a=fabsf(FIELD(car,f32,1156));
                b=fabsf(FIELD(car,f32,1248));
                if(b<a) sample=(s32)a; else sample=(s32)b;
                break;
            case 3:
                sample=(s32)(fabsf(FIELD(car,f32,1432))+fabsf(FIELD(car,f32,1340)));
                break;
            }
            magnitude=(f32)sample;
            if(D_8014A110==6) scale=3.0f;
            else scale=FIELD(FIELD(car,u8 *,4),f32,32);
            if((f32)curve->level[0]*scale<=magnitude) {
                if(curve->level[4]<sample) sample=curve->level[4];
                segment=0;
                while(curve->level[segment+1]<sample) {
                    if(++segment==4) break;
                }
                FIELD(car,f32,2040+i*4)=((f32)(sample-curve->level[segment])/(f32)(curve->level[segment+1]-curve->level[segment])+(f32)segment)*0.25f;
                if(curve->first<FIELD(car,f32,2040+i*4)) FIELD(car,u32,2004)|=D_80120EDC[i];
                if(curve->second<FIELD(car,f32,2040+i*4)) FIELD(car,u32,2004)|=D_80120EBC[i];
            } else if(FIELD(car,f32,2040+i*4)!=0.0f) {
                if(curve->first<FIELD(car,f32,2040+i*4)) FIELD(car,u32,2004)|=D_80120EDC[i];
                if(curve->second<FIELD(car,f32,2040+i*4)) FIELD(car,u32,2004)|=D_80120EBC[i];
                FIELD(car,f32,2040+i*4)-=0.125f;
                if(FIELD(car,f32,2040+i*4)<=0.0f) {
                    FIELD(car,f32,2040+i*4)=0.0f;
                    FIELD(car,u32,2004)&=~(D_80120EBC[i]|D_80120EDC[i]);
                }
            }
        }
    }
}

void standin_caller(s32 n)
{
    s16 i;

    for (i = 0; i < n; i++) {
        if (D_8014A250[i * 2056] != 0) {
            func_800E7FA0(i);
        }
    }
    func_800E7FA0(0);
}
