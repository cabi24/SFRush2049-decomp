# Data Model: Promotion Splicing

## Layout map (derived artifact, not a DB table)

`build/layout.us.json` — schema in contracts/layout-map.md. Regenerable at any
commit; its sha256 ("map hash") is embedded in every generated TU header so a
TU and the map that produced it are tied together. Conversion state itself
lives in `splat.us.yaml` (the checked-in source of truth that survives
re-extraction).

## `promotion_record` (exists since 001 — now actually used)

Gains columns (additive, PRAGMA-guarded, 002 pattern) as needed to satisfy
FR-007: `source TEXT` (path or candidate_id), `flags TEXT`, `evidence TEXT`
(JSON: scores / lock / override reason), `rom_tu TEXT`. `outcome` keeps its
existing values (promoted / failed).

## `matched.lock.json` migration semantics

Promotion moves a function's single source of truth into `src/rom/<name>.c`:
the transaction adds the new-path entry (fresh score-0 verification is implied
by the SHA-1 gate itself; record `verified: "rom-sha1"`), and removes the
reference-path entry for the same target in the same commit. `lock check`
stays sub-second and green throughout.

## `function_status` interplay

Existing state machine already has `matched -> verified` with
promotion_record required for `verified` (001). The transaction drives that
transition via the existing `status` module — no new states.

## Generated TU format

```c
/* GENERATED ROM-aligned TU — segment 0x8800 (rom/lib_8800)
 * layout map <map-hash>; regenerate via `pipeline.layout convert`.
 * Slots are GLOBAL_ASM passthroughs until promoted; do not hand-edit
 * passthrough lines. */
#include "rom_tu.h"   /* minimal common includes for promoted bodies */

#pragma GLOBAL_ASM("asm/us/nonmatchings/rom/lib_8800/memchr.s")
...
/* PROMOTED 2026-07-08 — strlen
 * Source: src/libc/string.c (locked) | Flags: -g0 -O2 ... | Evidence: score0
 */
u32 strlen(const char *str) { ... }
```
