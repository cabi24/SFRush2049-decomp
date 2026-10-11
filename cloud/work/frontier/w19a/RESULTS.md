# w19a - static boot tail, larger (>= 400 B) unmatched `func_*` under IDO 7.1 (wave 19)

Read-only except this lane dir and builder scratch `~/rush2049/scratch/frontier/w19a`. Nothing locked, promoted,
spliced into src/, or committed; no edits to src/, asm/, include/, tools/, locks. Static numbers only.
Proofs re-run against master `12c16181` (after w19b's 4-function lib_22300 promotion landed mid-wave).

Targets (`targets.txt`, 42 fns): w17g/inv_table.md rows >= 400 B that are not locked, not in w18c's matched set,
not in w18c's not-matched list (w19b's: func_80015720, func_8001DC08), not in the unconverted 0x26f60 segment.

## Headline

**9 functions, 5,448 B** production-ready in `sources/` (IDO 7.1, `-g0 -O2 -mips2 -G 0 -non_shared`), every one
`LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical` against the reloc-aware inventory target (toolkit 4586d230),
and all nine spliced together (promote_batch `--prefix-dedup` replica) into their six TUs build byte-identical to the
current TUs (`prove.log`). One TU edit (lib_22300, 2-line typedef hoist) is proven IDENTICAL alone.
Static by inventory sizes: 532/668 + 9 -> 541/668 (recount with `make progress`; w19b's promotions also landed).

Also: **6 more functions (4,792 B) are code-identical** (reloc_blind=0) but blocked by target-side relocation
artifacts or an owned jump table (`relocname/`, `ownrodata/`); see "Code identical, not lockable".

## Matches (verbatim, `prove.log`; TU column = whole-TU splice)

Category: A natural C. B type views (casts on whole pointers / private struct views). C shaping device (mask). +TU needs
the lib_22300 TU edit.

| function | B | seg | cat | LOCK (check.py = lock add verify_entry) | whole TU |
|---|---:|---|---|---|---|
| func_80023564 | 428 | 0x22300 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (110 rows)` | lib_22300 IDENTICAL (3382) |
| func_80025AB4 | 436 | 0x25bb0 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (158 rows)` | lib_25bb0 IDENTICAL (960) |
| func_80021F84 | 476 | 0x22300 | C | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (120 rows)` | lib_22300 IDENTICAL |
| func_8001D1F4 | 508 | 0x1cf90 | B | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (138 rows)` | lib_1cf90 IDENTICAL (1876) |
| func_8002193C | 608 | 0x22300 | B+TU | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (155 rows)` | lib_22300 IDENTICAL; TU edit alone IDENTICAL |
| func_80024988 | 616 | 0x22300 | B+C | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (161 rows)` | lib_22300 IDENTICAL |
| func_800139D4 | 688 | 0x11640 | B | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (193 rows)` | lib_11640 IDENTICAL (4276) |
| func_80021150 | 728 | 0x21210 | B | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (184 rows)` | lib_21210 IDENTICAL (1081) |
| func_80019490 | 960 | 0x17dc0 | B | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (261 rows)` | lib_17dc0 IDENTICAL (2598) |

check.py's own "TU:" row reads `COMPILE FAIL redeclaration` for six of them: it appends the ctx preamble without
promote_batch's prefix dedup (same as w17g/w18c); the spliced TUs (`tools/splice.py`, prefix dedup) are the proof.

Levers per function (all in the file headers): 80023564 test word1's low byte where used (no local), `(u8)lower` in
the clamp arm, command word built note-first. 80025AB4 service hooks by their own symbols D_80038014/18/1C (the D10
struct view of D_80038000 names `D_80038000+0x14`: true=370), no byte_count local, one `return 1` after both
branches. 80021F84 per-path `return 0`; jump index `word[1] & 0xFFFF` (C). 8001D1F4 outputs declared in retail slot
order + a real `handle` local (frame 72); list links typed StateNode (a cast on D_8004FD50 itself costs true=280).
8002193C no `result` local (`return deadline != 0` / `return 0`). 80024988 `u16 macro`, no `external` local, locals
state/macro/priority/program/index, SECTION stored before saved/child/parent, limit passed as `packed & 255` (C).
800139D4 `commands` not `&commands[0]` (addu order), loop length `unsigned int`. 80021150 `* 2` not `+= tmp`,
`value - 8192 + tmp`. 80019490 pending mark stored before the flag is cleared.

## Integration (coordinator; order matters)

1. **TU edit** (layout-neutral; `tu_patch/lib_22300.c.diff`, or `python3 cloud/work/frontier/w19a/tools/tu_edits.py src/rom`
   from the repo root, rewrites lib_22300 in place): hoist func_80021B9C's two typedef lines (MacroState, MacroCommand)
   above the func_8002193C slot - func_8002193C is defined with the TU's later prototype
   `u8 func_8002193C(MacroState *, MacroCommand *)`. `prove.log`: `lib_22300: IDENTICAL (3382 lines)` with the edit alone.
   ROM gate for it alone if wanted. No header patch.
2. **Locks** (7.1 toolkit):
```
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
for f in $(ls cloud/work/frontier/w19a/sources | sed 's/\.c$//'); do
  python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w19a/sources/$f.c:$f \
    --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71; done
```
3. **Context rows**: `cloud/work/frontier/w19a/ctx71_O2.jsonl` (9 rows, all `ok`, `"compiler": "ido7.1"`, computed on the
   builder against master 12c16181 BEFORE the TU edit, as the proof used; dedup removes the hoisted typedefs).
   Recompute: `python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 OUT.jsonl fn=cloud/work/frontier/w19a/sources/fn.c ...`
4. **promote_batch** (dry run first; `--prefix-dedup` required for every TU):
```
SEGS="0x11640 0x17dc0 0x1cf90 0x21210 0x22300 0x25bb0"
ONLY=$(ls cloud/work/frontier/w19a/sources | sed 's/\.c$//' | paste -sd,)
DEDUP=rom/lib_11640,rom/lib_17dc0,rom/lib_1cf90,rom/lib_21210,rom/lib_22300,rom/lib_25bb0
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w19a/ctx71_O2.jsonl $SEGS --only $ONLY --prefix-dedup $DEDUP --dry-run
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w19a/ctx71_O2.jsonl $SEGS --only $ONLY --prefix-dedup $DEDUP
```
5. No unit_overrides (static), no data-symbol changes, no owned data (no source emits .data/.rodata/.bss: the spliced
   TUs' data sections are identical). Nothing superseded (none of the nine was locked).

## Shaping devices and views (disclosed in each header)

- Masks (C): func_80021F84 `command->word[1] & 0xFFFF` (retail lw + andi; `(u16)word[1]` folds to lhu, true=745);
  func_80024988 `packed & 255` as the u8 argument (retail `move a1,s0; andi a1,a1,0xff`; `packed` / `(u8)packed`
  narrow straight from s0, true=420).
- Type views (B), no code (LOCK identical with and without where both compile): func_80021150 `void *` params as the
  TU's later prototype declares them + casts to the TU's ControlInput and a private packed VoiceState_80021150 (the TU's
  VoiceState follows the slot); func_80019490 `void *` param + typed local `state` (a real used local; declared after
  `index` it takes the slot D10's `context` pointer had) and a SequenceRequest view of sequence_context.h's
  `unknownFC8` (callee prototypes as the locked definitions: casts to `unsigned int` at the func_8001558C/800156E8
  calls); func_800139D4 casts the TU's `unsigned short *` D_800382D0 / `void *` D_800382E8 to D10's block view;
  func_8001D1F4 private emitter view whose links are `StateNode *`, `target` cast at the list / func_8001C860 /
  func_8001CCDC uses; func_8002193C packed MacroState_8002193C view by cast at each use; func_80024988 one cast of
  `&D_8004BEB8[index]` (TU VoiceState) to its packed view, casts to MacroState at two calls.
- No unused locals, no pad arrays, no false prototypes, no implicit declarations, no compiled-out blocks.
  func_80021150 keeps retail's uninitialised `combined` on an unsupported first combine mode (as D10).

## Code identical, not lockable (target-side artifacts; best sources kept)

| function | B | cause | best LOCK |
|---|---:|---|---|
| func_800114C0 | 904 | target pairs the float->u32 `lui at,0x8000` with the following `addiu ...,63` as a HI16/LO16 to D_8000003F | `LOCK: true=15 strict=15 reloc_blind=2 objdump-dr DIFFERS(70 rows) (260 rows)` (D10, `relocname/`) |
| func_80011104 | 840 | same artifact twice (`+1`); `(unsigned short)` conversion fixes the code | `LOCK: true=30 strict=30 reloc_blind=2 objdump-dr DIFFERS(142 rows) (257 rows)` |
| func_8001BE14 | 904 | loop end `&D_8004F300[32]` labelled D_8004F800 (3 sites) + own jump table jtbl_8002D8E8 | `LOCK: true=30 strict=30 reloc_blind=0 objdump-dr DIFFERS(11 rows) (242 rows)` |
| func_8001B9F8 | 960 | same D_8004F800 label (3 sites) + own jump table jtbl_8002D8D0 | `LOCK: true=30 strict=30 reloc_blind=0 objdump-dr DIFFERS(11 rows) (256 rows)` |
| func_80013DEC | 780 | the first D_80038292 read has NO relocation in the target (splat left `lui 0x8004; lhu -32110` unpaired) | `LOCK: true=10 strict=10 reloc_blind=0 objdump-dr DIFFERS(225 rows) (253 rows)` |
| func_80023BDC | 404 | own jump table jtbl_8002D950 in the global .rodata (needs an owned-rodata slot, as w18c func_80021548) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(2 rows) (105 rows)` |

Fixes made on the way (in the kept sources): 8001BE14/8001B9F8 `u32 i` (MusyX synthPauseVolume/synthVolume: retail
narrows and re-homes the group parameter; true 265/470 -> 30); 80013DEC `for (...; offset += 192, count++)
func_80013D70(index++, offset);`, `D_80038360 != target`, buffer swap indexed by `D_8003829E ^ 1` directly (355 -> 10);
80023BDC `s16 left, right` (1000 -> 0). These need the scorer/target fix (relocation by address) or the owned-rodata
path, not more C.

## Near misses and triage (stop rule reached or triage only)

| function | B | best | cause / next hypothesis |
|---|---:|---|---|
| func_8001824C | 508 | true=80 (`func_8001824C/best.c`) | s16 steps as `change <<= 1; change >>= 1` and `now` after `deadline` fixed 305 -> 80; residual = temp ring one register late from the reloaded-note block on (retail's +1 cursor step gets t8). TU also needs SequenceTrack +0x10..0x27 names. |
| func_80018448 | 492 | true=380 (`func_80018448/best.c`) | sibling of 8001824C (same levers 595 -> 380); the s16 increment and the secondTime20 load swap a0/t0 colours; retail hoists the +2 step. |
| func_80010E80 | 500 | true=460 (`func_80010E80/best.c`) | separate `size` for count*2 fixes registers to the bzero call (810 -> 460) but the frame is 56 vs 48: our count is spilled twice (home + temp). |
| func_800252AC | 552 | true=960 (`func_800252AC/best.c`, TU types) | schedule only: our u8 parameter reload `lbu option` is hoisted above the rate test; func_80025EB0 args loaded count-first. ~25 variants. |
| func_80011A64 | 440 | true=775 (D10) | schedule in the ratio block: retail hoists `mtc1 step`, 65536.0f in $f8 (ours $f14); 110 vs 109 insns. |
| func_80011D74 | 492 | true=420 (D10) | schedule only: retail `beqzl loop_length` after the multiply (likely-slot = else path's `lw length`). |
| func_80015D68, func_80015A0C | 448/512 | 1310/2600 (D10) | aligned lw/sw entry copy vs packed u16 field access; first `.id` read is a label view (relocname class). |
| func_80017D38, func_80018634 | 872/840 | 715/805 (D10) | need the sequence_context.h views; 18634 is allocation-only (0 opcode diffs, diagnose). |
| func_80022CD4 | 592 | 980 (D10) | D10 uses a label-address table cursor (`&D_8002D476`); MusyX pitch-table lookup. |

Not attempted (triage by `diagnose` only, `tools/triage.sh`): 80022160, 8001EF8C, 80016998, 80019F48, 8001E0E0,
80022874, 8001DA74, 8001A270, 8001671C; no D10 source: 8001C860, 800178B0, 8001F13C, 80012730, 80023E9C, 8001A658,
80012D18. All D10 re-scores under 7.1: `d10all.log`.

## What generalises

- **`x & 255` passed to a u8 parameter reproduces 7.1's copy-then-narrow** (`move a1,s0; andi a1,a1,0xff`), where a
  bare or `(u8)` argument narrows straight from the source register (func_80024988). Probably the lever for w19b's
  "copy-then-narrow" group (func_800217E4/80021764/80021844).
- Several D10 near-misses were D10's control-flow shaping, not codegen: one `result` variable vs `return` per path
  (8002193C, 80021F84, 80025AB4 is the reverse: one `return 1` after the branches).
- Struct views of a global hook table (`D_80038000.allocate`) name `D_80038000+0x18`; the target relocates to the
  per-word symbols - use them (80025AB4).
- Declaration order of locals still decides slots under 7.1 (8001D1F4 outputs, 80024988), and a real local can be the
  frame residual (8001D1F4 `handle`).
- s16 arithmetic in steps (`c = x; c <<= 1; c >>= 1;`) keeps 7.1's per-step sll/sra truncations; the one-expression
  cast form folds them (8001824C/80018448).
- Signed `int i` vs `u32 i` decides whether 7.1 narrows and re-homes a u8 parameter used only after a call
  (8001BE14/8001B9F8).
- Watch for target relocation artifacts before tuning: float->unsigned conversion constants paired with a nearby
  addiu (D_8000003F/D_80000001), loop-end labels on the next symbol (D_8004F800), and an unpaired absolute load
  (80013DEC). `reloc_blind=0` with a small true is the signal.

## Files

`sources/` (9), `ctx71_O2.jsonl`, `prove.log`, `tu_patch/lib_22300.c.diff`, `relocname/` (5), `ownrodata/` (1),
`func_*/best.c` (near misses), `d10all.log`, `targets.txt`. Tools (`tools/`): `prove.sh` (ctx rows + LOCK/TU + TU edit
alone + all spliced), `tu_edits.py`, `splice.py`/`tusplice.sh`/`check.py`/`sc.sh`/`dif.sh`/`rdiff.sh`/`d10.sh` (w18c),
`t.sh`/`vb.sh` (Pi: push + score one / a directory), `ct.sh`, `obj.sh`/`diag.sh`/`triage.sh` (workbench diagnose),
`d10all.sh`. Variants under `func_*/v/` (git-ignored). Builder scratch: `~/rush2049/scratch/frontier/w19a` (base copy +
rsync of src, include, asm, tools/{cloud,conveyor,asm-processor}, workbench, locks; `tgt/` = inventory targets via
w17k fetch_targets.py; `tools/ido-static-recomp/build71/out` -> the 4586d230 toolkit's ido/).

## Disclosures

- Mid-wave the coordinator landed `12c16181` (w19b, 4 lib_22300 functions); the builder tree was re-synced and every
  proof re-run against it (`prove.log` is the post-sync run).
- One sweep larger than intended: 120 declaration-order permutations for func_80024988 (found the slot order; the match
  itself came from a traced lever afterwards).
- The Pi scratchpad holds fetched target objects (read-only DB/blob reads). CLAIMS.md not edited.
- No permission denials.
