# Frontier wave 14 — agent w14za results (partial)

Builder scratch `~/rush2049/scratch/frontier/w14za` (copied from `base`; `src/blob`, `include`, `tools/cloud`,
`asm/us/blob` and `blob_matched.lock.json` synced from the Pi). Unit tag `w14za`. Nothing committed, spliced, or
copied into `src/`, `asm/`, `tools/`, `include/`, `cloud/matches/`. func_80087110, stat_race_update and
func_800FE5B0 not touched. No permission denials. Flags everywhere: `-g0 -O3 -mips2 -G 0 -non_shared`.

| Function | Bytes | State | Best | Scorer line |
|---|---:|---|---|---|
| `func_8010FBE0` | 128 | NOT MATCH, 16 of 32 words differ (was 30/32 at the start) | `func_8010FBE0/best2.c` | `score.py fn` / `FAIL`: `16/32 words differ` |
| `func_800C8918` | 628 | NOT MATCH, 21 of 157 words differ in the unit (no change) | `func_800C8918/best0.c` (= w6c) | `blob_unit --tag w14za score func_800C8918 --with …best0.c --neighbours`: `FAIL func_800C8918: 21 of 157 words differ` |

Neither function is a match or a provisional candidate. Nothing goes to `cloud/matches/` or `groups/`.

## func_8010FBE0 (single)

Copies an OSTask into D_80155238 and jams it into two queues. Retail runs the statements in source order
(`D_80155238.next = 0`, `D_80155288 = &D_80152750`, `D_8015528C = 0`, `D_80155240 = 2`, memcpy, two `osJamMesg`).

Batches (all scored with `vbatch.sh`, standalone `score.py` semantics via bscore):
- Round 0: w9d best, `score.py fn` = `30/32 words differ` (re-scored here, reproduced).
- Round 1 (`g1`, 144 variants): statement permutations, memcpy position 0–2, `OSTask *` vs `void *` parameter.
  Best 16 strict (`mnem-missing 9`).
- Round 2 (`g2`, 240 variants): permutations with memcpy at 0–4 (the source-order position was missing from
  round 1). Best still 16 strict (`mnem-missing 3`, `v0156`).
- Round 3 (`g3`, 1920 variants): all 120 orders of the four stores plus memcpy, times the `D_80155238.next` vs
  `D_80155238_next` symbol, `&D_80152750` vs `(OSMesgQueue *)` cast, `0` vs `(OSMesg)0`, `2` vs `2u`.
  Best still 16 strict (`v00912`, = `best2.c`).
- wbgen one-edit neighbourhood of best2 (`wb1`, 338 variants): no strict improvement (best 16).
- Diagnose (`tools.conveyor.pipeline.diagnose one func_8010FBE0 --source best2.c --flags ...`): verdict
  `mixed(constant:1, structural:6)`, frame delta 0, lever `none-known`, lanes pool slot 4 and temp.

Residual: the `lui at,0x8015` sharing. Retail uses one `lui at` for the stores to D_80155288 and D_8015528C (placed
before `lui t6`) and a separate `lui at` for D_80155238 and for D_80155240. Every ordering we tried gives the 288 and
528C stores their own `lui`, or moves the frame setup, so 16 words stay off. Not a colouring issue that forced
variants resolved; the next hypothesis is that retail's `D_80155288`/`D_8015528C` pair is one address web of a
struct-typed or array-indexed global (`((u32 *)&D_80155288)[0..1]`), which has not been tried.

Disclosure: best2 has no volatile, no unused locals, no padding. The 3 mnem-missing rows are the lui/addiu shape.

## func_800C8918 (group recipe, 628 B)

Unit baseline re-scored: `FAIL func_800C8918: 21 of 157 words differ` (`defined by c_best0.c (kept)`, 0 locked
bodies differ). The rows are the busy-wait START/END address materialisation (retail `lui t9; addiu v1,t9,lo`
then `move a1,v1`, START rematerialised with `lui v0; addiu v0`; ours computes END into v1 directly and keeps START
in a0). Also note `best0.c` declares `m` as `volatile`, which is not in its header; it must be disclosed or removed
before any landing.

Round 1 (`g1`, 64 variants): END forms (`D_801439D8`, `&D_80142DD8[128]`, `D_80142DD8 + 128`, byte-offset cast),
START forms (`D_80142DD8`, `&D_80142DD8[0]`), four loop shapes (nested do/while, while inner, for inner, for
outer), volatile on or off. Standalone scoring is meaningless here (the wrappers need the unit), so the variants
were scored with `blob_unit` on the Pi (the loop was still running when this was written; `g1/unit.txt`).
Results at report time (61 of 64; the last rows are still scoring in the background): best 21 of 157 (the baseline form, plus other variants that reproduce it), 23 for a
few, 71–73 with a changed compiled size (`compiled body is 155–165 words`). No variant beat 21. The batch is
partial: the remaining rows are in `g1/unit.txt`.

Not done: 3 rounds of 300+ variants, wbgen climbs, and a diagnose run on this function. Next hypothesis: END as
`start + 128` where start is a pointer parameter of an inlined helper (w6c's note), which would give the
`lui t9` shape.

## func_800C8918 — follow-up (coordinator protocol: diagnose, vgen rounds, volatile pricing)

**Diagnose** (`diagnose one func_800C8918 --source best0.c --flags "-g0 -O3 -mips2 -G 0 -non_shared"`):
verdict `mixed(constant:9, structural:11, register:27)`, frame_delta 8, lever `declare-the-pair-later`.
Caveat: the standalone diagnose compiles the file without the unit, so the wrappers are not inlined and 153 of
157 words differ. Its lever is not trustworthy for this function; the unit rows are the evidence.

**Standalone control**: `vbatch.sh` on best0.c gives `strict 155` (stable, reproducible), and 157 with
`--keep audio_effect_process,object_type1_create,object_type7_create`. The unit gives 21. Standalone
ranking is therefore not usable here; every round is scored in the unit (`unit_round.sh` in `g2/`, one
`blob_unit` run per variant, 4–5 s each when the Pi is not contended).

**Unit rows of best0** (`udiff.py --all`, tag w14za): 21 differing words, all in two places.
- +0x154..+0x18c: the busy-wait address setup. Retail computes END as `lui t9; addiu v1,t9` then copies it
  (`move a1,v1`) for the inner compare, and rematerialises START (`lui v0; addiu v0`). Ours keeps END in v1 and
  START in a0 with `move v0,a0`.
- +0x1a0 onward: the four `lw` into the t6–t9 ring are shifted by one register (t6 vs t7, t7 vs t8, ...)
  for the rest of the function. This is a consequence of the loop row, not a separate residual.

**Round 2** (`g2/`, 96 variants, unit-scored via `unit_round.sh`): END forms (4) x inner limit (`end`, a
copied `lim`, the global) x START form (2) x outer compare (2) x volatile on/off. The batch was still running at
report time: 18 of 96 scored (`g2/unit.txt`, `ROUND_DONE` marks the end). Best so far 21, no variant below 21.

**Volatile pricing** (from the first 18 rows of round 2): with `volatile ShutdownMsg *m` every row is 21 of
157; without it every row is 71 to 73 of 157 with a compiled body of 155 words (the `m->used` reload is then
CSE'd). The volatile earns about 50 words in this shape and is kept. It must be disclosed in the header.

**Not done**: rounds 3 and 4 (100+ variants each), wbgen climbs on best0, and the 3-round stop rule. The
unit scorer is slow on the shared Pi (about 4 s per variant when the box is free, 1 to 3 minutes per variant
during the batch). Neither the 300-variant total nor the 3-consecutive-round stop is met, so
func_800C8918 remains NOT MATCH, 21 of 157.

**Round 2 final** (`g2/unit.txt`, 96 of 96 scored): 16 variants at 21 of 157, 8 at 24, 16 at 71, 24 at 72, 32 at 73.
Every 21 is volatile `m` with the inner compare `m != end` (the copied `lim` and the global forms give 24, and
non-volatile gives 71 to 73). No variant is EQUAL. Round 2 gives no strict gain over the baseline 21, which is the
first of the three consecutive no-gain rounds. Total unit-scored variants for this function so far: 160 (64 in
round 1, 96 in round 2). Rounds 3 and 4 (wbgen climbs and vgen rounds) are not run, so the 300+ variant /
3-round stop rule is not yet met. func_800C8918 remains NOT MATCH, 21 of 157.
