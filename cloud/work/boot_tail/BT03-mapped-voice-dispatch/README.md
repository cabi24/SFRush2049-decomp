# BT03 mapped voice volume dispatch

**COMPLETE-NONMATCH, 960-byte native body, no new verified-body credit.**

The complete natural C89 reconstruction of `func_8001B9F8`
(`[8001B9F8,8001BDB8)`) compiles with pinned stock IDO 5.3 O2 to an exact
960-byte ELF function. After a separate GNU linker resolves every relocation,
83 of 240 full words still differ. All six generated switch-table entries equal
the specifically approved source-free table receipt, including its SHA-256.
This is useful source and relocation research, not an accepted strict match,
production storage assignment, runtime integration, or cartridge coverage.

Base: `cf4b9c619c72e83aa5da2e8b5c110765f9b03693` (PR77 tree).
Branch: `dot/boot-tail-mapped-voice-dispatch`.
Only this function and packet are owned here. Central owns shared claims,
ledgers and publication. No protected source, canonical target, scorer, symbol,
layout, lock, compiler or production build file was edited. No ROM bytes,
assembly dump, compiled object or donor source is included.

## Native role and genuine ABI

The role is a fixed-point volume-fader update. The five inputs are byte volume,
unsigned-halfword duration, byte group, byte sequence mode, and word sequence
identifier. The fifth word is read from incoming `sp+16`; the fourth input is
read as a byte in the default arm. All four complete canonical callers and the
complete sole helper were reviewed; hashes, seven call sites and layouts are
in `abi_proof.json`. The larger `800178B0` caller was read solely as ABI context;
no T050 body implementation was attempted.

A 40-byte fader holds current volume at 0, target 4, signed delta 8, converted time 12,
sequence identifier 16, type 20, mode 21, and pause fields 24..36. The inferred two
reserved bytes at22..23 are record layout, not invented stack padding. This
layout agrees with the already reconstructed initialization body and the parallel
pause-volume reconstruction. The target uses signed division after a wrapping
32-bit subtraction; the retained source makes that distinction explicit.

The whole native behavior is:

- Normalize duration 0 to 1, copy it to a word, call `8001E930` to scale it by 256.
- Selector 255 visits all 32 faders of type 0/1; selector 252 selects type 2/3.
- Selectors 250,251,253,254 select types 2,3,0,1 respectively.
- Each selected group fader gets target `volume<<16`, signed delta divided by
  normalized duration, scaled time, and identifier `0xFFFFFFFF`; mode is untouched.
- Other selectors directly index the array and also write caller mode/identifier.
  Valid direct indices are 0..31. Native provides no bounds check for 32..249;
  the C does not invent one and these indices are not part of valid-C testing.
- Return is void, with ordinary O32 register and stack preservation.

## Source family and platform differences

The inspected, pinned [AxioDL MusyX `synthVolume` source](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synth.c)
is a structural family lead, under its pinned
[CC0-1.0 license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE).
Both were fetched in this session; blob identities are in `references.json`.
The selector grouping and genuine five-input interface agree. Its newer float
faders, `SetupFader` call, active-bit globals and zero-time behavior differ and
were not imported. The N64 uses fixed-point arithmetic and direct record writes.
No arcade source checkout was present, and no arcade equivalence is asserted.

## Bounded source experiments

Sixteen archived source forms plus the same retained body's labeling-only
comment change were compiled. The full measured rows are in `verification.json`.
These are causal source/layout probes, not a flag or permutation sweep.

- The family-style simultaneous index/pointer loop unrolls four ways and has
 1572-byte output. Changing only index signedness leaves it unchanged.
- Pointer termination reduces it to 712 bytes; assigning an indexed pointer in
 the loop instead expands it to 1640. Neither has native topology.
- Direct array field accesses produce native pair unrolling and a 56-byte frame,
 but 972-byte output and extra narrow-formal conversion copies.
- Unsigned fixed-point fields with explicit signed difference remove C signed
 overflow ambiguity without altering that output. Early signed-field controls
 are rejected experiments and are not claimed equivalent for all bit patterns.
- Reordering genuine time/delta stores follows native ordering. Fully spelling
 the independently known pause fields preserves the same record layout.
- A signed index reused by the default arm (`i=vGroup`) removes the size drift
 and gives 960 bytes, the six native table destinations, and 83 differing words.
 Moving that assignment earlier produces the same score. Reverting to unsigned
 index reintroduces the 972-byte form. Widening the type-selection local is also
 rejected. No register-pressure trick or redundant operation is retained.

O1 is a rejected control: 1168-byte function, 239/240 words differ in the scorer's
compared slice, 49 nonzero excess words, and 291 differences in the independent
full-extent comparison. Its generated table entries differ too. A scorer
unpaired-HI16 diagnostic is preserved because its comparison slice ends before
that larger O1 function's matching LO16; independent GNU linking resolves the
entire object and does not hide this distinction.

Final workbench diagnosis uses complete, independently relocated function words:
240 words and 56-byte frame on both sides, 83 raw differences, 76 aligned register
differences, and three insertions/deletions apiece. The tool gives a heuristic
CFE-spelling route and no supported lever. Formal conversion allocation,
temporary-register sequence and scheduling remain the next research issue.
There is no new evidence justifying another local variant sweep.

## Relocations and the acceptance boundary

The unchanged canonical scorer reports 83/240, zero excess words, and two
unverified `.rodata` references at function offsets 0x68 and0x70. It is not an
accepted match. We do not use `--allow-unverified`, change its symbol map,
rename section symbols, mask bits, or patch its result.

`verify.py` separately compiles the source and links it in temporary storage with
text at 8001B9F8 and table at 8002D8D0. `SUBALIGN(4)` is necessary because the input
ELF otherwise imposes 16-byte placement and would move the native function 8 bytes.
This temporary research placement is not a production linker ownership claim.
The verifier checks actual output VMAs, exact STT_FUNC size, all 23 relocations,
absence of remaining REL/RELA entries, exactly six R_MIPS_32 table entries, and
zero-only section padding. Every emitted function word is then compared without
masking, and all six linked table destinations are compared with the approved
receipt. The 24-byte table SHA-256 is
`5802bcf542a95119599664217aa458924827df31404496eb0d9eaf483e162047`.
No further asset/data reads were made. Production ownership and the scorer's
local-rodata acceptance path remain maintainer gates even if a later body closes.

## Semantic verification and replay

`test_semantics.py` executes both complete canonical and freshly linked candidate
instruction streams against synthetic memory. It is a purpose-limited, fail-closed
MIPS interpreter, not a general emulator or cartridge test. The known four-word
helper is modeled as its audited word scaling contract with adversarial
caller-clobbered registers and a separate post-helper mutation family.

The independent oracle checks every modified and untouched byte across 1280 bytes,
all 38 valid selectors, duration boundaries, all 256 volume values on every grouped
arm, signed arithmetic wrap/extrema, both unrolled positions, first/last faders,
mode preservation and identifier writes. Deliberately dirty upper bits on O32
narrow inputs exercise the real width normalization. Stack initialization and
callee-save restoration are asserted. Native and candidate have 4,310 cases each. Each executes 212 of 240 distinct
instructions. The 28 unexecuted instructions are division-fault paths: seven
zero-divisor traps and seven three-instruction signed-overflow checks/traps.
Normalization constrains the divisor to 1..65535, so those paths are outside the
valid function domain; this is reported rather than called 100% instruction coverage.
A separate real C89 GCC harness runs 58,368 cases with AddressSanitizer and
UndefinedBehaviorSanitizer. LeakSanitizer is disabled because the container's
ptrace environment rejects it; address/UB checks remain enabled and the harness
has no dynamic allocation. See `semantics.json` for final source-bound evidence.

From the repository root:

```
source ../rush-recovery-env.sh
python cloud/work/boot_tail/BT03-mapped-voice-dispatch/verify.py
python cloud/work/boot_tail/BT03-mapped-voice-dispatch/diagnose.py
python cloud/work/boot_tail/BT03-mapped-voice-dispatch/test_semantics.py
```

The verifier also checks the protected manifest, 439-entry extent census equality,
and existing getter's strict match. Temporary native words and objects are never
published. Independent frozen-tree peer review is the final packet gate before
coordinator intake. No production/ROM tests were run or claimed.
