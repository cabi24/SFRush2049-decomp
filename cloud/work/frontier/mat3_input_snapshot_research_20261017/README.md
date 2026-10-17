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
stand-in callers were removed. Four existing members remain strict matches:
controller_poll (928 bytes), func_800C9590 (76), player_mode_set (60), and
player_state_set (60). These were already matched and receive no new credit.

Other context is unfinished and explicitly recorded: initialization is
48/63 words different; process_inputs is 86/89 with one extra nonzero word.
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
checks the full group, and enforces the empty-claims/nonmatch boundary.
It validates type layout by reading constants emitted by IDO, rather than
assuming compilation success proves a negative-array assertion. A deliberately
wrong reserved-byte width is detected by the emitted layout.

The behavior checks contain 1,014 fixtures and three independent executions:

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

Four independently compiled semantic negatives fail: wrong Y calibration,
collapsed Y output, the wrong analog-direction threshold, and replacing
accumulated pressed history instead of OR-ing it.

## Remaining work

The direct reconstruction's residual includes the held/pressed base-register
allocation, an early assembler-hoisted range-register move, and a CFE-created
narrow temporary reused by the two calibration increments. Ordinary declaration,
cast, real-call-context, axis-loop, and shared-field controls did not close it.
Explicit raw-value locals improve one local sequence but expand the frame;
those variants are not delivered as improvements.

This packet records a verified semantic reconstruction, not a byte match.
Build outputs, target bytes, object files, and raw assembly/trace dumps remain
outside the publication packet.
