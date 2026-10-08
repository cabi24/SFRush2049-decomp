/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Tgt { u8 pad[0x2C]; u8 **blk; } Tgt;
typedef struct Rec { u8 b0; u8 idx; u8 pad2[0x3E]; s16 f40; u8 pad3[6]; Tgt **tgt; } Rec;
extern s16 active_player_count;
extern s8 D_8014978C;
extern s8 D_80152570;
extern Rec D_8014A118[];
extern void *D_80146150[];
extern u8 D_80150F88[];
extern u8 D_80152818[];
extern u8 D_80151690[];
extern u8 D_80151AC0[];
extern s8 D_80144018;
extern f32 D_80149A78[];
void func_800CD8EC(void **arg0, u8 arg1);

void assign_drones(void)
{
    s32 i;
    s32 s6;
    s32 s7;

    s6 = D_8014978C;
    s7 = 0;
    if (D_80152570 != 0)
    {
        s6 = D_8014978C + 6;
    }
    for (i = 0; i < active_player_count; i++)
    {
        s32 pass;
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
            s32 t0;
            s32 k;
            u8 *car;
            if (pass == 0)
            {
                p = *((*D_8014A118[i].tgt)->blk) + 96 * s6 + 140;
            }
            else
            {
                p = D_80150F88 + 96 * s6;
            }
            car = D_80152818 + 952 * D_8014A118[i].b0;
            t0 = 0;
            if (car[0xEF] != 0)
            {
                for (k = 0; k < 5; k++)
                {
                    f32 cur;
                    f32 incoming;
                    cur = *((f32 *) (p + 0x2C + 4 * k));
                    incoming = *((f32 *) (car + 0xF0));
                    if (cur == 0.0f || incoming < cur)
                    {
                        s32 j;
                        for (j = 4; j > k; j--)
                        {
                            *((f32 *) (p + 0x2C + 4 * j)) = *((f32 *) (p + 0x2C + 4 * (j - 1)));
                        }
                        *((f32 *) (p + 0x2C + 4 * k)) = incoming;
                        break;
                    }
                }
            }
            *((u16 *) (p + 0x46)) = (u16) (*((u16 *) (p + 0x46)) + 1);
            *((u16 *) (p + 0x54)) = (u16) (*((u16 *) (p + 0x54)) + 1);
            *((u16 *) (p + 0x4E)) = (u16) (*((u16 *) (p + 0x4E)) + D_8014A118[i].f40);
            *((u16 *) (p + 0x44)) = (u16) (*((u16 *) (p + 0x44)) + D_80144018);
            for (k = 0; k < D_80144018; k++)
            {
                *((f32 *) (p + 0x40)) = *((f32 *) (p + 0x40)) + D_80149A78[(D_80144018 << 3) + k];
            }
            {
                s32 key;
                f32 f;
                key = *((s32 *) (p + 0x58));
                f = (f32) key;
                if (key < 0)
                {
                    f = f + 4294967296.0f;
                }
                f = f + *((f32 *) (car + 264)) / 528.0f;
                *((s32 *) (p + 0x58)) = (s32) (u32) f;
            }
        }
        func_800CD8EC((void **) D_8014A118[i].tgt, (u8) s7);
        s7 += 1;
    }
}
