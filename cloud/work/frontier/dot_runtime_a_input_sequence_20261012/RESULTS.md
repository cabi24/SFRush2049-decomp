# Image A input sequence: strict standalone match

Deliverable: `cloud/matches/ovl_a/func_8038F648.c`, one complete **236-byte**
standalone function. Gross claims: one function / 236 B. Incremental candidate
claim: **236 B**. Accepted-image and cartridge gain: **0 B** until the independent
checker splices and verifies it. No group supersession or locked-source edit.

## Reproduce

```
python3 tools/cloud/score.py fn cloud/matches/ovl_a/func_8038F648.c func_8038F648 --targets asm/us/ovl_a
func_8038F648:
  MATCH

python3 cloud/work/frontier/dot_runtime_a_input_sequence_20261012/verify.py
PASS: input sequence, one strict 236-byte claim; complete stock/GNU proof and bounded behavior

REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/conveyor/test_runtime_a_input_sequence.py -q -o addopts=''
1 passed in 0.32s
```

Stock IDO 5.3, `-g0 -O2 -mips2 -G 0 -non_shared`, with the scorer's canonical
`r4300_mul` flag. Compiler identity is checked through the current repository's
compiler policy. This is an overlay translation unit; no static ROM TU is rebuilt.
Replay also passes under optimized Python. The one packet test takes under one
second with IDO; input binding is checked before a missing-toolchain skip.

## Traced cause and source lever

The complete PR #311 seed reproduced at **2/59** differing words, with all 16
relocations resolved and no extra words or owned-data uncertainty. Workbench
`diagnose` on the complete target and candidate found identical register-lane
sequences: only the operand positions in equality branches at **+0x90/+0x98**
differ. Its raw-object relocation cautions are superseded by strict relocation
and the independent GNU link, not masked away as match evidence.

Wave 16's comparison-kind rule supplies the bounded hypothesis: turn the plain
held/pressed operand into an expression using unsigned `* 1`, then put that
expression first. Both identities optimize away. The stock compiler emits the
native branch ordering and all 59 words become exact. Three independently
compiled controls confirm the mechanism:

- ordinary expression-versus-variable comparisons: 2/59
- identity expressions with the old source order: 2/59
- commuted source order without the identities: 2/59
- both expression identity and source order: **MATCH**

These are three hypothesis controls, not a variant sweep. No patch to a compiler,
scorer, instruction, register assignment, target, frame, or buffer was needed.
The w20h/w21d shared-LUI data-alignment finding was read; this function has no
owned-data relocation or data-definition/alignment requirement.

## Independent proof and behavior

The verifier authenticates the target with the repository manifest and binds
the precise target words, image identity, referenced symbols, source, local
ELF reader, behavioral interpreter and verifier. It deliberately does not pin
shared tooling, lock files, raw assembly text, or non-master commits.

A separate ELF reader checks the complete compiled extent and zero-only alignment
tail. GNU ld independently resolves **16 complete HI16/LO16 records** at the native
address. The resulting whole function equals the authenticated target, without
masked bytes, unresolved symbols, nonzero tails, local data or borrowed neighbors.

**398 bounded fixtures** execute the native and independently GNU-linked bodies,
compare all memory-access/write traces, and cross-check an independent model and
GCC-compiled C. They cover all **59 instructions and 17 branch outcomes**:
null sequence, step zero/nonzero, deadline below/equal/above the clock, first/last
valid index including index 19, incomplete held/pressed masks, unrelated new/held
buttons, successful progression and completion, and deterministic random masks.
Every case checks saved integer/FP registers, stack pointer, return address and
all non-output memory. An unknown-opcode negative control must fail closed.

The finite-clock fixture interpreter is not a console/FCSR emulator. Tests keep
the clock stable during each invocation and do not prove asynchronous writer
behavior, exceptional floating-point/FCSR cases, invalid-index safety, retail
sequence contents, input-device integration or gameplay. Existing volatile clock
source precedent remains a contract hypothesis, disclosed in the deliverable.
The 164-byte union uses IDO-specific representation, not portable serialization.
It keeps the observed count byte and held/pressed offsets without
indexing beyond a twenty-word subarray. The caller must provide valid storage and
an index in 0..19 at each mask read.

## Scope and coordination

Baseline: master `93259a065`; reservation published in draft #323 at
`80305b3e3399cc5cd413a2949777cb446bed3648`. This packet follows that reservation;
shared `CLAIMS.md` remains owned by the reconciliation lane. PR #321's 803A7918
claim is untouched. The fallback 803A7180 remains separate research.

No protected source, lock, target, ROM bytes, extracted images, raw assembly dump,
compiled binary, credentials or unrelated private data is published. The source,
small verification tools, one test and hash-only receipt are the entire packet.
Publication stays draft; merging, splicing, image identity and cartridge SHA-1
acceptance belong to the independent checker.
