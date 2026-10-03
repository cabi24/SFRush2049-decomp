# D06 tire-force stack-home diagnosis: rejected hypothesis

**NONMATCH; frozen again. No matching or accepted coverage claim.** This packet
adds one negative compiler result and its replay, not a replacement reconstruction.
The complete preferred source remains in [PR #9](https://github.com/cabi24/SFRush2049-decomp/pull/9)
at commit `433611408270ede7fe6acdb027f1b82179209511`.

## Identity and ownership

- Packet D06; lane D. Base `d701b59592463d1e33bcee7011ed2a1e6c07b066`.
- Target `func_800E23A4`, address `0x800E23A4`, complete native 1,688 bytes / 422 words.
- Historical purpose: `drivsym.c:forces1`, vehicle tire-force updater. One real vehicle pointer argument.
- No context members edited. Current locks, PR 1–41 inventory, archived PR #9 evidence,
  and round 7–10 reservations were checked. This is explicitly old residual work.

## Fresh baseline and diagnosis

The archived source SHA-256 is
`a78bf705049cb5d0db9fc8bcb09efa0ac819ba1e11c4d65c6daf377a3f68e8bb`.
Fresh genuine O3 compilation reproduces the archived complete object SHA-256
`d0378d869bfc6c1993113473087cdeaa7a78bb4058988cb3bcb28d781bba8e8c`.
The O2 control has the same two word differences. Native and candidate both have
422 executable words and a 152-byte frame. ELF `.text` has 1,696 bytes including
8 zero alignment bytes; those bytes do not add native coverage.

Workbench `diagnose` was run before the source experiment. It reports identical
register lanes and one displaced stack home. Its unlinked-object overview also
flags 37 relocation sites because the protected target object is made from final
native words and therefore has no symbolic relocation records. That overview is
not a strict linking proof; the canonical scorer resolves ordinary symbols.

Only the store at native word 179 and reload at word 190 differ outside the six
local-pool references: native stack displacement 124, candidate 120. Both belong
to the same real rear-traction boolean across the first rear tire-force call.
All eleven pointer homes stay at their native positions. Inspection finds no
other direct native load/store of slot 124, and no baseline load/store of slot120
outside this pair. This is a local-home placement residual, not a register-color,
argument-count, frame-size or call-schedule problem. Slot movement is still a
full-word mismatch and cannot be ignored.

## One new bounded hypothesis

Vendored workbench law L51 describes an exact CFE value-form expansion of `&&`:
assign its first operand to the existing destination, then conditionally assign
the second operand. The archived tire packet records split traction scalars,
expression/layout controls, real helper contexts, donor BOOL and debug controls;
it does not record this precise same-local expansion. This experiment tests
whether that expansion changes the pooled temporary responsible for the home.
It adds no scalar, declaration, helper, argument or artificial operation.

The replay makes exactly one replacement of the rear predicate, keeping the
actual asymmetric wheel-zero read in the second operand. A 343-case comparison
checks both Boolean result and short-circuit read sequence across the three
relevant wheel values, including signed integer limits. This is a predicate
check only, not complete vehicle dynamics or N64 floating-point validation.

**Result: reject.** Both O2 and genuine O3 give 276/422 differing native words,
with one nonzero word beyond the target extent. The function has 424 executable
words (1,696 bytes), a 152-byte frame, and still spills/reloads the boolean at
120, now at word indices 181/192. Its first change is in predicate evaluation
at word143; two added instructions shift the later body. Thus L51's exact-object
observation on a different game/compiler recipe does not transfer to this case.
It does not remove or move the offending home and is not an improvement.

## Reproduce and limits

Fetch the source archive once if that commit is absent:

```
git fetch origin pull/9/head
export IDO_DIR=/absolute/path/to/pinned/ido53
python3 cloud/work/d06_tire_stack_diagnosis/replay.py
python3 -m pytest tests/conveyor/test_d06_tire_stack_diagnosis.py
```

The replay verifies the archive source hash, applies the single documented edit,
compiles both sources under O2 and the real uld/usplit/umerge/uopt/ugen/as1 O3
pipeline, checks exact expected residuals and excess-code rejection, and emits
only numerical metadata. Raw objects and instruction arrays are temporary and
are not committed. It uses the protected scorer without modification.

Both fresh scores have six section-relative pool references unverified by the
cloud scorer. PR #9 records a prior protected full-pool proof for the identical
baseline O3 object; this packet does not rerun that private image proof and does
not elevate the rejected control to fully relocated evidence. No ROM, image,
compression, splice, lock or accepted source is changed or verified here.

## Recommendation

Keep this target frozen. Do not reset its exhausted declaration/expression budget.
A useful future attempt requires concrete compiler symbol/home provenance that
explains why only this named consumed Boolean needs a different home while all
eleven pointer homes remain fixed. A generic heuristic about temporary pooling
is now insufficient. No further candidate was executed in this packet.

The handoff's next D06 target is `func_80087110` (1,780 bytes), but PR #9 already
contains its four-word edge-addition scheduling plateau. It is not a fresh target
and should be queued only with a genuinely new native-context explanation;
this packet does not reserve or reopen it.

## Independent review

Lane B independently reran all four builds and the two focused pytest tests,
reviewed the exact source replacement, and confirmed the rejected function's
terminal return delay slot ends at byte 1,696. Review approved this as bounded
negative research only. O2 whole-object hashes can vary with temporary absolute
source paths in ELF metadata; its compared code, spill positions and scores are
stable. O3 complete object hashes reproduced exactly in both runs.

Local canonical scorer controls pass: `sound_handles_clear` and all three
`resource_slot_clear` members. Focused tests: 2 passed. The first full repository
suite attempt stopped at collection because decomp-permuter's `src.scorer` was
absent in this fresh worktree; it is not counted as a suite pass. Exact-head CI
uses its normal recursive-submodule setup and remains a separate required check.
