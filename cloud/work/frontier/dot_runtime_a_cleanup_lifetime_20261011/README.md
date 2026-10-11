# Image A cleanup lifetime evidence: still NONMATCH

D05C remains **18/169 words different**, complete 676-byte extent
`[0x8039D05C, 0x8039D300)`. No source change, matching claim, accepted-image
byte or ROM gain is introduced. This additive packet binds the exact source in
`dot_runtime_a_record_context_20261011`; that packet's existing E200 claim is
unchanged and gets no duplicate credit here.

## New compiler finding

The unmodified stock IDO uopt `-l listing -zdbug:6` route emits D05C's complete
local-optimization, coloring and reemission record **before** failing later in
the group's renderer with its known `wrapper_ecvt` assertion. `diagnose.py`
reproduces this and publishes only target decisions. It does not assemble or
score diagnostic output. Thus the previous whole-listing failure does not
prevent using this target-complete partial trace.

The four residual allocations occur in this order: slot byte bound 2816 to s4,
panel byte bound 572 to s5, signed-byte player copy to s6, player counter to s7.
Retail instead puts the player counter in s4 and the other three in s5/s6/s7.
All 18 differences belong to that consistent allocation permutation. Actual
priority costs are not available from this listing, and no whole-group
listing fidelity is claimed.

31 recorded source forms test concrete hypotheses from the cleanup loops:
box-index reuse; guard and continuation structure; counter widths; post-tested
loops; explicit pointer/end scans; and native byte-offset induction. None beats
18 words. Narrow counter types and flattened-array forms are diagnostic
experiments only, with changed domains or C object-boundary concerns; none is
retained or represented as an equivalent production replacement. There is no
blind identifier sweep, padding, artificial read/pressure statement or fake
caller. The three adjacent empty native functions are not assigned speculative
bodies. Arcade ancestry is unknown because its checkout is absent.

A separate read-only hardware-breakpoint attempt failed at PTRACE_TRACEME with
EPERM. It was not retried through another route; no compiler, register or
executable-memory patch was made. The supported stock listing above is the
retained reproducible diagnostic.

## Complete-body proof and limits

Fresh ordinary canonical stock compilation uses the original O3 whole group
and as1 VR4300 multiply workaround. The strict scorer still reports 18/169,
zero unresolved/unverified relocations, zero own-data failures and zero excess
words. An independent GNU relocation link verifies all 56 relocations and the
complete 676-byte ELF STT_FUNC extent plus exact next boundary. D05C owns no
literal or storage data. The GNU step re-sections the complete function; this
is not original contiguous-TU placement proof.

512 native runs, 512 freshly GNU-linked runs and 512 independent ordered-cleanup
model runs agree in memory and service-call results. Native and compiled runs
also agree in every memory read/write and call trace. All 169 instructions and
37 branch outcomes execute; 209 cases mutate the player count from a helper,
checking its real reload boundaries. Inputs cover every signed-byte mode and
selected counts -32768, -1, 0, 1, 2, 3, 4, with positive counts capped at the
four-player storage. No exhaustive arbitrary-memory or unrestricted signed
count claim is made. This packet does not add a host-C execution proof.

The eight real external services are explicit stubs with arbitrary O32 integer
and FPU volatile clobbers and optional player-count mutation. Their actual
implementations, ownership/lifetime effects and gameplay are not established.
The independent model checks untouched bytes/guards as well as changed fields.
The decoder rejects unknown operations, escaped code, unaligned/unmapped
memory, and branches inside delay slots. A changed service-argument control
fails the model comparison.

D05C takes no arguments and intentionally clobbers s0 through s8; gp/sp/ra are
preserved. It is not a standalone O32 replacement. All four native and all
four fresh generated A1EAC callsites are enumerated; the caller's s0–s8
prologue/epilogue saves are checked. The caller remains nonmatching and is not
executed end-to-end.

## Reproduce

From the repository root, with IDO_DIR and GNU MIPS tools configured:

```
python3 cloud/work/frontier/dot_runtime_a_cleanup_lifetime_20261011/verify.py
python3 cloud/work/frontier/dot_runtime_a_cleanup_lifetime_20261011/diagnose.py
python3 cloud/work/frontier/dot_runtime_a_cleanup_lifetime_20261011/reproduce_research.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/conveyor/test_runtime_a_cleanup_lifetime.py tests/conveyor/test_runtime_a_record_context.py -q
```

Base: reviewed `84bdadc830834bde962aa532874a114b48353a06`. Deduplication inspected
master `0119270d89538f38195e8a36ad66269c04ebb4de` and all seven open PRs (163, 305–310) at
2026-10-11 14:43 UTC; D05C has no independent new match claim. Existing PRs
309/310 remain separately credited. Protected targets, manifests, compiler
policy, scorer, own-data verifier and accepted locks are unchanged. No native
bytes, assembly dumps, object files or credentials are included.
