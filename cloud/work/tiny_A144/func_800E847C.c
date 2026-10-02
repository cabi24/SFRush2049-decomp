/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
#define FIELD(p,t,o) (*(t *)((u8 *)(p)+(o)))
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
