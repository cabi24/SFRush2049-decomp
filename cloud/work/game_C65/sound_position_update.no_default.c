/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef struct Descriptor {
    u8 opaque0[16];s32 state,mode;u8 opaque24;s8 enabled;u8 pending,opaque27[5];float phase,fade;u8 opaque40[28];
} Descriptor;
typedef struct Owner {u8 opaque0[8];Descriptor *descriptor;} Owner;
typedef struct Slot {s16 id;u8 type;s8 used;Descriptor *descriptor;u8 opaque8[16];} Slot;
extern s32 D_80146200;
extern float D_80123A64,D_80123A68,D_80152748;
extern s32 D_80110250,D_8011024C;
extern Slot *D_80143CF0[],*D_80143AE8[];
extern Slot *func_80091B00(void);
void sound_position_update(Owner *owner,s32 age) {
    Descriptor *descriptor;
    Slot *slot;
    float phase;
    s32 state;
    descriptor=owner->descriptor;
    state=descriptor->state;
    switch(state) {
    case 2:break;
    case 1:return;
    }
    if((float)age>=(float)D_80146200*.75f) goto request_five;
    if(descriptor->enabled && state==2 && descriptor->fade<=0.0f) {
        phase=descriptor->phase+D_80123A64;
        if(descriptor->phase>D_80152748) phase-=14400.0f;
        if(phase<D_80152748) goto request_five;
    }
    if(state!=2 && (descriptor->fade>D_80123A68 || (!descriptor->enabled && descriptor->mode==1))) {
        slot=func_80091B00();
        D_80143AE8[D_8011024C++]=slot;
        slot->type=3;
        slot->descriptor=owner->descriptor;
        slot->descriptor->pending++;
    }
    return;
request_five:
    if(state==2) {
        slot=func_80091B00();
        D_80143CF0[D_80110250++]=slot;
        slot->type=5;
        slot->descriptor=owner->descriptor;
        slot->descriptor->pending++;
    }
}
