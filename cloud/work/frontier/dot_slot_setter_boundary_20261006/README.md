# Genuine slot setters do not recover the missing 92BF4 source boundary

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

## Integration portability update (2026-10-06)

All historical helper/consumer sources are read with `git show` at the recorded base commit and materialized only in temporary compiler workspaces. Replay equality excludes exactly the three accepted production-source digests; the archived caller source, packet source/verifier/recipe/host hashes, all native addresses, complete compiled bodies, relocations and behavior remain strict.

Focused packet tests passed with IDO available and with IDO absent. Compiler-dependent tests skip without IDO or the MIPS GNU linker. These scoped results do not claim the required aggregate repository-suite pass; that matrix is recorded separately before any publication. No protected production files, native assets, accepted locks or compiler policy are changed.


2026-10-06; research only, **no new matches, no claims, zero accepted bytes**.
Base: `cd22879d40b3de443cfde047b86e75e159b6cec6`.

`func_80092BF4` is a complete 100-byte / 25-word ordinary-ABI leaf. It loads
one model index from a 64-byte car-part record, narrows it to a signed halfword
for each of two stores, and writes dereferenced input words to model-record
fields +60 and +64. The first write precedes the second input read, which
matters when those locations alias. There are no direct J/JAL callers in the
manifest-verified native target population; this does not exclude indirect use.

## Concrete hypothesis

The archived A44 candidate computes one shared signed-index/multiply expression,
but native code retains two. Two real, accepted, caller-less API bodies perform
exactly the constituent stores: `func_8008E06C` (+60) and `func_80092BC8` (+64).
The latter immediately precedes the caller in the image. These are substantive
existing functions, not invented pressure helpers. The bounded hypothesis was
that genuine inlining would preserve the two independent argument conversions.

It did not. Both real setters inline in every tested composition, but the native
caller's duplicate computation does not reappear. Absence of inlining is not
the blocker. No claim of recovered original TU membership or source is made.

## Fixed compiler experiments

All groups use stock `-g0 -O3 -mips2 -G 0 -non_shared`, identical three-function
keep/member lists, and the scorer's standard multiply-erratum handling.

- Archived direct A44 source, O2 and O3: **22/25 differ; 80-byte ELF body**.
- Unchanged accepted setter sources plus the real two-call caller:
  **22/25 differ; 84-byte ELF caller**. Both complete 44-byte setters MATCH.
- Additive normalized shared-record definitions in `group.c`:
  **22/25 differ; 80-byte ELF caller**. Both complete 44-byte setters MATCH.
- The same normalized source with the two real helpers marked `__inline`:
  identical normalized output and residual. Both setters still MATCH.

The normalized source is the retained semantic model, not a new best match.
No variant/register/declaration sweep followed these tests.

## Record evidence and limits

The normalized car-part view uses the real bank start `D_80139320`, stride 64,
and full-word model field +20. It deliberately avoids declaring a fabricated
64-byte record beginning at the interior address `D_80139334`. This agrees with
unchanged accepted `sfx_stop`, freshly strict-MATCHed over all 396 bytes and
independently GNU-linked. Accepted `sfx_position_3d` also clears 3,328 bytes from
that bank start, consistent with 52 records. Only the former is freshly compiled
here; neither complete consumer is behaviorally executed by this packet.

The model-record view has stride 68 and word fields +60/+64, independently
witnessed by both exact setters and the native caller. Unknown bytes remain
explicit byte regions. These are inferred layout views, not original typedefs.
The packet neither defines nor claims ownership of either global bank. It does
not change any accepted source, shared/protected context, flags, target, or lock.

## Verification

From a checkout with the pinned IDO and GNU MIPS toolchains available:

    python3 cloud/work/frontier/dot_slot_setter_boundary_20261006/verify.py
    REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/conveyor/test_dot_slot_setter_boundary_20261006.py

`verification.json` is reproduced by a fresh compile, not trusted as acceptance.
The verifier records every ELF function's actual size, checks all relocated
bytes with both the production reader and GNU linking, rejects unresolved or
unverified relocations, and proves there is no owned data. Alignment bytes are
checked separately as zero; missing caller instructions remain mismatches.

Behavioral checks:

- 267,904 bounded native/linked function executions through the integer interpreter: all 65,536 signed-half
  index patterns for each helper, plus 2,880 caller fixtures covering six input
  alias arrangements, array-index boundaries, and upper-word narrowing bits.
- Actual return delay slots and every native instruction execute: 11/11,
  11/11 and 25/25. Full mapped memory, argument-home stores, and saved registers
  agree with an independent fixed-offset oracle. Scratch registers of void
  functions are not treated as return values.
- The unchanged normalized C89 source passes 2,880 matching alias fixtures with
  strict aliasing, undefined-behavior and bounds sanitizers, plus seven layout
  assertions. Two compiled semantic mutants (wrong field and early second read)
  are rejected.
- Unknown opcode, truncated return, and unmapped-input controls fail closed.

The tiny integer interpreter is intentionally limited to the actual supported
leaf instructions and rejects all others. It is not an N64 emulator. Exhaustive
negative helper indices are tested only as mapped native addresses. The hosted
C proof uses correctly typed arrays, key 0..5 and narrowed model 0..15; it makes
no claim for C negative indexing, invalid pointers, asynchronous mutation,
NaNs, gameplay reachability, or complete caller/whole-game behavior.

## Stop condition

A genuine new declaration/source boundary or independently measured compiler
mechanism is needed before reopening this residue. Inlining the known setters,
normalizing their witnessed global layout, and forcing those genuine inlines
are now measured negative hypotheses. Do not replace them with dead reads,
unused parameters, fabricated helpers, or pressure/volatile controls.

No production admission, shadow build, compressed image, full-ROM, hardware,
gameplay, full repository suite, or hosted CI result is claimed. No ROM bytes,
raw assembly, native objects, or credentials are included.

## Scoped regression status

The packet's three tests and selected scorer/guard/submission regressions pass:
**75 passed, 644 deselected**, with no selected failures or skips. The excluded
644 cases are the full locked-single matrix; most of their sources are omitted
from this space-constrained sparse checkout. An initial unfiltered invocation
reported 650 missing-source failures and 68 passes. After materializing the
small scorer sanity fixtures, the explicit selected run above passes. This is
not a full locked-source or repository-suite pass.

    REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/conveyor/test_dot_slot_setter_boundary_20261006.py tests/conveyor/test_cloud_score.py tests/conveyor/test_cloud_guard.py tests/conveyor/test_cloud_submissions.py -k 'not locked_single_function'

Independent review identified and closed an address-provenance gap: entry
addresses could previously drift without changing byte-only receipts. The
verifier now binds all four inspected native entries and every consumed
relocation-symbol address, records selected ranges, rejects unbound external
symbols, and has an address-only drift regression for each anchor. It does not
pin the entire manifest, so unrelated target updates remain allowed.
