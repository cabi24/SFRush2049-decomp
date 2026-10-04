/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
#pragma pack(1)
typedef struct VoiceState {
    u8 unknown00[16];u32 child,parent;u8 unknown18[12];
    u32 flags;u8 unknown28[34];u8 channel,set;u8 unknown4C[2];
    u16 base_key,key;u8 unknown52[14];u32 identifier;
    u8 unknown64[40];u32 glide;u8 unknown90[4];u32 pitch;
    u8 unknown98[40];s8 cents;u8 original_key;u8 unknownC2[222];
} VoiceState;
#pragma pack(0)
extern VoiceState D_8004BEB8[];
extern u8 D_8004FA18;
extern u8 func_8001467C(u32);
extern void func_8001EB10(VoiceState *);
extern u32 func_8001ECE0(VoiceState *);
extern void func_800217E4(VoiceState *);
extern void func_80020F4C(u8,u8,u8);
u32 func_80019C8C(u8 key,u8 channel,u8 set)
{
    u32 i,result,previous;
    VoiceState *state,*last;
    result=0xFFFFFFFFU;
    for (i=0,state=D_8004BEB8;i<D_8004FA18;i++,state++) {
        if (state->identifier!=0xFFFFFFFFU && state->channel==channel && state->set==set &&
            (state->flags&16) && (!(state->flags&8) || (state->flags&0x40000000)) &&
            func_8001467C(i)) {
            last=state;
            state->pitch=((u32)state->key<<16)+(state->cents*65536)/100;
            state->original_key=state->key;
            state->key=key+(state->key&255)-(u8)state->base_key;
            state->base_key=key;
            state->cents=0;
            state->glide=0;
            state->flags|=0x80800;
            func_8001EB10(&D_8004BEB8[i]);
            if (result==0xFFFFFFFFU) {
                state->child=0xFFFFFFFFU;
                state->parent=0xFFFFFFFFU;
                result=func_8001ECE0(&D_8004BEB8[i]);
                previous=state->identifier;
            } else {
                D_8004BEB8[(previous&255)].child=state->identifier;
                state->parent=previous;
                previous=state->identifier;
            }
        }
    }
    if (result!=0xFFFFFFFFU) {
        func_800217E4(last);
        func_80020F4C(last->channel,last->set,(u8)last->key);
    }
    return result;
}
