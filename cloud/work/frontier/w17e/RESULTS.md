# w17e — func_800D91A0 (3,860 B) cluster

Status: **NEAR-MISS, nothing installable.** func_800D91A0 is structurally solved (8 ops rows, frame 240 = retail,
966 vs 965 words, 212 word rows) but the cluster cannot land until **func_800D816C (699 w) is matched**: it is
D91A0's IPA partner (t2 register parameter, clobbers s0-s8 unsaved) and must be a member of the same group.
Astra's draft of it is 227 ops rows / 627 words off; it needs its own fresh reconstruction.

| Function | Bytes | State | Unit result (best.c + base/*, 66fe4d4f) |
|---|---:|---|---|
| func_800D91A0 | 3860 | near-miss | `FAIL func_800D91A0: 239 of 965 words differ; compiled body is 966 words, target 965` (udiff: 8 ops / 212 word rows, frame 240/240) |
| func_800D816C | 2796 | far (Astra draft) | `FAIL func_800D816C: 627 of 699 words differ; compiled body is 672 words, target 699` |
| render_replay_ui / render_results_screen / func_800D8078 / drone_throttle_calc | 132/196/220/1024 | EQUAL with real D91A0 | `EQUAL render_replay_ui: 33 words (internal, c_rows.c)` etc.; `locked bodies that differ in this unit: 0` |
| func_800D8154 (as `(s8 player, s32 item)`) | 24 | EQUAL | `EQUAL func_800D8154: 6 words (internal, c_d8154.c)` |
| func_800DA0B4 (inlined getter) | 8 | EQUAL | `EQUAL func_800DA0B4: 2 words (internal, c_best.c)` |

Reproduce (Pi, repo root): `EXTRA="--internal func_800DA0B4 --neighbours" cloud/work/frontier/w17e/sc.sh
cloud/work/frontier/w17e/d91a0/best.c cloud/work/frontier/w17e/base/target.c`.

## Setup
- Unit scoring script: `sc.sh D91A0.c D816C.c` (adds base/rows.c, base/predicate.c, base/dtc.c; internal
  D816C, render_replay_ui, func_800D8078, drone_throttle_calc; `$EXTRA` for more args).
- base/: Astra's caller.c (D91A0) / target.c (D816C) / rows.c (render_replay_ui byte-span form, PR #298) /
  predicate.c (func_800D8078, w8a) and dtc.c (w16f drone_throttle_calc without the zz stand-ins).

## Baseline (current unit, 66fe4d4f)
Astra caller.c + target.c: func_800D91A0 539 ops rows (1040 vs 965 words, frame 384/240);
func_800D816C 227 ops rows (672 vs 699 words, frame 296/232).
With them in the unit, all four provisional bodies are EQUAL:
render_replay_ui (33), render_results_screen (49), func_800D8078 (55), drone_throttle_calc (256).

## Findings so far
- No arcade ancestor: func_800D91A0 is the N64 per-player 10-character code/name entry screen
  (letters 0..25, cursor 0..11: 0-9 characters, 10 = 3-way option byte `save->opt` (+0x2B), 11 = done),
  with duplicate/look-alike detection (21/8, 22/9; 19 and 25 exempt), scroll window of D_8014A10A rows,
  pause-menu setup drone_throttle_calc(p) on first entry, func_800A1910 (compare) to mark D_80139300[slot] dirty.
- func_800D8154 is called with `sw s4,0(sp)` before each call and t-registers live across it: it is an
  INTERNAL IPA callee in retail with an extra leading s8 parameter passed in the caller's home slot:
  `s32 func_800D8154(s8 player, s32 item)` (body unchanged, still 6 words EQUAL as internal). It needs an
  inline blocker (`--block`; unit_overrides inline_blockers) or umerge inlines it.

## Progress log
- a1 (fresh, pointer locals): 341 ops rows. a3 (global arrays indexed by p, no pointer locals): 229.
- func_800D8154 as internal `(s8 player, s32 item)` + inline blocker: 130 (retail's `sw s4,0(sp)` reproduced).
- a6 (`else` arm instead of second `continue`): 126. a8 (separate `slot2` read before resource_type_select): 128,
  but s0 now holds it as in retail.
- a10: `func_800A1910((u8 *)D_8013C068 + slot2 * 10, ..., 10)` with the size parameter u32 (retail `multu s0,t1`,
  `li a2,10`): **94 ops rows**, 969 vs 965 words, frame 224 vs 240.
- c2: `D_80139300` declared `s8[]` (the stored 1 then shares the s32 constant-1 web in s6): **31 ops rows**.
- c4: the top clear loop and the all-inactive scan use `p` as their counter (as the rect loop does; retail homes
  all three at 232): 25 ops rows; spill-temp order now matches (A10C address first).
- c9: `slot2` (second slot read) declared s32: 23 ops. (s8 `dup` must stay s8: retail's sll/sra of 1.)
- d2: `D_80149DD4[p] = 0` written INSIDE both flag-clear loops (uopt hoists the invariant store into the
  preheader after the loop pointer init, exactly retail's `move v0,s1; sb zero,0(s3)` order): **10 ops rows**,
  966 vs 965 words, frame 224 vs 240.
- e1: init order `last = 0; done = 0; count = 0;`: 8 ops rows.
- g1: **the caller-less stub func_800DA0B4 is an inlined getter** `s8 func_800DA0B4(u8 slot) { return
  D_80144030[slot].f6; }` (retail's v1/v0 pattern at the four f6 tests; each inlined call reserves 8 B of frame).
- g3: block locals `s`, `v` hoisted to function level and the all-inactive flag uses `i`: frame 240 = retail,
  no unused locals. 8 ops rows, ~212 word rows (callee-saved register names in the head, t-ring numbering).
- g3 force oracle (`force.sh g3 func_800D91A0 p1:w392=c20,p1:w424=c19,p1:w391=c21,p1:w388=c22`): constant 1 -> s6,
  constant 21 -> s5, &D_80151AD0 -> s7, &D_80113ED4 -> s8 takes word rows 212 -> 164. The callee-saved names are a
  decision-ORDER difference: retail decides const 21 before const 1 (s5 then s6), ours decides 1 first
  (save 64.3 vs 42.6). Remaining after the force: short-web colours (rec->save pointer v0 vs v1; buttons value a0 vs
  a1 in the 0x1000/0x2000 chain and the resource_type_select argument; q pointer a0 vs v0; all-flag a0 vs t0), two
  spill slots 4 B apart (flags/scroll/cursor pointers at 140/144/148 vs 136/140/144), body-start store schedule.

## Recovered structure (func_800D91A0)
See the header of `d91a0/best.c`. Data: `InputRec input_rec0[4]` (0x4C: +1 u8 slot, +4 u32 buttons, +0x48 Save **),
`Save` (+8 SaveQ *q, +0x21 s8 name[10], +0x2B s8 opt), `SlotInfo D_80144030[]` (0x304 stride, +1 f1, +6 f6),
`Rect D_80113EE0[4][4]` (0x18: x,y,w,h), s16 D_8014A0F8/D_8014A100 [4] (cursor/scroll), s8 D_80149DA8[4][10],
s8 D_80149DD4/D_8014A10C/D_8015418C [4], s8 D_8013C068[][10] (names), Save *D_80146150[], s8 D_80139300[],
camera presets D_80112A98..AB4 (own .rodata: 0.9f, 1.3f, 1.3f at 0x80124218..20).

## Integration notes (for when D816C matches)
- Group members: func_800D91A0, func_800D816C, render_replay_ui, render_results_screen, func_800D8078,
  drone_throttle_calc (supersedes nothing locked except:) func_800D8154 (locked single, redefined with the extra
  leading `s8 player` parameter; same 6-word body) and func_800DA0B4 (locked 2-word stub, given the getter body).
- Internal: func_800D816C, render_replay_ui, func_800D8078, drone_throttle_calc, func_800D8154, func_800DA0B4.
  render_results_screen and func_800D91A0 kept.
- func_800D8154 needs an inline blocker (unit_overrides inline_blockers) or umerge inlines it into D91A0 (verified:
  without `--block` it is inlined 6x, D91A0 goes 263 ops rows). CONFLICT to resolve: retail func_800D816C DOES inline
  it (its 12-item count loop is the inlined `item != 8` test), so with a global blocker D816C must contain the
  test written out, or the real reason umerge skips D91A0 (caller size?) must be found. `score.py group` support
  for a per-group blocker is unverified.
- Disclose: func_800A1910 declared with a u32 size parameter (locked definition says s32 n); a9 (s32) is 96 vs
  a10 (u32) 94 ops rows at that stage -- retail loads the 10 with `li a2,10` instead of the constant register t1.

## Traced residual (best next hypothesis)
1. Callee-saved permutation (48 word rows): forced in the oracle above. Retail decides constant 21 before
   constant 1; ours gives const 1 the higher priority (save 64.3 vs 42.6, the four dup-test constants tie at 42.55).
   Next: find what lowers const-1's web priority in retail (fewer s32-1 uses: one of the `1` stores/args is a
   different constant web in retail, e.g. a u8/unsigned typed store) -- try each `= 1` site's type one at a time.
2. Short-web colours (v0/v1, a0/a1): likely to move with (1); re-check after it.
3. Spill slots 4 B off for the flags/scroll/cursor pointers: one coloured expression web appears earlier in ours
   (itable order) than in retail.
4. Then D816C: fresh reconstruction (float-heavy menu animation; uses camera presets, inlined D8154 count loop,
   memcpy 12, func_8008B32C/func_800B5898/func_800B5940, func_800B24EC, model_data_load/transform).

## Not tried / dead ends
- Whole cursor-validity test as an inlined static (`b1`/`b2`): umerge does not inline it (880 words).
- Getter returning `SlotInfo *` for both f6 and f1 (g5) or a second getter for f1 (g6): worse.
- `all` flag as its own variable: frame 248 (needs i reused); `slot` reused for the second read: 76 ops.
- Chained `D_80149DD4[p] = D_80149DA8[p][i] = 0`: 22 ops (two statements: 10).
