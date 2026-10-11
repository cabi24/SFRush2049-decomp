/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * func_800F45F8: end-of-round result record for the current player (D_801543D4), N64-only (no arcade
 * ancestor).  `values` holds one byte per car (6, or 4 when active_player_count != 1); for player 2
 * (D_801543D4 > 0) the first two entries are swapped.  Each value's low three bits are packed into the
 * player's session record (save data + 1780: 9 bytes per car at +20, bit position D_80154628 * 3); then
 * score += time, length++ (rounds), net_session_update(), D_80154628--, and func_800F43B8() (end-of-race
 * statistics) once length reaches D_80154640.  In mode 3 the session level (+7) is raised when the best
 * place (D_80154450[0/1].place, min over both players in two-player) and the points lead of first over
 * second place (D_80154450[].score, Ranking76 stride 76) are good enough for the current level.
 * Finally object_data_allocate(player handle).
 * score.py fn: MATCH; whole-program unit: EQUAL, 0 locked bodies differ (w16h).
 * Base: B80 native draft (cloud/work/near_miss_B80/func_800F45F8_native.c, 113/225).
 *
 * Shaping / disclosures:
 *  - the xor swap is one source line (as a SWAP macro): with three lines as1 orders the three byte
 *    stores [0],[1],[0]; on one line it emits retail's [0],[0],[1] (as1 uses the line number).
 *  - `data` and `session = data + 1780` are both locals: retail keeps the record base unfolded
 *    (`addiu a3,t5,1780` for the loop, `addiu t2,t5,1780` after it) instead of folding 1780 into offsets.
 *  - `level` is read once into a local (retail loads session->level once for the whole condition).
 *  - `first`/`second` are uninitialised on paths where no car holds place 0/1 (retail reads their stack
 *    homes there); declaration order fixes those homes (48/44).  No unused locals.
 */
typedef unsigned char u8;
typedef signed char s8;
typedef unsigned short u16;
typedef signed short s16;
typedef struct Resource { u8 *data; } Resource;
typedef struct Object { u8 opaque[44]; Resource *resource; } Object;
typedef struct Handle { Object *object; } Handle;
typedef struct Player76 { u8 index,selector; u8 opaque[70]; Handle *handle; } Player76;
typedef struct Session76 {
    u8 prefix[5], option, enabled, level;
    s8 mode, length;
    u8 gap[2];
    unsigned int seed;
    float score;
    u8 packed[56];
} Session76;
typedef struct Ranking76 { u8 index,place; u16 score; u8 rest[72]; } Ranking76;
extern Player76 input_rec0[];
extern Ranking76 D_80154450[];
extern u8 D_801543D4;
extern s8 D_80154628, D_80154640;
extern s16 active_player_count;
extern void net_session_update(void),func_800F43B8(void),object_data_allocate(Handle *);
#define SWAP(a, b) { (a) ^= (b); (b) ^= (a); (a) ^= (b); }

void func_800F45F8(u8 *values, float time)
{
    u8 *data;
    Session76 *session;
    int count, i, j, bit, best;
    int first, second, difference, level;

    data = input_rec0[D_801543D4].handle->object->resource->data;
    session = (Session76 *)(data + 1780);
    count = 6 - (active_player_count == 1 ? 0 : 2);
    if (D_801543D4 > 0) {
        SWAP(values[0], values[1]);
    }
    for (i = 0; i < count; i++) {
        bit = D_80154628 * 3;
        for (j = 0; j < 3; j++) {
            session->packed[i * 9 + (bit >> 3)] =
                (session->packed[i * 9 + (bit >> 3)] & ~(1 << (bit & 7))) |
                ((values[i] & 1) << (bit & 7));
            values[i] >>= 1;
            bit++;
        }
    }
    session->score += time;
    session->length++;
    net_session_update();
    D_80154628--;
    if (session->length >= D_80154640) func_800F43B8();
    if (session->mode == 3) {
        if (active_player_count == 1) best = D_80154450[0].place;
        else best = D_80154450[0].place < D_80154450[1].place ? D_80154450[0].place : D_80154450[1].place;
        for (i = 0; i < 6; i++) {
            if (D_80154450[i].place == 0) first = i;
            else if (D_80154450[i].place == 1) second = i;
        }
        difference = D_80154450[first].score - D_80154450[second].score;
        level = session->level;
        if ((best == 0 && level < 5 && difference >= 20) ||
            (best == 0 && level < 4 && difference >= 6) ||
            (best < 2 && level < 3) ||
            (best < 3 && level < 2) ||
            (best < 4 && level <= 0)) session->level = level + 1;
    }
    object_data_allocate(input_rec0[D_801543D4].handle);
}
