/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed int s32;
typedef signed short s16;
typedef signed char s8;
typedef unsigned char u8;
typedef float f32;
#define FIELD(e,t,o) (*(t *)((u8 *)(e)+(o)))
extern u8 player_array[], D_8014A250[], input_rec0[], D_801439D8[], D_801439F8[];
extern s8 D_80152744, D_80142DB4[6];
extern s16 D_80153FD2, D_80142D70;
extern f32 D_801543CC;
extern void func_800D2128(f32,void *,u8);
s32 func_80105B74(s32 arg0) {
    s16 order[6];
    u8 *car=player_array,*record=D_801439D8;
    s16 i,j,id,used;
    s32 count;
    do {
        func_800D2128(FIELD(car,s8,0xEF)==1 ? FIELD(car,f32,0xF0) : D_801543CC,record,0x66);
        car+=0x3B8;
        record+=8;
    }while(record!=D_801439F8);
    for(i=0;i<6;i++)D_80142DB4[i]=-1;
    count=D_80152744;
    for(i=0;i<count;i++) {
        id=0;
        j=0;
        if(count>0) {
            do {
                id=FIELD(D_8014A250+j*0x808,s16,0x7C6);
                j++;
                if(i==FIELD(player_array+id*0x3B8,s8,0xEE))break;
            }while(j<count);
        }
        order[i]=id;
    }
    used=0;
    if(D_80153FD2>0) {
        do { D_80142DB4[count-used-1]=input_rec0[used*0x4C];used++; }while(used<D_80153FD2);
    }
    for(i=0;i<count;i++) {
        id=order[i];
        if(FIELD(D_8014A250+id*0x808,s8,0x7CC)==1) {
            D_80142DB4[count-used-1]=id;
            used++;
        }
        if(used==count)break;
    }
    D_80142D70=1;
    return 1;
}
