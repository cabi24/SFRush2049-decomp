# w22c — instrumented IDO 7.1 (trace71) + the 7.1 allocation residuals

Worktree lane (branch `w22c`). Builder scratch `watchman2:~/rush2049/scratch/frontier/w22c` (base copy + rsync of
src/{blob,rom}, include, tools/{cloud,conveyor,asm-processor}, asm/us/{boot_tail,nonmatchings/rom},
cloud/work/boot_tail_promotion; `tgt/` = w17k fetch_targets.py; `lane` -> cloud/work/frontier/w22c copy;
`tools/ido-static-recomp/build71/out` -> the 4586d230 toolkit's ido/). Nothing locked, promoted, spliced or pushed.

## Headline

1. **trace71** (`cloud/work/frontier/tools/trace71/`, README there): traced IDO 7.1 uopt/ugen/as1 rebuilt from
   decompals/ido-static-recomp v1.2 (the release the pinned 7.1 binaries come from). Colouring trace + force oracle by
   procedure NAME with every variable web named (`vars71.sh`, from a -g3 build), callee/caller-saved cost records
   (`[CDX] calleecost`, `CUPLOG`), spill homes/area, ugen temp ring, as1 -R. Tests: `tests/conveyor/test_trace71.py`. Gate: all 18 boot-tail
   TUs byte-identical rel/rebuilt/traced-off/traced-on (54/54, `logs/fidelity71.log`); LOCK lines identical on 13
   locked sources with every trace on (`logs/lock_identity.log`).
2. **2 matches, 1,372 B** (static, lib_11640 + lib_1a660), both `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr
   identical`, with and without `-Wab,-r4300_mul`, TU identical, spliced TUs IDENTICAL (`prove.log`).

| function | B | state | trace71 cause | lever | LOCK (`-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`) |
|---|---:|---|---|---|---|
| func_80010E80 | 500 | MATCH | `count` p1 decision: caller cost 5 > callee cost 4 = max(4, 16 blocks/4) -> s1 (retail v1 with ugen saves); after merging X/count, size/loop index, count first: force v1 = 0 rows | mode-1 filter copy as a 4-trip loop -> 20 blocks -> callee cost 5 = caller cost, tie goes caller-saved v1 | `true=0 strict=0 reloc_blind=0 objdump-dr identical (139 rows)` |
| func_8001A270 | 872 | MATCH | w21b best_true0 code-identical; spill71: cfe area 32 vs 28 (7 locals vs retail's 6 slots); reusing `note` merges two webs (7.1: one web per variable) | entry index not a variable: `keymap[(u8)(key & 127)]` (CSE'd, same web, no slot); panning sum `int` declared in index's place | `true=0 strict=0 reloc_blind=0 objdump-dr identical (224 rows)` |
| func_80018634 | 840 | 127 rows (lead_jump_v1) | force oracle: exactly 4 colours (code a3, patterns a1, pattern a2, optional04 t0) give **0 rows**; root = patterns sees v1 free | not found (see below) | unchanged (w21a 780) |
| func_80018448 | 492 | 26 rows | colours all right (count t0, now t1 ...); ugen ring: retail pops t6/t7 at line 37 where ours pops t4/t5 | not found | unchanged (w21a 325) |
| func_80012D18 | 3148 | near miss, 15620 -> **12735** | `offset` split: tot 4 <= caller cost 5 (5 transit call blocks); retail t5 with 6 defs | `offset` assigned in every leaf (no top-level `= 0`): tot 7 > 5 -> t5 (`func_80012D18/best.c`) | `true=12735 strict=14066 reloc_blind=716 objdump-dr DIFFERS(679 rows) (810 rows)` |

## Integration (coordinator)

Both are static boot-tail functions, IDO 7.1, nothing superseded, no data, no header patch, no TU edit.

```
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
for f in func_80010E80 func_8001A270; do
  python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w22c/sources/$f.c:$f \
    --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71; done
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w22c/ctx71_O2.jsonl 0x11640 \
  --only func_80010E80 --prefix-dedup rom/lib_11640 --dry-run      # then without --dry-run
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w22c/ctx71_O2.jsonl 0x1a660 \
  --only func_8001A270 --prefix-dedup rom/lib_1a660 --dry-run
```

Both LOCK lines are identical with and without `-Wab,-r4300_mul`, so the default lock flagset works (lib_11640's
existing locks use it). Context rows: `ctx71_O2.jsonl` (2 rows, `ok`, `--compiler ido7.1`). Recompute:
`python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 OUT.jsonl
func_80010E80=cloud/work/frontier/w22c/sources/func_80010E80.c func_8001A270=cloud/work/frontier/w22c/sources/func_8001A270.c`.
Full proof: `sh lane/tools/prove.sh` on the builder (w21a recipe: context rows, check.py LOCK/TU with and without
`-Wab`, splice.py + tusplice.sh) -> `prove.log`.

Disclosures: no devices, no unused locals, no false prototypes. func_80010E80's parameter is `void *` like the TU's
caller prototype (`extern void func_80010E80(void *state)` in lib_11640) and its private view
`AudioConfiguration_80010E80` is reached by casts (a typed local copy adds a frame slot); `D_800382E8`/`D_800382EC`
declared with the TU's types. func_8001A270 keeps w21b's packed MusyX `Keymap` (`#pragma pack(1)`, byte loads of
priority) and its quirk `keymap[key].panning` (unmasked key, as retail).

## Notes per residual

- **func_80010E80**: w19a-w21a's "count spilled twice" = the p1 split of `count` (tot 2 <= 4); merging X and count
  (w20a's idea) gives the right web but callee cost 4 < caller cost 5 -> s1 (`ctrace71` + `[CDX] calleecost`
  `nblocks=16`, `XENV=CUPLOG=1`: units bb1 1 + bb2 2 + bb4 1 + transit bb3 1). With count forced to v1: 9 rows ->
  size reused as loop index (frame 48) -> count declared first (home sp+44) -> 0 rows. Then the cost: callee cost =
  max(4, nblocks/4); writing the mode-1 `filter[0..3]` copy as a loop gives 20 blocks (cost 5), and a 5/5 tie goes to
  the caller-saved register. Variants in `func_80010E80/v/` (git-ignored).
- **func_8001A270**: w21b's best_true0 differs only in sp offsets. spill71: `AREA f_readnxtinst area=32` vs 28 needed;
  homes otherwise equal. 7.1 builds one web per C variable (`note` reused for the panning sum = one web save 2.5 nocs
  6 -> t0, retail keeps the sum in v1 and note in t0), so the slot must come from a variable that can become an
  expression: the `u8 index`. `(key & 127)` alone changes code (101 rows); `(u8)(key & 127)` is the same web as the
  u8 local. A one-expression sum is reassociated by uopt ((a+b)-64, 85 rows), so the sum stays a two-statement int.
- **func_80018634**: on w21a's `lead_jump_v1.c`, `force71 "p1:w28=c6,p1:w61=c4,p1:w67=c5,p1:w82=c7"` = 0 rows. Forcing
  only patterns to a1 is not enough (pattern then takes v1): retail has v1 forbidden for both patterns and pattern, i.e.
  a v1 web interfering with the default case. Of the webs decided before them only the jump value (w40, v1, tie at
  save 200 with a lower web number) has v1, so in retail the jump web must be live in the default case's block.
  Not reproduced: u16/u32 jump, shared code/jump variable (merges -> code v1), jump in `sum`, dead reads of jump.
- **func_80018448**: no colour difference (all webs as retail: change t0, now t1, ...). Retail's ring at lines 46-51 cycles
  t2..t6 only (t7-t9 absent), ours the full ring: a ugen state question (ugt71 records saved under `build/trace71`).
- **func_80012D18**: `offset` was split (tot 4 <= caller cost 5; `[CUP] transit` = the 4 func_80012730 calls + 1).
  Retail has six `move t5,zero`/`subu t5` defs: one per leaf of the address/length chain (as1 moves two into delay
  slots). Our top-level `offset = 0` made the leaf ones redundant (uopt deletes them, so adding one changes nothing);
  assigning offset in each leaf instead raises its net to 7 > 5 -> t5 like retail: true 15620 -> 12735
  (`func_80012D18/best.c`, near miss). Remaining, traced: (1) retail frame layout = ours with two more 4-byte locals
  (one between `end` and `state`, one between `state` and `address`: retail state at -13, address at -28, length -36,
  offset home -40, mode -44, padded -52, cmd -56, mainRem -58, loopRem -60; filler locals reproduce every virtual
  offset but give frame 128: retail also has two fewer expression spill homes); (2) frames a1 / mainRem t1 / loopRem
  a0 forces are all declined (their neighbours hold those colours), i.e. more colouring differences upstream.

## What generalises

- **7.1 callee-saved cost = max(4, nblocks/4)**: a caller-saved web that retail keeps across calls (ugen save/reload)
  while ours goes to an s-register needs more blocks (a loop instead of unrolled statements) or fewer call crossings.
- **7.1 merges every use of a C variable into one web**: reusing a temporary for an unrelated value changes priorities;
  to drop a frame slot, turn a variable that is a pure expression into a (cast-exact) repeated expression.
- **Frame 8 B too big with identical code** = one 4-byte cfe local too many: `spill71.sh` `AREA f_readnxtinst`.
- **Oracle first**: `force71.sh` on the best source tells whether the whole residual is colouring (both matches and
  18634 were); `[CDX] calleecost` and `[CUP]` then say which cost decides.
