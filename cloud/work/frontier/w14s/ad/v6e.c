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
    s32 i;
    s32 s6;
    s32 s8;
    Rec *rp;

    s6 = D_8014978C;
    s8 = D_8014978C;
    if (D_80152570 != 0)
    {
        s6 = D_8014978C + 6;
        s8 = D_8014978C + 19;
    }
    rp = D_8014A118;
    for (i = 0; i < active_player_count; i++, rp++)
    {
        s32 sb;
        s32 pass;
        s32 k;
        u8 *cnt;
        sb = (u8) s8;
        cnt = D_80144018 + i;
        if (rp->tgt == 0)
        {
            rp->tgt = (Tgt **) &D_80146150[rp->idx];
        }
        if ((*rp->tgt)->blk == 0)
        {
            return;
        }
        for (pass = 0; pass < 2; pass++)
        {
            u8 *p;
            Car *cp;
            f32 *fq;
            if (pass == 0)
            {
                p = *((*rp->tgt)->blk) + 96 * s6 + 140;
            }
            else
            {
                p = D_80150F88 + 96 * s6;
            }
            cp = &D_80152818[rp->b0];
            fq = (f32 *) (p + 0x2C);
            if (cp->flag != 0)
            {
                for (k = 0; k < 5; k++)
                {
                    if (fq[k] == 0.0f || cp->inc < fq[k])
                    {
                        s32 j;
                        for (j = 4; j > k; j--)
                        {
                            fq[j] = fq[j - 1];
                            if (pass == 1)
                            {
                                D_80151AC0[10 + j] = D_80151AC0[9 + j];
                                *((s32 *) (D_80151690 + 60 * s6 + 40 + 4 * j)) = *((s32 *) (D_80151690 + 60 * s6 + 36 + 4 * j));
                            }
                        }
                        fq[k] = cp->inc;
                        if (pass == 1)
                        {
                            D_80151AC0[10 + k] = (u8) i;
                            if ((*rp->tgt)->f8 != 0)
                            {
                                *((s32 *) (D_80151690 + 60 * s6 + 40 + 4 * k)) = (s32) rp->tgt;
                            }
                        }
                        break;
                    }
                }
            }
            *((u16 *) (p + 0x46)) = (u16) (*((u16 *) (p + 0x46)) + 1);
            *((u16 *) (p + 0x54)) = (u16) (*((u16 *) (p + 0x54)) + 1);
            *((u16 *) (p + 0x4E)) = (u16) (*((u16 *) (p + 0x4E)) + rp->f40);
            *((u16 *) (p + 0x44)) = (u16) (*((u16 *) (p + 0x44)) + *cnt);
            for (k = 0; k < *cnt; k++)
            {
                *((f32 *) (p + 0x40)) = *((f32 *) (p + 0x40)) + D_80149A78[(i << 3) + k];
            }
            *((u32 *) (p + 0x58)) = (u32) ((f32) *((u32 *) (p + 0x58)) + cp->dist / 528.0f);
        }
        func_800CD8EC((void **) rp->tgt, (u8) sb);
    }
}
