# T008 re-measure shortfall (SC-004 stop rule)

Date: 2026-09-10. The full regeneration sequence (closure → datasyms →
protos ×2 → histogram ×2 → diff) over the enlarged population produced
**170 compiled**, below SC-004's 200. Per FR-010 the run stopped here: no
hand context was added, no symbolizer/hygiene rule was invented, no
promotion state was touched. Numbers below are from `build/m2c_histogram.json`
(population 1287, `population_complete: true`, image `bf7da3fa…`, layer
`40a46dc3…`).

## Result against the 006 baseline

| bucket | 006 baseline (885) | 007 (1287) | delta |
|---|---:|---:|---:|
| compiled | 60 | **170** | +110 |
| blocked | 298 | 462 | +164 |
| partial_decomp | 281 | 237 | −44 |
| decompiler_failure | 49 | 43 | −6 |
| no_disasm | 0 | 0 | 0 |
| extent_conflict | 197 | 375 | +178 |

`clusters diff research/baseline.json build/m2c_histogram.json`: 654 target
movements — 402 `absent →` (the discovered population: 70 compiled, 245
blocked, 73 partial_decomp, 14 decompiler_failure), 178 `→ extent_conflict`
(the suffix rows: at baseline they had been counted as 140 partial_decomp,
20 decompiler_failure, 14 blocked and **4 compiled** — i.e. 4 of the
baseline's 60 "compiled" were function suffixes), and on the 510 surviving
inventory functions: `blocked → compiled` 46, `blocked → partial_decomp` 24,
`partial_decomp → blocked` 2, `compiled → blocked` 1. Like for like, the
surviving inventory rows went 56 → 100 compiled.

Oracles other than SC-004:

- **SC-001** (in-blob `func_` blockers = 0): the *unresolved-callee* class
  that closure targets is **gone** — every in-blob call now resolves to a
  registered target (0 `undeclared func_8…` errors). 12 in-blob `func_`
  blocker classes remain in the token histogram, but they are a different
  mechanism: **call-arity disagreement** ("too many/too few arguments to
  `func_800B24EC`"): the callee's own-definition signature (006's
  precedence rule) has N parameters while a caller passes M. 29 non-blob
  `func_` classes remain by design: 24 are the out-of-image
  `0x8038A400–0x803A1EAC` callees (closure `invalid`, no image), 5 are
  static-range addresses with no static target row (`func_800205e4`,
  `func_800154a4`, `func_80018e2c`, `func_800201d0`, `func_80020274`) —
  declaration-layer/static-inventory business per the spec's edge case.
- **SC-002** (`x<addr>` eliminated): 151 classes → **2**
  (`x80150000` in `func_800DDEA4`/`func_800DDF28`, `x80151CE8` in
  `difficulty_select`). Both are the linear tracker's known blind spot: the
  `lui` and its `addu`+access sit on different control-flow paths with an
  intervening write to the same register (`move $v0,$zero`), so the
  binding is dropped before the access. Recorded, not chased.
- **SC-003**: buckets/targets/blockers identical across the two runs; the
  JSON differed only in the compile probe's random `/tmp/tmpXXXX.c` path
  inside stored `diagnostics`. Fixed (`_TMP_PATH_RE` scrub in
  `_histogram_data`); the two histograms are byte-identical modulo
  `run.timestamp` after the scrub, and the post-fix pair is recorded in
  quickstart §3.

## Residual walls, quantified (462 blocked targets)

Classified by the compile-probe error mechanism (a target can carry
several; "solo" = only that class):

| class | targets touched | solo | mechanism |
|---|---:|---:|---|
| **member access on a generated scalar** (`D_xxx.unk54`, `unk0/4/8/C` top blockers) | 227 | 98 | m2c emits struct-member syntax for any offset access off a global; the generated layer types every symbol as a scalar (contract §8) — struct-shaped symbols need generated struct shapes |
| member access on a local/arg (`var_->unkN`, `saved_reg_s1`) | 167 | 125 | m2c type inference on untyped locals/args (006 residual class, not a symbol problem) |
| `undeclared_other` (non-`func_`/non-`x` identifiers) | 100 | 5 | m2c artifacts (`?` casts, `MIPS2C_ERROR` remnants, `saved_reg_*`), mostly co-occurring with the above |
| parse errors (`expected expression/identifier before …`) | 95 | 17 | same artifacts surfacing as syntax |
| `invalid type argument of ->` | 57 | 16 | scalar-typed symbol/local dereferenced |
| call arity (`func_8…`, `memcpy`) | 23 | 3 | own-definition signature vs caller's argument count; `memcpy` appears as m2c's `? memcpy(...)` inferred declaration clashing with the shim |

The single largest lever — **generated struct shapes**: for a generated
symbol whose deriving accesses (or accesses off its formed pointer) hit
several distinct offsets, emit a `typedef struct { u8 pad…; <type> unkNN;
… }` from the observed `(offset, width)` set and declare the symbol with
it, exactly what `include/game_types.h` does by hand for a dozen symbols.
That is a contract change (§8 scalar rule) and needs its own spec; it is
not attempted here. Second lever: arity-tolerant declarations (`u32
f();`) only where callers disagree with the own definition (12 in-blob
classes, 23 targets) — a 006 precedence decision, also out of scope.

## Population honesty notes

- The enlarged population is 912 gate-passed functions (510 surviving
  inventory + 402 discovered) + 375 `extent_conflict`. The compiled ratio
  over gate-passed functions is 170/912 = 18.6% (baseline: 60/688 = 8.7%,
  or 56/510 = 11.0% over the rows that survived the suffix rule).
- 2086 generated data symbols; 646 are formation-only (typed `s32` and
  flagged) and one carries an int/FP width conflict.
- 28 calls from the blob go to `0x8038A400–0x803A1EAC`; no image in the
  project covers that range (a second overlay?). Worth a look before any
  further population work.
