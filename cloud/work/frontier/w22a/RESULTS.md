# w22a - static boot tail: func_800178B0 (seqStartPlay) and func_8001F13C (voiceAllocate) (wave 22)

IDO 7.1, `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`, toolkit 4586d230. Builder scratch
watchman2:~/rush2049/scratch/frontier/w22a (base + rsync of src/blob, src/rom, include, tools/{cloud,conveyor,
asm-processor}, asm/us/{blob,boot_tail,nonmatchings/rom}, locks, cloud/work/boot_tail_promotion; 7.1 symlinked at
tools/ido-static-recomp/build71/out; trace toolkit installed with `install.sh ... --reuse wtk`). Read-only elsewhere:
nothing locked, promoted, spliced into src/ or committed. Static numbers only.

## Headline

**1 function MATCH, 1,160 B** (func_800178B0, no devices, no unused locals). func_8001F13C moved from
true=6630 to **2420** with the colouring cause traced and fixed; the remaining residual is a uopt PRE insertion.

| function | B | state | flags | scorer line (check.py = lock add verify_entry) |
|---|---:|---|---|---|
| func_800178B0 | 1160 | **MATCH** | -g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (307 rows)`; `lib_17dc0: IDENTICAL (2598 lines: .text words+relocs, data sections)` |
| func_8001F13C | 1456 | near miss | same | `LOCK: true=2420 strict=2444 reloc_blind=138 objdump-dr DIFFERS(201 rows) (436 rows)` (start: true=6630 strict=6776 reloc_blind=252, 424 rows) |

(The check.py `TU:` line for func_800178B0 reports `COMPILE FAIL ... redeclaration of 'MapRecord'`: check.py does not
apply promote_batch's prefix dedup; the source repeats the TU's `MapRecord` typedef verbatim. The real splice replica
`tools/splice.py` dedups it and the spliced TU is IDENTICAL; same situation as w21c's func_80012730.)

## Integration (coordinator)

func_800178B0: no TU edit, no header patch, no owned data, nothing superseded. Needs `sequence_context.h` (already
included by lib_17dc0.c) for `SequenceContext`/`SequenceNode`/`D_80043EB8`; everything else is private
`*_800178B0` views plus the TU's verbatim `MapRecord` typedef (removed by prefix dedup).
```
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w22a/sources/func_800178B0.c:func_800178B0 \
  --flags "-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul" --toolkit $TK71
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w22a/ctx71_O2.jsonl 0x17dc0 \
  --only func_800178B0 --prefix-dedup rom/lib_17dc0 --dry-run
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w22a/ctx71_O2.jsonl 0x17dc0 \
  --only func_800178B0 --prefix-dedup rom/lib_17dc0
```
Context row: `ctx71_O2.jsonl` (1 row, `ok`, `ido7.1`). Recompute: `python3 cloud/work/boot_tail_promotion/context_check.py
--compiler ido7.1 OUT.jsonl func_800178B0=cloud/work/frontier/w22a/sources/func_800178B0.c`. Proof: `prove.log`
(`tools/prove.sh` on the builder against the Pi tree as of this wave's start). If the segment flagset still lacks
`-Wab,-r4300_mul` (w21c Integration 2b), the plain flagset gives the same object here (no back-to-back mul.s); the lane
rules ask for the token in every 7.1 proof.

## func_800178B0 (seqStartPlay): MATCH

- Trace: IDO 5.3 -O2 compiles this function to exactly the same code as 7.1 with the same 2-word residual, so the
  5.3 traced uopt works as a proxy (`ctrace.sh` on a single-file snapshot of the preprocessed reduced TU,
  `tools/pp.sh` + `snap.sh LABEL FILE KEEP`). In w21c's best, nseq is not a web at all: it is copy-propagated into
  the `&D_80043EB8[i]` expression web (w4, s3).
- A `volatile` copy declared 9th reproduced the code exactly (frame +8): retail's sp+0x74 is a declared local holding
  nseq that uopt cannot substitute at its last use.
- The natural reason (GameCube `seqId = GetPublicId(seqId); return seqId;`): the slot index is both the search
  variable and the return variable, `i = func_800175B4(i);` redefines it before `nseq->freeFC1 = 0`, so
  `&D_80043EB8[i]` is no longer available there; nseq keeps one late use, its whole-range net is below the callee
  cost, it is split, every piece but the tail gets s3, and the tail reloads the home.
- Frame: nseq must be the 9th declared word with one more word after it: `arr` (GameCube's `arr = (ARR *)song`, used
  for every song access), tracktab, trans, page, entry, i, j, t, nseq, then the u8 `seqId` copy passed to the u8
  callees. mdlocals (w21b) confirmed the slot order. No devices, no unused locals.

## func_8001F13C (voiceAllocate): near miss, 2420 / 201 rows (`func_8001F13C/best.c`)

IDO 5.3 reproduces the same allocation problem, so the 5.3 tracer was the oracle (`tools/cost53.sh`, `costdir.sh`).
1. **Fixed: callee-saved cost.** The D_80050548 / D_80050648 address webs have totalsave 28.0 against a callee
   cost of 28.5. That cost is `clamp(blocks * 0.25, 4, 60)` (uopt L471104: f24 = nblocks * 0.25, added as the first-use
   surcharge at L471afc), so they split instead of taking s2/s3. Retail has >= 3 fewer uopt blocks. Folding the two
   `if (flags24 & 2) continue;` tests and the skip loop's `block` test into the guarding `&&` conditions
   (`if (ALLOC(i).block == 0 && (!type_alloc || fxFlag == ...external4C) && !(...flags24 & 2))`, -0.25 each, all
   three code-neutral in 7.1) brings it to 27.6. Both bases then get s2/s3, the voice record gets s4 and the frame is 56
   as in retail: 6630 -> 2520. The GameCube boolean `type_alloc = (a >= b && c > b)` also lowers the block count, but
   it compiles branchless (slt/xori), and retail has the ternary's branches.
2. **Fixed:** `sfv = &D_80050440[voice]` (one CSE'd address, retail's v1) -> 2420.
3. **Open: PRE insertion.** pretrace (5.3): `mpy(voice,416)` / `ixa(D_8004BEB8, ...)` antloc {31,75,88,101}, DELETE 88,
   INSERT 60,87. The `_update` multiply is partially available from the skip loop's `D[voice].value28` compare (75),
   so uopt hoists it into `_update`'s two predecessors (the D_800504C0 path and the loop exit) as a shift expansion.
   Retail multiplies at `_update` with the 416 web in a2.
   - Confirmed: a separate loop variable (`best`, then `voice = best`) kills availability and gives retail's
     multu at `_update`, but it costs a move and a recolouring (3370).
   - `D_8004BEB8[(u8)voice]` at `_update` also stops the hoist (2100, 166 rows), but it is a device and expands the
     multiply.
   - No natural form found yet: tried GameCube 1.5.4 ordering, else-if, if/else nesting, return/goto forms,
     pointer-add forms (these remove the `ixa` hoist but not the `mpy` hoist), and u32 voice.
   - The same mechanism hoists fxFlag's narrowed copy into the unlink block (expr 84 INSERT 92; ours `move v1,s1`,
     retail tests s1). It may only be colouring: retail could have the hoisted temp in s1.
   - The maxVoices spill slot (24 vs 28: one more coloured expression web in ours) and the temp-ring rotations
     follow from these.
4. **Next hypothesis:** find what kills `voice` availability on the skip-loop exit path in retail without a second
   variable. Candidates: a loop shape that reassigns voice on every exit, or an older MusyX skip loop. Alternatively,
   use w22c's instrumented 7.1 uopt (PRE vectors) once it exists.
   Stop rule reached (~3 h on this function).

## What generalises

- **IDO 5.3 is a usable tracing proxy for IDO 7.1 boot-tail functions** when `dif53.sh` (5.3 -O2 of the same reduced
  TU) shows the same residual. Run the wave-15 trace toolkit on a single-file snapshot (`tools/pp.sh` preprocesses the
  reduced TU on the builder; `snap.sh LABEL FILE KEEP`), then `ctrace.sh`/`pretrace.sh`. This closed func_800178B0 and
  explained func_8001F13C.
- **The callee-saved cost is `clamp(nblocks/4, 4, 60)`**, so source block count is a colouring lever. `if (x)
  continue;` inside a guarded block, and nested `if`s, each cost a uopt block (0.25). Folding them into one `&&`
  condition keeps the code and lowers the cost. A "just below threshold" constant web (totalsave 28 vs cost 28.5) is
  the signature.
- **A memory-homed pointer that duplicates a coloured register value is a variable whose last use comes after one
  of its operands is redefined.** Here: index reused as the return value (`i = f(i);`). Look for GameCube-style
  `seqId = GetPublicId(seqId)`.
- A `volatile` (or `if (&x) {}`) copy at the right declaration slot is a quick oracle for "this is a declared local
  in memory". It is diagnostic only.
- `costdir.sh` (5.3 callee cost and address-web decisions + 7.1 score per variant) ranks block-count variants in
  seconds.

## Disclosures

- func_800178B0: no shaping devices, no unused locals, no pad arrays, no false prototypes, no implicit int.
  Private type views (`Page_/MidiChannel_/MidiSetup_/VolDef_/PlayPara_/Arr_/Track_/TrackState_/Seq_800178B0`, as in w21c),
  the TU's `MapRecord` typedef repeated verbatim (prefix dedup removes it).
- func_8001F13C/best.c is a near miss, not delivered for integration. It uses w21c's `volatile D_8004BE94` (as
  lib_113e0 declares it) and the private packed views.
- Volatile/`&X` and `(u8)voice` variants were oracles only; neither is in a delivered source.
- No permission denials. Builder: only `~/rush2049/scratch/frontier/w22a` (plus mktemp under the builder's /tmp from
  check.py/dif.sh, as earlier lanes). MusyX GameCube sources (doldecomp/ttyd, already in the session scratchpad from
  w21c) read as reference only.

## Files

`sources/func_800178B0.c` (MATCH), `ctx71_O2.jsonl`, `prove.log`, `func_8001F13C/best.c` (near miss),
`func_800178B0/best.c` (w21c's start point), `tools/` (w21c's set renamed, plus `dif53.sh` 5.3 diff, `pp.sh` builder
preprocess, `cost53.sh`/`costdir.sh` 5.3 colouring summary, `md.sh`/`mdlocals.py` from w21b). Variants under `*/v/`.
