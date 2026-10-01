/* flags: -g0 -O2 -mips2 -G 0 -non_shared */
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
extern s32 D_801170F8;
extern u8 *D_80116FE4;
extern u8 D_80116FE8[], D_8012E618[];
extern u16 D_801170E8, D_801170EC, D_801170F0, D_801170F4;
s32 func_800DC628(u32 arg0,u32 arg1) {
    register s32 i=0;
    u8 *src;
    u16 size;
    u8 *p;
    if(D_801170F8) {
        D_801170F8=0;
        src=D_80116FE4;
        do {
            D_80116FE8[*src++]=i++;
        }while(i!=32);
    }
    size=(arg0+arg1+7)>>3;
    D_801170E8=size;
    if(size>=33)return 0;
    if(arg1>=33)return 0;
    p=D_8012E618;
    if(size>0) {
        do { *p++=0; }while((u32)p<(u32)(D_8012E618+size));
    }
    D_801170EC=arg0;
    D_801170F0=arg1;
    D_801170F4=0;
    return 1;
}
