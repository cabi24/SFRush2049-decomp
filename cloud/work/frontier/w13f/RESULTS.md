# Wave 13, lane w13f: ready singles with the fewest prior attempts

Builder scratch `~/rush2049/scratch/frontier/w13f` (copied from base; src/blob, include, tools/cloud, asm/us/blob and the
lock synced from the Pi). Trace toolkit installed with `--reuse ~/rush2049/scratch/frontier/wtk`. No permission denials.

| function | bytes | state | flags | scorer output |
|---|---:|---|---|---|
| records_screen | 312 | **MATCH** (standalone + unit) | `-g0 -O3 -mips2 -G 0 -non_shared` | `score.py fn` → `MATCH`; unit `EQUAL records_screen: 78 words` |
| func_800E762C | 228 | **MATCH** (standalone + unit; also MATCH at -O2) | `-g0 -O3 -mips2 -G 0 -non_shared` | `score.py fn` → `MATCH`; unit `EQUAL func_800E762C: 57 words` |
| func_800A7480 | 136 | 17/34, unchanged | -O3 | `FAIL func_800A7480: 17 of 34 words differ` (w9d best, unit) |
| func_8010FBE0 | 128 | 6 aligned rows, unchanged | -O3 | `want 32 words, got 32; differing rows 6` (w9d best) |
| world_effect_update | 456 | 49 rows, unchanged (cause of 6 rows found) | -O3 | `want 114 words, got 114; differing rows 49` (w10f best) |
| brake_light_update | 448 | not attempted (w2a/w9d: 74/112, ~100+ variants) | | |

Unit confirmation (both matches together, current tree):
```
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w13f score records_screen func_800E762C \
      --with cloud/matches/records_screen.c --with cloud/matches/func_800E762C.c --neighbours
  EQUAL records_screen: 78 words (kept, c_records_screen.c)
  EQUAL func_800E762C: 57 words (kept, c_func_800E762C.c); stub tail: 6 words of deleted-procedure stubs follow (NOT as in the image)
  locked bodies that differ in this unit: 0
blob_unit score: 2/2 equal
```

## Integration notes

- Both are standalone singles: `PYTHONPATH=. python3 cloud/work/frontier/tools/splice_singles.py records_screen func_800E762C`.
  No overrides, no groups superseded, no stubs involved, no own rodata (func_800E762C's 0.5f is a `lui` immediate,
  0.0f is `mtc1 zero`; D_8002AFB8 is a real extern global).
- func_800E762C's "stub tail" note is the known unit-ordering artifact from the neighbouring locked file
  (w8d noted the same stubs after func_800E7710): the stubs are not from this file. The standalone splice path
  compiles the file alone, so it should not see them; if the splice gate objects, that is the cause.
- records_screen calls the overlay routine at 0x8039244C as `func_8039244C()` (same idiom as func_800B7FF8's
  `func_80391B00`). sound_stop is declared with its real `Voice *` parameter (locked src/blob/sound_stop.c).

## Per function

### records_screen: MATCH
Source: `cloud/matches/records_screen.c` (copy in `records_screen/best.c`). Started from tiny_A121's draft (4 words:
two `lui` swapped, two `beq` operand orders). Two disclosed shaping devices:
1. `neg`, a named local holding -1 (the empty-slot marker). With a literal -1 ugen always emits `beq v0,s1`
   (either source operand order); with the variable it emits retail's `beq s1,v0`.
2. The two loop pointers are initialised on one source line, status first: `status = D_80154350; row = D_801541A8;`.
   Found with as1t.sh + asm.sh: reassembling the ugen listing with the two `la` in status-then-row order and no
   `.loc` between them gives 0 rows. On separate lines as1's tie-break gives s0,s3,s0,s3 or s3,s0,s3,s0; on one
   line it gives retail's s3,s0,s0,s3 (status `lui` in the blez delay slot).
About 170 compiles (most of them generated permutation sweeps).

### func_800E762C: MATCH
Source: `cloud/matches/func_800E762C.c`. From w9d's 26/57. The loop body reads the globals `D_80143FF4` and
`D_801543CC` back instead of locals holding the stored values (the indexed `D_8014A250[i]` stores cannot alias
them, so uopt keeps them in registers). That makes `&D_80143FF4` a coloured address web (retail's a3) and gives
retail's FP colouring (the scaled value in f12, the divisor load in f2). With w9d's v/f locals both were wrong.
Kept from w9d: goto loop (for/do-while unrolls: 26 extra words) and the condition order. No unused locals.
About 40 compiles.

### func_800A7480: 17/34, unchanged
ugen trace (`ugt.sh`) shows the integer temp ring is LRU over t1..t9 (t0 is blue's coloured web). Retail's pack
sequence is exactly that LRU **with t1 unavailable** during the pack, and has no coloured web for the inlined
func_800A5560 `u16 color` parameter (ours: v1). Diagnostic only: writing the helper body as `color * 0x10001`
removes the v1 web (14/34, `diag_mul10001.c`), but that is not the locked helper body and the unit inlines the
locked one, so it is not admissible. Forcing the colour web to t1 (`force.sh p2:w43=c8`) does not give retail:
it removes t1 from the pool for the whole procedure and the parameter-narrowing moves stop being peepholed
(25 rows). Swept: 81 register-parameter types, 48 stack-parameter types, 40 helper type/body forms; no movement
below 17 with the real helper. Same root cause as arb_rate_set (w9d). **Next:** find what makes t1 busy only in
the pack region in retail (a web live only there), e.g. via a uopt trace of the inlined parameter.

### func_8010FBE0: 6 rows, unchanged
Retail shares one `lui at` between the stores at 0x80155288 and 0x8015528C. Findings: (1) a pair struct or a
2-element const-index array makes uopt create an address web (v0) for the pair; one big OSScTask struct makes
one for the whole record (a3, `save=2 tot=4 > best=2.1` in ctrace); separate scalar symbols give two `lui at`.
(2) `asm.sh` cannot test this lever: reassembling the listing with `sw D_80155238+80` / `+84` (same symbol, even
both `sw $0`) still emits two `lui at`, although the locked func_800BB7F4 shows retail sharing exactly that way
(`sw $0, D_8013C300` / `D_8013C300+4` from an unrolled loop). The sharing is decided in ugen's binary output and
is not visible in the text listing (`same_symbol_listing.s`). **Next:** find which same-symbol accesses uopt
leaves without an address web (unrolled-loop element stores do), e.g. a two-iteration loop over the pair.

### world_effect_update: 49 rows, unchanged
The 6 head rows (prologue scheduled below the first stores, swc1 in the bnez delay slot) are a block boundary:
inserting a label right after `.frame` in the ugen listing and reassembling (`asm.sh`,
`entry_label_listing.s`) makes the first 24 words identical to retail (prologue at entry, `bnez; nop`). In
source, an unused C label, a `do {} while (0)`, an inlined `func_800EE8AC` stub helper (empty, holding the
stores, or holding the stores plus the wait loop) all left the unit result at 49 rows. Reordering the three
alignment statements before the buffer stores (12 permutations) was worse (51–60 rows). **Next:** something
that leaves a real label at the function's first statement (a loop whose back edge uopt later deletes?).

### brake_light_update: not attempted
w2a/w9d spent well over 100 variants; w9d's forced colours leave 27 rows. Skipped for breadth.

## What generalises
- **Same-line statements change as1 tie-breaks.** as1's list scheduler breaks ties by source line; putting
  two initialisations on one line reordered a `lui`/`addiu` interleave to retail's. Prove it first by deleting
  the `.loc` between the instructions in the listing and reassembling with `asm.sh`.
- **A named local holding a constant flips `beq` operand order** (`beq s1,v0` vs `beq v0,s1`) where literal
  operand order in the source does not.
- **Read the global back instead of a local copy** when stores in the function cannot alias it (indexed
  stores into a different global array): uopt then keeps the global in a register, which can create the
  address web and FP colouring retail has.
- **A prologue left at entry with an unfilled first delay slot means a label right after the prologue.**
- **Some `lui at` sharing cannot be tested with `asm.sh`.** The text listing loses ugen's `at` reuse.
