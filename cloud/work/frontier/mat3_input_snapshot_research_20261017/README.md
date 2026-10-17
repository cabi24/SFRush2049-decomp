# N64 input snapshot reconstruction research

Research only. Empty claims, zero new matching bytes, and no canonical group,
image, or ROM change. This is native-controller work from the MAT3 comparison
campaign; no arcade or GameCube controller implementation is claimed as a donor.

The 676-byte input snapshot function, func_800E7134, is reconstructed as a
four-pad update under the input queue lock. The inherited context had two real
errors: Y used X's calibration, and the normalized-output pointer collapsed
instead of advancing through four float pairs. The readable source repairs
both and uses the native 16-byte record and separate signed X/Y axes.

The new body is still **108/169 words different**. Its 676-byte extent and
48-byte stack frame agree with the native function, but instruction allocation
and scheduling do not. Zero extra words, unresolved symbols, unverified
references, or scorer errors are observed for this function.

## Source and context

The group retains the genuine controller polling, normalization, player-mode,
player-state, initialization, and process-input routines. Inherited legacy
stand-in callers were removed. Five existing functions remain strict matches:
controller_poll (928 bytes), func_800C9590 (76), player_mode_set (60), and
player_state_set (60), and process_inputs (356). The latter is copied unchanged
from the accepted cloud/matches/process_inputs.c, replacing obsolete inherited
context. These were already matched and receive no new credit.

Initialization is also repaired: every calibration byte resets on every call,
including the first Y byte that inherited decompiler output had incorrectly
placed under the queue-creation branch. The asynchronously updated query status
is an unsigned byte, rather than the inherited word declaration. The corrected
initialization remains 47/63 words different and 248 bytes versus native 252.
This packet is not an installable replacement group.

The best type-correct controller service remains 4/149 words different with
this actual callee context. A void-return osRecvMesg declaration can remove
that isolated residual but violates the SDK contract and is excluded.
Adding the actual SDK receive body does not remove the residual. No source
prototype, register constraint, padding, or synthetic keeper is used to claim
an exact match.

## Verification

Run with stock IDO 5.3 available through IDO_DIR and MIPS binutils on PATH:

```sh
python3 cloud/work/frontier/mat3_input_snapshot_research_20261017/verify.py
python3 -O cloud/work/frontier/mat3_input_snapshot_research_20261017/verify.py
```

The replay binds complete inputs and authenticated native target hashes,
checks the full group and exact function extents, binds the complete object,
and enforces the empty-claims/nonmatch boundary.
It validates type layout by reading constants emitted by IDO, rather than
assuming compilation success proves a negative-array assertion. A deliberately
wrong reserved-byte width is detected by the emitted layout.

The snapshot behavior checks contain 1,014 fixtures and three independent executions:

- Native MIPS body and its real 19-word normalization helper
- Compiled candidate MIPS body and that matched helper
- Host-compiled C body extracted from the exact group source

Both MIPS executions and the host result agree with a separate field-level
model. Cases cover all four pads, disconnected state, signed-axis extremes,
analog-to-digital thresholds, reset-button behavior, independent X/Y
calibration, calibration saturation, existing held/pressed bits (including
upper bits), initialization, queue arguments, and all eight float outputs.
The MIPS checks also require saved-register and stack-pointer preservation.

Initialization and OS queue calls are deterministic boundary stubs; no N64
runtime execution or OS implementation equivalence is claimed. The native
normalization helper itself executes, including its private integer/FP
argument convention. The native tests reach 186 of 188 combined instructions;
two uncovered duplicate paths arise from branch-likely scheduling.

Four independently compiled snapshot negatives fail: wrong Y calibration,
collapsed Y output, the wrong analog-direction threshold, and replacing
accumulated pressed history instead of OR-ing it.

The initialization checks add 216 native/candidate/host fixtures. Both real
60-byte player-default setter helpers execute in the MIPS checks; every one
of the 63 native initialization instructions is reached. Cases cover zero and
nonzero initialization flags, first and later queue setup, all eight calibration
bytes, and six query-byte scripts including repeated busy values before release.
All visible state, queue arguments and ordering, query-read counts, saved
registers, and stack restoration are checked against a separate model.

Queue creation, queue seeding, and controller_poll are boundary stubs in these
initialization tests. The host C body and setter bodies are extracted unchanged;
volatile query reads use a scripted accessor to represent asynchronous updates.
No real-time scheduling or complete controller subsystem claim is made.

Five independently compiled initialization negatives are rejected: skipping the
first Y reset after queue creation, using a word-width query read, the wrong
queue capacity, a blocking initial seed, and reapplying player defaults on every
call. All proof checks use explicit exceptions and remain active under Python -O.

## Remaining work

The direct reconstruction's residual includes the held/pressed base-register
allocation, an early assembler-hoisted range-register move, and a CFE-created
narrow temporary reused by the two calibration increments. Ordinary declaration,
cast, real-call-context, axis-loop, and shared-field controls did not close it.
Explicit raw-value locals improve one local sequence but expand the frame;
those variants are not delivered as improvements.

Initialization additionally retains an unmatched constant-one lifetime and
queue-status addressing sequence. Ordinary loop and native field-type controls
do not close it.

This packet records verified semantic reconstructions, not new byte matches.
Build outputs, target bytes, object files, and raw assembly/trace dumps remain
outside the publication packet.

## Verification hardening follow-up

The original verifier checked source, target and object hashes but did not compare
the complete saved result. A reproduced negative showed altered receipt claims
and an unverified comparison row being ignored. Protocol 2 now validates the
entire saved report before compilation and requires exact equality after replay.
It binds the actual IDO executables, scorer and own-data verifier, accepted-match
lock, protected target manifest and symbol table, accepted process-input source,
and this README. All seven emitted native functions are inventoried and their
complete extents and protected word counts enforced. No C, recipe, old comparison
row, complete-object hash, model, fixture set or semantic outcome was changed.

The focused tests include an optimized full replay, receipt/input refusal cases
under Python -O, and the standard missing-IDO diagnostic. Tool fingerprints use
the scorer's availability check, so absent IDO skips locally and fails when
REQUIRE_TOOLCHAIN=1. The original independent review remains the source/behavior
review; the separate hardening review covers this verifier-only update.

    python3 -m pytest tests/cloud/test_mat3_input_snapshot_research.py -q
