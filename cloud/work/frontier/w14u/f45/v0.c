/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned short u16;
typedef signed int s32;
typedef unsigned int u32;
typedef float f32;
typedef struct Tgt { u8 pad[0x2C]; u8 **blk; } Tgt;
typedef struct Rec { u8 pad0; u8 idx; u8 pad2[0x46]; Tgt **tgt; } Rec;
extern u8 D_801543D4;
extern s16 active_player_count;
extern Rec D_8014A118[];
extern s8 D_80154628;
extern s8 D_80154640;
extern u8 D_80154450[];
extern s8 D_80154451;
extern s8 D_8015449D;
extern void *object_data_allocate_dummy;
void net_session_update(f32 arg0, u8 *arg1, s32 arg2, s32 arg3);
void func_800F43B8(void);
void object_data_allocate(void *arg0);

void func_800F45F8(u8 *arg0, f32 arg1, s32 arg2, s32 arg3)
{
    s32 v1;
    s32 limit;
    s32 i;
    s32 j;
    s32 bit;
    u8 *src;
    u8 *rec9;
    u8 *base;
    u8 *ub;
    u8 *cur;
    s32 a0v;
    s32 n;
    s32 sp30;
    s32 sp2C;

    base = (u8 *) D_8014A118[D_801543D4].tgt[0]->blk[0];
    if (active_player_count == 1) {
        v1 = 0;
    } else {
        v1 = 2;
    }
    limit = 6 - v1;
    if ((s32) D_801543D4 > 0) {
        u8 t;
        t = arg0[0];
        arg0[0] = arg0[1];
        arg0[1] = t;
    }
    i = 0;
    if (limit > 0) {
        src = arg0;
        rec9 = base + 0x6F4;
        do {
            i++;
            j = 0;
            a0v = D_80154628 * 3;
            do {
                bit = a0v & 7;
                cur = rec9 + (a0v >> 3);
                j++;
                cur[0x14] = (u8) (((*src & 1) << bit) | (cur[0x14] & ~(1 << bit)));
                a0v++;
                *src = (u8) (*src >> 1);
            } while (j != 3);
            src++;
            rec9 += 9;
        } while (i != limit);
    }
    ub = base + 0x6F4;
    ub[9] = (s8) (ub[9] + 1);
    *(f32 *) (ub + 16) = *(f32 *) (ub + 16) + arg1;
    net_session_update(arg1, (u8 *) a0v, (s32) src, j);
    D_80154628 = (s8) (D_80154628 - 1);
    if ((s8) ub[9] >= D_80154640) {
        func_800F43B8();
    }
    if ((s8) ub[8] == 3) {
        object_data_allocate(ub);
    }
}
