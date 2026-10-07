/* flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul */
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
void func_800F45F8(u8 *values, float time)
{
    Session76 *session;
    u8 *data;
    int count, i, j, bit, best, first, second, difference;
    data = input_rec0[D_801543D4].handle->object->resource->data;
    count = 6 - (active_player_count == 1 ? 0 : 2);
    if (D_801543D4 > 0) {
        values[0] ^= values[1];
        values[1] ^= values[0];
        values[0] ^= values[1];
    }
    for (i = 0; i < count; i++) {
        bit = D_80154628 * 3;
        for (j = 0; j < 3; j++) {
            data[1800 + i * 9 + (bit >> 3)] =
                (data[1800 + i * 9 + (bit >> 3)] & ~(1 << (bit & 7))) |
                ((values[i] & 1) << (bit & 7));
            values[i] >>= 1;
            bit++;
        }
    }
    session = (Session76 *)(data + 1780);
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
        if ((best == 0 && session->level < 5 && difference >= 20) ||
            (best == 0 && session->level < 4 && difference >= 6) ||
            (best < 2 && session->level < 3) ||
            (best < 3 && session->level < 2) ||
            (best < 4 && session->level <= 0)) session->level++;
    }
    object_data_allocate(input_rec0[D_801543D4].handle);
}
