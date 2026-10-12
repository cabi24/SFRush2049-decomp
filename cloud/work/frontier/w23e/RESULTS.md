# w23e RESULTS (wave 23)

No MATCH this wave: no cloud/matches files, no groups, no integration needed. All files under this lane dir are
work bases (best.c + variants in */v/, private oracle tools in tools/).

Targets: entity_render_setup (8 rows, from w22f best.c), then controller_poll and fresh singles.
Builder scratch `watchman2:~/rush2049/scratch/frontier/w23e` (toolkit reused from wtk; w16d's `src2/uopt.spl.c` copied).

| Function | Bytes | State | Flags | Scorer line |
|---|---:|---|---|---|
| controller_poll | 928 | near-miss, 92 -> 11 words (ops 30 -> 2), body 232/232, two disclosed devices | -O3 unit | `us.sh`: `best_full.c: FAIL controller_poll: 12 of 232 words differ \| words 11 ops 2 norm 2 \| frame 88/88` |
| func_800EC190 | 224 | unchanged, 2 words (w21g/w22f best re-scored); ~20 min | -O3 unit | `base.c: FAIL func_800EC190: 2 of 56 words differ` (unit) |
| entity_render_setup | 2048 | near-miss, unchanged at 8 ops rows; cause 3 proven by oracle (two conditions, below) | -O3 unit | `us.sh`: `best.c: FAIL entity_render_setup: 420 of 512 words differ; compiled body is 513 words, target 512; +0x354: .rodata+0xe \| words 52 ops 8 norm 8 \| frame 184/184` |

## entity_render_setup (0x8008C884, 2048 B) — ~2.4 h, stopped (stop rule)

`best.c` = w22f best.c unchanged (no source improvement found). Unit re-score in the current tree as in the table.

### Cause 3 (palette base) — oracle result: retail needs block 17 in the LR *and* a split into pieces
New private uopt oracles (on w16d's `uopt.spl.c`; `tools/patch_add.py`, `tools/patch_ulr.py`, driver `tools/oracle.sh`):
- `uopt_add`: env `ADD_SYM=s ADD_BB=b ADD_AT=c` creates a livbb for symbol s in block b just before s's first
  formlivbb in block c (b's block node is captured from an earlier formlivbb call in b).
- `uopt_ulr`: adds `ULR=sym`, a dump of a live range's livbb list (occurrences, f20..f23) at f_updatelivran entry/exit.

Results on the base snapshot (palette = sym/web 376, livbb {19,20,28}; 17 = mode test, 18 = `D_8014A110 == 6` test,
19/20 = mode-2 sub-arms, 28 = else arm):
| oracle | result (ops rows) |
|---|---|
| none | 8 (`la v1` at head of 28) |
| ADD 17 | 10: one load in 17 (v0), **no** reloads in 19/20 (the LR's gap block 18 is filled after colouring: lr+12 "extra" set) |
| force split p1:w376=s (no ADD) | 8-9: pieces keep their own head loads; split alone does nothing |
| ADD 17 + `p1:w376=s` | 8: pieces {19} and {17,20,28}; 20 still not reloaded |
| **ADD 17 + `p1:w376=s,p1:w404=s`** | **6, body 512**: pieces {19}, {20}, {17,(18),28}, all v1 = retail's palette code; left only the as1 order `lui v1` vs `li at,2` in block 17 and the cause-4 rows |

So retail's palette address is ONE live range containing block 17 that uopt *splits* (start blocks 19, then 20:
first livbb with occurrences in list order), leaving {17,28} with the load in 17. Both conditions are needed; ours has
neither (17 not in the LR; the whole LR is coloured, `tot 6 > best 0`). A natural split needs `totalsave <= bestcost`,
i.e. the whole LR must be expensive (spanning a call or competing for callee-saved registers) while its pieces are cheap.
How 17 gets into the LR: f_updatelivran (called for split pieces) adds the blocks of lr+12 (gap blocks) with f21 (load
at entry) when not all predecessors are in the LR; a constant's LR otherwise contains only occurrence blocks plus
partial-availability join blocks (w17d's alpha-table probe `v/a1.c` re-traced: LR {17,15,19,20,28,14}, 15 and 17
added as joins, load in 14 and 17, no reloads: one LR, not split).
Cause 4 (`&delta`, sym 381 {30,52}) has the same shape: retail loads a0/a1 in block 29 for the HUD call (30) and
recomputes `addiu a0,sp,100` at the world-path entries. ADD 29 makes &delta a coloured s0 web across the calls
(10 rows); ADD 29 + split gives pieces {30} and {29,52,...} (8 rows): the split must start at 29/30 together, which
again needs an occurrence in 29.

### Source forms tried this wave (all inert or worse; do not repeat)
- else arm spellings `(&D_8011B558[4])->r`, `((u8 *)D_8011B558)[16]`, `((u8 *)&D_8011B558[4])[0..2]` (e1-e3): 8 ops, identical.
- `Color4 *pal = D_8011B558;` before the mode test, else arm `pal[4].r` (p1): pal is copy-propagated and the else arm
  folds to `lui t8 / lbu -19096(t8)` (no register), frame 192, 9 ops.
- inlined getter `pal = get_palette();` (static, defined after the function) before the mode test (h1): the return
  value's RLDA stays in block 17 (a0) and the SUB-ARMS share it (no reloads) while the else arm folds as in p1; 13 ops,
  frame 192. (Shows a helper return does put an occurrence in 17, but it joins the sub-arms' web.)
- `switch (vehicle->mode) { case 2: ... default: ... }` (sw1): identical to the if/else.
- w17d's alpha table via the palette (a1): 11 ops (see above).

### Best next hypothesis
Find what makes the whole palette LR unprofitable in retail so uopt splits it: e.g. the else arm's palette value
used again after a call (an occurrence after func_8008C544/func_8008C074 would make the LR span calls, bestcost > 0),
or a source where the palette occurrence in 17 is a real one (the h1 helper put one there) combined with the
sub-arms being a separate split piece. Test candidates with `uopt_ulr` + `SPL=1` first: success = `[SPL] split lr=376`
lines with start bb 19 and 20 WITHOUT any CDX_FORCE.

## controller_poll (0x800C95DC, 928 B, group) — ~1.6 h, stopped (stop rule): 92 -> 11 words

Start: w22f best.c (`words 92 ops 30 | frame 88/88`, 230 words). `controller_poll/best.c` is a body; build with
`controller_poll/mk.sh best.c OUT.c` (w22f's prelude recipe). Variants under `controller_poll/v/`.

| step | variant | unit (us.sh) |
|---|---|---|
| base (w22f best) | v/base_full.c | words 92 ops 30, body 230 |
| `if (connected) {}` in the button loop | k1 | words 33 ops 12 |
| same device after the button loop instead | k2 | words 31 ops 12 |
| `0.2f` literal for `repeat = D_80123F94` | k3 | words 27 ops 7, body 231 |
| `held = *(volatile u32 *) &D_80156978[i];` | k5 | words 21 ops 4, body 232 |
| `gRepeatTime[i][b] = gTick;` before `D_80143A00[i] \|= mask;` | k7 | words 15 ops 4 |
| `raw = gStickRaw[i][0];` before the first call | m5 = best.c | **words 11 ops 2** |

Traced causes (ctrace on `cp0`/`k8` snapshots):
- **Register cascade = `connected`'s priority.** Retail colours `connected` t4 *before* the loop-pointer webs (all
  tied at save 6.83, tot 41, nocs 6); ours had connected at save 6.0 (tot 30, nocs 5), decided after them, so
  prevPressed/gStick took t4/t5 (both free: no call clobbers them inside the loop), connected fell to ra, and one
  s-register was left over for &D_8015694C (w231, s8) and the queue address (w225, s0). Raising connected above
  6.83 (any extra weighted use) gives retail's whole s0..s8/t4/t5 assignment, D_8015694C's v0 piece inside the
  `if (connected)` block and the rematerialised queue address — w22f's two "why does the queue/D_8015694C get a
  register" questions were both this one decision. Natural lever not found yet: retail needs connected's LR to be
  <= 16 blocks (nocs 4 -> save 7.5) or one more weighted use; the compiled-out `if (connected) {}` is disclosed.
- **0x80123F94 is this function's own .rodata (0.2f)**, not a global: writing the literal fixes the hoisted
  constant order ($f24 = 1.0 first, then $f20 = 0.2) and the `move t1,zero` delay-slot fill.
- **held is re-read from D_80156978[i] after the store** (`sw t9,0(s1); lw t0,0(s1)`). uopt forwards every plain
  form tried (`((u32 *) D_80156978)[i]`, chained `held = D_80156978[i] = gHeld[i]` (91 words), laundered
  `(u32 *)(u32)` (94 words, frame 160)); only a volatile read keeps the load (disclosed device).
- **raw/cal pointer registers** (s3/s4) follow IV web numbering = first appearance; reading raw into a local first
  fixes it (no frame cost: the function has no locals area). The same local for the second call is worse.
- **Second call's load order**: ugen emits raw first, as1 picks cal first (aftercycles 6 vs 0, `as1t.sh m5`).
  A block boundary after the raw load (`do {} while (0);`, v/p1) fixes the first call but not the second.
- **32 vs &D_8002AFB4** (t2/t3): tied priority 14.29 (tot 100, nocs 7); the address web is numbered first. `while`
  loop form (k8) inert.
- Tested: func_800C9590 with the parameters reordered `(s32 raw, f32 range, f32 calib)` (v/n1, candidate defines
  it): func_800C9590 stays EQUAL (IPA registers unchanged) but controller_poll is 13 words, no better.
Integration (when it matches): supersede group `controller_poll` under the same name (members func_800C9590,
player_mode_set, player_state_set + claims controller_poll; its group.c holds a draft controller_poll as context).
Best next hypotheses: a natural extra use or fewer blocks for `connected` (e.g. the arcade-style `if (!connected)
continue;` shape, or `connected` tested once with both |= updates after the button loop); a natural reason for the
held reload (an alias-killing store between, e.g. through a pointer to the stick record).

## func_800EC190 (224 B) — not improved (2 words), ~20 min
Lead from w22f's hypothesis (a code-free ugen temp between the static's increment and `li 2`):
- `D_80150C04 = D_8014A118 = D_801163F4;` (chained, v/c1.c; also without the `if (D_8014A118) {}` holder, c2):
  the extra temp appears and `2` goes to **t1 as retail**, but cfe's chain value is re-read from D_8014A118
  (`lb t0,0(v1)`, uopt does not forward an s16 -> s8 store), 9 words. So the missing allocation is a narrowing of
  the static's value for the s8 stores; a form whose narrowing survives to ugen as a `move` (deleted by as1) but
  without the reload would close it.
- Inert/worse: `D_80150C04 = D_8014A118;` re-read (9 words), `(s8)` cast on one or both stores (2 words with the
  holder, 10-11 without).

## What generalises
- **Constant live ranges in uopt (entity_render_setup trace):** a constant's LR is its occurrence blocks plus
  partial-availability join blocks; gap blocks (lr+12) are filled after colouring; f_updatelivran gives split pieces
  a load at entry (f21) where not all predecessors are in the piece. A load that retail places in a dominating
  branch block for one arm only, with the other arms reloading, is an LR that contains the branch block AND was split.
  Oracles: `tools/patch_add.py` (add a livbb), `tools/patch_ulr.py` (dump livbb flags), `tools/oracle.sh`.
- **One low-priority local can decide a whole function's s-register map** (controller_poll): a value that retail
  keeps in a call-safe t-register must be decided before the tied loop-pointer webs; otherwise the pointers take the
  free t-registers, a leftover s-register goes to an address constant and every s-register shifts.
- **An `extern f32 D_8012xxxx` in an old draft can be the function's own literal** (controller_poll's 0.2f); check
  the image word and whether any other function references it before keeping the extern.
- **Pointer-IV web numbers follow the first appearance of each array**; reading one call argument into a local before
  the call reorders them at no frame cost when the function has no locals area.
Permission denials: none. Builder: own scratch only, 2 jobs.
