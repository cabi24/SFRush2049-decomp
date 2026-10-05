/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Research NONMATCH: reset an inverse 32-entry byte permutation once, then
 * configure a bounded bit-pack buffer. Parameters use native u32 wraparound;
 * the computed byte count narrows through its genuine u16 global. The signed
 * local preserves the observed signed bounds/zero tests. No arcade donor is
 * established. The pointer comparisons intentionally use the N64 u32 domain.
 */
typedef signed int s32;
typedef unsigned int u32;
typedef unsigned short u16;
typedef unsigned char u8;
extern s32 D_801170F8;
extern u8 *D_80116FE4;
extern u8 D_80116FE8[], D_8012E618[];
extern u16 D_801170E8, D_801170EC, D_801170F0, D_801170F4;
s32 func_800DC628(u32 arg0,u32 arg1) {
    s32 i=0;
    u8 *src;
    s32 size;
    u8 *p;
    if(D_801170F8) {
        D_801170F8=0;
        src=D_80116FE4;
        do {
            D_80116FE8[*src++]=i++;
        }while(i!=32);
    }
    size=D_801170E8=(arg0+arg1+7)>>3;
    if(size>=33)return 0;
    if(arg1>=33)return 0;
    if(size>0) {
        p=D_8012E618;
        do { *p++=0; }while((u32)p<(u32)(D_8012E618+size));
    }
    D_801170EC=arg0;
    D_801170F0=arg1;
    D_801170F4=0;
    return 1;
}
