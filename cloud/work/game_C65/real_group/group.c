/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
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
    descriptor=owner->descriptor;
    switch(descriptor->state) {
    case 2:break;
    case 1:return;
    default:break;
    }
    if((float)age>=(float)D_80146200*.75f) goto request_five;
    if(descriptor->enabled && descriptor->state==2 && descriptor->fade<=0.0f) {
        phase=descriptor->phase+D_80123A64;
        if(descriptor->phase>D_80152748) phase-=14400.0f;
        if(phase<D_80152748) goto request_five;
    }
    if(descriptor->state!=2 && (descriptor->fade>D_80123A68 || (!descriptor->enabled && descriptor->mode==1))) {
        slot=func_80091B00();
        D_80143AE8[D_8011024C++]=slot;
        slot->type=3;
        slot->descriptor=owner->descriptor;
        slot->descriptor->pending++;
    }
    return;
request_five:
    if(descriptor->state==2) {
        slot=func_80091B00();
        D_80143CF0[D_80110250++]=slot;
        slot->type=5;
        slot->descriptor=owner->descriptor;
        slot->descriptor->pending++;
    }
}

extern Slot D_80142DD8[128];
Slot *func_80091B00(void) {
 Slot *slot;
 for(slot=D_80142DD8;slot!=D_80142DD8+128;slot+=4) {
  if(slot[0].used==0) {slot[0].used=1;slot[0].id=-1;return slot+0;}
if(slot[1].used==0) {slot[1].used=1;slot[1].id=-1;return slot+1;}
if(slot[2].used==0) {slot[2].used=1;slot[2].id=-1;return slot+2;}
if(slot[3].used==0) {slot[3].used=1;slot[3].id=-1;return slot+3;}
 }
 return 0;
}
