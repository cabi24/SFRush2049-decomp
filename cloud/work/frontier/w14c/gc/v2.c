/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef struct Tgt { u8 pad[0x2C]; u8 **blk; } Tgt;
typedef struct Rec { u8 pad0; u8 idx; u8 pad2[0x46]; Tgt **tgt; } Rec;
typedef struct Acc { u16 w0; u16 w2; u16 w4; u16 w6; u16 w8; u16 wA; } Acc;
extern s16 active_player_count;
extern s8 D_8014978C;
extern Rec D_8014A118[];
extern void *D_80146150[];
extern s8 D_80143F54;
extern s8 D_8012E67C[];
extern s8 D_80150B68[];
extern s8 D_80149428[];
extern Acc D_80151578[];
void func_800CD8EC(void **arg0, s32 arg1);

void graphics_chunk(void)
{
    s32 i;
    s32 pass;
    s32 k;
    s32 t2;
    Acc *a2;

    t2 = D_8014978C - 6;
    for (i = 0; i < active_player_count; i++)
    {
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
            if (pass == 0)
            {
                a2 = (Acc *) ((u8 *) (*((*D_8014A118[i].tgt)->blk)) + 12 * t2 + 0x60C);
            }
            else
            {
                a2 = (Acc *) ((u8 *) D_80151578 + 12 * t2);
            }
            for (k = 0; k < active_player_count; k++)
            {
                if (k != i)
                {
                    a2->w8 = (u16) (a2->w8 + D_80149428[i * 4 + k]);
                }
                if (D_80149428[k * 4 + i] < 0)
                {
                    a2->wA = (u16) (a2->wA - D_80149428[k * 4 + i]);
                }
                else
                {
                    a2->wA = (u16) (a2->wA + D_80149428[k * 4 + i]);
                }
            }
            a2->w4 = (u16) (a2->w4 + 1);
            if (i == D_80143F54 || D_8012E67C[i] == D_8012E67C[D_80143F54] || D_80150B68[i] == 1)
            {
                a2->w6 = (u16) (a2->w6 + 1);
            }
        }
        func_800CD8EC((void **) D_8014A118[i].tgt, (u8) D_8014978C);
    }
}
