# w24d RESULTS (wave 24)

Builder scratch `watchman2:~/rush2049/scratch/frontier/w24d` (toolkit reused from wtk). IDO 5.3, `-g0 -O3 -mips2 -G 0 -non_shared`.

| Function | Bytes | Before -> after | State | Scorer line |
|---|---:|---|---|---|
| controller_poll | 928 | 11 words (w23e) -> 0 | **MATCH as group member**, superseding group `controller_poll` (4 members, all claimed); no shaping devices | builder `score.py group cand/groups/controller_poll` (rc 0; `--claims` rc 0): `controller_poll: MATCH` / `own .rodata verified at 0x80123F94..0x80123F98` / `func_800C9590: MATCH` / `player_mode_set: MATCH` / `player_state_set: MATCH` |
| entity_render_setup | 2048 | 8 ops rows -> 8 (unchanged) | near-miss, stopped (stop rule; ~1.5 h here after w23e's 2.4 h) | `us.sh`: `best.c: FAIL entity_render_setup: 420 of 512 words differ; compiled body is 513 words, target 512; +0x354: .rodata+0x1 \| words 52 ops 8 norm 8 \| frame 184/184` |

## controller_poll (0x800C95DC, 928 B) — MATCH (group)

Deliverable: `groups/controller_poll/` (group.json + group.c). Body also in `controller_poll/best.c` (build the
unit candidate with `controller_poll/mk.sh best.c OUT.c`; mk.sh now cuts the prelude at the draft instead of a fixed line).

Unit, candidate (Pi): `blob_unit --tag w24d score controller_poll --with cloud/work/frontier/w24d/controller_poll/v/full_best.c --neighbours`
```
  EQUAL controller_poll: 232 words (kept, c_full_best.c)
  locked bodies that differ in this unit: 0
blob_unit score: 1/1 equal; object build/blob_unit/w24d/unit.o (9.0s)
```
Unit, installed-group view (the lane group replaces src/blob/groups/controller_poll, plus the extern_data override below;
`tools/unit_group.py`, read-only on the repo):
`EXTERN=D_80156958 PYTHONPATH=$PWD python3 cloud/work/frontier/w24d/tools/unit_group.py controller_poll cloud/work/frontier/w24d/groups/controller_poll -- --tag w24d score controller_poll func_800C9590 player_mode_set player_state_set process_inputs --neighbours`
```
  EQUAL controller_poll: 232 words (kept, g_controller_poll__group.c)
  EQUAL func_800C9590: 19 words (internal, g_controller_poll__group.c)
  EQUAL player_mode_set: 15 words (kept, g_controller_poll__group.c)
  EQUAL player_state_set: 15 words (kept, g_controller_poll__group.c)
  EQUAL process_inputs: 89 words (kept, s_process_inputs.c)
  locked bodies that differ in this unit: 0
blob_unit score: 5/5 equal; object build/blob_unit/w24d/unit.o (8.2s)
```

### The two levers (traced)
1. **Volatile pad state (as1 order of the stick loads).** Retail loads raw before cal for both calls, one cycle
   *worse* than the critical-path order as1 picks (cal's chain lb->mtc1->cvt has aftercycles 6), so the DAG must
   order them. Listing oracles (`asm.sh`): a label after each raw load gives retail's order (e6); `.set volatile` on
   both loads plus `.alias` between their base registers gives it for the first call (e9); volatile or alias alone does
   nothing (e2-e4, e8). Source: `gStickRaw`/`gStickCal` read through `volatile s8 (*)[2]` views (pad data written by
   the controller thread). ugen then emits the volatile loads in source order and as1 keeps it; `raw = gStickRaw[i][k];`
   before each call (the same local, reused) puts raw first for the second call too (c3: 11 -> 7 words). The
   w23e `*(volatile u32 *)&D_80156978[i]` cast becomes the same kind of view, `gPadHeld` (store, then volatile re-read).
2. **`if (b == 0) continue;`** instead of `if (b != 0) { ... }` in the 32-button loop. The tie (32 vs &D_8002AFB4,
   save 14.29 each) goes to the lower itable entry, and constants/addresses get their entries in ucode order: the
   continue makes the loop-test block (with 32) precede the body, so 32 is entry 236 and the address 239 (were 240/239):
   32 -> t2, address -> t3 as retail (7 -> 2 rows, the two own-rodata display rows; `force.sh` had proven colour-only).
   The same form gives `connected` t4 at save 6.0, so w23e's compiled-out `if (connected) {}` holder is gone.
   Inert: while / do-while / `!= 32` loop forms (w1, w3, w4).

### Integration
- Supersede group `controller_poll` with `groups/controller_poll/` (same name). Members: controller_poll,
  func_800C9590, player_mode_set, player_state_set (claims all four); context: func_800E7038, func_800E7134,
  process_inputs. group.json changes vs the locked one: controller_poll moved from context to members/claims and
  **added to `keep`** (without it uopt makes it internal in the group and drops its callee saves: frame 24).
- group.c changes vs the locked one: the draft replaced by the match; the gStickRaw/gStickCal views made volatile;
  `D_80156958` (gStick) **defined** (`D_80156958_Entry D_80156958[4];`, like the existing D_80149B64/D_80149B74
  definitions): the skip loop's two swc1 to gStick[k] share one `lui at` only when the symbol is defined (w15d);
  extern gives 178/232 in `score.py group`.
- **unit_overrides.json**: add `extern_data` `{"name": "D_80156958", "file": "src/blob/groups/controller_poll/group.c",
  "reason": "group controller_poll defines gStick for its lui sharing; audio_occlusion.c owns it in the unit"}` (without it
  uld: `s_audio_occlusion.u: D_80156958: multiply defined`). The existing force_keep entries stay.
- Data: none new (D_80156958 is .bss, defined in the group object only). Own .rodata 0.2f at 0x80123F94 verified by the group scorer.

## entity_render_setup (0x8008C884, 2048 B) — near-miss, unchanged at 8 ops rows; stopped

`entity_render_setup/best.c` = w23e/w22f best.c (re-scored in the current tree, line in the table). Private oracle
uopt rebuilt in my scratch (`bin/uopt_ulr` = wtk `uopt.trace.c` + w16d `patch_spl.py` + w23e `patch_add.py` +
`patch_ulr.py`; driver `tools/oracle.sh`, adapted from w23e).

### New facts (pretrace / ctrace on the base snapshot `eb`)
- **The palette is two constant live-range candidates, by use context**: `ldaS(D_8011B558)` ctx=0 (ixa base: the two
  mode-2 sub-arms, blocks 19, 20) and ctx=3 (ilod base with offsets 16..18: the else arm, block 28). Ours already
  matches retail in both sub-arms (each reloads v1); the only palette residual is the **ctx=3 piece: retail {17,28}
  (load in the mode-test block 17), ours {28}**. So "block 17 in the LR" concerns the else-arm (ilod@16) candidate.
- **&delta (`ldaM(-84)`, uses in block 30 and 52) is never coloured in ours: p1cand save=0.000 nocs=4**, so it is never
  picked and never split; ugen rematerialises `addiu a0,sp,100` at each call. Retail colours split pieces (a0 in
  {29,30}; the world-path piece gets its a0 from the C-arm exits' `bc1fl` delay slots). Retail's &delta LR therefore has
  positive totalsave: one more weighted use/occurrence (block 29) than ours. &quad (`ldaM(-56)`, w382 t0, save 5.25) is
  coloured in both; only its a1 argument copy for the HUD call sits in 29 in retail.

### Tried this wave (all inert or worse; do not repeat)
| variant | change | ops rows |
|---|---|---|
| d1 | `f32 delta[3]` (arcade style) with `delta[k] =` stores | 8 (identical) |
| i1 | delta via an inlined `static vec_sub(f32 *d, f32 *a, f32 *b)` defined after the function | 8, frame 200 |
| dp1 | `f32 *dp = &delta.x;` used for the stores and both calls | 8, frame 192 (dp copy-propagated) |
| dp2 | `(f32 *)(u32)&delta` laundered call arguments | 8 (identical) |
| q1 | `pal = D_8011B558;` before the mode test, used in all three arms | 9, frame 192 (copy-propagated; else arm folds) |
| h_a | default colour `color = D_8011B558[8];` (0x8011B578 is palette entry 8) | 20: Color4 copy becomes lwl/lwr (u8 alignment) |
| h_b, h_c | h_a + alpha table via the palette (`((u8 *)D_8011B558)[idx + 20]`, `(&D_8011B558[5].r)[idx]`) | 23 |
| h_d | `*(u32 *)&color = *(u32 *)&D_8011B558[8];` (aligned copy, diagnostic only) | 15: the palette becomes ONE coloured LR (a2, w367, save 1.5 nocs 6 tot 9 best 0) from block 0 through 17 and all arms, no reloads; forcing `p1:w367=s` gives 9 rows (sub-arms right, else-arm piece uncoloured and folded) |
| a1 + force split (`p1:w376=s`) | w17d alpha-via-palette LR forced to split | 9 (pieces start at the alpha block) |

### Best next hypotheses
1. Palette: give the ctx=3 (ilod-with-offset) candidate an occurrence or partial availability at block 17. h_a put a
   ctx=3 occurrence in block 0 (`color = D_8011B558[8]`) and the candidate became {0,28} but stayed uncoloured; with an
   aligned palette element type (e.g. Color4 as a union with a u32, so the default-colour copy is `lw` as retail) the
   {0,...,28} candidate would be partially available at 17. Measured (h_d, a u32 cast stand-in for that type): block 17
   does join the LR, but the merged LR is coloured whole (tot 9 > best 0) and a forced split leaves the else-arm
   piece uncoloured. So the default-colour copy is a route into 17 but the split/colour of the {17,28} piece still
   needs a cost reason; next: `SPL=1` on h_d with the force to see which piece takes 17, then look for what makes the
   whole LR unprofitable (retail's v0=record and a2=D_8014A110 occupy two of its candidates already).
2. &delta: find the extra use that lifts its save above 0 (an occurrence in block 29, e.g. the delta stores written
   through the address in a form uopt does not fold to `str varM`). Check each candidate with `ctrace` first: success =
   `p1dec ... web=<&delta> decision=split` without CDX_FORCE.

## What generalises
- **as1 keeps volatile loads in emission order** (when their bases may alias), even when that costs a stall. A retail
  schedule that is one cycle worse than as1's critical-path choice means a DAG edge you do not have: test
  `.set volatile` / labels in the listing with `asm.sh` before searching the source. Pad/controller state written by
  another thread is a natural `volatile` (controller_poll: two sticks' raw/cal loads and the held re-read, one lever).
- **Constant/address itable entries (and so web numbers for priority ties) follow ucode order**; `continue` to the
  loop test puts the loop bound's constant before the body's addresses. `if (c) continue;` vs `if (!c) { ... }` is a
  tie lever for loop-bound constants, and it also changes the block structure enough to move other ties.
- **Group packaging of a former context function**: add it to `keep` (else uopt makes it internal: no callee saves),
  and define (not extern) a global whose two offsets share one `lui at` in retail, with a unit_overrides `extern_data`
  for the unit's real owner. `tools/unit_group.py` runs blob_unit with a lane group swapped in (installed view) without
  editing src/.
- **uopt constant LR candidates are split by use context** (ixa base vs ilod-with-offset): one global can have a
  coloured piece for indexed uses and an uncoloured/folded one for constant-offset uses (entity_render_setup palette).
- An address web with `p1cand save=0.000` is never picked (so never split or coloured): retail colouring it means
  retail has one more weighted use.

Permission denials: none. Builder: own scratch only (`~/rush2049/scratch/frontier/w24d`), serial unit runs, <= 2 jobs.
