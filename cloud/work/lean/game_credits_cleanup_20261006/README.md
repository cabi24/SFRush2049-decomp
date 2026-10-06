# LEAN RESEARCH: complete credits_screen cleanup body

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: extracted-game `credits_screen` at 800D6698, 296 bytes / 74 words.

Standalone IDO 5.3 at `-g0 -O3 -mips2 -G 0 -non_shared` plus canonical
`-Wab,-r4300_mul` improves **72/74 + 6 nonzero excess words** from the old
near-miss seed to **63/74 + 4 excess**. Every relocation resolves; no unverified
references or errors. This is research, not a match or ROM coverage.

## Complete source and required real context

The old permuter seed contains a second allocation call absent from the native
body, redundant increment/decrement operations and zero-valued state updates.
This source removes those operations and reconstructs the complete native queue,
row-cleanup and sound-cleanup sequence. It is an N64 reconstruction; no arcade
ancestor is asserted. Historical function names are retained as labels.

The actual Slot18 declaration and pointer return come from
`src/blob/groups/frontier_slot18_alloc/func_80091B00.c`. Callback signature
`s16,int,int` agrees with `src/blob/entity_spawn_callback.c`. These helpers stay
external; no body is invented or substituted. The native row range is 12 records
of 64 bytes, with a signed full-word identifier at +4 narrowed only for the real
callback. Flag globals are signed bytes, and the reset/count globals use the
actual word stores. No fake formal, padding local, pressure operation or volatile
qualifier is introduced.

Important context limitation: native credits_screen uses s0-s3 without saving
them, so its eventual match needs real whole-program context. Its native direct
caller is `func_800D7634`; prior complete-source inputs are under
`cloud/work/s20261004/E/seeds/func_800D7634.c` and
`cloud/work/s20261004/E/src/ctx_800D7634.c`. The published score is explicitly the
standalone compiler observation. No stand-in caller is added and no group match
is claimed. The checker can reconcile the caller and already-locked allocator
context before integration.

Assumptions: a successfully allocated real 24-byte task slot, valid row storage,
and the established callback/queue contracts. As native does, this body does not
add a null-allocation fallback. Identifier -1 skips deletion; zero is valid.
Live global reads and the final callback/flag-clear order remain explicit.

## Minimal replay

Set `IDO_DIR` to the pinned compiler and run:

    python3 replay.py --repo /path/to/SFRush2049-decomp

Only named protected inputs from the frozen Git base are materialized locally.
Both baseline and candidate compile at the same flags, reporting canonical score
and true ELF extent. No independent review, behavior harness, full suite, CI
wait or ROM gate was run. Checker/Claude owns verification, acceptance and ROM
integration. No production source, lock, target or tool is changed.

The final sound-stop interface uses the current locked `sound_stop(Voice *)`
prototype and an opaque Voice-pointer handle global. Historical integer-handle
declarations were normalized without changing the helper body or its O32 slot.
