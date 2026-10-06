/* flags: -g0 -O3 -mips2 -G 0 -non_shared */
/* Complete research reconstruction, not a matching claim.
 * race_countdown_display [0x800D24C8,0x800D2928): N64 checkpoint crossing.
 * Arcade ancestry: historicalsource/rushtherock game/checkpoint.c:PassedCP
 * and get_next_checkpoint (revision 845329d7b36f5a384c5625ed9a0aef584ab46139).
 * N64 uses the supplied interpolated crossing time, per-player state, lap
 * timing and finish events instead of the arcade network reporting path.
 * Field names below describe observed accesses; original typedefs are unknown.
 * Intended flags: -g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul.
 */
typedef signed char s8;
typedef unsigned char u8;
typedef signed short s16;
typedef int s32;
typedef unsigned int u32;
typedef float f32;

/* Accessed 0x7EC prefix of the authentic 0x808 model record. This function
 * receives one pointer; it never uses sizeof(Model) or model-array indexing. */
typedef struct Model {
    u8 pad000[0x7C6];
    s16 slot;
    u8 pad7C8[0x7D4 - 0x7C8];
    u32 flags;
    u8 pad7D8[0x7DC - 0x7D8];
    s8 lap_enabled;
    u8 pad7DD[0x7E2 - 0x7DD];
    s16 last_cp;
    s16 next_cp;
    u8 pad7E6[2];
    s8 laps;
    u8 pad7E9;
    s8 finish_a;
    s8 finish_b;
} Model;

typedef struct GameCar {
    u8 pad000[0xEE];
    s8 place;
    s8 place_locked;
    u8 pad0F0[0x308 - 0xF0];
    s8 finish_a;
    s8 finish_b;
    u8 pad30A[2];
    f32 finish_timer;
    u8 pad310;
    s8 flags311;
    u8 pad312[0x3B8 - 0x312];
} GameCar;

typedef struct Player {
    u8 pad0[7];
    u8 type;
} Player;

/* Accessed ten-byte prefix of the checkpoint header; no array indexing. */
typedef struct Track {
    s16 unknown0;
    s16 loop_start;
    s16 finish_line;
    s16 before_finish;
    s16 count;
} Track;

extern Track D_80151CE8;
extern Player D_80153E88[];
extern GameCar D_80152818[];
extern s32 D_8014A110;
extern u32 D_801174B4;
extern s16 D_80152734;
extern s8 D_80152014;
extern s8 D_80152015;
extern s8 D_80110680[];
extern f32 D_80110668[];
extern volatile f32 D_8002EB90;
void func_800D2458(s32 player, f32 time);
void func_800D2054(s32 player, f32 time);
void car_setup_confirm(s32 player, f32 time);
__inline s32 func_800B61A8(s32 code, s32 player, s32 value, u8 mode);
u32 car_stats_display(s32 type, f32 delay, s32 callback, s32 data,
                      s32 nargs, ...);

/* Genuine next-checkpoint operation from the arcade donor. N64 wraps on
 * equality; native evaluates the next value separately in the return arm. */
static s32 get_next_checkpoint(s32 current)
{
    if (current + 1 == D_80151CE8.count) {
        return D_80151CE8.loop_start;
    }
    return current + 1;
}

void race_countdown_display(Model *m, f32 time)
{
    s32 old_remaining;
    s32 player;
    Player *link;
    GameCar *gc;
    s32 completed_lap;

    completed_lap = 0;
    player = m->slot;
    m->last_cp = m->next_cp;
    m->next_cp = get_next_checkpoint(m->last_cp);
    if (m->last_cp == D_80151CE8.finish_line && m->lap_enabled &&
        D_8014A110 != 1 && !(D_801174B4 & 8)) {
        m->laps++;
        completed_lap = 1;
    }
    if (!m->lap_enabled && m->last_cp == D_80151CE8.before_finish) {
        m->lap_enabled = 1;
    }
    link = &D_80153E88[player];
    if (link->type == 6) {
        if (m->last_cp == D_80151CE8.finish_line && !(D_801174B4 & 8)) {
            func_800D2458(player, time);
        }
        D_80152015 = m->next_cp == D_80151CE8.finish_line &&
            m->laps + 1 == D_80152734;
    }
    if (completed_lap) {
        func_800D2054(m->slot, time);
        if (m->laps == D_80152734 && !D_80152818[player].place_locked) {
            car_setup_confirm(m->slot, time);
        }
        if (link->type == 6 && (D_8014A110 != 2 || player == 0)) {
            old_remaining = D_80152014;
            D_80152014 = D_80152014 < D_80152734 - m->laps ?
                D_80152014 : D_80152734 - m->laps;
            if (m->laps == D_80152734 && D_80152818[player].place_locked != -1) {
                gc = &D_80152818[player];
                D_80110680[player] = 1;
                D_80110668[player] = D_8002EB90;
                switch (gc->place) {
                case 0:
                    func_800B61A8(64, player, 1, 1);
                    break;
                case 1:
                    func_800B61A8(68, player, 1, 1);
                    break;
                case 2:
                    func_800B61A8(67, player, 1, 1);
                    break;
                default:
                    func_800B61A8(65, player, 1, 1);
                    break;
                }
                gc->finish_a = 1;
                gc->finish_b = 1;
                gc->flags311 |= 1;
                gc->finish_timer = 0.0f;
                m->finish_a = 1;
                m->finish_b = 1;
                m->flags &= ~8;
            } else if (old_remaining != D_80152014) if (D_80152014 == 1) func_800B61A8(74, player, 1, 1); else car_stats_display(0, 2.0f, 1, 0, 2, D_80152014 + 79, 80);
        }
    }
}
extern signed char D_8010FFC0;
s32 entity_flags_apply(s32, s32, s32, u8);
__inline s32 func_800B61A8(s32 a0, s32 a1, s32 a2, u8 a3)
{
  if (D_8010FFC0 == 0) {
    return -1;
  }
  if (a0 == -1) {
    return -1;
  }
  return entity_flags_apply(a0, a1, a2, a3);
}
