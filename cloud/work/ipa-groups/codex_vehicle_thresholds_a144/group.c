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

extern u8 D_8014A250[],D_80152818[],D_80034840[],D_80153E88[];
extern s8 D_8011156C[];
extern f32 *D_80111560[];
extern f32 D_801543CC,D_801244B0,D_801244B4,D_801244B8,D_801244BC;
extern void osPfsChecker_full(void *);
extern void osStartThread(void *);
extern void battle_mode_setup(f32);
extern void math_utility(f32 *,f32 *);
extern void func_80090F44(f32,f32 *);
extern void func_8009EA68(f32,f32 *);
extern void func_8009E820(f32 *,f32 *,f32 *);
extern void func_800E7FA0(s16);
void func_800E847C(void)
{
    s16 car_index,tire,j,best;
    s16 counts[4];
    u16 event;
    u8 *car,*object,*tire_dst;
    f32 sum[3];
    f32 *curve;
    f32 value,bound;
    osPfsChecker_full(D_80034840);
    battle_mode_setup(D_801543CC);
    for(car_index=0;car_index<6;car_index++) {
        car=D_8014A250+car_index*2056;
        if(!FIELD(car,s16,1992)) continue;
        object=D_80152818+car_index*952;
        if(FIELD(object,s8,857)>=2) continue;
        FIELD(object,u32,0)=FIELD(car,u32,1808);
        FIELD(object,f32,4)=FIELD(car,f32,1812);
        FIELD(object,f32,8)=FIELD(car,f32,1940);
        FIELD(object,f32,12)=FIELD(car,f32,1944);
        FIELD(object,f32,16)=FIELD(car,f32,1948);
        FIELD(object,f32,872)=FIELD(car,f32,736);
        FIELD(object,f32,876)=FIELD(car,f32,740);
        FIELD(object,f32,880)=FIELD(car,f32,744);
        FIELD(car,f32,736)=0.0f;
        FIELD(car,f32,740)=0.0f;
        FIELD(car,f32,744)=0.0f;
        math_utility((f32 *)(car+1952),(f32 *)(object+44));
        math_utility((f32 *)(car+1952),(f32 *)(object+80));
        sum[0]=FIELD(car,f32,112)+FIELD(car,f32,100);
        sum[1]=FIELD(car,f32,116)+FIELD(car,f32,104);
        sum[2]=FIELD(car,f32,120)+FIELD(car,f32,108);
        sum[0]+=FIELD(car,f32,124);
        sum[1]+=FIELD(car,f32,128);
        sum[2]+=FIELD(car,f32,132);
        sum[0]+=FIELD(car,f32,136);
        sum[1]+=FIELD(car,f32,140);
        sum[2]+=FIELD(car,f32,144);
        curve=D_80111560[D_8011156C[FIELD(car,u8,8)]];
        value=sum[2]-curve[0];
        if(value>0.0f) {
            bound=curve[2];
            value*=bound/curve[1];
            if(bound<value) value=bound;
        } else {
            value=sum[2]-curve[3];
            if(value<0.0f) {
                bound=curve[5];
                value*=(-bound)/curve[4];
                if(value<bound) value=bound;
            } else value=0.0f;
        }
        FIELD(object,f32,224)=FIELD(object,f32,224)*D_801244B0+D_801244B4*value;
        func_80090F44(FIELD(object,f32,224),(f32 *)(object+80));
        bound=curve[6];
        value=sum[0]-bound;
        if(value>0.0f) {
            bound=curve[8];
            value*=(-bound)/curve[7];
            if(bound<value) value=bound;
        } else {
            value=sum[0]+bound;
            if(value<0.0f) {
                bound=-curve[8];
                value*=bound/curve[7];
                if(value<bound) value=bound;
            } else value=0.0f;
        }
        FIELD(object,f32,228)=FIELD(object,f32,228)*D_801244B8+D_801244BC*value;
        func_8009EA68(FIELD(object,f32,228),(f32 *)(object+80));
        for(tire=0;tire<4;tire++) {
            tire_dst=object+tire*12;
            FIELD(tire_dst,f32,176)=FIELD(FIELD(car,u8 *,0),f32,112+tire*12);
            FIELD(tire_dst,f32,180)=FIELD(FIELD(car,u8 *,0),f32,116+tire*12);
            FIELD(tire_dst,f32,184)=FIELD(FIELD(car,u8 *,0),f32,120+tire*12);
            func_8009E820((f32 *)(FIELD(car,u8 *,0)+112+tire*12),(f32 *)(tire_dst+116),(f32 *)(object+44));
            FIELD(tire_dst,f32,116)=FIELD(object,f32,8)+FIELD(tire_dst,f32,116);
            FIELD(tire_dst,f32,120)=FIELD(object,f32,12)+FIELD(tire_dst,f32,120);
            FIELD(tire_dst,f32,124)=FIELD(object,f32,16)+FIELD(tire_dst,f32,124);
        }
        FIELD(object,f32,20)=FIELD(car,f32,1928);
        FIELD(object,f32,24)=FIELD(car,f32,1932);
        FIELD(object,f32,28)=FIELD(car,f32,1936);
        FIELD(object,f32,32)=FIELD(car,f32,1916);
        FIELD(object,f32,36)=FIELD(car,f32,1920);
        FIELD(object,f32,40)=FIELD(car,f32,1924);
        if(FIELD(D_80153E88,u8,car_index*8+7)==0 || FIELD(D_80153E88,u8,car_index*8+7)==6) {
            best=0;
            for(tire=0;tire<4;tire++) {
                FIELD(object,u16,836+tire*2)=FIELD(car,u16,1580+tire*2);
                counts[tire]=0;
                if(FIELD(car,u16,1564+tire*2)!=8 || FIELD(car,u16,1580+tire*2)!=0) {
                    for(j=tire+1;j<4;j++) {
                        if(FIELD(car,u16,1580+tire*2)==FIELD(car,u16,1580+j*2)) counts[tire]++;
                    }
                }
                if(counts[tire]>=counts[best]) {
                    best=tire;
                    event=FIELD(car,u16,1580+tire*2);
                }
            }
            FIELD(object,u8,858)=(event&0x100)>>8;
            if(FIELD(car,f32,980)!=0.0f) FIELD(car,u32,2004)|=0x1000;
            else FIELD(car,u32,2004)&=~0x1000u;
            if(FIELD(car,f32,1008)>20.0f && FIELD(car,f32,196)!=0.0f) FIELD(car,u32,2004)|=0x80;
            else FIELD(car,u32,2004)&=~0x80u;
            if(FIELD(car,f32,1008)>20.0f && FIELD(car,f32,220)!=0.0f) FIELD(car,u32,2004)|=0x4000;
            else FIELD(car,u32,2004)&=~0x4000u;
            if(FIELD(car,f32,1008)>20.0f && FIELD(car,f32,208)!=0.0f) FIELD(car,u32,2004)|=0x100;
            else FIELD(car,u32,2004)&=~0x100u;
            if(FIELD(car,f32,1008)>20.0f && FIELD(car,f32,232)!=0.0f) FIELD(car,u32,2004)|=0x2000;
            else FIELD(car,u32,2004)&=~0x2000u;
            if(FIELD(car,s8,1602)>=3) FIELD(car,u32,2004)|=0x800;
            else if(FIELD(car,s8,1602)==0) FIELD(car,u32,2004)&=~0x800u;
            if(FIELD(object,s8,777)!=0 && FIELD(car,s8,2027)==0) FIELD(car,u32,2004)&=~8u;
            else if(FIELD(object,s8,777)==0 && FIELD(car,s8,2027)!=0) FIELD(car,u32,2004)|=8;
            func_800E7FA0(car_index);
            FIELD(object,f32,164)=FIELD(car,f32,64);
            FIELD(object,f32,168)=FIELD(car,f32,68);
            FIELD(object,f32,172)=FIELD(car,f32,72);
            FIELD(object,s16,248)=FIELD(car,s16,1880);
        }
        FIELD(object,u32,232)=FIELD(car,u32,2004);
        FIELD(car,u32,2004)&=~0x100000u;
        FIELD(object,s8,776)=FIELD(car,s8,2026);
        FIELD(object,s8,777)=FIELD(car,s8,2027);
        if(FIELD(object,s8,777)==0 && FIELD(object,f32,780)==0.0f) {
            FIELD(object,s8,785)=-1;
            FIELD(object,s8,784)=-1;
            FIELD(object,f32,780)=FIELD(car,f32,1812);
        }
        if(FIELD(object,s8,777)==1) FIELD(object,f32,780)=0.0f;
        FIELD(object,s8,236)=FIELD(car,s8,2013);
    }
    osStartThread(D_80034840);
}
