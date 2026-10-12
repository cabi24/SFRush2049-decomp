# Image-B callback initializer: source and native ABI research

## Deliverable and accounting

Complete native-led `candidate.c` for `func_8038C910`, interval
`[0x8038C910,0x8038CA24)`, 276 bytes. **NONMATCH: 13/69 words differ.**
Empty claims; zero new strict-match, accepted-image, gameplay-oracle or ROM bytes.
No locked body, production source or integration setting is changed.

Current base: `84d8a4827e7fed06365846e013351b580593d921`. Fresh locks and open
PRs were checked before taking this disjoint target. Prior teardown research
reported a broad C910 scout but did not supply a committed C910 source. This
packet does not repeat the existing A408 research or claim already accepted
image-B helpers.

The initial fresh O3 scout reproduced 55/69 differences, but it used a chained
narrowed result assignment. The full-word result-store reconstruction in this
packet instead has an honest natural baseline of **58/69**, complete emitted
extent 272 bytes versus native 276. The initial chain is equivalent only under
the native producer's sign-extended-16 result contract; no broader equivalence
is assumed. The retained baseline and candidate both preserve full v0 in the
control record before narrowing the callback handle.

## Recovered semantics and remaining source uncertainty

For a valid selected player, do nothing if the signed callback handle is
nonnegative. Otherwise initialize callback state to zero, callback address to
B:AA8C, time from D_8002EB90 and player index; create the scene with resource,
layer 15, full parent word, flags 0x2000, player, column -1 and dynamic 1.
Store the returned word into the 64-byte control record and its low halfword
into the callback. Reload the vehicle's model after creation, then apply the
model-plus-13 vector through the real position helper. Helper mutations and
post-call rereads are preserved. Names are descriptive; no original source or
whole-function arcade ancestry is established.

Two explicit pointer/address/pointer round trips recover the observed address
materialization and remove IDO no-alias assumptions. The callback round trip
prevents embedded-parent folding; the time round trip prevents direct global
load folding. Original source spelling is unknown. These are disclosed source
shaping, not recovered author intent. Both preserve pointer values under the
verified native 32-bit ABI; the logical-host build uses sufficiently wide
unsigned long addresses. No extra local, arbitrary buffer capacity, padding,
fabricated formal/caller/helper, volatile or assembly is added.

Stock causal rollback results, including complete original-REL GNU replay:

- Natural typed baseline: 58/69, 272-byte full body
- Remove only callback address round trip: 42/69
- Remove only time address round trip: 49/69
- Final: 13/69, complete 276-byte body, 64-byte frame

Workbench identified structure/phase displacement before the address changes.
Stock UGEN tracing now shows parameter t0 in the callback store, control index
scale and outgoing argument store; stock as1 substitutes the earlier sign-
extension temporary t6 and reorders call setup. All 13 final differences are
within +0x60..+0xA0. Diagnostic-on/off whole objects are identical, M=20/T=28/P=4
and frame=64. No patched compiler or force-only object is accepted. Bounded
branch, expression-width, consumed-local and address-form controls did not
close the remaining scheduling gap and are not publication artifacts.

## AF5E0 return/argument contract: explicit integration blocker

The accepted `src/blob/save_slot_valid.c` declares void and narrow C formals.
C910's native call passes seven full O32 words, including a full 32-bit parent;
AF5E0 consumes the low signed halfwords of layer/parent/row/column. It finishes
by calling E26C and then only restores ra/sp and returns. E26C explicitly
returns its signed-halfword scene index, sign-extended in v0. C910 consumes
that result in both a full-word store and a halfword store.

The proof binds complete native AF5E0/E26C/D6FC bodies and the accepted wrapper
source. In a temporary control only, changing the wrapper to return s32 and
return its actual E26C result **still strictly matches all 176 native bytes**.
This establishes a return-bearing native ABI, not permission to edit its locked
production source or count another match. The candidate's address-spelled,
seven-word external declaration is an ABI-facing research contract, **not a
claim that it is compatible with the currently accepted C declaration**.
Shared declaration reconciliation remains an independent integration blocker.

The complete direct caller census across game/A/B target words is exactly
`audio_frame_update+0xD4`. Caller and helper words are input-bound. C910's native
entry sign-extends the low incoming halfword and homes the original argument.
Negative or unbacked player indices are not claimed as valid C inputs.

## Verification

Run from a normal current checkout with the pinned stock IDO 5.3 and GNU MIPS
binutils:

    python3 cloud/work/frontier/dot_runtime_b_callback_init_20261012/verify.py --score
    python3 cloud/work/frontier/dot_runtime_b_callback_init_20261012/verify.py --deep
    python3 -O cloud/work/frontier/dot_runtime_b_callback_init_20261012/verify.py --deep
    python3 cloud/work/frontier/dot_runtime_b_callback_init_20261012/trace.py
    REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_b_callback_init_research.py -q -o addopts=''

- Scorer performs full unmasked comparison, no uncertainty waiver. Candidate
  has no extra words, unresolved symbols, unverified sites or owned-data errors.
- Independent ELF reader and direct GNU ld consume all **16 original RELs**;
  verify every external address, complete STT_FUNC, allocated-section set,
  288-byte text and 12 zero suffix bytes. No owned data. Baseline's missing
  native tail word is counted, not borrowed from a neighbor.
- **1,024 field-oracle fixtures, 3,072 machine executions** compare protected
  native, complete GNU candidate and complete GNU baseline. All 69/69/68
  instructions execute across the matrix. All mapped nonstack memory and
  helper-boundary state/arguments agree. Caller-save GPR/FPR poisoning and
  callee-save preservation pass. Upper incoming argument patterns, absent/
  existing handles, model rereads and helper mutations are exercised. Unknown
  instructions fail closed; a compiled wrong creation flag is rejected.
- **8,192 actual-source logical-host fixtures** pass ordinary and ASan/UBSan/
  bounds builds, with sanitizer recovery disabled. Only LeakSanitizer is
  disabled. Complete selected and adjacent records are checked. Native IDO
  compile-time checks separately prove 16 sizes/offsets/pointer facts.
- Normal and optimized-Python deep replays bind their complete outputs to the
  receipt. One cheap test recompiles only candidate/baseline after validating
  inputs. Missing IDO skips via shared conftest; REQUIRE_TOOLCHAIN enforces it.
- Protocol-v2 receipts bind source, own verifier/interpreter/harness/trace/test,
  selected complete target/caller/helper words, relevant symbols, accepted
  wrapper source and recognized compiler passes. No shared-tool, whole-lock,
  Makefile or raw assembly-text hash pins. No static ROM TU is rebuilt.

## Limits and next step

Finite helper-hook fixtures do not execute real scene creation/position code,
prove unrestricted aliases, exceptional floats, asynchronous mutation, invalid
model storage, object lifetimes, full gameplay or original C declarations.
The matrix is a bounded machine/source experiment, **not a gameplay oracle**.
No accepted-image composition, recompression or ROM hash was run.

A useful next step is native-backed as1 dependency/alias evidence or original
callback-record declarations that explain why native retains t0 during setup,
plus independent reconciliation of the real AF5E0 shared C contract. Keep this
research draft. The independent checker owns merging and any integration.
