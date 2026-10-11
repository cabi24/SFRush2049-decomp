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

void func_800EC190(s8 mode)
{
    s16 i;
    s16 j;
    s16 index;

    D_8014978C = mode;
    D_801543CA = 6;
    i = 0;
    j = 0;
    /*@{lp*/do {
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
    } while (i < 6);/*@| for (; i < 6; i++) {
        if (i >= 6) {
            D_80153E88[i].flag = 0;
            D_80153E88[i].value = i + 6;
        } else {
            D_80153E88[i].ordinal = j++;
            D_80153E88[i].value = 0;
            D_80153E88[i].flag = 176;
            D_80153E88[i].mode = D_8014978C;
        }
    } @| while (i < 6) {
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
    /*@{ix*/index = D_801163F4 + 1;
    if (index >= 6) {
        index = 0;
    }/*@| index = D_801163F4 + 1;
    if (index >= 6) index = 0; @}*/
    D_8014A118 = index;
    D_80150C04 = index;
    D_80150F14 = 2;
    D_801163F4 = index;
}
