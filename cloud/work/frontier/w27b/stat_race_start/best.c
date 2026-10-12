/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/*
 * stat_race_start (0x800FE080, 1,068 B) -- historical label. Decodes a packed
 * 70-bit setup code from a byte string into the current player's session record:
 * championship_standings(input) loads the string into the bit reader
 * (func_800DC628 sets the 86-bit/14-char geometry), func_800DC57C(&v, n) reads
 * n bits. Fields: mode (3), option (1), length (5), seed (20), level (3) and six
 * 9-bit values. Validity: length in [1, 4*D_80111784[mode]), the largest value
 * must not exceed D_8011176C[0]*length, and the values must sum to
 * length * sum(D_8011176C[0..5]). On success the fields are stored in the session
 * record (player handle -> object -> resource data + 1780), the length and the six
 * values (as s16) in the per-selector tables D_80155140 / D_80155148, then
 * net_session_update(). Returns 1, or 0 for a rejected code.
 *
 * N64-only (no arcade counterpart found). Shaping notes (no devices):
 *  - declaration order sets the frame: total_weights/total_values sit between
 *    values[] and ranks[], and i/j before session (session's spill home 64(sp)).
 *  - the first sum's init is in the for-init (`for (total_weights = 0, i = 0; ...)`):
 *    a separate `total_weights = 0;` statement schedules `move a0,zero` ahead of the
 *    strength-reduced pointer setup (3 words).
 * Prior drafts: cloud/work/near_miss_B73 (130/267).
 */
typedef unsigned char u8;
typedef signed short s16;
typedef unsigned int u32;
typedef signed int s32;

typedef struct Resource { u8 *data; } Resource;
typedef struct Object { u8 pad0[44]; Resource *resource; } Object;
typedef struct Handle { Object *object; } Handle;
typedef struct Player76 { u8 index, selector; u8 pad2[70]; Handle *handle; } Player76;
typedef struct Session76 {
    u8 pad0[5], option, enabled, level, mode, length;
    u8 pad10[2];
    u32 seed;
} Session76;

extern Player76 input_rec0[];
extern u8 D_801543D4;
extern u8 D_80155140[];
extern s16 D_80155148[][6];
extern u32 D_80111784[];
extern u32 D_8011176C[6];

extern s32 championship_standings(u8 *input);
extern s32 func_800DC628(u32 a, u32 b);
extern s32 func_800DC57C(u32 *out, u32 bits);
extern void func_800FDF88(s32 mode);
extern void net_session_update(void);

s32 stat_race_start(u8 *input)
{
    u32 mode, option, length, seed, level;
    u32 values[6];
    u32 total_weights, total_values;
    u32 ranks[6];
    s32 i, j;
    Session76 *session;

    session = (Session76 *)(input_rec0[D_801543D4].handle->object->resource->data + 1780);
    func_800DC628(86, 14);
    if (!championship_standings(input)) return 0;
    func_800DC57C(&mode, 3);
    func_800DC57C(&option, 1);
    func_800DC57C(&length, 5);
    func_800DC57C(&seed, 20);
    func_800DC57C(&level, 3);
    for (i = 0; i < 6; i++) func_800DC57C(&values[i], 9);
    if (length == 0 || length >= D_80111784[mode] * 4) return 0;
    for (i = 0; i < 6; i++) ranks[i] = i;
    for (i = 0; i < 5; i++) {
        for (j = i + 1; j < 6; j++) {
            if (values[ranks[i]] < values[ranks[j]]) {
                ranks[i] ^= ranks[j];
                ranks[j] ^= ranks[i];
                ranks[i] ^= ranks[j];
            }
        }
    }
    if (D_8011176C[0] * length < values[ranks[0]]) return 0;
    for (total_weights = 0, i = 0; i < 6; i++) total_weights += D_8011176C[i];
    total_values = 0;
    for (i = 0; i < 6; i++) total_values += values[ranks[i]];
    if (total_values != total_weights * length) return 0;
    func_800FDF88(mode);
    session->enabled = 1;
    session->option = option;
    session->length = length;
    D_80155140[input_rec0[D_801543D4].selector] = length;
    session->seed = seed;
    session->level = level;
    for (i = 0; i < 6; i++) D_80155148[input_rec0[D_801543D4].selector][i] = values[i];
    net_session_update();
    return 1;
}
