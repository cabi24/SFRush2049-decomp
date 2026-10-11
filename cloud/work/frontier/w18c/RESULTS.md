# w18c - static boot tail `func_*` under IDO 7.1 (wave 18)

Read-only except this lane dir and builder scratch `~/rush2049/scratch/frontier/w18c`. Nothing locked, promoted,
converted, spliced into src/, or committed; no edits to src/, asm/, include/, tools/, locks. Static numbers only.

## Headline

**39 functions, 7,080 B** production-ready in `sources/` (all IDO 7.1, flagset `-g0 -O2 -mips2 -G 0 -non_shared`),
every one `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical` against the reloc-aware inventory target
(toolkit 4586d230 = the 7.1 bundle `lock add --toolkit` uses), and all of them spliced together into their 10 TUs
(after the TU edits / header patch below) build byte-identical to the current TUs (`prove.log`). This includes all
nine `w17g/lockonly/` functions (2,532 B; not claimed by Astra in CLAIMS.md when taken). Static would move
489/668 (87,756 B) -> 528/668 (94,836 B) by inventory sizes; recount with `make progress`.

What generalises: under 7.1 the boot-tail D10 near-misses were mostly over-shaped for 5.3. Rewriting the natural C
closed most of them in 1-3 tries: drop temporaries D10 added (`key & 127` in the argument, no `whole`/`stream`
locals, no destination pointer), statement order (`result = 0` after the calls, link stores before the head store),
the TU's real declarations (`volatile` D_8004BE94, typed globals instead of `void *` + casts: casts flip addu operand
order), and narrow locals (`s16 delta`). 7.1 folds `(s16)word` / bare u8 assignment of a word into lh/lbu; retail's
`lw` + in-register narrowing comes from a mask (`(s16)(w & 0xFFFF)`, `w & 0xFF`).

## Results (verbatim, `prove.log`; TU column = w16g check.py TU step: real TU text before the slot + ctx preamble)

Category: A natural C, no devices. B type views only (struct tag / casts on whole pointers). +TU needs a TU edit
(below). +H needs the sequence_context.h patch. C shaping device (mask, disclosed). D relocation-label view (below).

| function | B | seg | cat | LOCK (check.py = lock add verify_entry) | TU |
|---|---:|---|---|---|---|
| func_80014E64 | 44 | 0x15a10 | B | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (11 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (11 rows) |
| func_80014E90 | 44 | 0x15a10 | B | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (11 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (11 rows) |
| func_80014EBC | 44 | 0x15a10 | B | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (11 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (11 rows) |
| func_80014EE8 | 44 | 0x15a10 | B | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (11 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (11 rows) |
| func_800205E4 | 44 | 0x207b0 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (8 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (8 rows) |
| func_80015318 | 48 | 0x15a10 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (12 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (12 rows) |
| func_8001C19C | 60 | 0x1a660 | D (w17g lockonly) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (18 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (18 rows) |
| func_8001C390 | 60 | 0x1cf90 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (18 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (18 rows) |
| func_80014E1C | 72 | 0x15a10 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (17 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (17 rows) |
| func_80019A60 | 72 | 0x1a660 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (19 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (19 rows) |
| func_80023D70 | 72 | 0x22300 | C | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (18 rows)` | check.py no-dedup redeclaration; whole-TU splice IDENTICAL |
| func_80014C60 | 76 | 0x11640 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (19 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (19 rows) |
| func_80018AEC | 80 | 0x17dc0 | B+H | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (21 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (21 rows) |
| func_8001BDB8 | 92 | 0x1a660 | B+TU (w17g lockonly) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (24 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (24 rows) |
| func_800223D0 | 108 | 0x22300 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (27 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (27 rows) |
| func_80019420 | 112 | 0x17dc0 | D | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (33 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (33 rows) |
| func_80019ED0 | 120 | 0x1a660 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (31 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (31 rows) |
| func_80023AD4 | 124 | 0x22300 | D | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (33 rows)` | check.py no-dedup redeclaration; whole-TU splice IDENTICAL |
| func_8001A5D8 | 128 | 0x1a660 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (33 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (33 rows) |
| func_800255F0 | 128 | 0x25bb0 | D | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (36 rows)` | check.py no-dedup redeclaration; whole-TU splice IDENTICAL |
| func_80023B50 | 140 | 0x22300 | D | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (37 rows)` | check.py no-dedup redeclaration; whole-TU splice IDENTICAL |
| func_80024FD4 | 152 | 0x25bb0 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (42 rows)` | check.py no-dedup redeclaration; whole-TU splice IDENTICAL |
| func_8001E7BC | 168 | 0x1f340 | D | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (49 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (49 rows) |
| func_800202C4 | 172 | 0x207b0 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (53 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (53 rows) |
| func_8001897C | 180 | 0x17dc0 | B+H (w17g lockonly) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (51 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (51 rows) |
| func_80023754 | 196 | 0x22300 | C+TU | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (49 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (49 rows) |
| func_80018184 | 200 | 0x17dc0 | B+H (w17g lockonly) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (54 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (54 rows) |
| func_8001D0F0 | 208 | 0x1cf90 | B+TU (w17g lockonly) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (55 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (55 rows) |
| func_800180A0 | 228 | 0x17dc0 | B+H (w17g lockonly) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (58 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (58 rows) |
| func_80023DB8 | 228 | 0x22300 | C | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (58 rows)` | check.py no-dedup redeclaration; whole-TU splice IDENTICAL |
| func_8001F6EC | 256 | 0x1f5b0 | B+TU | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (76 rows)` | check.py no-dedup redeclaration; whole-TU splice IDENTICAL |
| func_800232A4 | 268 | 0x22300 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (67 rows)` | check.py no-dedup redeclaration; whole-TU splice IDENTICAL |
| func_8001D764 | 332 | 0x1cf90 | B (w17g lockonly) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (89 rows)` | check.py no-dedup redeclaration; whole-TU splice IDENTICAL |
| func_8001B4A4 | 336 | 0x1a660 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (91 rows)` | check.py no-dedup redeclaration; whole-TU splice IDENTICAL |
| func_8001B5F4 | 336 | 0x1a660 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (91 rows)` | check.py no-dedup redeclaration; whole-TU splice IDENTICAL |
| func_8001FBB0 | 380 | 0x207b0 | B+TU | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (107 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (107 rows) |
| func_8001C580 | 496 | 0x1cf90 | A | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (136 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (136 rows) |
| func_80011F60 | 604 | 0x11640 | B+TU (w17g lockonly) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (170 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (170 rows) |
| func_800123A8 | 628 | 0x11640 | B+TU (w17g lockonly) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (173 rows)` | true=0 strict=0 reloc_blind=0 objdump-dr identical (173 rows) |

Whole-TU proof (`tools/prove.sh`, IDO 7.1 both sides, asm-processor like the Makefile ROM-TU rule; .text words +
relocation (type, symbol) + .data/.rodata/.bss), from `prove.log`:
```
== TU edits alone
lib_11640: IDENTICAL (4276 lines)  lib_1a660: IDENTICAL (2626)  lib_1cf90: IDENTICAL (1876)
lib_1f5b0: IDENTICAL (1152)        lib_207b0: IDENTICAL (661)   lib_22300: IDENTICAL (3382)
== all sources spliced (into the edited TUs)
lib_11640 IDENTICAL (4276)  lib_15a10 IDENTICAL (578)   lib_17dc0 IDENTICAL (2598)  lib_1a660 IDENTICAL (2626)
lib_1cf90 IDENTICAL (1876)  lib_1f340 IDENTICAL (154)   lib_1f5b0 IDENTICAL (1152)  lib_207b0 IDENTICAL (661)
lib_22300 IDENTICAL (3382)  lib_25bb0 IDENTICAL (960)
```
`hdr.log`: `lib_17dc0: header patch IDENTICAL (2985 dump lines)` (the only TU including sequence_context.h, built with
the original and the patched header). The "COMPILE FAIL redeclaration" TU rows are check.py appending the preamble
without promote_batch's dedup (same as w17g); the spliced TUs (`tools/splice.py` = w17g splice71.py = promote_batch
splice with `--prefix-dedup`) compile and are identical.

## Integration (coordinator; order matters)

1. **Header patch** (layout-neutral, `include_patch/sequence_context.h.diff`): name SequenceNode +0x0C..0x17
   (`deadline0C, fraction10, whole14`), SequenceContext 0x118/0x11C (`fraction118` u32, `whole11C` int) and
   0x568 (`SequenceTrack tracks568[64]`, new 0x28-byte typedef). `cp cloud/work/frontier/w18c/include_patch/sequence_context.h include/`
2. **TU edits** (`tools/tu_edits.py`, declaration moves/completions inside promoted blocks only; no passthrough line
   touched; proven byte-identical alone): `python3 cloud/work/frontier/w18c/tools/tu_edits.py src/rom` (repo root;
   rewrites lib_11640, lib_1a660, lib_1cf90, lib_1f5b0, lib_207b0, lib_22300 in place). Edits:
   lib_1f5b0 VoiceState byte +0x2E named `channel2E` and ChannelLink/D_80050440 hoisted above func_8001F6EC;
   lib_207b0 VoiceState+externs (func_8001FD2C block) hoisted above func_8001FBB0; lib_22300 Macro*_80023818
   typedefs + func_80023754 prototype hoisted above func_80023754; lib_1a660 ChannelState + D_8004F300 hoisted above
   func_8001BDB8; lib_1cf90 Vector hoisted above func_8001D0F0; lib_11640 AudioNode / AudioCacheNode completed to
   the full node layouts (same offsets for every field used), hoisted above func_80011F60 / func_800123A8, and the
   `extern void *` pool pointers (func_800121BC, func_8001261C blocks) given their element types.
   Then the ROM gate (`make` + `make test`) for 1+2 alone.
3. **Locks** (7.1 toolkit):
```
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
for f in $(ls cloud/work/frontier/w18c/sources | sed 's/\.c$//'); do
  python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w18c/sources/$f.c:$f \
    --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71; done
```
4. **Context rows**: `cloud/work/frontier/w18c/ctx71_O2.jsonl` (39 rows, all `ok`, `"compiler": "ido7.1"`, computed by
   `context_check.py --compiler ido7.1` on the builder with the patched header). Recompute after step 1 if wanted:
   `python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 OUT.jsonl fn=cloud/work/frontier/w18c/sources/fn.c ...`
5. **promote_batch** (dry run first):
```
SEGS="0x11640 0x15a10 0x17dc0 0x1a660 0x1cf90 0x1f340 0x1f5b0 0x207b0 0x22300 0x25bb0"
ONLY=$(ls cloud/work/frontier/w18c/sources | sed 's/\.c$//' | paste -sd,)
DEDUP=rom/lib_11640,rom/lib_15a10,rom/lib_17dc0,rom/lib_1a660,rom/lib_1cf90,rom/lib_1f340,rom/lib_1f5b0,rom/lib_207b0,rom/lib_22300,rom/lib_25bb0
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w18c/ctx71_O2.jsonl $SEGS --only $ONLY --prefix-dedup $DEDUP --dry-run
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w18c/ctx71_O2.jsonl $SEGS --only $ONLY --prefix-dedup $DEDUP
```
   `--prefix-dedup` is required for every TU (the proof used prefix dedup). The nine lockonly items supersede nothing
   locked (they were never locked); `w17g/lockonly/*.c` stay as history.
6. No unit_overrides (static), no data-symbol changes, no owned data (no source carries .data/.rodata/.bss).

## Shaping devices and views (all disclosed in the file headers)

- **Masks (C)**: func_80023D70 `(s16)(word[1] & 0xFFFF)`, func_80023754 `combination = word[1] & 0xFF`,
  func_80023DB8 `word[1] & 0xFF` as a u8 argument: retail loads the whole word and narrows in a register; 7.1 folds the
  unmasked forms to lh/lbu. (func_800232A4 reaches the same code with no mask: a u16 variable built in two statements.)
  func_80023DB8 also writes its jump as `index + program` (operand order) and keeps D10's 0/1-only switch.
- **func_80023754** has a used local `controller` declared between `index` and `combination`: it is real data flow
  (the translated controller), and its slot gives retail's byte slots (0x1F/0x1E/0x1D). No unused locals anywhere.
- **Relocation-label views (D)**, 6 fns, 732 B: the reloc-aware targets name some base+offset addresses by an
  interior label (splat symbol at that address). The natural C links to identical bytes but scores true=10..20
  reloc_blind=0. Following docs/reloc_reconciliation.md ("rewrite ... external symbol names to OURS") and w17g's
  func_8001C19C (`D_8004F314[][40]`), these address the data through the target's label:
  func_80019420 `D_800443E0[index][selector]` (= D_80043EB8[index].channels528), func_8001C19C (w17g, unchanged),
  func_80023AD4/80023B50 `(&D_8004BE78[16])[index - 16]` (= D_8004BE98[index - 16]; D_8004BE78 is a u32 in
  sequence_context.h, declared here as an s16 array view in another TU), func_800255F0 `D_80057430[token][0]`
  (= D_80056230[token].busy), func_8001E7BC `(&D_8002D452[-1023])[1023 - p]` (= D_8002CC54[1023 - p]).
  If the owner rejects label views, drop these six (the rest are independent); a scorer/target-naming change that
  treats symbol+addend by address would make the natural forms (recorded in each header) score 0.
- **Type views (B)**: func_80014E64..EE8 take `struct ResourceOffsets *` (completed by the TU's later typedef) and read
  the offset through an `unsigned int *` view; func_8001D764 casts `(LinkNode *)state` / `(SpatialState *)state`;
  func_8001F6EC casts its argument to func_8001EE9C's `VoicePrefix_8001EE9C *`; func_80023AD4/B50/D70 pass the voice as
  `VoiceVariables_80023AD4 *` / `VoiceState *`; func_8001BDB8 `(int)state->flags08 < 0` (the TU types flags08 u32).
  None changes code (LOCK identical with and without, where both compile).

## Not matched (near misses; stop rule reached)

| function | B | best | cause / next hypothesis |
|---|---:|---|---|
| func_800177EC, func_80017824 | 56+56 | true=120 | retail narrows both u8 params in registers (`andi a3,a0`; `andi a1,a1`), 7.1 reloads them from the home slots (`lbu 27/31(sp)`) for a bare pass-through; tried int/u16/char params, K&R, casts, masks, locals, -O1, unprototyped callee. Next: whatever makes uopt colour a single-use u8 param (an extra use?). |
| func_800217E4, func_80021764, func_80021844 | 96/128/136 | 165 (217E4) | retail `move a1,v0; andi a1,a1,0xff` (u8 index narrowed in its own register after a copy); every local/param form gives `andi a1,v0` or sinks the narrowing. Shared idiom of the three. |
| func_80023190 | 140 | 160 | the s8 gain lives in a coloured a2 (srl/sll/sra in place); all local/order forms give temps. |
| func_800154A4, func_8001536C | 208/312 | 50/60 | `addu a0,s0,tN` (group first) vs `addu a0,tN,s0` x5-6; every spelling/cast/local form canonicalises the fresh load first; E64's running-pointer lever makes the frame worse here. |
| func_80016E68, func_80016F80 | 120/152 | 225/240 | retail re-reads D_80042678 / D_8004269C after storing it (non-volatile addressing); volatile reloads but materialises the address. |
| func_8001DC08 | 472 | 140 | f20/f22 swapped for the two hoisted limits (upper loaded first but coloured second). |
| func_800230B0 | 224 | 120 | with `offset = word1 & 0xFF` the code is right except a frame 8 B larger in retail (40 vs 32) and one load order; unused locals do not move the 7.1 frame; next: find the two-slot local (an address-taken temporary?). |
| func_80021548 | 96 | code identical | jump table in its own .rodata (`jtbl_8002D930`): needs an owned-rodata slot (w17g osAiSetNextBuffer pattern). |
| func_8001661C, 800158D8, 80015F28, 80015720, 80015C0C, 800161A0 | 1,928 | reloc_blind=0 | relocation-label artifacts NOT expressible in C: the label falls on a compiler-made address (loop end pointer `&a[count-1]` = count variable's label, or the peeled first iteration's `a[0].id`). Code identical; needs the scorer/target fix. Best sources in `relocname/` (1661C/158D8/15F28 with the compare-order fix). |

Not attempted: the rest of inv_table above ~200 B with D10 7.1 scores >= 300 (10DD8, 16C20, 17040, 17720, 11910,
230B0, 23DB8, 232A4, ...), and 0x26f60 (unconverted asm segment).

## Files

`sources/` (39), `ctx71_O2.jsonl`, `include_patch/` (sequence_context.h + .diff), `include_orig/` (the header as on
master, for hdr.sh), `relocname/` (3 best sources, reloc-name-only), `prove.log`, `hdr.log`, `tools/`: `check.py`
(= w16g), `sc.sh` (LOCK+TU one fn), `dif.sh`/`rdiff.sh` (word / relocation diff), `run.sh` (ctx rows + check for all),
`splice.py` (= w17g splice71.py, path + TUSRC), `tusplice.sh`, `tu_edits.py`, `prove.sh`, `hdr.sh`, `d10.sh`.
Variants under `work/v/` (git-ignored). Builder scratch: `~/rush2049/scratch/frontier/w18c` (base copy + rsync of src,
include, tools/{cloud,conveyor,asm-processor}, asm; `tgt/` = inventory targets via w17k fetch_targets.py;
`tools/ido-static-recomp/build71/out` -> the 4586d230 toolkit's ido/).

## Disclosures

- Builder: two throwaway files were briefly created and removed under the builder's `/tmp` (`/tmp/w18c_t` typedef
  test, `/tmp/x`), outside my scratch. Nothing else outside `~/rush2049/scratch/frontier/w18c`.
- The Pi scratchpad holds the fetched target objects (read-only DB/blob-store reads).
- CLAIMS.md not edited; the lockonly nine were taken because no Astra claim existed at the time (checked twice).
- No permission denials.
