# Independent behavior-first packet review

**Approved for research-only publication. No blocking issue found.** The fresh
formulation and the preserved four-word candidate remain separate. Neither is
an accepted match, and this review establishes no game-image or cartridge gate.
The exact seven reviewed packet files are hash-bound in `peer_review.json`.

## Freshly rerun

The reviewer first ran the SDK-backed host/native/oracle phase without IDO,
then the complete stock-IDO compile and linked replay, and then all three
unchanged-body SDK-context controls. Results:

- Fresh source: **6,926** host C99/UBSan, complete protected-native, independent
  scalar-oracle, and GNU-linked IDO behavior cases pass.
- All 445 target instruction offsets and all 16 mode/flip/stretch combinations
  are exercised. Compiled size is **540 bytes versus 1,780**, deliberately and
  correctly rejected by exact-extent/matching checks.
- The six deliberate wrong-contract controls reproduce all reported counts.
  The narrowed-y control is missed by all **3,828** old host-domain cases and
  rejected by exactly **two** new discriminators.
- All three SDK-context variants retain identical linked 1,780-byte bodies
  with the same four residual offsets. Each complete 6,828-case replay passes,
  followed by 98 additional native cases.
- Reproduced receipts agree with the author's receipts except whole-object
  hashes. The fresh object differs only in `.mdebug` path metadata; linked
  bytes, source bindings, relocations, extents, and behavior agree.

The new source was also inspected operation by operation. Clipping comparisons
remain signed; texture and command arithmetic uses unsigned 32-bit operations.
Pre-stretch height, strict rejection, mode-dependent far edges and derivatives,
vertical-flip-only phase, and absence of a second bottom clamp agree with the
native contract. A single authentic SDK macro emits the three packets. No
signed-overflow trick, volatile pressure, instruction injection, or acceptance
gate change is used.

## SDK provenance and layout

Independent GitHub reads of
[2.0I gbi.h](https://github.com/n64decomp/libreultra/blob/master/include/2.0I/PR/gbi.h)
and [2.0I mbi.h](https://github.com/n64decomp/libreultra/blob/master/include/2.0I/PR/mbi.h)
return the exact two Git blob IDs pinned in the packet. The local connector
copies have one additional final newline; removing that newline for identity
checking reproduces both Git blob hashes. Macro/union content is not replaced.
These are authentic mirrored SDK sources, not evidence of the game's unique
original SDK revision. Downloaded headers are not included in publication.

The review measured host `sizeof(int) = sizeof(unsigned int) = 4` and
`sizeof(Gfx) = 16`. The host harness intentionally compares command words and
counts packet elements; it does not claim an eight-byte LP64 union. The compiled
MIPS replay separately verifies actual eight-byte strides, 24-byte advancement,
buffer guards, and callee-save preservation. C89 syntax checking succeeds with
an existing SDK bitfield-extension warning.

## Regression tests and the false-assumption correction

`tests/cloud/test_texture_rect_fresh_behavior.py` adds SDK-free CI coverage for
source/design/context bindings, all six named and rejected mutants, the shorter
nonmatch extent, and the unchanged four-word SDK-context residual. It executes
all 98 new cases against the complete protected native stream and scalar oracle.

The y tests compute the historical 3,828-case sensitivity gap and both new
counterexamples, rather than trusting receipt assertions. They establish that
an unconditional narrowing operation changes broad native-word behavior. They
do **not** establish that either input occurs in gameplay or prove that the
original source could not have used a restricted narrow-argument domain.

CI fetches no headers or network resources, invokes no compiler, and does not
silently substitute a fabricated SDK definition. Full authentic-SDK host/IDO
replay stays an explicit local reproduction documented in `REPORT.md`.

## Limits

The native interpreter and scalar oracle are shared with the earlier verifier;
this review independently reruns them against the fresh source and compiled
output, rather than claiming a second independently written emulator. The
proof domain has valid disjoint ordinary globals and packet storage, with no
concurrent mutation. Different read order is permitted only within that domain.
The finite tests do not establish hardware rendering, gameplay reachability,
all caller lifetimes, original parameter types, or a unique original C layout.
The earlier baseline and all protected files remain unchanged.

## Completed checks

All **11** new SDK-free tests pass; combined with the nine existing rectangle
verifier tests, **20** focused tests pass. Python syntax compilation and
`git diff --check` pass. These checks are additional to the explicit SDK-backed
host, stock-IDO, full-link, six-mutant, and three-context reruns above; they are
not a claim of whole-repository CI or cartridge acceptance.
