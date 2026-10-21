# w37d RESULTS (wave 37, SHORT wave): targets/text/misc

Builder scratch: watchman2:~/rush2049/scratch/frontier/w37d (copied from base @ c12837976).

## Status

| function | GC identity | bytes | state | best file |
|---|---|---|---|---|
| func_8010C974 | _AnimateMine@targets.c | 2636 | not reattempted (time; w36s 167/659 stands) | ../w36s/func_8010C974/best.c |
| func_8010D3C0 | _StartWeaponIcon@targets.c | 704 | not advanced (w36w 23 rows: as1 xbb hoist; MAT3 keeps the car pointer, so the w36w2 .noalias lever is not natural here) | ../w36w/func_8010D3C0/best.c |
| func_8010D680 | _AnimateWeaponIcon@targets.c | 476 | not advanced (22 rows unit; op holders tried) | ../w36w/func_8010D680/best.c |
| race_position_update | TB_EllipseText@TB_text.c | 484 | not reattempted (time) | ../w36w/race_position_update/best.c |
| object_bytes_sum_global | TB_GetFontLineHeight@TB_text.c | 84 | 4 words (unit), colour only; 8 spellings no gain | ../w36w/text/best_object_bytes_sum_global.c |
| object_bytes23_sum | TB_GetFontHeight@TB_text.c | 72 | not worked (same t4 cause as object_bytes_sum_global) | - |
| sound_position_update | _UpdateFx@audiomgr.c | 444 | 40 words unit (23 rows; was 51): FP operand order fixed; left = tag/state colour swap + constant-1 web | sound_position_update/best.c |
| func_800E73D8 | N64 per-frame pad read (no ancestor) | 596 | **standalone MATCH + unit EQUAL** (1 disclosed compiled-out block end) | cloud/matches/func_800E73D8.c |
| func_800979A0 | N64 sequence start (no ancestor) | 292 | **group MATCH + unit EQUAL** (slot_sound superseded in place; 1 disclosed compiled-out holder) | groups/slot_sound/ |
| world_trigger_activate | N64 gfx-thread message handler | 464 | 25 words (one-file group), not advanced; v1 forbidden traced | ../w36w3/world_trigger_activate/best.c |

## Notes

### func_800979A0 (292 B): MATCH (group + unit), slot_sound superseded in place
- Base: w36w3 group_plain (= research packet dot_sequence_start_979a0_20261006 caller on the accepted slot_sound
  context): 12 words, all from the final `D_8011EAA0 = func_800156E8(...)` store, which retail writes through a
  materialised address in v1 (`lui v1; addiu v1,v1,-5472; sw v0,0(v1)`, the same shape as the D_80151AD4 store
  earlier, whose address web has a second occurrence: the reload after display_list_alloc).
- Lever: a compiled-out holder `if (D_8011EAA0) {}` after the last store (`D_80151A6C = arg0;`) gives the
  D_8011EAA0 address web a later occurrence, so uopt colours the store's piece in v1 (retail). Found in a
  9-variant batch (local static ADC, && condition, early returns, temp, pointer local, store order, reversed
  compare: all 12-18 words).
- Scorer (builder): `score.py group cand/slot_sound` (no --claims): every old member MATCH (func_80096288,
  slot_value_get, display_list_alloc, player_state_get, slot_deactivate, sound_update_channel, mode_byte2_set,
  object_type_byte2_get, object_type_byte3_get, mode_byte_set, func_800A4E58, entity_render_mode) and
  `func_800979A0: MATCH`; with --claims `func_800979A0: MATCH`.
- Unit (Pi): `blob_unit --tag w37d score func_800979A0 --neighbours --with
  cloud/work/frontier/w37d/groups/slot_sound/func_800979A0.c` -> `EQUAL func_800979A0: 73 words (kept,
  c_func_800979A0.c)`, `locked bodies that differ in this unit: 0`, `blob_unit score: 1/1 equal`.
- Integration: group `slot_sound` superseded in place (HEAD files byte-identical; new file func_800979A0.c;
  members/files/keep += func_800979A0; claims [func_800979A0]). No overrides.
- RECEIPT: the research packet `cloud/work/frontier/dot_sequence_start_979a0_20261006/verify.py` requires
  `config['members'] == accepted['members'] + [NAME]` against src/blob/groups/slot_sound; after this install the
  accepted group already contains func_800979A0, so its test_frozen_receipt_replays will fail and needs the
  integrator's receipt patch (owner applies).

### func_800E73D8 (596 B): MATCH (standalone + unit)
- Base: w36w3 best (4 words: the function-local static counter's web in v1, retail v0).
- Trace: force.sh `p1:w44=c12,p1:w5=c1` is declined with forbidden=0x50020000 (v0, a0, s0) even after moving the
  only earlier v0 web: v0 is forbidden by a PRECOLOURED register in the counter's basic block (the drain call
  `osRecvMesg(&D_80035458, &msg, 0)` defines v0/uses a0 in the same uopt block; live ranges are block-granular).
- Lever: a block boundary between that call and `++queryTimer`. Every compiled-out block end tried matches:
  `do {} while (0);`, `if (msg) {}`, `if (msg != 0) {}`, `do { osRecvMesg(...); } while (0);`,
  `if (osRecvMesg(...)) {}`, `if (D_80035458.validCount) {}`; an empty `{ }` does not. Kept `do {} while (0);`
  (the w15d precedent), disclosed in the header. Holders reading queryTimer at 5 positions, file-scope static
  (116 words) / defined global, j-before-i and ++ spellings: no change.
- Scorer (builder): `score.py fn cand/fv/func_800E73D8.c func_800E73D8 --flags "-g0 -O3 -mips2 -G 0 -non_shared"`
  -> `MATCH` / `own .data verified at 0x80111964 (4 bytes; .data is laid out per translation unit)`.
- Unit (Pi): `blob_unit --tag w37d score func_800E73D8 --neighbours --with cloud/matches/func_800E73D8.c` ->
  `EQUAL func_800E73D8: 149 words (kept, c_func_800E73D8.c)`, `locked bodies that differ in this unit: 0`,
  `blob_unit score: 1/1 equal`.
- Integration: standalone single (splice_singles). The file has static copies of pak_queue_init/pak_lock/pak_unlock
  (inlined; same as the matched func_800E7710 file style) and a function-local static (own .data 29).

### sound_position_update = _UpdateFx (444 B): 51 -> 40 words (unit), not closed
- `D_80146200 * 0.75f` (gMaxSfx * 0.75f; MAT3/DC spell `0.75f * gMaxSfx`) gives retail's `mul.s $f8,$f10,$f4`
  operand order and the whole FP temp ring: us.sh `FAIL sound_position_update: 40 of 111 words differ | words 23
  ops 2 norm 2 | frame 24/24` (w36w3 best 51 / 33 rows).
- Left: (1) stag->tag (w5, save 1.4, tot 7/5 blocks) beats tag->state (w6, save 1.2, tot 6/5) -> v1/a3, retail a3/v1;
  (2) constant 1 is not a web (retail `li t1,1` used by `beql t1,v1` and `bnel t1,t9`). force.sh
  `p1:w5=c6,p1:w6=c2` -> 15 rows, all the constant-1 rows + branch-offset shift + own-rodata reloc rows.
- The MAT3/DC form with the `tFxSequence *tag = stag->tag;` local DOES make constant 1 a web (t0) but splits the
  state CSE (state reloaded, 2 no longer a web; 115-116 words). 7 of 30 generated tag/stag->tag hybrids (head, cond, stop, start
  parts) and three head forms: all worse (62-109 words). Best = w36w3 form + the operand-order fix (best.c).

### object_bytes_sum_global = TB_GetFontLineHeight (84 B): 4 words, not closed
- Re-measured w36w best in the current unit: `FAIL object_bytes_sum_global: 4 of 21 words differ | words 4 ops 0
  norm 0 | frame 24/24` (&D_801497F0 web t2, retail t4). ctrace: t1..t5 all cost 0 for the cross-call webs
  (sound_update_channel preserves them), so retail must have had t2/t3 taken by two interfering webs.
  Retail state_utility keeps x/y in t2/t3 across both sound_update_channel calls, so the callee's IPA summary is
  NOT the cause. 8 spellings (named locals, a TB_GetFontHeight call, call orders, a non-static leading helper,
  cast forms): best stays 4 (non-static helper `lead_w` also 4).

### world_trigger_activate (464 B): 25 words, not advanced
- force.sh: the case-1 &D_80114730 web (w68, type 1, nocs 2, numintf 5) has v0/v1 forbidden; moving the switch
  value (w3), cur (w17) and w67 together frees v1 (55 rows: the moved webs then differ), any one alone does not.
  So in retail the address web does not overlap the switch value / cur ranges. 7 variants (bss counter, holder
  removal (55), do{}while(0) block ends at 4 places, counter spelling): all 25-30.

### func_8010D680 = _AnimateWeaponIcon (476 B): 22 rows, not advanced
- Unit (us.sh): `FAIL func_8010D680: 82 of 119 words differ ... | words 22 ops 12 norm 12 | frame 80/80` (the 82 is the
  one-word shift; rows are the measure). Residuals as w36w: op narrowed into t7 (retail a1 = coloured web), as1 xbb
  hoists of `lui v1` (gFrameSeconds) and `move a1,zero; move a0,t0`. `if (op) {}` holders at three places and
  `entity_transform_apply(v, op + 1)`: 22-53 rows.

## What generalises

- **A precoloured register in the same uopt basic block forbids that colour.** uopt live ranges are block-granular:
  a call in the block (its v0 return, its a0..a3 arguments) forbids those registers to every web that lives in the
  block, even when the web starts after the call. force.sh shows it as a declined force whose `forbidden` mask
  survives moving every earlier web of that colour. Lever: a compiled-out block end between the call and the
  web (`do {} while (0);`, `if (msg) {}`, `if (call()) {}`) - func_800E73D8's last 4 words (w36w3 had searched
  spellings of the counter itself).
- **A single-occurrence global store through a materialised address (`lui v1; addiu v1; sw v0,0(v1)`) is a coloured
  lda web with a later occurrence.** A compiled-out read of the same global after the store (`if (G) {}`) extends
  the address web so uopt colours the store's piece - func_800979A0's last 12 words (no volatile device needed).
- **Commutative FP operand order again**: `gMaxSfx * 0.75f` vs MAT3/DC `0.75f * gMaxSfx` fixed the whole FP temp
  ring of _UpdateFx (51 -> 40 words); check the mul.s operand order against retail before colour work.
