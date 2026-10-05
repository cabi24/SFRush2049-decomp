/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct { f32 x,y,z; } Vec3;
/* This consumed view reaches the actual owner byte92; no unrelated
   unobserved node capacity is imposed. */
typedef struct {
    u8 prefix[4]; u8 flags; u8 gap5[11]; s16 metadata;
    u8 gap18[38]; Vec3 position;
    u8 gap68[12]; s16 effect; u8 gap82[10]; s8 owner;
} EffectView;
typedef struct {
    u8 prefix[232]; u32 flags; u8 to_kind[900-236];
    s8 kind,amount; s16 timer; u32 field904,extra_flags;
    f32 boost,field916,field920,field924;
    u8 field928,color,state,gap931; u32 field932;
    f32 field936; u8 tail[952-940];
} Car952;
typedef struct {
    u8 prefix[8]; u8 model; u8 to_speed[252-9];
    f32 speed; u8 gap256[8]; f32 boost;
    u8 to_length[1620-268]; f32 length; u8 tail[2056-1624];
} Vehicle2056;
typedef struct { u8 prefix[28]; void *resource; u8 tail[16]; } Metadata48;
typedef struct { f32 bias,maximum,y,z; } Limits16;
extern Car952 D_80152818[];
extern Vehicle2056 D_8014A250[];
extern Metadata48 D_80117530[];
extern s32 D_80121D60;
extern s32 D_80121D64;
extern s32 D_80121D68;
extern s32 D_80121D6C;
extern s32 D_80121D70;
extern s32 D_80121D74;
extern s32 D_80121D78;
extern s32 D_80121D7C;
extern Limits16 D_8011F844[];
extern void stat_lap_split(void *,s32,Vec3 *,s32);
extern void model_data_load(void *,s32,s32);
/* Original inactive path forwards the incoming a0 pointer unchanged.
   No overlay body, additional formal or hidden contract is claimed. */
extern void func_80391490(void);
extern f32 sqrtf(f32);
#pragma intrinsic (sqrtf)
void func_8010D3C0(EffectView *effect)
{
    Car952 *car;
    Vehicle2056 *vehicle;
    s32 kind,amount;
    Limits16 *limits;
    f32 maximum;
    if(effect->flags&4) {
        car=&D_80152818[effect->owner];
        vehicle=&D_8014A250[effect->owner];
        effect->flags&=~6;
        if(car->flags&16) {
            effect->flags|=2;
            return;
        }
        stat_lap_split(D_80117530[effect->metadata].resource,effect->owner,&effect->position,2);
        switch(effect->effect) {
        case 350: kind=0; amount=D_80121D60; break;
        case 351: kind=1; amount=D_80121D64; break;
        case 352: kind=2; amount=D_80121D68; break;
        case 353: car->timer=800; return;
        case 354:
            car->extra_flags|=1;
            car->state=1;
            car->boost=30.0f;
            car->color=255;
            car->field936=0.666667f;
            return;
        case 355: kind=3; amount=D_80121D6C; break;
        case 356: kind=4; amount=D_80121D70; break;
        case 357: kind=5; amount=D_80121D74; break;
        case 358: kind=6; amount=D_80121D78; break;
        case 359:
            car->extra_flags|=10;
            car->field916=0.0333333f;
            car->field924=0.0f;
            car->field920=0.05f;
            return;
        case 360: kind=7; amount=D_80121D7C; break;
        default: kind=1; amount=D_80121D64; break;
        }
        if(car->kind==kind) car->amount+=amount;
        else {car->kind=kind;car->amount=amount;}
        if(kind==5) {
            if(amount) {}
            limits=&D_8011F844[vehicle->model];
            vehicle->speed=vehicle->boost=limits->bias+3.0f;
            maximum=vehicle->speed>limits->maximum?vehicle->speed:limits->maximum;
            vehicle->length=sqrtf(maximum*maximum+limits->y*limits->y+limits->z*limits->z);
        }
    } else {
        func_80391490();
        effect->flags&=~2;
        effect->flags|=32;
        model_data_load(*(void **)((u8 *)effect+12),0,15);
    }
}
