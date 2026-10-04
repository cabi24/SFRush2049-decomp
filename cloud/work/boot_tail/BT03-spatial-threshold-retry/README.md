# BT03 spatial threshold retry

Exclusive retry claim: `func_8001DC08`, 472 bytes. Branch
`dot/boot-tail-bt03-spatial-threshold-retry` starts from clean
`68a48a5e2bb3f88625d3af20cfd5a8e899a43944`; the original PR74 packet
and cut15 are unchanged. This retry adds no matching credit unless independently
reviewed whole-function equality and the exact ELF function extent both pass.

## Evidence before experiments

All six retained DC08 source forms, the prior 30-row packet control record,
independent review, native layout/address proof, and actual-source tests were
read. The complete native DC08 body, sole direct boot-tail caller DDE0, and
complete B1D0 and CCDC helper bodies were inspected. The no-input ABI, cached
unknown external thresholds, all five float arguments, counter wrapping, and
live post-callback list/state ordering remain required.

The published source reproduces exactly 3/118 differing words, 472-byte ELF
function size, with differences only at offsets 0x48, 0x5c and 0x64: a
permutation of the two threshold loads and the flags-clear mask materialization.
Canonical relocated temporary objects give a schedule-only diagnosis; using an
unrelocated object produces misleading relocation noise and is not used for
classification. The archived upper-first form reproduces 5/118 and swaps both
threshold FP registers as well. Stock `-Wa,-R` tracing leaves each whole object
byte-identical to the matching compile. All raw traces/objects stay temporary.

## Directed hypotheses

1. The vendored IDO laws L59/L80 describe physical-line tie-breaking and hoisted
   loop-header line ownership. The fresh stock trace exposes different source-line tags among preheader
   nodes, justifying one bounded grouping control without assuming decoder
   accuracy. One token-identical join of snapshot/group initialization
   and the loop header is a bounded scheduling control, not a whitespace sweep.
2. Pinned CC0 `AxioDL/musyx` revision
   `78d2e16e4905fc675952162d331c24d5198b2687`, `src/musyx/runtime/snd3d.c`,
   `StartContinousEmitters`, uses the genuine else-if threshold chain,
   prefix-incremented counter test and assignment-in-condition start result.
   Those shapes were not among the prior DC08 controls. Their native-supported
   forms can test threshold web birth/order without changing ABI or semantics.
   Public-family literal threshold values, newer room/studio/voice-count fields,
   extra arguments, flags and helpers are excluded.

Sources and verification records below will distinguish every control from an
accepted match. No target, scorer, lock, layout, symbol or old-packet file changes
are authorized.
3. Because the upper-first prior control has native load order but inverted FP
   colors, one scoped `const float` declaration-initializer form tests whether
   truthful immutable snapshots separate those two decisions. This is distinct
   from the exhausted external-const/direct-read control: both real external
   values are still read exactly once before callbacks.

## Result and stopping condition

**COMPLETE-NONMATCH, 2/118 words; exactly 472-byte ELF function extent.** No
function or byte receives matching credit. The retained source is token-identical
(to whitespace normalization) to the independently reviewed published source.
Only the preheader's physical line grouping changes.

Three new source forms / six archived compiler rows were tested:

- Preheader line join: O2 3 -> 2 words. O1 remains 118 with 11 excess words.
- Genuine family condition forms: O2 stays 2; O1 is 118 with 12 excess words.
- Scoped immutable snapshot initializers in native read order: O2 4, with the
  threshold registers swapped consistently; O1 is 118 with 11 excess words.

All O2 forms have exactly 472-byte ELF function size. The retained residual is
only at +0x48 and +0x64. Native order is upper `D_8002D904` then lower
`D_8002D908`; the retained source emits the same two fully relocated loads in
reverse order. Their registers, comparisons and every other relocated word
agree. Both external float contents remain unknown, with no inferred literals.
The trace shows a dependence through the shared address register, so another
line placement cannot simply reverse the remaining pair at the assembler stage.
Upper-first forms repair load order but change FP allocation instead.

Independent review found 49 explicit parser disagreement labels for the
baseline and 47 for the retry. A further 17 baseline and 21 retry `list-order`
labels follow earlier filtering that had already excluded the actual winner.
These labels cannot prove exact scheduler tie causes. The reviewer separately
compared the raw 12-node preheader descriptors and initial ready list: removing
only each `lineno` value makes them byte-identical. All other instruction,
relocation, dependency, hazard, successor and ordering metadata is unchanged.
The source tags collapse from lines 35/36/37/38 to line 35. This direct comparison
supports the physical-line experiment without asserting a precise scheduler
tie-key chain. The token-identical 3-to-2 result, exact instruction multiset,
canonical-relocated classification and whole-object trace-inert gates remain
independently measured.

Candidate generation stops here. A further retry requires new source/compiler
input on immutable constant-pool hoisting or threshold web creation that retains
both real external snapshots. No such source-supported form is established in
this packet. Repeating snapshot order, local-order, const, condition or line
controls, inventing values, forcing registers, adding fake locals/formals, or
editing targets/scoring is not a next hypothesis. This is a bounded stop, not
an impossibility claim.

## Validation

`verify.py` reproduces every new compiler row, the original three-word baseline,
the token-only equivalence, exact ELF identity/extent, whole-body relocation and
source/compiler fingerprints. IDO assertions independently prove native sizes
and every directly used field offset. Each unknown external threshold is loaded
once and resolves to its original address. The protected target manifest and
439-function / 99,120-byte census agree.

`scheduling.py` rebuilds canonical relocated temporary objects, reruns the
unchanged workbench, gates stock trace inertness against full object equality,
and records source-free scheduling metadata. It independently finds DDE0 as the
only direct boot-tail caller. The complete DC08/DDE0/B1D0/CCDC native audit
corroborates the no-input ABI, genuine three-input start and six-input parameter
helper. Helper declarations remain declarations only.

`test_semantics.py` reuses the reviewed actual-source strict-C89
ASan/UBSan/float-cast-overflow fixture against four source paths (the three new
forms plus the identical retained path): **9,600 actual source invocations**,
2,400 per path. It covers synthetic threshold boundaries, halfword wrapping,
failure flags, callback threshold mutation, live group count/list reloads and
argument forwarding. Only LeakSanitizer is disabled. Host LP64 behavior checks
and IDO native-layout checks remain separate. Valid bounded storage/list state
and ordinary FP comparison semantics are assumed; no malformed-list,
asynchronous-mutation or downstream playback-safety claim is made.

No original packet, shared ledger, cloud match, target, scorer, lock, layout,
symbol, gated helper or runtime file is changed. No ROM bytes, raw dumps,
objects, credentials or unrelated private data are included. Central owns
integration/publication; the independent checker alone merges.

## Independent review

Bounded independent PASS covers source head
`a31493f7abac7077d09855d7fd457b93f334189d`, tree
`9acd8c26e45af98ff6923fcb8091e3c132efa4fb`. All six new controls, 30 prior
controls, 9,600 sanitizer calls, full native/ABI/provenance audit, manifest,
protected paths and 161 locks pass. `independent_review.json` binds the source,
scripts and recorded results by hash. This later receipt/documentation commit
adds the review and clarifies its scheduler caveat; it changes no source or
verification result. No hosted-CI or publication claim is included.
