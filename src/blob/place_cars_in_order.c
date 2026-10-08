/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* place_cars_in_order (0x800F54C0): per active player, adds the car's dist/528 to the u32 sort key at +0x58 of
 * the player's two 96-byte records (the D_8014A118[i].tgt block, then D_80150F88), then calls func_800CD8EC
 * like its sibling graphics_chunk. Not the arcade place_cars_in_order (which sorts by place).
 * Lanes w14c/w14i/w14m built the walk (named `cp` car base, (u8) call argument); the last 8 words were the
 * FP temp ring: the sum and the float->u32 conversion are one expression (no named f32 local), so ugen's ring
 * assigns $f18/$f4 as in retail. No shaping devices. */
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
void func_800CD8EC(void **arg0, u8 arg1);

void place_cars_in_order(void)
{
    s32 i;
    s32 s6;
    s32 s7;

    s6 = D_8014978C;
    s7 = D_8014978C;
    if (D_80152570)
    {
        s6 += 6;
        s7 += 19;
    }
    for (i = 0; i < active_player_count; i++)
    {
        s32 pass;
        Car *cp;
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
            if (pass == 0)
            {
                p = *((*D_8014A118[i].tgt)->blk) + s6 * 96 + 140;
            }
            else
            {
                p = D_80150F88 + s6 * 96;
            }
            cp = &D_80152818[D_8014A118[i].b0];
            *((u32 *) (p + 88)) = (u32) ((f32) *((u32 *) (p + 88)) + cp->dist / 528.0f);
        }
        func_800CD8EC((void **) D_8014A118[i].tgt, (u8) s7);
    }
}
