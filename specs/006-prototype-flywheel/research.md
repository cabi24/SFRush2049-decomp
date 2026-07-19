# Research: Prototype Layer & Seed Flywheel

Grounding was measured this session (2026-07-19) directly against the
shipped 005 code and the regenerated baseline histogram; no open unknowns
remain.

## D1 — Signature source: the callee's own definition line

**Decision**: A callee's prototype comes from **its own** m2c decompilation
— the definition line `_extract_fn` already isolates (return type + name +
parameter list, text up to the opening brace) — captured during the same
per-target m2c pass the histogram already performs. Targets whose
decompilation fails (the 49 crashes) or is partial get the fallback
`s32 name();` (C89 empty parens: declares no parameter information, so it
is call-compatible with any call site).

**Rationale**: This makes conflict resolution unnecessary by construction —
one callee, one definition, one signature — where call-site inference would
need a precedence rule over disagreeing sites (spec edge case). 005 proved
the underlying mechanism: `emit_src`/`_extract_fn` reliably isolate the
definition, and the SC-005 guard already depends on that isolation.

**Alternatives considered**: call-site inference (m2c's guessed externs per
seed) — rejected: conflicts are common, and m2c's `?`-typed guesses are the
very blocker class being fixed; hand-written prototypes at scale — rejected:
005 measured ~75 hand prototypes as a session of LLM work for +14 compiles,
and the user's standing preference is deterministic compute.

## D2 — Two-pass generation closes the fixpoint deterministically

**Decision**: Generation runs exactly two passes: pass 1 decompiles every
derivable target with the current context *without* any generated layer and
captures signatures; pass 2 regenerates with the pass-1 layer active and
captures again; the emitted artifact is the pass-2 result. No iteration to
convergence.

**Rationale**: Signatures influence m2c's typing of *other* functions, so a
callee's own definition can improve once its callees are declared (the
layer feeds itself). A convergence loop risks oscillation and unbounded
runtime; two passes capture the first-order improvement and are
deterministic by construction (fixed pass count, sorted emission). Each
pass costs one histogram-equivalent (~4 min measured), fitting the <10 min
goal.

**Alternatives considered**: single pass (leaves first-order typing gains
on the table for the dominant blocker class); iterate-to-fixpoint
(non-deterministic runtime, oscillation risk, no evidence it beats two
passes).

## D3 — Precedence via an already-declared-names filter

**Decision**: The generated layer is included **last** in `_context()`'s
chain, and the generator filters out any name already declared by the
preprocessed hand-curated context (types.h, PR/*.h, m2c_types.h,
game_types.h) or belonging to a known static/library target with a real
signature there. The filter operates on the preprocessed context text
(function-declarator name extraction), and the stamp records every omitted
name with the reason (`hand_context`, `static_target`).

**Rationale**: Implements FR-003's precedence chain with zero redefinition
risk: C89 tolerates *identical* redeclarations but not conflicting ones,
and inferred signatures will conflict with real ones (e.g. `memcpy` — the
histogram shows its arcade-hinted prototype line). Omission is strictly
safer than emission-and-hope. The population-wide zero-redefinition gate
(SC-002) verifies the filter empirically over all 688 targets.

**Alternatives considered**: `#ifndef`-style guards (don't exist for
declarations); emitting first and letting hand context override (C has no
override for conflicting declarations — this is exactly the failure mode).

## D4 — Hygiene rules: mechanical, bounded, honest

**Decision**: Two `_clean_m2c` additions, each regression-tested with a
no-overreach case:
(a) **saved-register locals**: when the body references `saved_reg_s[0-7]`
(m2c's emission for callee-saved spills it couldn't allocate a variable
for) and no declaration exists in the seed, inject `s32 saved_reg_sN;`
declarations at function top — additive, semantics-preserving.
(b) **scalar-cursor generalization**: 005's byte-cursor rule (rewrite
`x->unkNN` on `u8 *x` locals to explicit offset loads) generalizes to
locals declared with any scalar/scalar-pointer type — the `unk0/unk4/unk8`
blockers and the `invalid type argument of '->' (have 's32')` class are
this same emission wart on other declared types. The rewrite keys on the
*declared local* (never on globals or typed externs), preserving 005's
no-overreach property.
`M2C_ERROR` is explicitly **not** a hygiene target: its presence in a
seed's text classifies the target `partial_decomp` (FR-005) before the
compile probe runs; defining the macro away would produce compilable seeds
whose semantics m2c itself declared unknown — corrupting both the
instrument and any downstream search that starts from them.

**Rationale**: Bounded to the two artifact classes the histogram
quantifies (~69 saved_reg_*, ~74 local unkN); the honesty split is the
spec's central integrity decision.

**Alternatives considered**: running m2c with `--valid-syntax` to get
M2C-macro output — rejected in 005 already (changes all output including
static seeds, and hides genuine failures); post-hoc `#define M2C_ERROR(...)`
— rejected as laundering.

## D5 — Probe/instrument artifact separation + diff view

**Decision**: `cmd_clusters` writes to `build/m2c_histogram.{json,md}` only
for full-population runs (no `--targets`, no `--limit` truncation); scoped
probes write `build/m2c_probe.{json,md}`. A new `clusters diff A.json
B.json` (or `--against <old>`) view reports bucket movements per target and
blocker-class deltas.

**Rationale**: FR-006 (the clobbering was observed live: the 005 close-out
probe overwrote the population record and it had to be regenerated for this
spec's baseline); FR-010's attribution view is a pure JSON comparison — the
artifacts are already deterministic and per-target keyed.

## D6 — Flywheel placement, priority, and dedupe

**Decision**: A cycle step in the existing farm daemon: read the population
histogram JSON's `compiled` list; for each target with no score evidence
(no `permuter_search` row in `work_unit` and no `matrix_entry` score),
submit via the existing `submit_one` path at **priority 60** with the
standard budget; harvest is the existing farm step, untouched. Coverage
line (`extracted compiled: N, scored: M`) added to `cli report`'s data.
Already-scored targets are never resubmitted by the flywheel (append-only
evidence; explicit re-score stays a manual `autodecomp seed` invocation).

**Rationale**: The coordinator leases strictly by ascending `priority`
(`ORDER BY priority, created_at`; measured constants: farm verify=1,
promote-verify=10, autodecomp static seeds=30, default=100), so 60
guarantees Track A precedence (FR-008/SC-005) without new scheduling
machinery. Reading the histogram JSON keeps the flywheel decoupled from
compile-probe internals and makes its input auditable.

**Alternatives considered**: separate daemon (violates no-new-service
assumption); DB-driven compile status (duplicates the histogram's job and
invites drift); same priority as static (risks starving Track A on a
16-core node with 3600s searches).

## Resolved unknowns

All Technical Context items were resolved by direct measurement this
session: priority semantics (coordinator/queue.py ORDER BY), artifact
paths (module constants), signature isolation (`_extract_fn`), pass cost
(~4 min per population m2c pass), and the baseline blocker decomposition
(spec Assumptions).
