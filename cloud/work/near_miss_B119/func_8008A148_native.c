/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
typedef unsigned int u32;
typedef unsigned char u8;
typedef struct Gfx {u32 w0,w1;} Gfx;
extern Gfx *D_80149438;
extern void *D_8012E6D0;
#define COMMAND(first,second) do {Gfx *command=D_80149438++; command->w0=(first);command->w1=(second);}while(0)
void func_8008A148(void *image,int mode,int palette,int bank)
{
    u8 *bytes=(u8 *)image;
    if(image==D_8012E6D0)return;
    if((mode==2 || mode==5) && palette==1) {
        COMMAND(0xFD100000,(u32)image);
        COMMAND(0xE8000000,0);
        COMMAND(0xF5000100,0x07000000);
        COMMAND(0xE6000000,0);
        COMMAND(0xF0000000,0x073FC000);
        COMMAND(0xE7000000,0);
    } else if(mode==2 && palette==0) {
        COMMAND(0xFD100000,(u32)image);
        COMMAND(0xE8000000,0);
        COMMAND(0xF5000000 | ((((bank&15)<<4)+256)&511),0x07000000);
        COMMAND(0xE6000000,0);
        COMMAND(0xF0000000,0x0703C000);
        COMMAND(0xE7000000,0);
    } else if(mode==4 || mode==3) {
        COMMAND(0xFA000000,((u32)bytes[0]<<24)|bytes[3]|((u32)bytes[1]<<16)|((u32)bytes[2]<<8));
        COMMAND(0xFB000000,((u32)bytes[4]<<24)|bytes[7]|((u32)bytes[5]<<16)|((u32)bytes[6]<<8));
    }
    D_8012E6D0=image;
}
