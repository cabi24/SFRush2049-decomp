/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Tgt { u8 pad[0x2C]; u8 **blk; } Tgt;
typedef struct Rec { u8 b0; u8 idx; u8 pad2[0x46]; Tgt **tgt; } Rec;
typedef struct Car { u8 pad[264]; f32 dist; u8 pad2[952 - 268]; } Car;
extern s16 active_player_count;
extern s8 D_8014978C;
extern s8 D_80152570;
extern Rec D_8014A118[];
extern void *D_80146150[];
extern u8 D_80150F88[];
extern Car D_80152818[];
void func_800CD8EC(void **arg0, s32 arg1);

void place_cars_in_order(void)
{
    /*@{d1*/s32 i; s32 s6; s32 s7;/*@| s32 s7; s32 s6; s32 i; @}*/

    s6 = D_8014978C;
    s7 = D_8014978C;
    if (/*@{c1*/D_80152570 != 0/*@| D_80152570 @}*/)
    {
        s6 += 6;
        s7 += 19;
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
            f32 f;
            if (pass == 0)
            {
                p = *((*D_8014A118[i].tgt)->blk) + /*@{m1*/96 * s6/*@| s6 * 96 @}*/ + 140;
            }
            else
            {
                p = D_80150F88 + /*@{m1*/96 * s6/*@| s6 * 96 @}*/;
            }
            /*@{K*/
            {
                f32 q;
                q = D_80152818[D_8014A118[i].b0].dist / 528.0f;
                f = q + (f32) (u32) *((s32 *) (p + 88));
            }
            /*@| 
            {
                s32 key;
                f32 g;
                key = *((s32 *) (p + 88));
                g = (f32) key;
                if (key < 0)
                {
                    g = g + 4294967296.0f;
                }
                f = g + D_80152818[D_8014A118[i].b0].dist / 528.0f;
            }
            @| 
            {
                s32 key;
                key = *((s32 *) (p + 88));
                if (key < 0)
                {
                    f = (f32) key + 4294967296.0f;
                }
                else
                {
                    f = (f32) key;
                }
                f = f + D_80152818[D_8014A118[i].b0].dist / 528.0f;
            }
            @| 
            {
                f = D_80152818[D_8014A118[i].b0].dist / 528.0f + (f32) (u32) *((s32 *) (p + 88));
            }
            @| 
            {
                f32 q;
                q = (f32) (u32) *((s32 *) (p + 88));
                f = q + D_80152818[D_8014A118[i].b0].dist / 528.0f;
            }
            @| 
            {
                u32 uk;
                uk = *((u32 *) (p + 88));
                f = (f32) uk + D_80152818[D_8014A118[i].b0].dist / 528.0f;
            }
            @}*/
            *((s32 *) (p + 88)) = (s32) (u32) f;
        }
        func_800CD8EC((void **) D_8014A118[i].tgt, (u8) s7);
    }
}
