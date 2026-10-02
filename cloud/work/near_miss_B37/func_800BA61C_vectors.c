/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed short s16;
typedef unsigned short u16;
typedef float f32;
typedef int s32;
typedef struct Record {char prefix[0xC];f32 position[3];f32 direction[3];s32 range;char suffix[0x28];} Record;
extern Record D_80151CE8[];
extern u16 D_801407F0;
extern s16 (*D_801407F4)[3];
extern f32 D_80123E00;
s16 func_800BA61C(s16 record_index) {
    Record *record=&D_80151CE8[record_index];
    f32 direction[2];
    f32 minimum=D_80123E00;
    f32 previous_distance;
    f32 delta[2],distance;
    s16 previous_sign;
    s16 best_index;
    s16 step=-1;
    s16 index=0;
    s16 sign;
    s32 count=D_801407F0;
    direction[0]=record->direction[0];
    direction[1]=record->direction[2];
    for(;step<count;step++,index++) {
        if(index==count) index=0;
        delta[0]=(f32)D_801407F4[index][0]-record->position[0];
        delta[1]=(f32)D_801407F4[index][2]-record->position[2];
        sign=1;
        if(direction[1]*delta[1]+delta[0]*direction[0]<0.0f) sign=-1;
        distance=delta[1]*delta[1]+delta[0]*delta[0];
        if(distance<minimum) {
            minimum=distance;
            best_index=index;
        }
        if(step>=0 && distance<=(f32)record->range && previous_distance<=(f32)record->range && sign!=previous_sign) break;
        previous_sign=sign;
        previous_distance=distance;
    }
    if(step==count) return best_index;
    return index;
}
