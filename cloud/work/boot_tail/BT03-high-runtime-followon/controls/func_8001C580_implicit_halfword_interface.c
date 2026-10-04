/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef unsigned char u8;
typedef unsigned short u16;
typedef unsigned int u32;
typedef signed char s8;
typedef signed short s16;
typedef int (*SampleCallback)(short *,u32,short *,u32,u32);
typedef struct SampleBuffer {u8 mode,unknown01[3];SampleCallback callback;short *buffer;u32 samples,position,context;} SampleBuffer;
#pragma pack(1)
typedef struct SampleInfo {u32 frequency;void *data;u32 offset,length,loopStart,loopLength;u8 format;} SampleInfo;
#pragma pack(0)
extern u8 D_8002C630;
extern int D_8004F800;
extern SampleBuffer D_8004FA50[];
extern void func_80014594(void);
extern void func_800145DC(void);
extern int func_8001F898(u8);
extern void func_800146B4(u32,SampleInfo *,u8);
extern void func_80014A0C(int,u16);
extern void func_80014A74(int,u32,u32,u32,u32);
extern void func_800149BC(int);
int func_8001C580(u8 request,short *buffer,u32 samples,u32 frequency,
                  u8 volume,u8 pan,u8 span,u8 aux,SampleCallback callback,u32 context)
{
    int index;
    SampleInfo info;
    float ratio;
    if (D_8002C630) {
        func_80014594();
        index=func_8001F898(request);
        if (index != -1) {
            D_8004FA50[index].mode=1;
            D_8004FA50[index].position=0;
            info.frequency=frequency | 0x400000;
            info.offset=0;
            info.loopStart=0;
            info.format=0;
            D_8004FA50[index].samples=samples;
            info.length=samples;
            info.loopLength=samples;
            D_8004FA50[index].buffer=buffer;
            info.data=buffer;
            D_8004FA50[index].callback=callback;
            D_8004FA50[index].context=context;
            func_800146B4(index,&info,1);
            ratio=(float)frequency/(float)D_8004F800;
            func_80014A0C(index,(u32)(ratio*4096.0f));
            func_80014A74(index,(u32)volume<<16,(u32)pan<<16,(u32)span<<16,(u32)aux<<16);
            func_800149BC(index);
        }
        func_800145DC();
        return index;
    }
    return -1;
}
