# Symbol Misattribution Audit (libultra timer / thread cluster)

**Audited 2026-07-14** against `reference/repos/ultralib` (decompals). Several
symbols in `symbol_addrs.us.txt` were given generic or wrong names. This blocks
`corpus` name-pairing (it pairs a target to a same-named canonical function) and,
worse, causes the canonical `__osTimer*` names to point at unrelated functions.

## Verified findings

| Addr | Current label | Actually is | Evidence | Status |
|------|---------------|-------------|----------|--------|
| 0x8000C050 | `dll_remove` | **`__osDequeueThread`** | walks singly-linked list at offset-0 `next`, unlinks arg match — exact ultralib `__osDequeueThread` | **clean** (no collision, not locked) |
| 0x8000C490 | `dll_get_priority` | **`osGetThreadPri`** | `NULL → __osRunningThread`, returns offset-4 `priority` — exact ultralib `osGetThreadPri` | **clean** (no collision, not locked) |
| 0x8000C090 | `dll_init` | `__osTimerServicesInit` | inits `__osTimerList` circular list + VI accum globals | timer knot |
| 0x8000C11C | `dll_update` | `__osTimerInterrupt` | walks `__osTimerList`, `__osSetCompare`, `osGetCount`, `osJamMesg` | timer knot |
| 0x8000C294 | `dll_reschedule` | `__osSetTimerIntr` | reschedules the compare interrupt | timer knot |
| 0x8000C308 | `dll_insert` | `__osInsertTimer` | inserts into the sorted delta queue | timer knot |
| 0x80006ED0 | `osSetTimerIntr` | a VI-event setter (writes `__osViContext` +0x10/+0x14/+0x2) | **LOCKED** under wrong name | timer knot |
| 0x80009E18 | `__osTimerInterrupt` | a PFS/controller fn (`__osPfsSelectBank`, `__osContRamRead`) | — | timer knot |
| 0x8000E390 | `__osInsertTimer` | a PI-manager getter (reads `__osPiMgrState`) | **LOCKED** under wrong name | timer knot |

The real timer queue is the `dll_*` cluster in `asm/us/CC50.s`; the canonical
`__osTimer*` names were scattered onto three unrelated addresses, two of which
are locked regression anchors (`osSetTimerIntr`, `__osInsertTimer`).

## What has been done (safe, no build/link/lock risk)

`tools/conveyor/pipeline/corpus.py` `CANONICAL_ALIASES` maps the two **clean**
canonical names to their real target so corpus pairs them correctly:
`__osDequeueThread → dll_remove`, `osGetThreadPri → dll_get_priority`. Unit
tests in `tests/conveyor/unit/test_corpus.py`
(`test_submit_pairs_canonical_alias_to_misattributed_target`,
`test_alias_dedup_keys_on_target_id_not_candidate_name`).

## What is deferred (gated on watchman / SHA-1 rebuild)

A proper linker-symbol rename is **not** done here because:

1. `dll_remove` / `dll_get_priority` are called via `jal` from other asm files
   (`C990.s`, `8E10.s`, `7630.s`). Renaming means editing every `glabel` **and**
   every cross-file `jal` site — a change on the ROM link path that must be
   re-gated through the full matching build (SHA-1 must stay
   `3f99351d7bb61656614bdb2aa1a90cfe55d1922c`).
2. The timer knot's canonical names collide with two **locked** functions
   (`osSetTimerIntr` @ 0x80006ED0, `__osInsertTimer` @ 0x8000E390). Renaming
   those requires updating each lock's `target_id` + `src/rom_auto/*.c` filename
   and **re-verifying score 0 on the pool** — which needs watchman.
3. Watchman was **down** at audit time, so neither the SHA-1 gate nor lock
   re-verify could run. Blind link-path renames are not committed.

### Rename plan (run when watchman is up)

For each rename: edit `symbol_addrs.us.txt` (definition) + the `glabel` and
`nonmatching` marker in its `.s` + **every** `jal <old>` across `asm/us/*.s`,
then `make` and confirm SHA-1. For the two locked timer-knot functions, first
give 0x80006ED0 / 0x8000E390 honest names (their real VI/PI identities), update
their locks + `src/rom_auto` filenames, `lock add ... --wait` to re-verify, then
free the `__osSetTimerIntr` / `__osInsertTimer` names for the `dll_*` cluster.
Once symbols are correct, `CANONICAL_ALIASES` can be emptied.
