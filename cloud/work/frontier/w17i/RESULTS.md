# Wave 17, lane w17i: func_80096130, audio_frame_update (register-split residuals)

Status: final. func_80096130 MATCH (group extension, one disclosed compiled-out read); audio_frame_update stopped (stop rule), cause narrowed.

| Function | Bytes | State | Flags | Scorer output |
|---|---:|---|---|---|
| func_80096130 | 264 | **MATCH** (unit EQUAL --neighbours; `score.py group` MATCH); one disclosed `if (slot) {}` | -O3 unit / group | `EQUAL func_80096130: 66 words (kept, c_func_80096130.c)` |
| audio_frame_update | 600 | not matched: 22/150 words, 21 rows (structure now retail; colour + as1 residual) | -O3 unit | `FAIL audio_frame_update: 22 of 150 words differ` / `locked bodies that differ in this unit: 0` (`audio_frame_update/best.c`) |

## Tools (builder `~/rush2049/scratch/frontier/w17i`)
- `bin/uopt_spl` = toolkit uopt.trace.c + w16d `patch_spl.py` + w16b `splitpatch.py` (env SPL=1/W5D_PROC, SPLITLOG=ord,
  SPL_SKIP, SPL_REJ/SPL_SYM). Runner: `spl.sh LABEL PROC ORD "ENV"`.
- `bin/uopt_boost` = uopt_spl + `boostpatch.py`: `SPL_BOOST=lr:save` overrides compute_save's priority of a live range
  (so a split piece uopt drops with save<=0 stays a p1 candidate); combine with `CDX_FORCE`. Runner: `boost.sh`.
- Decoded `f_compute_save` tail: save = total / n, n = livbbs + card(lr+0xc), and for n >= 3 n = (n-2)/4 + 2;
  flag lr+35 = 1 (candidate) iff save > 0, else 2 (dropped). That is why a one-use tail piece is never coloured.

## func_80096130 (264 B): MATCH

### Trace
1. `if (slot) {}` BEFORE the store (`func_80096130/v/p2b.c`, = w16e m/p2Z4 minus its `if (0) {}`): retail colours (slot v1 unsplit,
   alloc a3) AND retail homes (index*8 at 32): `FAIL func_80096130: 11 of 66 words differ | words 9 ops 8 norm 8`.
   The 9 rows are only as1's placement of `sw zero,12(v1)` (ours sinks it into the memset delay slot).
2. Cause (asm oracle, `asm.sh p2b fn1_e.s`): ugen's `.noalias $3,$sp` region for v1 runs to the end of the uopt block
   holding the store (after `jal memset`), so as1 may move the store past `lw v0,72(sp)`. Moving `.alias $3,$sp` to
   right after the store in the listing gives **0 rows**. So retail's v1 noalias region ends right after the store,
   while index*8 still shares a block with sym 5 (home conflict -> 32).
3. Laundering slot (`(Slot20 *)((u32)&D_80156D38[index])`, na2) removes the noalias entirely: tail exact, but the head
   load can no longer be hoisted above the sp stores (24 words). Retail has noalias at the head, not at the tail.
4. Boost oracle on the plain base (`SPL_BOOST=33:0.1 CDX_FORCE=p1:w7=c6,p1:w33=c2`): split pieces head v1 + tail v1
   reproduce p2b exactly (9 rows): ugen merges same-register pieces into one noalias region. So "tail piece coloured"
   alone is not retail either.

### The lever: early `return` (fall-through arm) + the disclosed `if (slot) {}`
`func_80096130/v/r2.c` = deliverable: `if (slot->allocation == 0) { return; }` instead of the nested `if (!= 0) {...}`, with
the compiled-out `if (slot) {}` right before the store. In the ugen listing the `.noalias $3,$sp` opened at the slot
address definition is closed right after block 0 (`.alias $3,$sp` at the first Ulod after the return arm, where v1 is
dead), and is not reopened on the main path (no new definition). So the head load keeps its noalias (hoisted above
the sp stores, retail) and the tail store is ordered against the sp reload (retail). Colours (slot v1 unsplit, alloc
a3) and homes (index*8 at 32) are p2b's. Measured (unit): r1 (early return, no read) 21; r2 EQUAL; r2b (read after the
store) 3; r2c (read before dma_wait) EQUAL; r2d (unbraced return) EQUAL; r3/r4 (`else return;`) 21/3.
Why the read is still needed: without it slot's web has totalsave 3 = caller-save cost 3 (osRecvMesg, reverb, jam),
so it is split and the head re-decided after alloc (alloc then takes v1). Any code-free extra use before the store
gives totalsave 4 > 3. No natural source form for that use was found (w16e's recompute/launder attempts CSE back).

Scorer output (Pi, tree 534d2cbe):
```
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w17i --remote-dir rush2049/scratch/frontier/w17i --jobs 2 score \
    func_80096130 wheel_params_set audio_reverb_update audio_effect_process \
    --with cloud/work/frontier/w17i/groups/codex_heap_release_a25/func_80096130.c --neighbours
  EQUAL func_80096130: 66 words (kept, c_func_80096130.c)
  EQUAL wheel_params_set: 58 words (kept, g_codex_heap_release_a25__wheel_params_set.c)
  EQUAL audio_reverb_update: 61 words (internal, g_codex_heap_release_a25__group.c)
  EQUAL audio_effect_process: 23 words (kept, g_codex_heap_release_a25__group.c)
  locked bodies that differ in this unit: 0
blob_unit score: 4/4 equal; object build/blob_unit/w17i/unit.o (5.3s)
```
(At 0119270d the same run also listed `func_800F0674` as differing; that was pre-existing in that tree: it failed with
no `--with` at all, `.bss+0x3a64 at 0x80156948 and .bss+0x3a70 at 0x80156950 overlap in the image`.)

`score.py fn` standalone cannot match (66/66): without func_800960CC's real definition the helper is a `jal`, not
inlined. So it is packaged as an extension of the locked group (builder, `score.py group cand/codex_heap_release_a25`):
all 17 members MATCH including `func_80096130: MATCH`; context 6/6 MATCH.

### Integration notes (func_80096130)
- Deliverable: `groups/codex_heap_release_a25/` = the locked `src/blob/groups/codex_heap_release_a25/` (every file a
  byte-identical copy, `cmp` clean) plus `func_80096130.c`; group.json: members += func_80096130, keep +=
  func_80096130, files += func_80096130.c, claims [func_80096130]. Same name, every old member kept.
  Alternatively land `func_80096130.c` as a plain single `src/blob/func_80096130.c`: in the unit it is a kept single and
  func_800960CC's real body already comes from the existing prefer_definition override; the unit result is the same.
- No new unit_overrides, no data-symbol changes. Splice claims func_80096130: **+264 B game**. Unblocks
  wheel_setup_initial (frontier show: sole_blocker_for).
- Shaping: one compiled-out `if (slot) {}` (disclosed in the header). No unused locals, no pad, no implicit int.

## audio_frame_update (600 B): not matched, stopped (about 2.5 h of trace work)

Start: w16f best (= w4b, `audio_frame_update/v/b0.c`): `FAIL audio_frame_update: 21 of 150 words differ` (24 rows,
16 mnemonic rows). Deliverable best: `audio_frame_update/best.c` (= `v/f1.c`):
`FAIL audio_frame_update: 22 of 150 words differ` / `defined by c_best.c (kept)` / `locked bodies that differ in this
unit: 0` (`blob_unit --tag w17i score audio_frame_update --with .../best.c --neighbours`). One strict word worse than
b0, but 21 rows / 12 mnemonic rows and the preheader structure is retail's; it is the right base. No group delivered.

### Lever found 1: why the D_801543CC load is not hoisted (PRE), and the base form that fixes it
`pretrace.sh` on w16f's g5 (float read in the loop, `car->` stores): the float load (`ilod.R@0(lda D_801543CC)`) is
not locally anticipated in the loop block, because the five stores before it go through the *variable* `car`
(`ixa(varM car, i*24)` before copy propagation), and a store through a pointer variable kills every global load.
With `player_array[slot].x` stores (g1-g8) the load is hoisted, but cfe emits the scaled form `ixa(lda, slot)` there,
while `&player_array[slot]` / the car variable give `ixa(lda, mpy(slot,952))`: two different expressions, so the
loop base is recomputed (157-158 words).
Fix: pointer arithmetic for both: `car = player_array + slot;` (used only for the laundered m) and
`(player_array + slot)->snd[6 + i].x = ...` in the loop, float read inside the loop. One shared car web (retail
`addu a2..; addiu v0,a2,272`), loop stores lda-based, PRE hoists the float load (retail `lui at; lwc1 $f0,17356(at)`),
150 words (`v/e1.c` 22 words, `v/f1.c` = e1 minus the dead `ft` local, same). `(&player_array[slot])->` (c1-c3) is
canonicalised back to the scaled form (43 words). m must stay laundered: `m = (SndSlot *)((u8 *)car + 0x110)` or
`car->snd` folds the +272 into the store offsets (`sw t8,292(a1)`, m1/m2 37 words).

### Lever found 2: anim before ctl
ctl `&D_80139320[slot]` and anim `anim_state_update` tie at save 5.5; the lower web number (first appearance) wins.
Writing the `cb = anim_state_update` store before the `handle = ctl->vals[i]` store (`v/f2.c`) gives anim the earlier
web and the retail order (anim a3 before ctl t0 once the v1 web exists), and also puts the m stores at retail's place
in the unforced schedule. f2 alone: 25 words (the colours are still one register down).

### Traced cause of the rest: a web in v1 ahead of the counter
ctrace (f1/f2): w78 car+i*24 70 -> v0, **i 30.5 -> v1**, 24 10 -> a0, car 6 -> a1, anim/ctl 5.5 -> a2/a3. Retail:
v0, i a0, 24 a1, car a2, anim a3, ctl t0, v1 unused in the final block. Oracle (`boost.sh lf2 ... SPL_BOOST=110:40`,
`v/l_for2.c` where `i != 4` makes the constant 4 a register web): raising one loop-live web above i's priority puts
it in v1, and the natural cascade then gives exactly retail's i a0, 24 a1, car a2, anim a3, ctl t0. So retail has one
more web, live across the whole loop, decided before i (save > 30.5, i.e. about six loop-weighted occurrences), whose
register is never written in retail's final block (no `li v1`/`lui v1` there). Forcing retail colours on f2
(`force.sh f2 ... "p1:w74=c3,p1:w109=c4,p1:w64=c5,p1:w79=c6,p1:w82=c7"`) leaves **8 rows**: as1 places the two m stores
before `sll t9,s3,6` and `move a0,zero` early (retail: stores right before the lwc1). The as1 trace shows the stores'
priority comes from the alias edge to the float load (aftercycles 1); reordering the ucode (asm oracle e1_x1..x3)
does not move them, removing that edge (x4) does. In the unforced f2 schedule the stores are at retail's place, so the
store placement depends on the register assignment (WAR edges), and will likely follow once the v1 web is found.
Not it (measured): holders `if (m) {}`/`if (car) {}` in the loop (25), `if (dst) {}` (63-68), `if (t) {}` (130),
loop forms for/while/goto (24-25), `i != 4` (22 words, 18 rows: the constant-4 web goes to a1, not v1).

Best next hypothesis: the v1 web is a second loop-carried value that uopt keeps but ugen never materialises: e.g. a
second induction variable eliminated into i after colouring, or a loop-invariant address whose only uses become
offsets. Use `pretrace.sh` on a candidate and look for a LIVE-RANGE web across block 17 with ~6 occurrences; test with
`boost.sh` first. Start from `audio_frame_update/v/f2.c` (anim-first order) or best.c.

### Integration notes (audio_frame_update)
Nothing to integrate. A future match must supersede the locked group `audio_frame_update` under the same name and
keep func_800B0A88 as a member.

## What generalises
1. **ugen closes a `.noalias` region at the first `Ulod` after a block where the register is dead, and does not reopen
   it without a new definition.** A fall-through arm where the pointer is dead (early `return`) ends the region; the
   head keeps noalias (loads hoisted above sp stores) and the tail loses it (stores stay ordered against sp reloads).
   Diagnose with `asm.sh` by moving `.alias $N,$sp` in the listing (func_80096130: 9 rows -> 0).
2. **Two split pieces of one web in the same register merge into one ugen noalias region** (boost oracle), so "colour
   the tail piece" does not change scheduling by itself.
3. **compute_save** (decoded): save = total / n, n = livbbs + card(lr+0xc), for n >= 3 n = (n-2)/4 + 2; a piece with
   save <= 0 gets flag35 = 2 and is never a p1 candidate. `boostpatch.py` overrides it (oracle for "what if this
   piece/web were decided earlier").
4. **`&a[i]`, `a + i` and `a[i].f` are three different uopt expressions** (mpy / add / scaled ixa); only the same form
   is CSE'd. A store through a pointer *variable* kills every global load for PRE; pointer arithmetic on the array
   keeps the stores lda-based and lets PRE hoist global loads (audio_frame_update's float).
5. **`if (x) {}` placement relative to a store decides spill-home sharing**: before the store it keeps the store in
   the next block's live range (home conflict, retail 32(sp)); after it, it ends the block (home reuse, 36(sp)).
6. Tools: `spl.sh`, `boost.sh`, `boostpatch.py` in this dir; builder binaries `bin/uopt_spl`, `bin/uopt_boost` in
   `~/rush2049/scratch/frontier/w17i`.

No permission denials.
