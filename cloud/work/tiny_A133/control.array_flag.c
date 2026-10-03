/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef signed short s16;
typedef unsigned char u8;
typedef int s32;
extern s32 D_801174B4;
extern s8 D_80152744;
extern u8 D_8014A250[];
extern s8 D_8011EAE4;
extern s16 D_801525F0[];
extern void func_800D5E64(void *);
void func_800D60AC(void)
{
    s32 i;
    u8 *entry;
    if((D_801174B4 & 8) || (D_801174B4 & 0x007C0000)) {
        for(i=0,entry=D_8014A250;i<D_80152744;i++,entry+=2056) {
            func_800D5E64(entry);
        }
    }
    if(!(D_801174B4 & 8)) D_8011EAE4=1;
    D_801525F0[0]=1;
}
