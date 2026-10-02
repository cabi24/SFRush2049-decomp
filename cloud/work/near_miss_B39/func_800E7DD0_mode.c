/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef signed char s8;
typedef signed short s16;
typedef int s32;
typedef float f32;
typedef struct Record {
    char prefix[0x3F0]; f32 timer;
    char before_parent[0x3D2];s16 parent;
    char before_mode[4];s8 mode;
    char before_selector[0x1B];s8 selector;
    char suffix[0x1F];
} Record;
extern Record D_8014A250[];
extern s8 D_80142760,D_80152744;
extern s16 D_8014A108,D_80152734,D_8015273A;
extern s32 D_80143F10,D_8014A110;
extern f32 D_801525F4,D_8002EB94;
s32 func_800E7DD0(void) {
    s16 i;
    s32 count,success,mode;
    Record *record;
    if(D_80142760 && D_8014A108>=2 && D_80143F10) {
        if(D_801525F4>0.0f) {
            D_801525F4+=D_8002EB94;
            if(D_801525F4>4.0f) return 1;
        } else {
            for(i=0;i<D_8014A108;i++) {
                if(D_8014A250[i].selector==D_80152734) D_801525F4=D_8002EB94;
            }
        }
        return 0;
    }
    mode=D_8014A110;
    if(mode==2) count=1;
    else count=D_80152744;
    success=1;
    for(i=0;i<count;i++) {
        record=&D_8014A250[D_8014A250[i].parent];
        if(record->mode==2 && (!D_80142760 || record->selector==D_80152734) && record->timer>5.0f) {
            success=0;
            break;
        }
    }
    if(success && mode==4) return D_8015273A;
    return success;
}
