# Pooled object constructor: NONMATCH research

`func_800B3704`, retail extent 0x800B3704–0x800B37E8 (exclusive),
57 instructions / 228 bytes. Natural C compiles to 56 instructions / 224 bytes:
**41 of 57 full resolved words differ**, including the absent final target word.
No claims, accepted matches, coverage, integration or ROM identity are asserted.

## New reconstruction

The count at D_80149788 selects one of 200 pointer-table entries at D_80149450.
At count >= 200 the routine returns NULL without calls or writes. On success it
increments the count, initializes specific object fields, invokes three callbacks,
and returns the selected object. The first callback may update the resource at
+0x0c: the second call must read it afterward. The second call receives the original
full-width x/y arguments even if the first callback changes the object's narrowed
coordinate fields. Its return is truncated to the halfword at +0x34 before the
third callback observes it. The original object pointer survives all callbacks.

PoolObject is a 64-byte accessed-prefix view, not a proven allocation size. Unknown
bytes retain their contents. The word at offset zero may ultimately be a pointer;
`u32 owner` deliberately preserves its observed bits without claiming provenance.
Other names, including resource/handle/x/y, are hypotheses. Real standard o32
arguments and saved s0/ra are preserved; no fake arguments, helper/padding calls,
volatile barriers, pointer laundering or optimizer-only casts are introduced.

Precondition: count is nonnegative, valid table entries reference accessible,
nonaliasing writable objects, and callbacks obey the o32 ABI. A negative count
indexes before the table in retail and is intentionally outside the tested valid
domain. Signed narrowing of x/y and the returned handle outside s16 is
implementation-defined C; IDO and the tested host compilers use the observed low
16 bits. There is no concurrency guarantee. Callbacks are modeled with explicit
ABI-compliant stubs, not actual game execution.

No direct arcade equivalent was established. The documented arcade source tree
`reference/repos/rushtherock` is absent in this checkout; the repository xref and
existing source references were searched. Classify this as N64-specific resource
construction, with naming unconfirmed.

## Causal experiments and residual

Baseline O2 and standalone O3: 41/57 different, 224 versus 228 bytes.
O1: 56/57 different plus 14 nonzero excess words. Both explicit count-local and
separate count-increment variants reproduce 41/57; no improved source evidence.
Workbench diagnose classifies a structure mismatch, true instruction delta -1.
Manual full-word/relocation review identifies shared -1 materialization in the
candidate versus two retail immediates, global-address/live owner allocation, and
subsequent shifted temporary-register schedule. No artificial field-type split
or redundancy was introduced to force duplicate constants.

## Verification

- GNU MIPS linker independently resolves all global and call relocations; complete
  raw retail and linked candidate words plus all differing offsets are retained.
- Canonical scorer agrees NONMATCH41/57, no unresolved/unverified relocations or
  nonzero excess instructions. Protected target manifest is validated by scorer.
- 10,000 complete target-versus-candidate executions pass a fail-closed bounded
  MIPS interpreter, with delay slots, signed guard, poisoned caller-saved registers,
  preserved s0–s7/gp/fp/sp, exact writes and call order, callback mutation, resource
  reread, full-width original arguments and result narrowing.
- 100,000 host cases pass ASan/UBSan; the byte-complete independent expected-state
  oracle checks untouched bytes, offsets, overflow boundary, callbacks and return.
- Three pytest regression tests include UBSan host behavior, the full differential
  replay (when IDO/binutils are available), and unknown-opcode rejection.
- Independent peer review is recorded separately.

ASan leak detection alone is disabled because this runtime runs under ptrace;
address and undefined-behavior sanitizers remain enabled. No dynamic allocation.

## Reproduce

With pinned IDO in IDO_DIR and GNU MIPS tools in PATH:

    python3 cloud/work/dot_pool_object/verify.py
    cc -std=c99 -O1 -g -fsanitize=address,undefined -fno-omit-frame-pointer -Wall -Wextra -Werror cloud/work/dot_pool_object/host_test.c -o /tmp/pool-test
    ASAN_OPTIONS=detect_leaks=0 /tmp/pool-test
    python3 -m pytest tests/conveyor/test_pool_object_research.py -o addopts='' -q

Research verifier success means the documented NONMATCH and bounded behavior were
reproduced, not that matching gates passed. No ROM or complete game image is
available, so image splice, compressed identity, full-ROM SHA-1 and make test were
not run. Accepted source, locks, scorer, protected paths and build wiring unchanged.

Local aggregate validation: 1,310 passed, 41 skipped, 9 deselected in the
repository conveyor suite; all 161 static locked functions intact.
