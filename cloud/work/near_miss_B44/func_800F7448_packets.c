/* flags: -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned int u32;
typedef unsigned short u16;
typedef struct Command {u32 words[2];} Command;
typedef struct FrameBuffer {void *buffer; unsigned char opaque[124];} FrameBuffer;
extern Command *msgq_ptr;
extern int D_8002AFC0,D_8002AFC4;
extern signed char D_8015F72D;
extern FrameBuffer D_80156C5C[];
extern u32 osVirtualToPhysical(void *);
void func_800F7448(u16 color) {
    Command *sync,*image,*fill,*rectangle,*finish;
    sync=msgq_ptr++;
    sync->words[0]=0xE7000000;
    sync->words[1]=0;
    image=msgq_ptr++;
    image->words[0]=0xFF100000|((D_8002AFC0-1)&0xFFF);
    image->words[1]=osVirtualToPhysical(D_80156C5C[D_8015F72D].buffer);
    fill=msgq_ptr++;
    fill->words[0]=0xF7000000;
    fill->words[1]=(color<<16)|color;
    rectangle=msgq_ptr++;
    rectangle->words[0]=0xF6000000|(((D_8002AFC0-1)&0x3FF)<<14)|(((D_8002AFC4-1)&0x3FF)<<2);
    rectangle->words[1]=0;
    finish=msgq_ptr++;
    finish->words[0]=0xE7000000;
    finish->words[1]=0;
}
