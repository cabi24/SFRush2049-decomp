# Frame dispatcher: source-supported observation contract

## Current receipt schema (2026-10-06)

Current receipts omit whole-file manifest, scorer/own-data tool, lock and redundant
production-context digests. The recorded BASE and `git show BASE:path` source reads
remain, as do native target authentication, own packet/verifier/C-harness bindings,
compiler executable identities, complete compiled extents, relocations, owned data
and behavioral evidence. Historical compatibility normalizers accept only their
explicitly listed legacy fields; unknown proof fields and changed invariants still
fail comparison. Descriptions of the earlier receipt schema below are historical.
This schema correction adds no matching or accepted bytes; fresh replay and the
aggregate test matrix are separate required checks.

Base: `e24b47d89a0c8ffade1e4c75ad76b9d390a1c232` (2026-10-05).
Target: `func_800B7FF8`, complete **0x800B7FF8–0x800B80C8**, 208 bytes / 52 words.

**Strict matching candidate; zero accepted-byte or ROM-coverage gain.**
The normal IDO O3 and O2 single-function pipelines both report MATCH. The
candidate is registered at `cloud/matches/func_800B7FF8.c`. No production source,
protected target, symbol, shared header, scorer, compiler recipe or lock changes.

## Source change and admissibility

Archived `game_C31/func_800B7FF8.bits.c` already reconstructed the complete
state/flag guard, paused angle accumulator, particle update and mode-dependent
overlay callback. Its exact-size source freshly reproduces **6/52 differing
words**. The only substantive change is the declaration of `D_8002EB94` as a
volatile float. That closes all six words without new locals, pressure, casts,
dead operations, helper bodies or source-layout searches.

The qualifier is a **reconstructed repeated-observation contract**, supported
independently of either its matching score or the declarations of accepted C:

- The native symbol map identifies **0x8002EB94 as `__osScDeltaTime`**. This is
  different from the elapsed-time clock at 0x8002EB90.
- Native `viTickStart` stores to EB94 at **0x80001390**. Its protected game caller
  is `game_late_init`, which calls it at 0x800EED44.
- Native `viUpdateTime` stores to EB94 at **0x800014DC**. Its protected game caller
  is `game_mode_handler`, which calls it at 0x800C9BB0.
- Independently, protected `func_8010E694` reads EB94 at offsets
  **+0x4C, +0x50 and +0x64**, with no intervening call or store. All three
  observations contribute to vector components. The verifier checks these
  effective-address and instruction-class witnesses directly.
- Separate read-only review established that the scheduler retrace increments
  its tick counter at 0x8002EB64, rather than directly writing EB94. The clock is
  **VI-derived and frame-boundary-updated**. Native `game_loop` calls
  `game_mode_handler` and `Effects_UpdateEmitters`; the latter calls this target.

This evidence supports applying the independently witnessed clock view to the
new body. It does **not** recover an original N64 declaration or prove that
asynchronous writes occur between these reads in actual gameplay. Arcade
IRQTIME/VS32 evidence concerns EB90 and is not borrowed for EB94. **No authentic
arcade donor for this dispatcher is claimed.** Source admission and exact code
identity remain separate review questions.

## Runtime contract

The dispatcher does nothing when the state is -1 or neither flag 0x200000 nor
0x400000 is set. Otherwise, when not paused, it adds pi times frame delta to its
angle accumulator. The external single-precision pi at **0x80123DE0** is checked
against protected data. It calls `particles_update(0)` for modes 0–4, then
**rereads mode** before calling the separate runtime-overlay function
`func_80391B00` for mode 4 or 6. A particle callback can change that decision.

The function has no parameters, a void result, ordinary O32 calls and a 24-byte
frame that saves only ra. No owned literals or writable data. The runtime-overlay
callee remains external; only its call address and observable hook contract
are exercised here.

## Complete verification

`verify.py` uses the unchanged scorer and pinned IDO, then independently links
each **complete ELF function** with GNU ld and checks GNU readelf symbol extents.
All 16 candidate relocations resolve; all 52 words agree with no masks. The
ordinary-clock baseline is also GNU-linked and retains its six differences.
There are no shortened prefixes, omitted excess words or hidden data references.

A genuine two-body O3 context uses the **unchanged accepted
`Effects_UpdateEmitters` source** and the candidate. Both remain MATCH, with
complete 256-byte and 208-byte GNU proofs. The protected call scan identifies
Effects_UpdateEmitters as the sole direct JAL caller; indirect entry is not
excluded. Per-body linking does not reconstruct original contiguous TU placement.
No fake callers, stand-ins, injected exports or substitute callees are used.

Behavioral proof covers **8,920 cases**, each compared across an independent
scalar oracle, the unchanged host candidate compiled C89 with UBSan, protected
native code and GNU-linked code: **17,840 bounded MIPS executions**. It covers:

- All 52 instruction offsets and both outcomes of every conditional branch
- Both flags independently, the -1 state exception, paused/unpaused modes,
  modes 0–7 plus signed extremes, and mode changes inside the particle callback
- Binary32 operation order, pi identity, signed zero and subnormals
- Callback argument/order and mutations of mode, delta and accumulator
- Exact native stack-write locations, full mapped-memory canaries, ra/SP and
  O32 callee-save GPR/FPR preservation, with adversarial caller-save clobbers

Five compiled wrong-source controls are rejected: inverted pause, wrong flag,
wrong overlay mode, wrong coefficient and mode cached across the callback.
Unknown instruction, wrong stack store and truncated-body controls fail closed.

## Reproduce

From the repository root with pinned IDO and GNU MIPS binutils configured:

```
python3 cloud/work/frontier/dot_frame_dispatch_20261005/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/cloud/test_dot_frame_dispatch_match.py -o addopts=''
python3 tools/cloud/score.py fn cloud/matches/func_800B7FF8.c func_800B7FF8 --flags '-g0 -O3 -mips2 -G 0 -non_shared'
```

`verification.json` binds the source, selected native bodies, referenced symbol
addresses, coefficient data, writer files, accepted caller, compiler and proof
implementation. Current native manifests are always validated. Whole-image
manifest and symbol-map digests are historical provenance, so unrelated native
source comments do not invalidate the selected-body receipt. Negative controls
reject selected body, symbol and data changes, and manifest validation failures.
The receipt contains only metadata, counts and hashes; no ROM bytes, raw
assembly, objects or binaries.

## Limits

The native model is bounded and fail-closed, not an N64 emulator. Helpers use
explicit mutation-aware O32 hooks. Actual particle/overlay internals and caller
execution, FCSR effects, signaling NaNs, interrupt interleavings and gameplay
are outside this proof. The frame-delta qualifier is not a C-thread-safety claim.

No whole-game shadow, source-image, compression, ROM SHA-1 or remote CI gate has
run. Production integration and merging remain with the independent checker.

Validation including the receipt portability checks: **742 scoped tests passed**,
including 18 packet checks and the existing scorer, guard, integrity and
submission suites.
Both documented scorer sanity examples pass. All **402 static source locks** are
intact. This is scoped local validation, not a full-suite or CI pass.


## Integration-portable replay (2026-10-06)

Production context and lock facts are historical evidence at the receipt's stated
base commit, read through `git show BASE:path` when used. Later source splices or
lock additions do not change those historical facts. Whole manifests and tool or
accepted-context digests are provenance, excluded by explicit field/path lists
from portable proof equality. Packet source/verifier bindings, selected native
words, complete extents, relocations, owned data and bounded behavior remain
binding. Tests are not hashed into the receipt. Compiler-dependent tests skip
when pinned IDO or the MIPS GNU linker is absent; source/native-only checks run.

The standalone candidate and genuine-caller replay both use the exact bare O3
recipe. The standalone match does not require a caller, inline helper or
deleted-static stub; the separate caller proof preserves the real caller body.
