# T009 declaration-layer shortfall

Date: 2026-07-19. The T009 stop rule fired because the converged declaration
layer produced 58 compiling extracted seeds, below SC-001's target of 200.
No hand-written context was added and no farm, flywheel, scoring, or promotion
state was touched.

## Acceptance results

- Double generation was byte-stable: both headers had SHA-256
  `c95bffd469a910c7d9cf02ad1672e43d7881577fa9622914d2ca70819211aec9`.
- Layer evidence covers 718 symbols: 596 declarations (586
  `own_definition`, 10 `fallback`) and 122 omissions (58 `hand_context`, 64
  `static_target`). The sets are disjoint and no hand-context name was emitted.
- Histogram: `58 compiled / 252 blocked / 329 partial_decomp /
  49 decompiler_failure / 0 no_disasm / 197 extent_conflict` (885 total).
- SC-002 passed: zero target diagnostics contain `redeclar` or
  `conflicting types`. The combined context also compiles cleanly under the
  probe's `-std=gnu89 -fno-builtin` mode.
- Known-target `func_<addr>` blockers are zero. There are 161 unresolved
  `func_<addr>` blocker classes, but none of their addresses resolves to an
  `n64_target`; these are outside the declaration-layer coverage contract.
- The `math_utility` spot blocker fell to zero.
- SC-001 failed: 58 is 142 short of 200.

## Attribution

`clusters diff specs/006-prototype-flywheel/research/baseline.json
build/m2c_histogram.json` reports `compiled +16`, `blocked -345`, and
`partial_decomp +329`; `decompiler_failure`, `no_disasm`, and
`extent_conflict` are unchanged. The committed baseline predates the honest
`partial_decomp` split. Against T005's post-hygiene actual counts
(`37/299/303/49/0/197`), the declaration layer moved compiled +21, blocked
-47, and partial_decomp +26.

## Residual blocker classes

The 252 blocked targets have overlapping diagnostics. Ranked by mechanical
class:

| Class | Symbols | Incidences | Blocked targets covered |
|---|---:|---:|---:|
| unresolved `func_<addr>` outside known targets | 161 | 240 | 130 |
| raw-address symbols (`x<addr>`) | 119 | 162 | 84 |
| local/member artifacts (`unkN`) | 55 | 87 | 43 |
| other named types or symbols | 52 | 73 | 59 |
| omitted static-library calls | 3 | 8 | 7 |

Another 59 blocked targets have compiler failures which the existing blocker
token classifier does not name. The largest individual residuals are
`func_80091fbc` (11 targets), `func_8009211c` (11), `x80150000` (8),
`func_8008e26c` (7), `unk0/unk4/unk8` (7 each), and `x801407F0` (7).
Separately, 329 targets remain honestly classified `partial_decomp`, and 49
remain `decompiler_failure`; the declaration layer cannot turn either class
into valid seeds without additional work outside T006-T009.

The remaining failures are therefore dominated by unknown-address callees,
raw data addresses, and m2c member artifacts—not referenced known-target
callee declarations. Per the stop rule, expanding the hand context or adding
new symbolization/hygiene rules is out of scope for this task.

## Post-fix addendum (2026-07-19, separator-bug gate)

The T009 residue investigation found a derivation bug upstream of the
shortfall: seven-plus-character mnemonics (`cvt.s.w`, `trunc.w.s`, ...)
were emitted with no separator before their operands (`{mnemonic:<7}` pads
7-wide and concatenates), so m2c parsed one unknown token and emitted
M2C_ERROR for essentially every FP-converting function. Fixed in `37d1083`
(DERIVATION_VERSION 2→3); layer regenerated (still 596 declarations,
byte-stable at `93b72973…`).

Post-fix histogram: `60 compiled / 298 blocked / 281 partial_decomp /
49 decompiler_failure / 0 no_disasm / 197 extent_conflict`. The fix moved
48 targets out of `partial_decomp`, but mostly into `blocked` (+46), not
`compiled` (+2): the newly decompiled FP bodies expose additional unknown
callees — `func_<addr>` blocker classes rose 161 → 184, all still
unresolvable to any `n64_target` row. **SC-001 remains not met (60 < 200)
and the shortfall is structural, not mechanical.**

Residual walls, quantified for the next feature:

1. **Callee closure** (largest): 135+ real blob functions are called by
   extracted code but have no target rows — the work-inventory was never
   complete. Registering them via the existing extent-scan machinery makes
   them both declarable (collapsing the func_ classes) and scoreable
   (growing the population itself).
2. **Raw data addresses at scale** (~119 symbols / 84 targets): the 005
   symbol-table class beyond the surveyed cluster; automatable from
   derived-asm access widths under the third-amendment evidence rule.
3. **Remaining M2C_ERROR causes** (sampled): "Read from unset register"
   (dominant — likely non-standard register-argument conventions in
   optimized game code), stack-arg detection, `cfc1` coprocessor reads —
   m2c capability/convention work, not context work.
