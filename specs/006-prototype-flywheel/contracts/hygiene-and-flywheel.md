# Contract: Seed Hygiene, Honest Buckets, Flywheel

Consumers: `autodecomp.py` (`_clean_m2c`, `cmd_clusters`), `farm.py`.

## Hygiene rules (normative)

1. **saved-register locals**: iff the seed body references
   `saved_reg_s[0-7]` and the seed contains no declaration for that name,
   inject `s32 saved_reg_sN;` into the function's declaration block.
   Additive only; never rename or delete code. Regression tests: fires on
   undeclared use; does NOT fire when m2c itself declared it; does NOT
   fire on e.g. `saved_reg_s0x` or non-matching names.
2. **scalar-cursor members**: 005's byte-cursor rule generalizes — for any
   *local declared in the seed* with a scalar or scalar-pointer type
   (`[us](8|16|32) *? name;`), rewrite `name->unkNN` (hex NN, optionally
   negative) to `(*(s32 *) (name ± 0xNN))`. MUST NOT fire on identifiers
   not declared as locals in the seed (globals, typed externs, struct
   pointers). Regression tests include the 005 cases plus an s32-local
   case and a no-overreach case (typed struct pointer keeps `->`).
3. **error placeholders**: `M2C_ERROR` occurrences are never rewritten,
   defined, or stripped. Classification is `partial_decomp` (below),
   checked on the raw m2c output BEFORE hygiene, so hygiene cannot mask
   the class.

## Histogram (normative deltas to 005's contract)

4. **`partial_decomp` bucket**: a target whose m2c output contains an
   error placeholder is bucketed `partial_decomp`, taking precedence over
   `compiled`/`blocked` (but not over `decompiler_failure`/`no_disasm`/
   `extent_conflict`, which mean no usable output at all). Coverage,
   exclusivity, counts-sum, and byte-determinism guarantees extend to the
   six-bucket schema.
5. **Artifact routing**: `build/m2c_histogram.{json,md}` is written ONLY
   by full-population runs; any `--targets` filter or truncating
   `--limit` routes output to `build/m2c_probe.{json,md}`. The population
   JSON carries a `run.population_complete: true` marker; consumers
   (flywheel) MUST refuse an input without it.
6. **Diff view**: `clusters diff <old.json> [<new.json>]` prints
   deterministic per-target bucket movements, bucket-count deltas, and
   blocker-class deltas. Exit code 0 always (it is a report, not a gate).

## Flywheel (normative)

7. **Cycle**: on each farm daemon cycle, read the population histogram
   JSON (refusing incomplete/probe inputs per rule 5); for each `compiled`
   target with NO score evidence (no `permuter_search` `work_unit` row AND
   no scored `matrix_entry`), submit one search via the existing seed
   path with **priority 60** and the standard budget. Never resubmit a
   target that has any score evidence (append-only; manual re-score stays
   `autodecomp seed`).
8. **Priority ladder**: static work keeps strict precedence (verify=1,
   promote=10, static seeds=30 < flywheel=60 < default=100; coordinator
   leases by ascending priority). A regression test asserts the flywheel
   constant is greater than every static-path constant.
9. **Reporting**: the standard report gains
   `extracted: compiled N, scored M, in_search K`; SC-004's acceptance is
   `M == N` after one unattended window (with `partial_decomp` and
   accepted-failure targets excluded from N by construction).
10. **Firewall**: unchanged from 005; a post-window check of
    `promotion_record` MUST show zero extracted entries (SC-006).
