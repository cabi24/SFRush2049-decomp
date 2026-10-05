typedef signed char s8; typedef unsigned char u8; typedef short s16; typedef unsigned short u16;
typedef int s32; typedef unsigned int u32;

typedef struct {
    s8 active;      /* 0 */
    u8 pad1[3];
    u8 flag;        /* 4 */
    u8 pad5[19];
} Entry;            /* 24 */

extern Entry *D_80144C48;
extern s32 D_801460F4;
extern u8 D_8011028C;

void func_8009591C(u8 *flag)
{
    s32 i;

    for (i = 0; i < D_801460F4; i++) {
        if (D_80144C48[i].active) {
            D_80144C48[i].flag = 1;
        }
    }
    *flag = 1;
}

void func_800958B8(void)
{
    func_8009591C(&D_8011028C);
}
