/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Slot64 {
    s32 unknown0,id;
    f32 angle,matrix[3][3],position[3];
    s32 unknown3C;
} Slot64;
typedef struct Resource { u8 unknown0[16]; u8 player; } Resource;
typedef struct Owner { u32 unknown0,unknown4; Resource **resource; } Owner;
typedef struct Player76 { u8 unknown0[72]; Owner **owner; } Player76;
typedef struct Blit Blit;
extern s16 D_8014A108;
extern s8 D_803B4679[][7],D_803B4678,D_80157244;
extern s32 D_803B4634,D_803B4638,D_803B469C,D_803B46A0,D_8014A110;
extern u32 D_801174BC,D_801174B4,D_8015694C;
extern f32 D_803B463C,D_803B4640,D_803B4644,D_803B4648,D_803B464C,D_803B4650;
extern f32 D_803BAC18,D_8011418C[3][3];
extern f32 D_8002EB94;
extern Slot64 D_803B42F4[13];
extern u8 D_80140BDC;
extern char D_803B8884[],D_803B8894[];
extern Player76 D_8014A118[];
extern Blit *D_803B467C;
extern s32 func_800F7644(s32);
extern s8 func_800A361C(s32);
extern void *memcpy(void *,const void *,u32);
extern void math_utility(void *,void *);
extern void func_800B5898(f32,f32 (*)[3]);
extern void func_800B5940(f32,f32 (*)[3]);
extern void func_8008B32C(f32 (*)[3],f32 (*)[3],f32);
extern void func_8008D870(s16,s32,s32);
extern void *func_800B24EC(char *,s16 *,s32,s8,s32);
extern void model_data_load(s32,s32,u32),model_transform_setup(s32,s32,u32);
extern void func_800B5570(s32),func_803998C0(void);
extern void resource_type_select(u32),audio_distance_atten(u32);
extern void sound_call_minimal(s16),sound_stop(Blit *),ambient_sounds_clear(void);
extern f32 fabsf(f32);
#pragma intrinsic (fabsf)

s32 func_80399394(s32 index)
{
    s32 value=D_803B4679[D_8014A108][index];
    if(index==5 && !func_800F7644(18))value=0;
    return value;
}

void func_803993EC(void)
{
    f32 oldPosition[3];
    f32 selectedY;
    s16 texture;
    s32 i;
    f32 y;
    Slot64 *slot;
    if(D_803B4634) {
        y=D_803B4648;
        for(i=0;i<7;i++) {
            slot=&D_803B42F4[i];
            if(i==D_803B469C) {
                selectedY=y;
                if(slot->angle<3.1415927410125732f)slot->angle+=12.566370964050293f*D_8002EB94;
                if(slot->angle>3.1415927410125732f || D_803B4638==1)slot->angle=3.1415927410125732f;
            } else {
                if(slot->angle>0.0f)slot->angle-=12.566370964050293f*D_8002EB94;
                if(slot->angle<0.0f || D_803B4638==1)slot->angle=0.0f;
            }
            memcpy(slot->matrix,D_8011418C,36);
            memcpy(oldPosition,slot->position,12);
            slot->position[0]=D_803B4644;
            slot->position[1]=y;
            slot->position[2]=D_803B464C;
            if(D_803B4638==0) {
                if(oldPosition[1]+180.0f*D_8002EB94<slot->position[1])slot->position[1]=oldPosition[1]+180.0f*D_8002EB94;
                if(oldPosition[1]-180.0f*D_8002EB94>slot->position[1])slot->position[1]=oldPosition[1]-180.0f*D_8002EB94;
            }
            func_800B5898(slot->angle-1.5707963705062866f,slot->matrix);
            if(slot->angle>1.5707963705062866f)
                func_8008D870((s16)slot->id,(s32)func_800B24EC(D_803B8884,&texture,0,D_80140BDC-1,1),-1);
            else func_8008D870((s16)slot->id,(s32)func_800B24EC(D_803B8894,&texture,0,D_80140BDC-1,1),-1);
            if(!func_80399394(i))model_data_load(slot->id,0,15);
            else {
                model_transform_setup(slot->id,0,15);
                y-=D_803B4640*D_803B463C;
            }
        }
        slot=&D_803B42F4[7];
        memcpy(oldPosition,slot->position,12);
        slot->angle+=6.2831854820251465f*D_8002EB94;
        if(slot->angle>6.2831854820251465f)slot->angle-=6.2831854820251465f;
        math_utility(D_8011418C,slot->matrix);
        slot->position[0]=D_803B4644-80.0f;
        slot->position[1]=selectedY*D_803B4650/D_803B464C;
        slot->position[2]=D_803B4650;
        if(D_803B4638==0) {
            if(D_803BAC18<fabsf(slot->position[1]-oldPosition[1])*60.0f/10.0f) {
                D_803BAC18=fabsf(slot->position[1]-oldPosition[1])*60.0f/10.0f;
                D_803BAC18=D_803BAC18<540.0f*D_803B463C ? D_803BAC18 : 540.0f*D_803B463C;
                D_803BAC18=D_803BAC18>180.0f*D_803B463C ? D_803BAC18 : 180.0f*D_803B463C;
            }
            if(oldPosition[1]+D_803BAC18*D_8002EB94<slot->position[1])slot->position[1]=oldPosition[1]+D_803BAC18*D_8002EB94;
            else if(oldPosition[1]-D_803BAC18*D_8002EB94>slot->position[1])slot->position[1]=oldPosition[1]-D_803BAC18*D_8002EB94;
            else D_803BAC18=180.0f*D_803B463C;
        }
        func_800B5940(slot->angle,slot->matrix);
        func_800B5898(1.5707963705062866f,slot->matrix);
        func_8008B32C(slot->matrix,slot->matrix,0.4f);
        D_803B4638=0;
    }
}

void func_80399D10(void)
{
    s32 i;
    Player76 *player;
    Slot64 *slot;
    if(D_801174BC!=1) {
        if(D_801174BC==D_801174B4)D_801174BC=1;
        else {
            if(D_801174BC==0x4000)func_800B5570(0x4000);
            else func_800B5570(0x80);
            return;
        }
    }
    if(!D_803B4678)func_803998C0();
    while(!func_80399394(D_803B469C)) {
        if(++D_803B469C>=7)D_803B469C=0;
    }
    func_803993EC();
    if(D_8015694C&3) {
        resource_type_select(D_8015694C);
        D_803B46A0=1;
    } else if(D_8015694C&4) {
        resource_type_select(D_8015694C);
        D_803B46A0=2;
    } else if(D_8015694C&0x400) {
        audio_distance_atten(D_8015694C);
        do {
            if(--D_803B469C<0)D_803B469C=6;
        }while(!func_80399394(D_803B469C));
    } else if(D_8015694C&0x800) {
        audio_distance_atten(D_8015694C);
        do {
            if(++D_803B469C>=7)D_803B469C=0;
        }while(!func_80399394(D_803B469C));
    }
    for(i=0;i<D_8014A108;i++) {
        player=&D_8014A118[i];
        if((*player->owner)->resource && func_800A361C((*(*player->owner)->resource)->player)) {
            for(slot=D_803B42F4;slot<D_803B42F4+13;slot++) {
                if(slot->id!=-1) { sound_call_minimal(slot->id); slot->id=-1; }
            }
            D_803B4634=0;
            if(D_803B467C) { sound_stop(D_803B467C); D_803B467C=0; }
            ambient_sounds_clear();
            D_803B4678=0;
            func_800B5570(0x20);
            return;
        }
    }
    if(D_80157244) {
        for(slot=D_803B42F4;slot<D_803B42F4+13;slot++) {
            if(slot->id!=-1) { sound_call_minimal(slot->id); slot->id=-1; }
        }
        D_803B4634=0;
        if(D_803B467C) { sound_stop(D_803B467C); D_803B467C=0; }
        ambient_sounds_clear();
        D_803B4678=0;
        func_800B5570(4);
        return;
    }
    if(D_803B46A0) {
        for(slot=D_803B42F4;slot<D_803B42F4+13;slot++) {
            if(slot->id!=-1) { sound_call_minimal(slot->id); slot->id=-1; }
        }
        D_803B4634=0;
        if(D_803B467C) { sound_stop(D_803B467C); D_803B467C=0; }
        ambient_sounds_clear();
        D_803B4678=0;
        switch(D_803B46A0) {
        case 1:
            switch(D_803B469C) {
            case 0: D_8014A110=0; func_800B5570(0x80); break;
            case 1: D_8014A110=1; func_800B5570(0x80); break;
            case 2: D_8014A110=2; func_800B5570(0x80); break;
            case 3: D_8014A110=3; func_800B5570(0x4000); break;
            case 4: D_8014A110=4; func_800B5570(0x80); break;
            case 5: D_8014A110=5; func_800B5570(0x80); break;
            case 6: D_8014A110=6; func_800B5570(0x80); break;
            }
            break;
        case 2: func_800B5570(0x20); break;
        }
        D_803B46A0=0;
    }
}

typedef struct Color4 { u32 word; } Color4;
typedef struct MultiBlit {
    const char *texname;
    s16 dulx,duly,width,height,top,bot,left,right;
    u32 zdepth,alpha;
    s32 (*animfunc)(Blit *);
    u32 animid;
} MultiBlit;
extern Color4 D_803B46A4;
extern char *D_803B42D4[];
extern char D_803B8864[],D_803B886C[],D_803B8870[],D_803B8874[];
extern MultiBlit D_803B4654;
extern void visual_objects_update(s32),particle_lifetime_set(void);
extern void particle_velocity_set(void),entity_audio_update(void);
extern s32 string_copy_format(char *,s8,s8,s8);
extern s32 sign_extend_call(u32,u32,s32,u32);
extern void func_8008E06C(s16,Color4 *);
extern Blit *sound_control(s16,s16,const MultiBlit *,s16);
extern u8 func_800CDED8(void **);

void func_803998C0(void)
{
    u32 flags;
    s32 object;
    Color4 color;
    s32 i;
    s16 texture;
    Slot64 *slot;
    f32 (*matrix)[3];
    visual_objects_update(1);
    particle_lifetime_set();
    color=D_803B46A4;
    for(i=0;i<13;i++) {
        slot=&D_803B42F4[i];
        if(slot->id==-1) {
            matrix=slot->matrix;
            memcpy(matrix,D_8011418C,36);
            object=string_copy_format(D_803B42D4[slot->unknown0],0,D_80140BDC-1,1);
            if(slot->unknown0==0 || slot->unknown0==1)flags=0x42000; else flags=0; slot->id=sign_extend_call(object,(u32)matrix,-1,flags);
            func_8008E06C(slot->id,&color);
        }
    }
    func_8008D870(D_803B42F4[7].id,(s32)func_800B24EC(D_803B8864,&texture,0,D_80140BDC-1,1),-1);
    func_8008D870(D_803B42F4[8].id,(s32)func_800B24EC(D_803B886C,&texture,0,D_80140BDC-1,1),-1);
    memcpy(D_803B42F4[8].matrix,D_8011418C,36);
    func_800B5898(-1.5707963705062866f,D_803B42F4[8].matrix);
    D_803B42F4[8].position[0]=75.0999984741211f;
    D_803B42F4[8].position[1]=55.20000076293945f;
    D_803B42F4[8].position[2]=100.0f;
    func_8008B32C(D_803B42F4[8].matrix,D_803B42F4[8].matrix,0.5f);
    memcpy(D_803B42F4[9].matrix,D_8011418C,36);
    func_800B5898(-1.5707963705062866f,D_803B42F4[9].matrix);
    func_800B5940(3.1415927410125732f,D_803B42F4[9].matrix);
    D_803B42F4[9].position[0]=87.0999984741211f;
    D_803B42F4[9].position[1]=55.20000076293945f;
    D_803B42F4[9].position[2]=100.0f;
    func_8008D870(D_803B42F4[10].id,(s32)func_800B24EC(D_803B8870,&texture,0,D_80140BDC-1,1),-1);
    memcpy(D_803B42F4[10].matrix,D_8011418C,36);
    func_800B5898(-1.5707963705062866f,D_803B42F4[10].matrix);
    D_803B42F4[10].position[0]=-75.0999984741211f;
    D_803B42F4[10].position[1]=55.20000076293945f;
    D_803B42F4[10].position[2]=100.0f;
    func_8008B32C(D_803B42F4[10].matrix,D_803B42F4[10].matrix,0.5f);
    memcpy(D_803B42F4[11].matrix,D_8011418C,36);
    func_800B5898(-1.5707963705062866f,D_803B42F4[11].matrix);
    D_803B42F4[11].position[0]=-87.0999984741211f;
    D_803B42F4[11].position[1]=55.20000076293945f;
    D_803B42F4[11].position[2]=100.0f;
    func_8008D870(D_803B42F4[12].id,(s32)func_800B24EC(D_803B8874,&texture,0,D_80140BDC-1,1),-1);
    memcpy(D_803B42F4[12].matrix,D_8011418C,36);
    func_800B5898(-1.5707963705062866f,D_803B42F4[12].matrix);
    D_803B42F4[12].position[0]=0.0f;
    D_803B42F4[12].position[1]=115.0f;
    D_803B42F4[12].position[2]=200.0f;
    D_803B4634=1;
    D_803B4638=1;
    particle_velocity_set();
    D_803B467C=sound_control(0,0,&D_803B4654,1);
    D_803B46A0=0;
    entity_audio_update();
    D_803B469C=func_800CDED8((void **)D_8014A118[0].owner);
    switch(D_803B469C) {
    case 1: D_803B469C=1; break;
    case 2: D_803B469C=2; break;
    case 3: D_803B469C=3; break;
    case 4: D_803B469C=4; break;
    case 5: D_803B469C=5; break;
    case 6: D_803B469C=6; break;
    default: D_803B469C=0; break;
    }
    D_803B4678=1;
}
