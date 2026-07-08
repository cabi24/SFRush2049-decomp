# Implementation Plan: Promotion Splicing

**Branch**: `004-promotion-splicing` | **Date**: 2026-07-08 | **Spec**: [spec.md](spec.md)
**Input**: Feature specification from `/specs/004-promotion-splicing/spec.md`

## Summary

Adopt the standard splat architecture the rest of the decomp community ships
with: convert splat `asm` subsegments to `c` subsegments one at a time; each
converted segment gets a generated ROM-aligned TU where every function starts
as `GLOBAL_ASM("asm/us/nonmatchings/...")` processed by asm-processor + the
pinned IDO, so a zero-promotion TU is byte-identical by construction.
Promotion = replace one pragma with verified C, rebuild, SHA-1 gate, commit or
rollback, migrate the lock. The layout map is derived from splat.us.yaml +
003's region index + symbol_addrs. Matching builds with converted TUs run on
watchman; `pipeline.promote` wraps the transaction locally and the conveyor
`verify_promote` job is upgraded to call the same code path on the builder.

## Technical Context

**Language/Version**: Python 3.9+ (tooling), C89 (TUs), GNU make
**Primary Dependencies**: splat (already drives extraction; supports `c`
subsegments + `nonmatchings` asm export), **asm-processor (to vendor into
`tools/asm-processor/`, pinned commit)**, IDO via ido-static-recomp on
watchman, existing conveyor fabric
**Storage**: layout map generated into `build/` + checked-in conversion state
in `splat.us.yaml`; promotion records in the conveyor DB (`promotion_record`
exists since 001)
**Testing**: pytest local suite for map/generation/transaction logic (mock
build); real builds only on watchman
**Target Platform**: Pi (map, generation, orchestration) + watchman (matching
builds, promotion gate)
**Project Type**: single project — `tools/conveyor/pipeline/` + Makefile +
splat config
**Performance Goals**: full matching build on watchman ≤ 5 min; one promotion
transaction ≤ 10 min end to end
**Constraints**: SHA-1 gate absolute; splat regeneration must not lose
conversions (they live in splat.us.yaml); non-matching Pi build path
untouched; C89/IDO flag pins from flag_registry per TU
**Scale/Scope**: 88 static segments; first conversions cover the TUs holding
the 12 locked functions (≈6 segments); 229-line spec, ~5 touched build files,
2 new pipeline modules

## Constitution Check

| Principle | Status | Notes |
|---|---|---|
| I. Matching First | PASS | The feature makes the SHA-1 the sole promotion authority; scaffolding is hash-neutral by construction. |
| II. Arcade Rosetta | PASS | Untouched; promoted bodies carry provenance headers per V. |
| III. Progressive Disclosure | PASS | This IS the constitution's GLOBAL_ASM workflow (Matching Workflow step 6), finally wired: per-function progress with a always-buildable ROM. |
| IV. Portability tiers | PASS | Layout map records segment category; no tier decisions changed. |
| V. Documentation as Artifact | PASS | Promotion records + provenance headers (FR-007). |
| Tool Requirements | PASS | Constitution already lists asm-processor as a required tool — this feature vendors it. |

No violations.

## Project Structure

```text
specs/004-promotion-splicing/
├── plan.md, research.md, data-model.md, quickstart.md, tasks.md, HANDOFF.md
└── contracts/
    ├── layout-map.md          # derivation, schema, refusal rules
    └── promotion-transaction.md  # splice/build/gate/commit/rollback semantics

tools/asm-processor/           # NEW: vendored, pinned commit
tools/conveyor/pipeline/
├── layout.py                  # NEW: derive map; generate/convert TUs; coverage
├── promote.py                 # NEW: the transaction (local CLI + library)
└── (verify_promote job upgraded to call promote as a library on the builder)
src/rom/<segment>.c            # NEW: generated ROM-aligned TUs (checked in)
asm/us/nonmatchings/<segment>/<func>.s   # NEW: splat-exported per-function asm
splat.us.yaml                  # converted subsegments: asm -> c
Makefile                       # link converted C objects; asm-processor rule;
                               # matching-build guard (refuse without IDO);
                               # progress: linked-C coverage lines
tests/conveyor/unit/test_layout.py, test_promote.py   # NEW
```

**Structure Decision**: ROM-aligned TUs live under `src/rom/` — distinct from
the existing reference `src/` tree, which stays as WIP/reference until each
function is promoted (then superseded per FR-006). splat's `nonmatchings`
export provides the per-function `.s` files GLOBAL_ASM includes.

## Rollout note

Walking skeleton first (US1 on ONE segment — the strlen segment 0x8800), with
the full SHA-1 build proving hash-neutrality before any second segment is
converted. Promotions land per-function, each gated. The conveyor job upgrade
(US3/FR-010) reuses the same `promote` library — no second implementation of
the transaction.
