# func_800EC270: tested initializer research, NOT MATCHED

## Result

This is a standalone natural-C reconstruction, **not a matching submission**.
The complete native extent is 136 bytes / 34 words at `0x800EC270`.
The candidate ELF function is **132 bytes / 33 words**, with three zero alignment
words in its 144-byte text section. Strict comparison reports **19/34 differing
words**, no nonzero excess words, and no unresolved/unverified relocations or
relocation errors. The extent difference is real; alignment does not make the
candidate a 136-byte function. `verification.json` records all residual offsets.

No new claims, accepted source changes, lock changes, target changes, scorer
changes, build integration, image coverage, or cartridge coverage are proposed.

## Recovered behavior and source scope

Ordinary O32 ABI: two pointers in a0/a1, no calls, no stack frame, no callee-saved
register use, void result. Minimal offset-view types are used; original type and
field names and full allocation capacities have not been recovered.

- State halfword824 becomes0; float788/792 become positive zero; float820 copies
  global `D_801543CC`; halfword826 becomes-1.
- If signed vehicle byte1996 equals1, the signed global counter is narrowed into
  both state halfwords828/830, then incremented with native32-bit wrapping. A
  signed updated value at least4 resets the global to0.
- Otherwise only halfword828 becomes0: halfword830 must stay untouched.
- When signed global halfword `D_80151CEE` is0, vehicle byte**2012** becomes1.
- State halfwords250/252/254 become-1/0/0.

The historical typed `newtargets_hi/v2_func_800EC270.c` placed the last vehicle
byte at2016. This reconstruction corrects it to the native2012 offset. The
historical byte-offset `tiny_A44` candidate already had the correct offset.
The explicit unsigned addition preserves native wrap without C signed-overflow
undefined behavior; conversion back to signed assumes IDO/GCC two's-complement
conventions. Legitimate runtime counter values may be narrower, but no such
range invariant is assumed by the tests.

One direct caller is found in the protected inventory: `world_physics_tick`,
word260. Indirect/computed callers are not excluded. No direct arcade equivalent
is established; the arcade reference tree is unavailable in this checkout.

## Causal work and remaining blocker

The tiny_A44 byte-offset baseline reports20/34. Typed state views with a cached
counter and actual named constants (`enabled=1`, `invalid=-1`) reproduce the
opening eleven words, then differ in how the global counter address is held and
how stores/branches are scheduled. The best ordinary C remains19/34. Moving the
counter declaration, spelling the actual increment separately, and changing the
actual constant's signed width do not improve it. A global-definition control
does not help and is not included. No extra input, helper body, padding operation,
volatile barrier, pointer laundering, arbitrary reflow sweep, or weakened scoring
was used. Further work should examine IDO address-web selection from genuine
source context rather than inventing compiler pressure.

## Verification

- Author native-instruction interpreter versus host-compiled candidate:
  **90,896 cases pass**, covering all256 input byte values, all65,536 mode
  halfwords, counter sign/narrowing/wrap extremes, float bit patterns including
  NaNs/infinities/subnormals, and10,000 deterministic random cases.
- Full vehicle/state buffers compared; untouched bytes must remain unchanged.
- Separate ASan/UBSan host semantic harness: **500,000 cases pass**. Leak detection
  alone is disabled because this execution environment uses ptrace.
- Independent reviewer: **67,584 separate host cases**, complete source/ABI
  review, fresh strict compile and132-vs136-byte extent check pass for research.
- Candidate C89 strict syntax/warnings pass; native target manifest verifies;
  all161 current static locks remain intact.
- Repository CI-style suite: **1,307 passed, 41 skipped, 9 deselected**.
- Current blob/group source hashes pass their canonical checker; all23 protected
  manifest files verify. Protected paths remain unchanged.

Tests use disjoint accessible vehicle/state/global regions. Concurrent mutation,
aliasing between those regions, real N64 execution and FCSR behavior are not
established. Semantic tests do not imply an exact compiler match.

## Reproduce

From repository root with pinned IDO and the repository's test dependencies:

```sh
# Expected exit1, 19/34 differing words.
python3 tools/cloud/score.py fn cloud/work/dot_state_initializer/candidate.c \
  func_800EC270 --flags '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
python3 cloud/work/dot_state_initializer/verify_semantics.py --output /tmp/state-semantics.json
```

This directory is intentionally outside automatic matching-submission selection;
normal PR CI checks protected paths, accepted locks, scorer sanity and repository
tests. A green CI status does not rerun this research harness or prove matching.
No original ROM, complete extracted image or derived blob layout is available.
Image splicing, compression identity, full-ROM SHA-1 and `make test` have not run.
