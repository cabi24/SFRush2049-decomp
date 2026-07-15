# Implementation Plan: Game-Code Context Bootstrap (Track B Walking Skeleton)

**Branch**: `005-game-context-bootstrap` | **Date**: 2026-07-15 | **Spec**: [spec.md](spec.md)
**Input**: Feature specification from `/specs/005-game-context-bootstrap/spec.md`

## Summary

Widen the existing autodecomp loop (`m2c seed → local compile check →
permuter_search score`) from `population='static'` to the 885 `extracted`
game-code targets. Three moving parts, all extensions of shipped machinery:

1. **Extent repair first** (forced by the dry-run finding): extracted extents
   come from `work/**/info.txt` gap inference and are provably wrong in ~40%
   of sampled cases (`countdown` is ~2.7 KB, recorded as 2 instructions).
   A control-flow scan over `build/game_code.bin` (furthest-forward-branch /
   `jr $ra`+delay-slot algorithm) re-derives each extent; changed extents
   re-carve the target object, and feature 003's supersession machinery
   automatically purges stale evidence. Without this, scoring is against
   truncated truth.
2. **Derived disassembly + game-type context**: a new `pipeline/disasm.py`
   turns carved bytes into m2c-consumable GAS (the three normalizations the
   dry run validated), symbolizing `jal` targets from the target table and
   `%hi/%lo` data refs for a small, provenance-stamped game symbol table.
   A new `include/game_types.h` (GState, input records, per-car fields —
   verbatim-derived from `rushtherock/` with file:line provenance) joins the
   existing preprocessed m2c context chain.
3. **Population-parametrized seeding and histogram**: `autodecomp seed/one/
   clusters` grow a `--population` axis and a `--targets` list (the game-loop
   cluster ships as a committed list). `cmd_clusters` becomes the FR-006
   histogram: every extracted target lands in exactly one outcome bucket,
   written as a durable, diffable report. Promotion/lock guards refuse
   extracted-population sources (FR-010).

## Technical Context

**Language/Version**: Python 3.9+ (Pi orchestrator), stdlib only
**Primary Dependencies**: `mips-linux-gnu-objdump`/`-as`/`-gcc` (all already
installed and already used by `targets.py` / `_seed_compile_errors`); vendored
mips_to_c (`tools/mips_to_c/`, unchanged); IDO toolkit on builder (unchanged)
**Storage**: SQLite at `~/.conveyor/conveyor.db`; content-addressed blob store;
derived-asm cache under `build/m2c_asm/` (regenerable, not committed)
**Testing**: pytest `tests/conveyor -m "not node_required"`; new unit tests for
extent scan, disasm normalization, bucket exclusivity
**Target Platform**: Pi 5 for everything except score runs (builder/pool)
**Project Type**: single project — extends `tools/conveyor/` + `include/`
**Performance Goals**: full 885-target extent scan + histogram completes in
under 30 minutes on the Pi (m2c ~1–3 s/function dominates; scan is trivial)
**Constraints**: determinism (extent scan and derived asm are pure functions of
`game_code.bin` + DB extents ⇒ SC-003 reproducibility); no network for the
histogram; no node-side/toolkit changes required (seeds ride the existing
permuter_search job type)
**Scale/Scope**: 885 extracted targets, 10-function cluster, ~4 touched modules
(`targets.py`, `autodecomp.py`, new `disasm.py`, `lock.py`/`promote.py` guards),
1 new include header, 0 schema migrations (reuses `tier`/`gate_reason`)

## Constitution Check

| Principle | Status | Notes |
|---|---|---|
| I. Matching First | PASS | Extent repair makes extracted target objects a faithful representation of the blob before anything scores against them; score 0 remains the only "matched" claim. No ROM-hash claim is made (explicitly out of scope, FR-010 guards it). |
| II. Arcade Source as Rosetta Stone | PASS | `include/game_types.h` is built verbatim-first from `rushtherock/` with file:line provenance per definition (FR-003); research/arcade-structs.md is the sourced harvest. |
| III. Progressive Disclosure | PASS | Walking skeleton by design: one cluster end-to-end, histogram prices the rest; seeds are scaffolding, never claimed as matches. |
| IV. Platform-Aware Portability | PASS | game_types.h annotations mark arcade-hardware fields (cabinet switches, ZOID objnum, linked-cabinet state) as N64-inapplicable, per the harvest. |
| V. Documentation as Artifact | PASS | Histogram report is a durable evidence artifact; seeds carry origin stamps; game_types.h carries provenance comments. |
| 001–004 conventions | PASS | Stdlib-only, no new job types, no toolkit rebuild, additive DB usage (no migration), content-addressed artifacts, deterministic re-runs. |

No violations; Complexity Tracking not needed.

## Project Structure

### Documentation (this feature)

```text
specs/005-game-context-bootstrap/
├── plan.md              # this file
├── research.md          # decisions D1–D7 (codex ground-truth consumed here)
├── data-model.md        # extent lifecycle, outcome buckets, report schema
├── quickstart.md        # end-to-end bring-up + acceptance walkthrough
├── contracts/
│   ├── extent-repair.md      # scan algorithm, conflict rules, supersession
│   └── seed-derivation.md    # disasm format, context chain, histogram format
├── research/            # codex inputs (committed): cluster-data-refs,
│   └── …                #   arcade-structs, m2c-dryrun, codex-prompts
└── tasks.md             # /speckit.tasks output (not created by /speckit.plan)
```

### Source Code (repository root)

```text
tools/conveyor/pipeline/
├── targets.py           # MODIFIED: extent scan + repair pass in extract flow;
│                        #   gate_reason values extent_repaired/extent_conflict
├── disasm.py            # NEW: carve → objdump → normalize → symbolize;
│                        #   cache under build/m2c_asm/<target_id>.s
├── autodecomp.py        # MODIFIED: population-parametrized _asm source
│                        #   (static: asm/us glabels; extracted: disasm.py),
│                        #   --targets cluster runs, cmd_clusters → full
│                        #   bucketed histogram, preserved m2c stderr
├── lock.py              # MODIFIED: refuse extracted-population adds (FR-010)
└── promote.py           # MODIFIED: same guard on promotion entry points

tools/conveyor/clusters/
└── game_loop.txt        # NEW: committed cluster member list (10 target_ids)

include/
└── game_types.h         # NEW: arcade-derived game structs + N64 globals,
                         #   provenance-stamped; appended to m2c context chain

build/                   # regenerated, not committed
├── m2c_asm/*.s          # derived per-target disassembly cache
├── m2c_histogram.md     # ranked blocker report (FR-007)
└── m2c_histogram.json   # per-target outcome buckets (diffable, SC-003)

tests/conveyor/
├── test_extent_scan.py  # NEW
├── test_disasm.py       # NEW
└── test_autodecomp_population.py  # NEW (bucket exclusivity, static no-regress)
```

**Structure Decision**: single project; every change extends `tools/conveyor/`
in place, consistent with 001–004. The only file outside it is
`include/game_types.h`, which belongs with the other m2c context headers
(`include/m2c_types.h`) that `autodecomp._context()` already consumes.

## Implementation Phases (for /speckit.tasks)

1. **Extent repair** (`targets.py` + tests): scan, conflict classification,
   object re-carve, supersession ride-along, `extract` report line. Gate: the
   10 cluster extents match codex's ground-truth end addresses.
2. **Disassembly derivation** (`disasm.py` + tests): dry-run normalization
   rules as code; jal/data symbolization; determinism test. Gate: all 10
   cluster functions m2c cleanly (the dry run's 11 failures were all extent
   artifacts, which phase 1 removed).
3. **Game-type context** (`game_types.h`): GState/gstate, input records,
   bounded CAR_DATA subset, confirmed globals — provenance per definition.
   Gate: SC-005 static seed byte-identity before/after.
4. **Population widening + cluster run** (`autodecomp.py`): `--population`,
   `--targets`, flagset default for extracted; submit cluster, harvest scores.
   Gate: SC-001/SC-002.
5. **Histogram** (`cmd_clusters` rework): full-population buckets, durable
   md+json artifacts, arcade hints. Gate: SC-003/SC-004.
6. **Guards + docs** (`lock.py`/`promote.py`, README ops note, quickstart
   walkthrough). Gate: SC-006.
