# Frontier wave 1, agent w1h (2026-10-04)

Scorer: `tools/cloud/score.py` in the builder copy `watchman2:~/rush2049/scratch/frontier/w1h`
(`IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a.../ido`). `sc.sh SRC NAME [args]` in this directory copies a
source over and runs `python3 tools/cloud/score.py fn cand/NAME.c NAME [args]`; the commands below are that call.
`grp.sh DIR` runs `score.py group`. `full.sh` / `batch.sh` are the aligned-diff and batch helpers from agentC
(`full.py` here passes `-z` to objdump, which fixes an index error on zero words).
Nothing was committed, spliced or locked. All flags are `-g0 -O3 -mips2 -G 0 -non_shared`.

| # | function | bytes | state |
|---|---|---:|---|
| 1 | audio_queue_process | 676 | **strict MATCH** (one file with its real neighbours; also as a real group) |
| 2 | func_8009D708 | 660 | **strict MATCH** |
| 3 | menu_control_settings | 656 | **strict MATCH** (also at -O2) |
| 4 | func_800D5524 | 628 | **strict MATCH** (needs matched `player_conditional_check` in the file) |
| 5 | display_settings | 608 | code identical, own `.data` unverified (function-local static) |
| 6 | func_800EC914 | 596 | 3/149 words (one `as1` ordering) |
| 7 | func_800F43B8 | 576 | **strict MATCH** |
| + | func_8008A704 | 112 | **strict MATCH** (not assigned; fell out of #1) |
| + | func_800FD41C | 72 | **strict MATCH** (not assigned; probe for #5) |

Strict total: 3,380 bytes (5 assigned = 3,196, plus 184 unassigned).

## 1. audio_queue_process — strict MATCH (and func_8008A704)

```
score.py fn cand/audio_queue_process.c audio_queue_process --flags "-g0 -O3 -mips2 -G 0 -non_shared"
audio_queue_process:
  MATCH
score.py fn cand/audio_queue_process.c func_8008A704 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_8008A704:
  MATCH
score.py fn cand/func_8008A704.c func_8008A704 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_8008A704:
  MATCH
score.py group cand/<copy of groups/rumble_thread>      (via grp.sh)
Members:
func_8008A704:
  MATCH
audio_queue_process:
  MATCH
Context (informational; excluded from exit status):
sync_release_video:
  MISMATCH (2 extra words (nonzero beyond target length))
```
Sources: `cloud/matches/audio_queue_process.c`, `cloud/matches/func_8008A704.c`, group
`cloud/work/frontier/w1h/groups/rumble_thread/` (claims both; `sync_release_video` is context and reads
"2 extra words" only because the two deleted-static stubs follow it in this object).

Semantics: the Rumble Pak thread (N64-only). Takes the SI lock, walks the four 0x304-byte controller records at
`D_80144030` (phase counter +0x80, requested level +0x7D, current level +0x7E, motor state +0x7C, pfs +0x0C), duty
table `D_8011ECEC`, enable flag `D_8011EAE4`. `func_8008A704` is the lazily initialised lock acquire.

What it took (in the order found; the prior best was 117/169, `cloud/work/near_miss_B128`):

1. **The two `jr ra; nop` stubs next to it are deleted statics and both matter.**
   - `func_8008A6FC` (in front of the lock) is the lock's one-time initialisation as a separate static. With it,
     `func_8008A704` matches (it was stuck at 2/28 through ~1,500 variants: `li t7,1` vs `sw ra` order; the
     inlined call puts the `li` in the predecessor block). Inside `audio_queue_process` the nested inline also
     fixes the `sb`/`lui at` order and supplies 8 frame bytes.
   - `func_8008A774` (in front of this function): **inferred** to be the unlock (`osJamMesg` on the lock queue).
     Evidence is indirect: each inlined call costs 8 bytes of the caller's frame (measured: going from 2 to 3 to 4
     inlined calls grew the frame by 8 each time), the frame has room for exactly three (lock, lock-init, one more), and calling
     the matched `sync_release_video` instead gives identical code with a frame 8 bytes too large.
2. `volatile` on record fields +0x7E and +0x80 (retail reloads both; without it two loads are commoned).
3. Block shape: `switch (present) { case 0: skip: continue; }` with every exit of the body as `goto skip`. Retail has
   an empty block after the presence test that all exits join (`bnezl body; b next`). A plain `if (!present)
   continue;` folds it (`near_plain_if_continue.c`, 9 aligned rows). A label alone (`if (...) { skip: continue; }`)
   gives the shape but swaps s3/s4. The on/off decision is in goto form to reproduce retail block order (off block,
   `level < 10` test, on block, duty-table test last).
4. Unused local `Controller *p` supplies 4 frame bytes; operand order `table & (1 << phase)`.
5. **Alignment.** The epilogue after the endless loop is behind `.align 5`, so the nop count depends on the
   function's offset in the object. Retail has `sync_release_video`, stub, lock, stub in front (0xAC bytes from a
   32-byte boundary). umerge emits uncalled roots in reverse definition order, so `sync_release_video` is defined
   last in the file to come first in the object.

Honesty: items 3 and 4 are shaping, not recovered source; item 1's body for `func_8008A774` is a hypothesis that
reproduces the bytes. The stubs are currently locked as empty functions (plan decision D3 applies).

Layout recovered: `Controller` 0x304 = { +0x06 s8 present, +0x0C pfs[0x70], +0x7C s8 motorOn, +0x7D s8 request,
+0x7E volatile s8 level, +0x80 volatile s16 phase }. `D_80154378` retrace queue, `D_801497D0` lock queue,
`D_8011194C` lock-initialised flag, `D_80035458` SI queue, `D_8002E8E8` scheduler.

## 2. func_8009D708 — strict MATCH

```
score.py fn cand/func_8009D708.c func_8009D708 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_8009D708:
  MATCH
```
Source `cloud/matches/func_8009D708.c`. Builds a fixed-point `Mtx` from a 3x3 rotation + position, relative to
`D_80150B70[idx]` (0x98-byte view records, rot at 0, pos at 0x24) unless `absolute`; offsets longer than 1600 are
pulled in to 1600 and the rotation scaled to match. Prior best 155/165 (`near_miss_B29`).
-O3 only (at -O2 `scale` is spilled to its home slot). Quirks: distance reuses `s` (a `dist` local takes `$f20` and a
frame); `s *= 65536` int literal vs `scale * 65536.0f` (two constant webs, as in wave 0); `(s32)(scale * 65536.0f)`
written twice instead of a local (as a local it takes a2 and moves `m` to a3). A FTOFIX-style macro form does not
common the conversions (+42 words).

## 3. menu_control_settings — strict MATCH

```
score.py fn cand/menu_control_settings.c menu_control_settings --flags "-g0 -O3 -mips2 -G 0 -non_shared"
menu_control_settings:
  MATCH
```
(also `MATCH` with the default -O2 flags). Source `cloud/matches/menu_control_settings.c`.
This is arcade **`initroad()`** (`reference/repos/rushtherock/game/road.c:17`), proven by structure: fmatcopy of the
start uvs, `bodtorw(offset)` + `vecadd` into RWR, the EPRWR / base_RWR copies, and the four-tire loop with
suscomp/tpcomp/roadcode/roadboost/sound_flags and the BODYR transform. `func_8009E820` = `bodtorw`,
`math_utility` = 3x3 copy.
The 12/164 residual of `codex_road_context_a140` was operand load order in the six loop adds. No quirk was needed:
vectors as `F32[3]` arrays with the arcade's `vecadd` macro and the arcade's operand order
(`vecadd(TIRERWR[i], RWR, TIRERWR[i])`, `vecadd(initin.pos, temp, RWR)`). With `struct {x,y,z}` the commutative
operands canonicalise the other way and no source reordering reaches it.
MODELDAT offsets recovered (N64): +0x0F4 BODYR[4][3], +0x22C RWR, +0x244 TIRERWR[4][3], +0x274 BODYRWR[4][3],
+0x2A4 copy of BODYRWR, +0x2EC uvs, +0x5CC suscomp[4], +0x5DC tpcomp[4], +0x60C u32[4], +0x61C u16 roadcode[4],
+0x624 u16[4], +0x62C u16[4], +0x6D4 initin.pos, +0x6E0 initin.uvs, +0x704 initin.offset, +0x74C base_RWR,
+0x794 EPRWR, +0x7A0 second uvs, +0x7C6 s16 slot; pointer at +0 to the car record (`D_80152818`, 0x3B8:
+0x08 RWR, +0x2C uvs, +0x70 TIRER[4][3]). Field names above +0x5CC are by position in arcade `initroad`, not proven
individually.

## 4. func_800D5524 — strict MATCH

```
score.py fn cand/func_800D5524.c func_800D5524 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800D5524:
  MATCH
score.py fn cand/func_800D5524.c player_conditional_check --flags "-g0 -O3 -mips2 -G 0 -non_shared"
player_conditional_check:
  MATCH
```
Source `cloud/matches/func_800D5524.c`. Releases every looping sound handle owned by one car.
- The matched neighbour `player_conditional_check(rec, flag)` (0x800D54E0) is **inlined at all six call sites**; it
  must be defined in the file (unchanged from `src/blob`). Hand-expanding the same statements keeps the loop bound
  40 in s7 (`near_hand_expanded_helper.c`, 151 words off); with the helper the bound is `li at,40` as in retail.
  Tell-tale: retail's 56-byte frame has 8 bytes more than its saved registers need.
- Quirk: dead read `if (i) {}` in the first loop. Without it 19 words differ, all register names (counter/pointer
  s1/s2 and m/constant-2 s3/s4 swapped; `near_without_dead_read_19words.c`).
Tables: `D_80140420[slot]` 0x54 = s32 + four 0x14 handle records {s32 handle; f32[4]} (records 0,1,3 used here),
`D_80140640[slot]` 0x14, `D_801406C0[slot][3]`, s32 handles `D_80140AE0/D_801407E0/D_801407C0[slot]`,
`D_80140A08` s16[], `D_80140B10` f32[], flags `D_8010FFC4/D_8010FFCC` s8[]. MODELDAT +0x7C6 s16 slot, +0x7CC s8 mode.

## 5. display_settings — code identical, own .data unverified

```
score.py fn cand/display_settings.c display_settings --flags "-g0 -O3 -mips2 -G 0 -non_shared"
display_settings:
  MATCH (4 section-relative relocations unverified: .data+0x0 at +0x40, .data+0x0 at +0x44, .data+0x0 at +0x94, .data+0x0 at +0x98)
```
Source `cloud/work/frontier/w1h/display_settings/best.c` (not in `cloud/matches`). Every text word equals retail.
The four unverified words are the references to the function-local `static s8 D_80116DB8 = 0;` ("slot table
initialised"). 0x80116DB8 is referenced by no other function in the image and the retail word there is 0. As an
`extern` its address is kept in a register and `mode` leaves a1 (57+ words). Closes with workstream B (own-`.data`
verification), like `func_800EC270`.
Prior best 127/152 (`near_miss_B66`). What mattered, in order:
1. `Slot.busy`/`.enabled` are `s8` like the two flags: all stored 1s and the `mode == 1` test become one constant web (s3).
2. Scheduler frame counter `D_8002E8E8+0x27C` is `volatile` (unfolded `lui/addiu; lw 636(reg)`).
3. Function-local static (above).
4. Local `s8 present` for the controller flag (a uopt web in v0; inline it is a ugen temp, 21 words).
5. **Every slot access is spelled `D_80153FD8[player][j]`.** Through a pointer local (`slot = ...; slot->x`) the
   instructions and registers are identical but 59 words differ in order (`near_pointer_local_59words.c`): ugen emits
   `.noalias reg,$sp` only for a register derived from a global array, and without it `as1` will not move the
   argument-home loads (`lw tN,56..68(sp)`, even `lw ra`) above stores through that register.
Layout: `D_80153FD8[4][2]`, record 0x2C = { s8 busy, s8 enabled, u8, s8 player, u8 a[4], u8 b[4], u8 c[4],
s8 present@0x10, s32 mode@0x14, s16 arg[4]@0x18, u32 deadline@0x28 }. `D_80144030[p].present` is the same +6 byte
as in #1. `D_8002AFB4` f32 (frames per second scale), `D_80116DB4` s8 set when mode == 10.

## 6. func_800EC914 — 3/149

```
score.py fn cand/func_800EC914.c func_800EC914 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800EC914:
    +0x170  want 258c2768 addiu t4,t4,10088             got 3c0d8015 lui t5,0x8015
    +0x174  want 3c0d8015 lui t5,0x8015                 got 25ad3fd2 addiu t5,t5,16338
    +0x178  want 25ad3fd2 addiu t5,t5,16338             got 258c2768 addiu t4,t4,10088
  3/149 words differ
```
Source `cloud/work/frontier/w1h/func_800EC914/best.c`. Arcade **`munge_gLink_data()`**
(`reference/repos/rushtherock/game/mdrive.c:463`), proven by structure (active-car list through `model[n++].slot`,
we_control / in_game / drone_type, then the humans / drones / our_drones lists).
Residual lane: schedule only (`as1`). ugen's order is `la t3; la t4; la t5; sh; lh; sh; sh; lb; ble` (from `cc -S`);
retail's `as1` output keeps `la t4` whole, ours splits it around `la t5`. Not source-line driven (joins and extern
order have no effect), not the dead `i = 0`.
What was found on the way (about 110 variants; the last 20 on this one residual):
- `D_801543CA` (number of cars) is `volatile` (address kept in t3, re-read through `0(t3)`): 142 -> 19.
- zeroing is `D_8015274C = 0; j = D_8015274C; D_80152768 = j; D_80153FD2 = j;` (a C chain re-reads each lvalue).
- loops 1-2 need the arcade's `m`/`gc` pointer locals (index form there: 138 words); loop 3 must index `model[]`
  directly (named pointer: `.noalias` lost, we_control load not hoisted; `near_named_m_9words.c`).
- the loop-3 counts go through the same local `j` as the zeroing (`j = n; arr[j] = index; n = j + 1`); that keeps j
  out of a2, so the model pointer and the our_drones count take a2. This is shaping, not proven source.
Best next hypothesis: the three count globals are not three separate `extern s16`s (for example members of one
struct or one array), which would change how ugen materialises their addresses in the preheader; or the store order
of the zeroing differs with a different temp. Untested for lack of evidence about the layout: 0x8015274C,
0x80152768 and 0x80153FD2 are far apart, so a single struct is unlikely.
Layout: link records `D_80153E88[6]` 8 bytes {u8 @5, u8 flags@6 (0x80 active), u8 owner@7}; MODELDAT +0x7C6 s16 slot,
+0x7C8 s16 in_game, +0x7CA s16 we_control, +0x7CC s8 drone_type (1 drone, 2 human); CAR_DATA(0x3B8) +0xEE u8,
+0x35B s8 place; `D_80152744` s8 num_active_cars; `D_80153FD2`/`D_80152768`/`D_8015274C` s16 num_humans /
num_drones / num_our_drones; `D_80143A40`/`D_801527D8`/`D_80152808` s16 humans[] / drones[] / our_drones[].

## 7. func_800F43B8 — strict MATCH

```
score.py fn cand/func_800F43B8.c func_800F43B8 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800F43B8:
  MATCH
```
Source `cloud/matches/func_800F43B8.c`. -O3 only (136/144 at -O2). End-of-race statistics for the current player:
the same update on the save-data record (`base + 0x684 + sel * 0x1C`) and on the session copy `D_80151618[sel]`,
then `menu_dialog_close(ref, sel)` (re-checksum). No quirks. First draft was 20 words off with a `Player76 *p`
local; spelling every access `D_8014A118[D_801543D4].ref` makes that value one CSE web for the whole function
(t1 at the top and before the final call), which is what retail has.
Layout: `Player76` (0x4C, `D_8014A118[]`) +1 u8 index, +0x48 `Car **ref` (lazily `&D_80146150[index]`);
Car +0x2C data; data +0 base; base +0x684 Rec[4], +0x6FC s8 selector, +0x704 f32 time. Rec 0x1C = { s32 checksum,
u16 total, u16 count[3], u16, u16, u16 best, f32 bestTime, s32 sum }. `D_80154450` = { u8, u8 kind, u16 value }.

## Bonus: func_800FD41C — strict MATCH

```
score.py fn cand/func_800FD41C.c func_800FD41C --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800FD41C:
  MATCH
```
Source `cloud/matches/func_800FD41C.c` (also MATCH at -O2). `(f32)(u32)(sched.frameCount - D_80111958) * D_8002AFB8`.
Used as the probe that showed the scheduler frame counter is volatile.

## What generalises

1. **A caller-less `jr ra; nop` stub next to a function is evidence about that function.** Two cases here: the
   lock's init static (`func_8008A6FC`) closed a function that had eaten ~1,500 variants, and a second stub fixed
   the frame of `audio_queue_process`. Check `frontier show` -> `stub_neighbours` first.
2. **Each inlined call adds 8 bytes to the caller's frame** (below the inlinee's own locals). A frame 8 bytes larger
   than saved registers + locals explain is an inlined neighbour: look at the small matched functions next door
   (`player_conditional_check` for `func_800D5524`). An inlinee's locals sit below the caller's.
3. **`volatile` shows in the address form.** A volatile global or field is never folded into `%lo(sym+off)`: retail
   has `lui/addiu base` then `off(base)`, and re-reads. Seen for the scheduler frame counter (`D_8002E8E8+0x27C`,
   nine more functions use that form: display_settings, controller_poll, func_800CD104, control_settings,
   world_collision_response, world_trigger_activate, emitter_update, game_loop, func_80109A60), for `D_801543CA`
   and for two Rumble Pak fields.
4. **A global whose address is not commoned across its uses, and that no other function references, is a
   function-local `static`.** (`display_settings`, as `func_800EC270` in wave 0.) Scan the image for other
   references before trying extern forms.
5. **Pointer locals cost `.noalias`.** ugen emits `.noalias reg,$sp` only for registers it derived from a global
   array itself. With `p = &G[i]; p->x` the instructions are the same but `as1` cannot move stack loads over
   stores through `p`. A residual that is "same instructions, loads not hoisted above stores" means: index the
   global directly. (`cc -S` at -O2 shows the directives.)
6. **One expression, one web.** Repeating `G[idx].field` verbatim makes its value one CSE web over the whole
   function; a named local for the same thing makes separate webs with different registers (func_800F43B8,
   20 -> 0). Conversely a named local shared by unrelated statements keeps a register busy across them
   (func_800EC914's `j`).
7. **Operand order of commutative float ops depends on the lvalue type, not on source order.** `F32[3]` arrays
   with the arcade macro and arcade operand order matched; `struct {x,y,z}` did not under any reordering.
8. **`.align 5` after an endless loop** makes the nop count depend on the function's object offset. Such a
   function can only be scored with its real predecessors in the file, in an order that reproduces the offset
   (umerge emits uncalled roots in reverse definition order, callees before callers).
9. `as1` deletes a dead `move`; `cc -S -O2` output is a usable view of ugen order even for -O3 targets when the
   -O2 and -O3 code agree.
