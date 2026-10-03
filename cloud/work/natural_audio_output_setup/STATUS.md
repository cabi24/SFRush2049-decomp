# audio_output_setup: semantic reconstruction, NONMATCH

## Result and scope

The primary `audio_output_setup.c` is an ordinary typed-list reconstruction.
It is **not a matching submission**: strict scoring reports **24/37 differing
words**, including the final missing word (144 compiled bytes versus 148 target
bytes). No source is added to `cloud/matches`, no lock is changed, and no ROM,
splice, or accepted coverage improvement is claimed.

This builds on `cloud/work/game_C29/audio_output_setup.directed.c`. The useful
new contribution is the independent full-body verification and tests, not a
new byte match. The sum and node count field use unsigned 32-bit arithmetic
because the target's `addu` implements wrapping addition. Field/subsystem names
are provisional; the repository's function name is retained.

## Reconstructed behavior

1. Receive a blocking message from `D_80152770` using `osRecvMesg(queue, 0, 1)`.
2. Select the explicit list, or read `D_801527C8` after acquisition when null.
3. Traverse its head at offset 8. Each entry has next at offset 4, count at
   offset 12, and a signed exclusion byte at offset 20. Sum only zero-exclusion
   entries, modulo 2^32.
4. Return the token with `osJamMesg(queue, 0, 0)`, then return the sum.

The calling convention is ordinary o32: a pointer in a0, result in v0, a
32-byte stack frame, and preservation of callee-saved registers. OS function
return codes are deliberately ignored, as in the native target. Valid acyclic
lists and valid global pointers are required; no invalid-pointer/cycle behavior
is claimed.

## Evidence

`verify.py` independently parses the protected target and verifies every
manifest file; it does not import the scorer. A fresh pinned IDO compile is
linked with GNU MIPS ld against verified symbol addresses. The complete object
is compared without relocation masks. `verification.json` and `score.txt`
record the reproducible NONMATCH result.

A fail-closed integer MIPS interpreter executes both complete bodies with
branch-likely annul and delay-slot handling. It tests 4,010 scenarios (8,020
executions): empty lists, explicit/default selection, signed-byte exclusion,
unsigned wraparound, and seeded randomized chains up to 49 entries. OS stubs
check exact arguments, poison every integer caller-saved register, and change
the default pointer on acquisition. Assertions require balanced acquire/release,
reads inside the acquired interval, no non-stack stores, no count reads from
excluded entries, preserved callee-saved registers and sp, and agreement with
the independent mathematical sum. This is a bounded interpreter, not hardware
emulation or a ROM validation. Negative tests check unknown instructions and
incorrect calls are rejected.

Run with pinned IDO and MIPS binutils on PATH:

    python3 cloud/work/natural_audio_output_setup/verify.py
    python3 -m pytest tests/conveyor/test_audio_output_setup_reconstruction.py
    python3 tools/cloud/score.py fn cloud/work/natural_audio_output_setup/audio_output_setup.c audio_output_setup

The last command must fail honestly with 24/37 differing words. The workbench
classifies the current geometry as a structure mismatch: 37 versus 36 real
instructions, with changed branch/load placement and register allocation.

## Rejected lower-score experiment

`layout_experiment.c` erases the cursor's type so it first holds a list and then
an entry. It scores 12/37, but independent source review rejected it as a natural
reconstruction: the type erasure has no original-source provenance and was
chosen to shape compiler allocation. It is retained only to explain the
rejected experiment, **not as a recommended source or matching improvement**.
No dummy helpers, padding locals, dead reads, ABI tricks, or protected-file
changes are used in the primary reconstruction.

## Independent review and host tests

A separate reviewer recompiled the frozen typed source, confirmed the strict
24/37 result and o32 frame/argument/return behavior, inspected the interpreter,
and independently ran 20,000 host scenarios under AddressSanitizer and UBSan.
`peer_review.json` records that review; `host_test.c` preserves its test harness.
The pytest wrapper reruns those host cases with UBSan and also verifies the
complete target/candidate interpreter proof. The host test uses native host
structure layout only to test C semantics; MIPS layout is checked by the
independent compiled-word/interpreter path.
