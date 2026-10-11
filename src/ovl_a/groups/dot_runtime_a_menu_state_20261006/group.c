/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Blit Blit;
typedef struct MultiBlit {
    const char *texname;
    s16 dulx,duly,width,height,top,bot,left,right;
    u32 zdepth,alpha;
    s32 (*animfunc)(Blit *);
    u32 animid;
} MultiBlit;
typedef struct PlayerPresence { s8 present; u8 unknown1[15]; } PlayerPresence;
extern s32 D_803B9BB0,D_803B9BB4,D_803B46B4;
extern s8 D_803B9BB8,D_803B46A8,D_803B46B0,D_80149414;
extern s16 D_803B9BBA,D_803B9BBC,D_803B9BC4,D_803B9BC6;
extern s16 D_803B9E38,D_803B9E3A,D_803B9E3C;
extern void **D_803B9BC0;
extern u32 D_80149784,D_8015694C,D_80156944,D_801174B4;
extern Blit *D_803B46AC;
extern MultiBlit D_803B46B8;
extern PlayerPresence D_80156CF0[];
extern u32 func_800A35BC(s32);
extern s8 func_800A1A3C(s32),func_800A35F8(s32),func_80094FC4(s32),func_800A361C(s32);
extern void **func_80398BF0(s32);
extern void func_800A2680(void **);
extern u32 entity_flags_apply(u32,u32,u32,u8);
extern void display_settings(s32,s32,s32,s32,s32,s32);
extern void resource_type_select(u32);
extern void audio_doppler(u32);
extern void func_800B5570(s32);
extern void ambient_sounds_clear(void);
extern void sound_stop(Blit *);
extern Blit *sound_control(s16,s16,const MultiBlit *,s16);
extern void particle_lifetime_set(void);
extern void player_state_set(s32,s32);
extern s32 func_8039A24C(s32);
extern void control_settings(s32,s8 *,s8 *,s8 *,s32 (*)(s32));

void func_80398B40(s32 mode)
{
    switch(mode) {
    case 0: break;
    case 1:
        D_803B9BB4=8;
        D_803B9BBC=func_800A35BC(D_803B9BBA);
        D_803B9BC4=16;
        if(D_803B9BB4<D_803B9BC4)D_803B9E3A=D_803B9BB4;
        else D_803B9E3A=D_803B9BC4;
        break;
    case 2: D_803B9BB8=0; break;
    }
    D_803B9BB0=mode;
}

void func_80398C3C(void)
{
    if(D_80149784&0x400) {
        entity_flags_apply(40,0,1,0);
        if(--D_803B9BC6<0)D_803B9BC6=D_803B9BC4-1;
        if(D_803B9E3A<D_803B9BC4) {
            if(D_803B9E3C==0) {
                if(--D_803B9E38<0)D_803B9E38=D_803B9BC4-1;
            } else D_803B9E3C--;
        }
    } else if(D_80149784&0x800) {
        entity_flags_apply(39,0,1,0);
        if(++D_803B9BC6>=D_803B9BC4)D_803B9BC6=0;
        if(D_803B9E3A<D_803B9BC4) {
            if(D_803B9E3C==D_803B9E3A-1) {
                if(++D_803B9E38>=D_803B9BC4)D_803B9E38=0;
            } else D_803B9E3C++;
        }
    }
    if(D_80149784&0xC00)D_803B9BC0=func_80398BF0(D_803B9BC6);
    if(((D_8015694C&0x100)&&(D_80156944&0x80)) ||
       ((D_80156944&0x100)&&(D_8015694C&0x80))) {
        entity_flags_apply(43,0,1,0);
        if(D_803B9BC0)func_80398B40(2);
    } else if(D_8015694C&5) {
        entity_flags_apply(38,0,1,0);
        func_80398B40(0);
    }
}

void func_80398E70(void)
{
    if(D_8015694C&0x400) {
        entity_flags_apply(40,0,1,0);
        if(--D_803B9BBA<0)D_803B9BBA=3;
    } else if(D_8015694C&0x800) {
        entity_flags_apply(39,0,1,0);
        if(++D_803B9BBA>=4)D_803B9BBA=0;
    }
    if(D_8015694C&2) {
        entity_flags_apply(37,0,1,0);
        if(!D_80156CF0[D_803B9BBA].present) {
            display_settings(D_803B9BBA,1,10,50,300,140);
            D_803B46B4=1;
        } else if(!func_800A1A3C(D_803B9BBA)) {
            display_settings(D_803B9BBA,2,10,50,300,140);
            D_803B46B4=2;
        } else if(func_800A35F8(D_803B9BBA))func_80398B40(4);
        else if(func_80094FC4(D_803B9BBA))func_80398B40(3);
        else {
            D_803B9E38=D_803B9E3C=D_803B9BC6=0;
            D_803B9BC0=func_80398BF0(D_803B9BC6);
            func_80398B40(1);
        }
    } else if(D_8015694C&5) {
        resource_type_select(D_8015694C);
        func_800B5570(D_80149414 ? 0x40000000 : 2);
        if(D_803B46A8) {
            D_803B46A8=0;
            if(D_803B46AC) {
                sound_stop(D_803B46AC);
                D_803B46AC=0;
            }
            ambient_sounds_clear();
        }
    }
}

void func_803990D0(void)
{
    s8 complete;
    s8 result;
    s8 choice;
    MultiBlit descriptor;
    s32 i;
    if(!D_803B46A8) {
        descriptor=D_803B46B8;
        func_80398B40(0);
        if(D_801174B4&0x7C03FFFE)particle_lifetime_set();
        D_803B46AC=sound_control(0,0,&descriptor,1);
        player_state_set(-1,1);
        D_803B46A8=1;
    }
    if(D_803B46B4) {
        control_settings(D_803B9BBA,&result,&choice,&complete,func_8039A24C);
        if(complete)D_803B46B4=0;
        D_803B46B0=0;
    } else {
        D_803B46B0=1;
        for(i=0;i<4;i++) {
            if(func_800A361C(i)) {
                func_80398B40(0);
                break;
            }
        }
        switch(D_803B9BB0) {
        case 0: func_80398E70(); break;
        case 1: func_80398C3C(); break;
        case 2:
            if(D_8015694C&0x3000) {
                audio_doppler(D_8015694C);
                D_803B9BB8=!D_803B9BB8;
            }
            if(D_8015694C&2) {
                entity_flags_apply(37,0,1,0);
                if(D_803B9BB8) {
                    func_800A2680(D_803B9BC0);
                    D_803B9BBC=func_800A35BC(D_803B9BBA);
                    D_803B9BC0=func_80398BF0(D_803B9BC6);
                }
                func_80398B40(1);
            } else if(D_8015694C&5) {
                resource_type_select(D_8015694C);
                func_80398B40(1);
            }
            break;
        case 3:
            if(D_8015694C&7) {
                resource_type_select(D_8015694C);
                func_80398B40(0);
            }
            break;
        case 4:
            if(D_8015694C&7) {
                resource_type_select(D_8015694C);
                func_80398B40(0);
            }
            break;
        }
    }
}
