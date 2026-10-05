/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned char u8;
typedef short s16;
typedef int s32;
typedef float f32;
extern u8 D_80151CE8[],D_801407F0[];
extern f32 D_80152800,D_801543AC;
extern s16 func_800BA61C(s16);
extern s16 func_800BA2B8(s16,s16);
extern s16 audio_priority_find(s16,s16);
extern f32 audio_channel_alloc(s32,s32);
#define HALF(p,n) (*(s16 *)((p)+(n)))
#define FLOAT(p,n) (*(f32 *)((p)+(n)))
void audio_mixer_main(s16 mode) {
    s16 position[3];
    s32 i,j,next,loop;
    u8 *row,*slot;
    for(i=0;i<HALF(D_80151CE8,8);i++) {
        row=D_80151CE8+i*80;
        position[0]=(s16)FLOAT(row,12);
        position[1]=(s16)FLOAT(row,16);
        position[2]=(s16)FLOAT(row,20);
        if(mode<0) {
            slot=row;
            for(j=0;j<20;j++) {
                if(j==0) HALF(slot,46)=func_800BA61C((s16)i);
                else if(j<5) HALF(slot,46)=func_800BA2B8((s16)i,(s16)(j-1));
                else HALF(slot,46)=audio_priority_find((s16)(j-5),(s16)i);
                slot+=2;
            }
        } else {
            slot=row+mode*2;
            if(mode==0) {
                HALF(slot,46)=func_800BA61C((s16)i);
                slot=row;
                for(j=0;j<D_801407F0[8];j++) {
                    HALF(slot,56)=audio_priority_find((s16)j,(s16)i);
                    slot+=2;
                }
            } else HALF(slot,46)=func_800BA2B8((s16)i,(s16)(mode-1));
        }
    }
    D_80152800=0.0f;
    D_801543AC=D_80152800;
    loop=0;
    row=D_80151CE8;
    for(i=0;i<HALF(D_80151CE8,8);i++) {
        if(i+1==HALF(D_80151CE8,8)) next=HALF(D_80151CE8,2);
        else next=i+1;
        FLOAT(row,88)=audio_channel_alloc(HALF(row,46),HALF(D_80151CE8+next*80,46));
        if(i>0 && i==HALF(D_80151CE8,4)) loop=1;
        if(!loop) D_80152800+=FLOAT(row,88);
        if(i>=HALF(D_80151CE8,2)) D_801543AC+=FLOAT(row,88);
        row+=80;
    }
}
