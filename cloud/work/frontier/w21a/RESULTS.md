# w21a - static boot tail, w20a leftovers under IDO 7.1 (wave 21)

Targets: func_80018448, func_80018634, func_80022CD4, func_80010E80, func_80011D74, func_80011A64, func_800252AC.
IDO 7.1 toolkit 4586d230, targets = reloc-aware inventory objects (w17k fetch_targets.py). Builder scratch
watchman2:~/rush2049/scratch/frontier/w21a (base copy + rsync of src/{blob,rom}, include, tools/{cloud,conveyor,
asm-processor,workbench.py}, third_party workbench, asm/us/{blob,boot_tail,nonmatchings/rom}, locks,
cloud/work/boot_tail_promotion; `tgt/` = inventory targets; `cloud/work/frontier/w21a` -> `lane` symlink so context rows
carry repo paths). Tools copied from w20a (paths renamed) plus `tools/vgen.py` (multi-line choice points `@{a@|b@}`)
and `tools/relock.sh` (score a segment's locks under a flagset). Nothing committed, locked, promoted or spliced.

## Headline

**3 functions, 1,484 B production-ready** in `sources/` (all `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr
identical`, spliced together into lib_11640 and lib_25bb0, both IDENTICAL: `prove.log`), **plus 1 function, 592 B,
code-identical with a function-local static table** (`ownslot/`, needs an owned .data slot). Two of the three need
a lock-flagset decision (Finding 1).

| function | B | TU | state | scorer line |
|---|---:|---|---|---|
| func_800252AC | 552 | lib_25bb0 | MATCH, plain flagset | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (166 rows)`; `lib_25bb0: IDENTICAL (960 lines)` |
| func_80011D74 | 492 | lib_11640 | MATCH with `-Wab,-r4300_mul` | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (124 rows)`; `lib_11640: IDENTICAL (4276 lines)` |
| func_80011A64 | 440 | lib_11640 | MATCH with `-Wab,-r4300_mul` | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (111 rows)`; same TU |
| func_80022CD4 | 592 | lib_22300 | code identical, owned .data | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(6 rows) (152 rows)` (rows = 4 kf relocs) ; TU context same |
| func_80018448 | 492 | lib_17dc0 | 325 (unchanged) | `LOCK: true=325 strict=325 reloc_blind=30 objdump-dr DIFFERS(27 rows) (129 rows)` |
| func_80018634 | 840 | lib_17dc0 | 780 (unchanged; lead) | `LOCK: true=780 strict=780 reloc_blind=173 objdump-dr DIFFERS(114 rows) (215 rows)` |
| func_80010E80 | 500 | lib_11640 | 460 (unchanged) | `LOCK: true=460 strict=502 reloc_blind=50 objdump-dr DIFFERS(51 rows) (139 rows)` |

Static by inventory sizes: 568/668 + 3 -> 571/668 (+1,484 B) once integrated; +1 more (592 B) with the owned slot.
Recount with `make progress`.

## Finding 1: the boot tail is built with as1 `-r4300_mul`; lock flagsets omit it

The Makefile passes `-Wab,-r4300_mul` to every ROM TU (CFLAGS); context_check.py and the TU proofs use it; but
`lock add --flags "-g0 -O2 -mips2 -G 0 -non_shared"` verifies WITHOUT it. The option stops as1 placing a multiply in a
branch delay slot. func_80011D74 (`mul.d` stays before `beqzl loop_length`, the likely slot takes the else path's
`lw length`) and func_80011A64 (`mul.s` stays before `bgez step` with a nop slot; 65536.0f keeps $f8) are EXACT with
it and unchanged C (only the TU header's field names). These were the w19a/w20a "as1 scheduling" residuals (420/775).
Evidence that it is the segments' real flagset: all 67 current lib_11640 locks score `true=0 strict=0 reloc_blind=0
objdump-dr identical` with it added (`relock_11640.log`), and 307 of the other 314 boot-tail-segment locks too
(`relock_boottail.log`); the remaining 7 = 4 with the same 2 label rows they show WITHOUT the option (func_8001B9F8,
func_8001BE14, func_80021548, func_80023BDC) + the 3 w20b ownbss locks whose body is not in lib_16320.c yet (no
output). **No current boot-tail lock changes under the option.**
Generalises: **any boot-tail near-miss with a multiply next to a branch should be re-scored with `-Wab,-r4300_mul`
before anything else** (w21b/w21c and the ~100 remaining boot-tail near-misses; probably also the game/overlay
`score.py fn` flags if they omit it - blob_splice/ovl_rom already add it).

## Finding 2: func_800252AC - index the global, not a running pointer (alias information)

w20a's "u8 parameter reload hoisted" residual is alias information: with `stream = D_80056230; ... stream++` uopt
emits no `.noalias $16,$sp`, so as1 may not move the `lbu sp` argument reloads above the `sb (s0)` stores (true=960).
`for (selected...) if (D_80056230[selected].busy == 0) break;` then `stream = &D_80056230[selected];` (the TU's
func_800251A8 idiom) gives retail's schedule (true=0 strict=8). Last 4-byte spill-slot shift = local count: the buffer
size goes straight into `stream->buffer_count` (five named locals; all 720 orders of the six-local form leave the
spill at sp+40; 6 of 12 five-local orders give 0). Same alias effect seen in 22CD4 (strength-reduced `kf[i]` gets
`.noalias $7,$sp`, an explicit cursor does not, and as1 fills a branch slot differently).

## Finding 3: func_80022CD4 = MusyX DoSetPitch with its function-local `static u16 kf[13]`

`ownslot/func_80022CD4.c`: kf indexed (`kf[i]`, uopt strength-reduces it to the a3 cursor) and the semitone scan
written `for (i = 11; i >= 0; i--)` with unsigned i (always-true test; gives retail's test-at-top loop in BOTH copies;
`for (i = 11;; i--)` lets uopt put the first copy's exit inline, true=500). LOCK and TU (lib_22300 text before the
slot + preamble) `true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(6 rows)`: the rows are the four kf relocations
(`.data`+0x16 here, the splat label `D_8002D476` = &kf[11] in the target). Object .data (0x20 B: kf + 6 zero bytes)
== ROM 0x8002D460..0x8002D47F (`ownslot/func_80022CD4.proof.log`). An extern base `D_8002D460[]` gives the same
code with relocs `D_8002D460+22` (true=20, would need a new data symbol); a label view `&D_8002D476 - 11` gives
`D+22`/`-22(a3)` (true=40). Same flags with or without `-Wab,-r4300_mul`.

## Integration (coordinator)

### A. func_800252AC (no decision needed)
```
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w21a/sources/func_800252AC.c:func_800252AC \
  --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w21a/ctx71_O2.jsonl 0x25bb0 \
  --only func_800252AC --prefix-dedup rom/lib_25bb0 --dry-run      # then without --dry-run
```

### B. func_80011D74 + func_80011A64 (lib_11640): lock flagset decision
Their lock must be `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul` (without it verify_entry scores 420/775). With
one such lock, `layout._segment_flagset` sees two flagsets in lib_11640 and returns None, and promote_batch refuses
(`segment flagset None != lock ...`). Two ways:
1. (preferred, tools change, owner review) treat `-Wab,-r4300_mul` like `-Xcpluscomm` in `_segment_flagset` and
   compare normalised flagsets in promote_batch.py (`seg["flagset"] != e["flagset"]`, row flagset). The Makefile adds
   the option to every ROM TU, so it never distinguishes segments.
2. (no tools change) re-lock all 67 lib_11640 entries with the `-Wab` flagset and `--toolkit $TK71` (all verified 0 in
   `relock_11640.log`; note this rewrites `rom-sha1` entries as `score0`), then add ours. Context rows would then need
   `context_check.py --flags "-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul"` (the shipped rows use the default).
Then:
```
for f in func_80011A64 func_80011D74; do
  python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w21a/sources/$f.c:$f \
    --flags "-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul" --toolkit $TK71; done
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w21a/ctx71_O2.jsonl 0x11640 \
  --only func_80011A64,func_80011D74 --prefix-dedup rom/lib_11640 --dry-run    # then without --dry-run
```
Both include `boot_tail_audio_record.h` (already included by lib_11640) and use its field names (`pending` 0x01,
`sample_position` 0x08, `value22` 0x22, `value24` 0x24, `tail` 0x3C); no header patch, no data, no TU edit.

### C. func_80022CD4 (needs an owned .data slot, tooling)
`lock add ownslot/func_80022CD4.c:func_80022CD4 --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71` should
pass (verify_entry = scoring.score, true=0). context_check refuses `.data` outside an owned slot, so it needs a
`rom_owned_data.json` rom_slot like `timer_services_pointer`: owner func_80022CD4, tu src/rom/lib_22300.c, source
assets/us/data.bin, container .data at 0x800277d0, owner_section .data, offset 0x5c90, size 0x20 (sha256 of ROM
0x8002D460..+0x20). `D_8002D476` (undefined_syms_auto) is referenced only by this function. Preamble for the splice:
`ownslot/func_80022CD4.additions.h` (types.h is already in the TU).

ctx rows: `ctx71_O2.jsonl` (3 rows, `ok`, `ido7.1`, default flagset, computed on the builder against master 72a34ea7).
Recompute: `python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 OUT.jsonl
fn=cloud/work/frontier/w21a/sources/fn.c ...`. Proof: `sh lane/tools/prove.sh` on the builder -> `prove.log` (the
`TU: COMPILE FAIL ... redeclaration` row for 252AC is check.py's non-deduplicated preamble, as in w17g-w20b; the
spliced TU with prefix dedup is IDENTICAL).

## Not closed (traced causes, next hypotheses)

- **func_80010E80** (460): traced to uopt copy propagation: ours stores count to its home (sp+48) and uses the
  expression register $3 for `count * 2` across the D_80038018 call (ugen spill sp+36); retail has one slot (sp+44),
  stores before the call and RELOADS count after it for the `sll` and again for `length` (frame 48). `volatile count`
  reproduces frame 48 and the reload pattern (true=1095, X/192 registers swapped; `probe_volatile_frame48.c`, device,
  not proposed). Tried: X as i/inline/own var/count, size var or not, all declaration orders (234 variants), types,
  definition splits, count reused as loop var. Next: whatever kills availability of count's value across a call in
  7.1 uopt (needs a 7.1 uopt trace).
- **func_80018634** (780): retail colours the 0xFFFE case's jump value (`lhu v1,10(s2)`), so the default case's webs
  shift one register (patterns a1, pattern a2, code a3, optional04 t0, and t0 is then never a temp). A `u16 jump`
  local (`lead_jump_v1.c`) gets v1 like retail (true=835 overall) but the default-case webs still start at v1;
  retail's default-case webs avoid v1 for a reason not found. Next: find what keeps v1 live in the default case.
- **func_80018448** (325): unchanged; layout sweeps (48), part-2 arm orders/one-line forms and `-O`/as1 option probes
  change nothing. The as1 choice of which arm's cursor step to hoist follows the ugen temp numbering.

## Disclosures

- Devices: none in the three matches (no unused locals, no compiled-out code, no pad arrays, no false prototypes);
  22CD4's always-true `i >= 0` is source shape, disclosed in its header. Private type views: 22CD4
  PitchState/PitchCommand (suffixed); 252AC uses the TU's StreamState_80025264.
- MusyX GameCube source (doldecomp/ttyd synthmacros.c DoSetPitch) read via `gh api` into the session scratchpad as
  reference only; nothing copied. ROM bytes read from baserom.us.z64 (Pi) for the kf proof.
- Permission denial (not retried): a `rm -f v/l2/*` inside a compound command was refused by the rm safety check;
  variant dirs were created fresh instead (`v/l2`, `v/l2b` remain, git-ignored).
- Builder: check.py/dif.sh use mktemp under the builder's /tmp (as earlier lanes). Only my scratch otherwise.

## Files

`sources/` (3), `ctx71_O2.jsonl`, `prove.log`, `ownslot/` (22CD4 source, additions, proof log),
`relock_11640.log`, `relock_boottail.log`, `func_*/best.c` (current best per target; `best0.c` = w20a start),
`func_80018634/lead_jump_v1.c`, `func_80010E80/probe_volatile_frame48.c`, `tools/` (w20a set + vgen.py, relock.sh).
