# Implementation Plan: Game-Code Image Rebuild (blob stage 1)

**Branch**: `008-blob-image-rebuild` | **Date**: 2026-09-24 | **Spec**: [spec.md](spec.md)

## Summary

Point feature 004's proven machinery at the decompressed image instead of the
cartridge. Three modules, one gate:

1. **`pipeline/blob_layout.py`** — derive a deterministic map of the
   647,072-byte image into TU-sized regions from the live `n64_target` rows
   (912 gate-passed extents) plus the image itself; every byte lands in
   exactly one region, non-function bytes marked opaque. Emits
   `build/blob_layout.json`.
2. **`pipeline/blob_tu.py`** — generate `src/blob/blob_<vaddr>.c`: each
   function a `GLOBAL_ASM` passthrough over its extracted asm, each opaque run
   an `.incbin` slice of `build/game_code.bin`, plus `blob.ld` placing the TUs
   at `0x80086A50`. Byte-identical by construction.
3. **`pipeline/blob_build.py`** — build on the builder (IDO + asm-processor +
   `mips-linux-gnu-ld`), extract the loaded image from the linked ELF, compare
   its sha256 to `build/game_code.bin`, and refuse anything else. Splice /
   revert / coverage ride on top, mirroring `promote.py`.

Rationale for reusing rather than inventing: the static path already survives
this exact discipline in production (8 converted segments, 19 promoted
functions, a SHA-1 gate that was drilled after it was caught being vacuous).
The failure modes are known — stale builder trees, per-file optimization
levels, alignment padding at region edges — and the code that handles them
exists.

## Technical Context

**Language/Version**: Python 3.9+ (Pi orchestration), C89 (TUs), IDO for the
compile, `mips-linux-gnu-{as,ld,objcopy}` for assembly and link
**Storage**: derived `build/blob_layout.json`; checked-in `src/blob/*.c`,
`blob.ld`, and splice state
**Testing**: pytest local suite; the image gate is itself the acceptance test
**Target Platform**: derivation on the Pi, build on watchman2 (only IDO host)
**Constraints**: image byte-identity is absolute; no partial-credit pass;
cartridge ROM build, hash and existing locks must be untouched
**Scale**: 647,072 bytes, 912 functions, 213 interior gaps, ~85 KB tail data

## Key design decisions

**Region granularity.** One TU per contiguous run of functions, split at large
opaque gaps — the same shape as the static `lib_<off>.c` files. Rationale: a
TU that spans a big `.incbin` is fine, but keeping data runs at TU boundaries
makes alignment behaviour obvious and keeps individual files reviewable.
Target ~30–60 functions per TU, so roughly 20–30 TUs.

**Opaque bytes stay opaque.** 132,748 bytes (interior gaps + tail) enter as
`.incbin`. We do not attempt to decompile or even classify them in this
feature. They are what makes byte-identity achievable on day one.

**The gate compares the loaded image, not the ELF.** Link, then `objcopy
-O binary` the loaded range and compare to `build/game_code.bin`. Anything
about the ELF's own layout is irrelevant.

**Splices reuse the lock discipline.** A spliced body is pinned by source hash
exactly like `matched.lock.json` entries, but in a separate file
(`blob_matched.lock.json`) so the cartridge lock's meaning stays unambiguous
and `make check-matched` keeps covering only ROM-linked code.

**Firewall unchanged.** This feature gives extracted evidence a *build*, not a
*promotion*. Nothing here writes to the ROM TU paths, the ROM lock, or
`promotion_record`. The 005 firewall test stays green as written.

## Risks and how the plan meets them

| risk | mitigation |
|---|---|
| Alignment padding differs at region edges | `_strip_trailing_nops`-style handling already exists in 003; the gate names the offset, and region boundaries are chosen at function starts |
| A spliced function's data references cannot be resolved | the link fails or the image differs; that function is reported unspliceable and left as passthrough (SC-002 expects some) |
| Per-TU optimization level wrong | the map carries a flagset per TU, defaulting to `-O2`; a mismatch shows up as an image difference, not silent drift |
| The gate is vacuous (the 004 lesson) | SC-003 is a deliberate corruption drill, run before the gate is trusted |
| Builder-tree skew (bit us twice already) | the build script rsyncs the TU set, `blob.ld` and the Makefile fragment every run, and prints the builder's git hash |
| `build/game_code.bin` drifts | its sha256 is recorded in the map and re-checked on every build |

## Phases

1. **Map** — `blob_layout.py` derive + report; complete coverage and
   determinism tests. Gate: every byte assigned, second run byte-identical.
2. **TUs** — `blob_tu.py` generate; all-passthrough build on the builder.
   Gate: SC-001 image byte-identity.
3. **Splice** — one function, then batch; provenance lock; revert.
   Gate: SC-002 (≥50 spliced) and SC-005 (revert restores).
4. **Drill and report** — SC-003 corruption drill, coverage reporting with the
   cartridge caveat, docs. Gate: SC-003, SC-004.

## Constitution Check

| Principle | Status | Note |
|---|---|---|
| I. Matching First | PASS | Byte-identity is the gate; nothing is "close enough" |
| II. Arcade Source | PASS | Untouched |
| III. Progressive Disclosure | PASS | Opaque bytes stay opaque and are labelled as such |
| IV. Portability | PASS | No source-tier changes |
| V. Documentation as Artifact | PASS | The map, the lock and the coverage report are the record |
| 001–007 conventions | PASS | Stdlib-only Pi tooling, deterministic artifacts, firewall intact |

One deliberate tension to record: this feature creates a second coverage
number that is **not** cartridge-verified. Principle I is satisfied for the
image, not for the ROM. FR-008 exists so that distinction is stated in the
output rather than left for a reader to infer.
