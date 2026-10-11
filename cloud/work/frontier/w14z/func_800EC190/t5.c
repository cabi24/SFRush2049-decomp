/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;

typedef struct {
    s8 mode;        /* 0x00 */
    u8 pad1[4];     /* 0x01-0x04 */
    u8 ordinal;     /* 0x05 */
    u8 flag;        /* 0x06 */
    u8 value;       /* 0x07 */
} Entry;

extern s8 D_8014978C;
extern s16 D_801543CA;
extern Entry D_80153E88[];
extern s16 D_801163F4;
extern s8 D_8014A118;
extern s8 D_80150C04;
extern s8 D_80150F14;

void func_800EC190(/*@{pt*/s8 mode/*@| s32 mode @}*/)
{
    /*@{dl*/s16 i;
    s16 j;
    s16 index;/*@| s16 j;
    s16 i;
    s16 index; @}*/

    /*@{so*/D_8014978C = mode;/*@|@}*/
    /*@{ln*/D_801543CA = 6;/*@| *(s16 *)((u32)&D_801543CA) = 6; @}*/
    /*@{so*//*@| D_8014978C = mode; @}*/
    /*@{f2*//*@| D_80150F14 = 2; @}*/
    /*@{ij*/i = 0;
    j = 0;/*@| j = 0;
    i = 0; @}*/
/*@{lp*/    do {
        if (i >= 6) {
            D_80153E88[i].flag = 0;
            D_80153E88[i].value = i + 6;
        } else {
            D_80153E88[i].ordinal = j++;
            D_80153E88[i].value = 0;
            D_80153E88[i].flag = 176;
            D_80153E88[i].mode = D_8014978C;
        }
        i++;
    } while (i < 6);/*@|     for (; i < 6; i++) {
        if (i >= 6) {
            D_80153E88[i].flag = 0;
            D_80153E88[i].value = i + 6;
        } else {
            D_80153E88[i].ordinal = j++;
            D_80153E88[i].value = 0;
            D_80153E88[i].flag = 176;
            D_80153E88[i].mode = D_8014978C;
        }
    } @|     while (i < 6) {
        if (i >= 6) {
            D_80153E88[i].flag = 0;
            D_80153E88[i].value = i + 6;
        } else {
            D_80153E88[i].ordinal = j++;
            D_80153E88[i].value = 0;
            D_80153E88[i].flag = 176;
            D_80153E88[i].mode = D_8014978C;
        }
        i++;
    } @}*/
    /*@{ix*/index = D_801163F4 + 1;/*@| index = *(s16 *)((u32)&D_801163F4) + 1; @}*/
    /*@{ix*/
    if (index >= 6) {
        index = 0;
    }/*@| if (index >= 6) index = 0; @}*/
    /*@{ln2*/D_8014A118 = index;/*@| *(s8 *)((u32)&D_8014A118) = index; @}*/
    D_80150C04 = index;
    /*@{f2*/D_80150F14 = 2;/*@| @}*/
    /*@{ln3*/D_801163F4 = index;/*@| *(s16 *)((u32)&D_801163F4) = index; @}*/
}
