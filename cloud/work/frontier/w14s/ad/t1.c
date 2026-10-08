/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Tgt { u8 pad[8]; s32 f8; u8 pad2[0x2C - 12]; u8 **blk; } Tgt;
typedef struct Rec { u8 b0; u8 idx; u8 pad2[0x3E]; u16 f40; u8 pad3[6]; Tgt **tgt; } Rec;
typedef struct Car { u8 pad[239]; u8 flag; f32 inc; u8 pad2[264 - 244]; f32 dist; u8 pad3[952 - 268]; } Car;
extern s16 active_player_count;
extern s8 D_8014978C;
extern s8 D_80152570;
extern Rec D_8014A118[];
extern void *D_80146150[];
extern u8 D_80144018[];
extern u8 D_80150F88[];
extern Car D_80152818[];
extern u8 D_80151690[];
extern u8 D_80151AC0[];
extern f32 D_80149A78[];
void func_800CD8EC(void **arg0, u8 arg1);

void assign_drones(void)
{
    /*@{d1*/s32 i; s32 s6; s32 s8;/*@| s32 s8; s32 s6; s32 i; @}*/

    s6 = D_8014978C;
    s8 = D_8014978C;
    if (D_80152570 != 0)
    {
        /*@{d2*/s6 = D_8014978C + 6;/*@| s6 = s6 + 6; @}*/
        s8 = D_8014978C + 19;
    }
    for (i = 0; i < active_player_count; i++)
    {
        /*@{d3*/s32 sb; s32 pass;/*@| s32 pass; s32 sb; @}*/
        sb = (u8) s8;
        if (D_8014A118[i].tgt == 0)
        {
            D_8014A118[i].tgt = (Tgt **) &D_80146150[D_8014A118[i].idx];
        }
        if ((*D_8014A118[i].tgt)->blk == 0)
        {
            return;
        }
        for (pass = 0; pass < 2; pass++)
        {
            u8 *p;
            Car *cp;
            s32 k;
            if (pass == 0)
            {
                p = *((*D_8014A118[i].tgt)->blk) + /*@{m1*/96 * s6/*@| s6 * 96 @}*/ + 140;
            }
            else
            {
                p = D_80150F88 + 96 * s6;
            }
            /*@{c1*/cp = &D_80152818[D_8014A118[i].b0];/*@| cp = (Car *) (D_80152818 + D_8014A118[i].b0); @}*/
            if (cp->flag != 0)
            {
                for (k = 0; k < 5; k++)
                {
                    f32 cur;
                    cur = *((f32 *) (p + 0x2C + 4 * k));
                    if (cur == 0.0f || cp->inc < cur)
                    {
                        s32 j;
                        for (j = 4; j > k; j--)
                        {
                            *((f32 *) (p + 0x2C + 4 * j)) = *((f32 *) (p + 0x2C + 4 * (j - 1)));
                            if (pass == 1)
                            {
                                D_80151AC0[10 + j] = D_80151AC0[9 + j];
                                *((s32 *) (D_80151690 + /*@{m2*/60 * s6/*@| s6 * 60 @}*/ + 40 + 4 * j)) = *((s32 *) (D_80151690 + 60 * s6 + 36 + 4 * j));
                            }
                        }
                        *((f32 *) (p + 0x2C + 4 * k)) = cp->inc;
                        if (pass == 1)
                        {
                            D_80151AC0[10 + k] = (u8) i;
                            if ((*D_8014A118[i].tgt)->f8 != 0)
                            {
                                *((s32 *) (D_80151690 + 60 * s6 + 40 + 4 * k)) = (s32) D_8014A118[i].tgt;
                            }
                        }
                        break;
                    }
                }
            }
            *((u16 *) (p + 0x46)) = (u16) (*((u16 *) (p + 0x46)) + 1);
            *((u16 *) (p + 0x54)) = (u16) (*((u16 *) (p + 0x54)) + 1);
            *((u16 *) (p + 0x4E)) = (u16) (*((u16 *) (p + 0x4E)) + D_8014A118[i].f40);
            *((u16 *) (p + 0x44)) = (u16) (*((u16 *) (p + 0x44)) + D_80144018[i]);
            for (k = 0; k < D_80144018[i]; k++)
            {
                *((f32 *) (p + 0x40)) = *((f32 *) (p + 0x40)) + D_80149A78[(i << 3) + k];
            }
            {
                f32 f;
                f = (f32) *((s32 *) (p + 0x58));
                if (*((s32 *) (p + 0x58)) < 0)
                {
                    f = f + 4294967296.0f;
                }
                f = f + cp->dist / 528.0f;
                *((s32 *) (p + 0x58)) = (s32) (u32) f;
            }
        }
        func_800CD8EC((void **) D_8014A118[i].tgt, (u8) sb);
    }
}
