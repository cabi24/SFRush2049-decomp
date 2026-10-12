# w23c - static boot tail: func_8001E0E0, func_80019F48 + triage of the remaining boot tail (wave 23)

Read-only except this lane dir and builder scratch `~/rush2049/scratch/frontier/w23c`. Nothing locked, promoted, spliced
into src/, or committed. IDO 7.1 toolkit 4586d230 (`-g0 -O2 -mips2 -G 0 -non_shared`, `-Wab,-r4300_mul` where noted;
it is a NEUTRAL_FLAGS flag), IDO 5.3 for lib_10d60. Static numbers only. Proofs replayed against master `dbc211cf`
(after w23a's func_80018634 promotion), `prove.log` (builder `sh lane/tools/prove.sh`).

## Headline

**13 functions, 5,212 B** (the two targets + 11 from the triage). Every one is code identical (reloc_blind=0) under the
TU's pinned compiler; packaging class per row:

| function | B | TU | class | scorer line (check.py = lock add verify_entry) / proof |
|---|---:|---|---|---|
| func_8001E0E0 | 864 | lib_1ece0 | owned .rodata (5 literals) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(14 rows) (230 rows)` (rows = `.rodata` vs D_8002D910..D_8002D920); linked TU .text == baserom, .rodata 0x8002D910+0x14 == baserom |
| func_80019F48 | 808 | lib_1a660 | MATCH | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (207 rows)`; lib_1a660 IDENTICAL |
| func_8001605C | 324 | lib_16320 | MATCH | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (92 rows)`; lib_16320 IDENTICAL |
| func_8001E50C | 380 | lib_1ece0 | MATCH | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (100 rows)` (flags + r4300_mul) |
| func_800233B0 | 368 | lib_22300 | MATCH | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (96 rows)`; lib_22300 IDENTICAL (its last slot) |
| func_80017720 | 204 | lib_17dc0 | MATCH | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (50 rows)`; lib_17dc0 IDENTICAL |
| func_80015A0C | 512 | lib_16320 | relocname (+TU edit) | current `LOCK: true=20 strict=20 reloc_blind=0 objdump-dr DIFFERS(6 rows) (146 rows)`; re-split `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (146 rows)` |
| func_80015D68 | 448 | lib_16320 | relocname (+TU edit) | current `LOCK: true=20 strict=20 reloc_blind=0 objdump-dr DIFFERS(6 rows) (130 rows)`; re-split `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (130 rows)` |
| func_80016C20 | 192 | lib_16320 | ownbss | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(17 rows) (66 rows)` (rows = `.bss`+N vs D_80042630..40) |
| func_80016CF0 | 336 | lib_16320 | ownbss | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(13 rows) (101 rows)` |
| func_80017040 | 200 | lib_16320 | ownbss | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(4 rows) (61 rows)` |
| osPfsIsPlug (0x80010160) | 396 | lib_10d60 (5.3) | no inventory target | ultralib verbatim: `osPfsIsPlug: target 99 words, ours 99, differing 0` (tools/asmcmp.py vs the retail .s); lib_10d60 IDENTICAL |
| __osPfsRequestData (0x800102EC) | 180 | lib_10d60 (5.3) | no inventory target | `__osPfsRequestData: target 45 words, ours 45, differing 0`; lib_10d60 IDENTICAL |

With all six lib_16320 bodies spliced (1605C + ownbss trio + relocname pair, edited TU) lib_16320 has **no GLOBAL_ASM
left**; linked with its .bss at 0x80042630 its .text sha1 b777c6b2... == baserom 0x16320+0x1AA0 and .bss = 0x80.
lib_22300 also becomes all-C with func_800233B0.

## Levers (traced; details in each file header)

- **func_8001E0E0** (MusyX hw_volconv CalcBus ancestor; w21b 2215, "FP colouring only"). ctrace71: 7.1 colours this
  function entirely in p2, **in web-number order**, and constant webs are numbered after every variable/expression
  web. Retail's $f20 (callee-saved) holder of 32767 is therefore a *constant* web: D_8002D910..D_8002D920 are the
  function's own float literals (`46fffe00 3f3538ef 3f3538ef bf3538ef 3f3538ef`; 7.1 does not merge equal literals:
  the compiled .rodata is byte-identical). Natural literals 32767.0f/0.7079f/-0.7079f: 2215 -> 1565; the front-surround
  fraction through `fraction` (a 64-variant sweep of inline-vs-variable over the six interpolations: only that one
  matters, its expression web is then numbered before factor's) -> 1260; `-Wab,-r4300_mul` (retail's nop after
  mul.s) -> 0. An extern or a `scale` variable is web 0 and takes $f0 (925..2910). force71 was not needed.
- **func_80019F48** (MusyX StartLayer; w21b 2235/2453). (1) The key-range test as one `&&` condition around the body
  (no `continue`): with `continue` the count reload is a coloured PRE web (v1) (2235 -> 695). (2) `priority =
  ((packed >> 8) & 255) + layer->priority` as one expression; operand orders `layer->transpose + (key & 127)`,
  `layer->volume * volume` (-> 635). (3) MusyX's `switch (layer->id & 0xC000) { case 0: ... break; }`: ctrace showed i
  at tot 31/11 blocks vs channel/set/&D_8004BEB8/416 at 30/10; the switch's blocks make i's save beat theirs (i = s4
  as retail; force71 with the five retail colours confirmed the colour part) (-> 210). (4) `layer = &layers[i]` from
  a separate `layers` call result: the strength-reduced pointer starts in the loop preheader (retail's `move s1,v0`
  after the guard) and `layers` is the extra 4-byte local of retail's 176-byte frame (-> 0).
- func_8001605C (dataInsertSDir): MusyX local order (frame homes) + pointer increment in the for header.
- func_8001E50C (pitch from sample info): `note != reference`; the semitone ratio as a conditional expression
  multiplied in place (a named float is a variable web and goes second in mul.s); `-Wab,-r4300_mul`.
- func_800233B0 (envelope macro command): its locked sibling func_800232A4's idiom `curve = (u8)w1; curve |= (u8)(w0
  >> 24) << 8;` (610 -> 0). The TU prototypes it `(MacroState *, MacroCommand *, u32)`: fields via a cast-macro view.
- func_80017720 (DoPrgChange): `(macro << 16) | (priority << 8) | maxVoices` in field order (925 -> 0).
- func_80015A0C / func_80015D68 (dataInsertLayer / DATA_TAB insert): w21b's 1671C recipe (MusyX shape, long locals,
  packed-fields/word-union); 15D68's existing-entry path calls func_800145DC *before* the reference increment.
- func_80016C20 / 80016CF0 / 80017040 (dataGetMacro / dataGetSample / dataGetFX): MusyX's function-local statics are
  exactly retail's D_80042630..D_800426A0 (each function's static block 8-aligned in the TU).

## Integration (coordinator; static only, no unit_overrides)

```
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
L=cloud/work/frontier/w23c
```
**A. Plain matches** (ctx `$L/ctx71_O2.jsonl`, rows ok, ido7.1; recompute with
`python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 OUT.jsonl fn=$L/sources/fn.c ...`):
```
for f in func_80019F48 func_8001605C func_800233B0 func_80017720; do
  python3 -m tools.conveyor.pipeline.lock add $L/sources/$f.c:$f --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71; done
python3 -m tools.conveyor.pipeline.lock add $L/sources/func_8001E50C.c:func_8001E50C --flags "-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul" --toolkit $TK71
python3 cloud/work/boot_tail_promotion/promote_batch.py $L/ctx71_O2.jsonl 0x1a660 0x16320 0x22300 0x17dc0 0x1ece0 --only func_80019F48,func_8001605C,func_800233B0,func_80017720,func_8001E50C --prefix-dedup rom/lib_1a660,rom/lib_16320,rom/lib_22300,rom/lib_17dc0,rom/lib_1ece0 --dry-run
# then without --dry-run
```
**B. func_8001E0E0 (owned .rodata constants)**: `owned_data prepare $L/data/owned_func_8001E0E0.json` (row: lib_1ece0,
data.bin 0x6140 / 0x14 = 0x8002D910, alignment 4, logical_extent, logical_kind "constants", sha256 44a21168...;
validated with `rom_slots(check_source=True)` and `section_fits(row, 0x20)` here; func_8001E440's extern D_8002D924
follows it, as func_8001C860's slot w21i). Then `lock add $L/sources/func_8001E0E0.c:func_8001E0E0 --flags "-g0 -O2
-mips2 -G 0 -non_shared -Wab,-r4300_mul" --toolkit $TK71`, recompute its ctx row on the prepared registry (here: ok,
owned `.rodata` 32), promote_batch `0x1ece0 --only func_8001E0E0 --prefix-dedup rom/lib_1ece0`.
**C. Relocname pair**: append `$L/relocname/reloc_addrs_w23c.txt` (8 entries; `reloc_split check --file`: `all 8
reloc_addrs entries encode their ROM words`) to reloc_addrs.us.txt, `make extract`, refresh the two inventory targets
(as w21b/w20c; emulated by `tools/resplit_target.py`: gate_target OK), apply the layout-neutral TU edit
`python3 $L/tools/tu_edits.py src/rom` (two declaration hoists in lib_16320; proven IDENTICAL alone), then lock add
(plain flags) + promote_batch with `$L/ctx71_relocname.jsonl` `0x16320 --only func_80015A0C,func_80015D68`.
**D. ownbss trio**: the lib_16320 .bss row must grow from 0x80042670/0x30 to **0x80042630/0x80** (same owner
func_80016E68 and companion path, co_owners + func_80016C20/80016CF0/80017040: `$L/data/owned_func_80016E68_bss_grown.json`).
`owned_data.replace_slot` requires the same container/offset, so a block that grows *downward* needs a tooling
change (or a row rewrite in the promotion transaction). Context rows `$L/ctx71_ownbss.jsonl` were computed on a
scratch registry with that row (ok, owned .bss 32/48/16). Lock add plain flags.
**E. lib_10d60 libultra pair**: no inventory rows exist for 0x80010160 (99 insns) and 0x800102EC (45); register
targets (as w16i's targets_rename / w20c's refresh_split_targets), then `lock add $L/libultra/<fn>.c:<fn> --flags
"-g0 -O2 -mips2 -G 0 -non_shared"` (5.3 default toolkit), promote_batch with `$L/ctx53_O2.jsonl 0x10d60`.
No data-symbol renames; packet verifiers that assume "no allocated data" in lib_1ece0/lib_16320 need the owned-slot
admission (w20c §Tests class).

## Remaining boot tail after this lane (asm/us/boot_tail/extents.json minus locks, by address)

| function | B | TU / segment | class | best score | next step |
|---|---:|---|---|---|---|
| func_80012D18 | 3148 | lib_11640 | C (audio HW) | w23a | w23a |
| func_80018448 | 492 | lib_17dc0 | C | w23a | w23a |
| func_8001A658 | 2812 | lib_1a660 | C (synthHandle) | w23b 2770 | w23b |
| func_8001F13C | 1456 | lib_1f5b0 | C (voiceAllocate) | w23b 2045 | w23b |
| func_8001558C | 348 | lib_15a10 | C (MusyX seq play lookup) | D10 180 (7 rows) | loop-invariant u16 `id` copy lands in the preheader (ugen listing), retail's in the guard block; 12 shape/layout variants no change; trace uopt's hoist placement |
| func_80010DD8 | 168 | lib_11640 | C (audio HW, jalr) | D10 1155 | untouched |
| func_80011910 | 208 | lib_11640 | C | D10 2495 | untouched |
| func_80012234 | 372 | lib_11640 | C (callee of 12D18) | no D10 | untouched |
| func_80010450 | 372 | lib_11050 | C (audio thread/DMA) | no D10 | untouched |
| func_80026360, 80026558, 80026640, 800268D0 | 504+232+656+3840 | 0x26F60 (asm, unconverted) | C-compiled codec (bitstream decoder; callees at 0x800277D0+, past the boot-tail code end) | - | segment conversion first (w17k: derive refuses a pin) |
| bcmp (0x8000F8D0), __osSetCompare (0x8000FB90), __osProbeTLB (0x8000FCB0) | 284+12+184 | asm segments 0x104D0/0x10790/0x108B0 | hand asm (ultralib .s) | - | not C targets |

Pre-tail boot code (below 0x8000F3A4, outside extents.json) still has 36 GLOBAL_ASM slots / 13,152 B (lib_1050*, lib_5610,
lib_d580, ...), not triaged here.

## What generalises
1. **7.1 colours a no-pressure function in p2 in web-number order, and constants are numbered last**: a value retail
   keeps in a callee-saved FP register after all caller-saved ones are taken, with no call in the function, is a
   *literal* (own .rodata), not a global. Check the bytes at the extern's address against the literal before chasing
   colour order; 7.1 keeps duplicate literals (five words for 32767, 0.7079 x3, -0.7079).
2. **`continue` vs one `&&` body**: the continue edge reaching the loop bottom without a kill makes the count reload a
   PRE web; the nested form keeps it in ugen temps (19F48, 2235 -> 695).
3. **MusyX `switch` with one case** adds the blocks that order callee-saved webs; `x = &array[i]` from a separate
   result variable reproduces a preheader-started strength-reduced pointer and the extra frame word.
4. **MusyX function-local statics** explain whole runs of boot-tail `D_` .bss labels (dataGet* family); the TU packs
   each function's static block at 8-byte alignment in definition order (lib_16320: 0x14+4, 0x24+4, 0x30, 0xC).
5. **Sibling idioms**: func_800232A4's `curve |=` closed func_800233B0 directly; check locked siblings in the same TU first.
6. Libultra slots without inventory rows can still be proven against the splat .s (`tools/asmcmp.py`).

## Files
`sources/` (6), `ownbss/` (3), `relocname/` (2 + reloc_addrs_w23c.txt), `libultra/` (2), `data/` (2 owned rows),
`ctx71_O2.jsonl`, `ctx71_ownbss.jsonl`, `ctx71_relocname.jsonl`, `ctx53_O2.jsonl`, `prove.log`; tools: `prove.sh`,
`tu_edits.py` (new), `asmcmp.py` (new), `t.sh`/`vb.sh` (Pi push+score), `linkproof.sh`, `resplit_*.py`, w21b's
`check.py`/`sc.sh`/`dif.sh`/`splice.py` (SRCDIR/TUSRC aware)/`tusplice.sh` (CC override). Variants in `func_*/v/`.

## Disclosures
- No shaping devices, no unused locals, no pad arrays, no implicit int, no false prototypes. Type views: cast macros
  VOICE_233B0 (func_800233B0) and SEQ_17720 (func_80017720) over the TU's prototype types (as func_8001C860's EM);
  private packed views (Layer_80019F48, SampleHeader/SampleInfo_80016CF0, FxKey_80017040, ProgramEntry_80017720).
- func_80015D68 increments the reference count after re-enabling interrupts: that is retail's order (call first).
- Builder: only `~/rush2049/scratch/frontier/w23c` (+ mktemp dirs under the builder's /tmp by check.py/dif.sh);
  scratch-only copies of rom_owned_data.json with the proposed rows, baserom and symbol files, and symlinks
  `tools/ido-static-recomp/build{,71}/out` -> the pinned toolkits. Pi: target objects and ttyd MusyX files (gh api,
  reference only) in the session scratchpad. No permission denials.
