# camera_transform: context-first diagnosis

Research only. No accepted-source edit, match, coverage, image integration, or ROM claim.
The supplied structural candidate is **538/557 words different**, worse than the
clean baseline's **533/557**. It is retained to make the diagnosis reproducible,
not as a proposed production replacement. `claims` remains empty.

## What is established

The native helper contract is private whole-program allocation, not ordinary O32:

- `MP_TargetSteerPos` receives the entry in `s0`, volume in `f20`, and pan in `f22`.
- The native helper uses a 24-byte frame and saves only `ra`. It writes `f24`
  without saving it, so its caller must account for that clobber.
- The native caller saves four FP pairs and uses `f26` for the -2.0 sentinel.
  The exported baseline saves two FP pairs and uses `f20` for that sentinel.
- The exported helper instead receives `a0/a1/a2`, homes the float arguments,
  saves `s0` itself, and has a 32-byte frame. The caller uses an 80-byte frame
  rather than the native 88-byte frame. These are verified code-shape differences;
  they do not prove that context explains most of the 533 positional differences.
- The target corpus has exactly one direct helper call: `camera_transform+0x2e4`
  (0x80097F84). A search of committed assembly regions found no other direct
  call encoding. The allocation API invoked by the helper occurs only in this
  helper in the game target corpus. This does not recover eliminated pre-optimization
  callers or prove that an original address-taken/dead-source context never existed.

The existing historical keeper-based group still reproduces the helper itself,
but its artificial extra caller is not evidence of authentic source context.
No keeper, dead-code blocker, assembly, padding, invented caller, volatile pressure,
or altered compiler flags were added to the candidate here.

## Targeted context probes

All use the canonical IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared` group pipeline,
including the canonical assembler multiply-erratum option. The normal compiler
driver's verbose trace also confirmed its default O3 `-Olimit 5000`; that setting
is not an accidental discrepancy introduced by the group scorer.

| Hypothesis | Observed result |
| --- | --- |
| Exported clean baseline | 533/557, 523-word caller |
| Make genuine helper internal in same file | Inlined; 537/557, 56 excess words |
| Put helper in a separate C file | Identical result to same-file internal case |
| Forward-declare helper and define it after caller | Identical result to same-file internal case |
| Include real `func_80098FB8`/outer caller group from the existing effect-tick reconstruction | Still inlined; 550/557, 26 excess words |
| Reuse shared volume/pan setter helpers in both start helper and queue | Start helper still inlined; new non-native setter bodies cause four unresolved internal references; invalid matching candidate |

Consequently, neither file separation, declaration order, nor simply adding the
known genuine outer caller recovers the missing out-of-line/private-ABI context.
No successful context repair is claimed. The separate-file negative result is
consistent with the repository's whole-program `uld -kp` findings.

## Semantic/basic-block alignment

Independent manual review aligned these regions, using native addresses and
baseline object offsets (the latter include the preceding helper):

| Region | Native | Baseline object |
| --- | --- | --- |
| Completion history rotation | 0x80097CE4 | 0x224 |
| Flagged-entry scan | 0x80097D28 | 0x264 |
| Completed-voice cleanup | 0x80097DD0 | 0x314 |
| Queue prefilter | 0x80097EDC | 0x438 |
| Start / stop commands | 0x80097F70 / 0x80097FB0 | 0x4A4 / 0x4E4 |
| Volume / pan | 0x8009803C / 0x80098134 | 0x570 / 0x654 |
| Depth / rate | 0x8009823C / 0x80098344 | 0x748 / 0x83C |
| Global command | 0x8009845C | 0x934 |
| Retry bookkeeping | 0x80098498 | 0x974 |
| Final blocked-flag clearing | 0x800984F0 | 0x9AC |

Two concrete structural differences were identified independently of register names:

1. Native code has separate recycling callback pairs for early stop, normal
   completion, and retry exhaustion. Baseline uses a shared pair. Including the
   completed-voice cleanup, native has four static call sites for each list API;
   baseline has two. The existing effort packet's H06 already explored this
   duplication. This investigation diagnoses why that shape is relevant rather
   than claiming it as a new discovery or an exact-score improvement.
2. Native code turns each of the four setter callback results into a boolean with
   an explicit branch diamond. Baseline's `return callback(...) == 0` becomes
   branchless. Explicit `if (callback(...) != 0) return 0; return 1;` reproduces
   the diamond shape. Together with the recycling split, this adds 28 instructions:
   **523 becomes 551, compared with native 557**. The canonical mismatch is still
   538/557. A source-polarity control using `if (...) == 0` also has 551 words and
   scores 537/557; it is not selected merely for that positional score.

These source changes reproduce the native static call counts and remove much of
the instruction-count gap, but do not recover native register allocation, exact
scheduling, or the helper ABI. Length and call counts are diagnostic features,
not acceptance criteria. The canonical scorer is unchanged.

Manual review checked predicates, callback order, pointer reloads after callbacks,
volume/pan/depth/rate formulas, sentinels, history/budget updates, byte retry wrapping,
and the kind-zero retry exemption. It found no concrete normal-input behavioral
mismatch. The native rate path subtracts before comparing with zero while the
baseline compiler compares the product with one; floating-point exceptional/FCSR
behavior was not verified. This review is not a proof of whole-function equivalence.

## Verification and reproduction

With the existing IDO/toolchain environment configured, run from repository root:

```sh
python3 cloud/work/camera_context_20261007/run_probes.py build/camera-context
python3 cloud/work/camera_context_20261007/audit_context.py build/camera-context/exported_baseline/candidate.o
python3 cloud/work/camera_context_20261007/check_semantics.py
python3 -m unittest discover -s cloud/work/camera_context_20261007 -p 'test_*.py'
python3 tools/cloud/score.py group cloud/work/camera_context_20261007
```

The seven reproducible probes compile. The shared-setter probe is intentionally
reported as invalid due to unresolved references, not accepted or compared as a
clean candidate. Four synthetic instruction-field audit tests pass. The structural
candidate passes 2,048 deterministic host-C scenarios against the clean baseline,
covering callback traces and state. That test uses host pointers/layouts, bounded
inputs and callbacks, and does **not** execute the native code or establish complete
O32/FCSR or retail equivalence. The harness is preserved from the prior frozen
camera effort with the same limitations.

The audit emits numeric metadata, symbol names, source hashes and derived offsets;
it does not publish ROM bytes or raw disassembly. It is deliberately a syntactic
shape audit, not a general dataflow analysis or a replacement match gate.

## Timing and stopping condition

Investigation began 2026-10-07 21:21:06 UTC. Native-contract verification and the
fresh baseline object were ready at 21:22:17. File-boundary probes completed at
21:22:52; genuine-outer/shared-helper probes at 21:23:56. Structural alignment and
branch probes were complete by 21:29:03. The final seven-probe reproduction and
host verification ran at approximately 21:30:36.

The final reproduction's per-probe machine elapsed and child CPU times are in
`probe-results.json`. They total well under one second on this existing cached
cloud toolchain. Roughly ten minutes of elapsed investigation includes reading,
alignment, reasoning, review, tool orchestration, and artifact preparation; it must
not be represented as ten minutes of compiler work. Publication is later and is
not included in those compilation measurements.

Stop here rather than invent more callers or run an unbounded source search.
The next justified context step needs evidence of original source-level call graph,
visibility, or compiler inlining decisions that explains why this genuine single
caller did not inline its helper. Current committed target bytes constrain the
resulting ABI but do not uniquely identify that missing pre-optimization context.
