# Real-caller encoded-string measurement match

`func_800B66B0`, **[0x800B66B0, 0x800B6748), 152 bytes / 38 words**, is a
complete strict **MATCH** with the canonical O3 group pipeline. The claim is
only this helper. This packet changes no production source, lock, target or
compiler setting; accepted-byte and ROM-coverage gain remain zero until the
maintainer's separate integration gates.

## What closed the match

The archived `frontier/agentA/func_800B66B0/best.c` source already represented
the complete helper. Standalone compilation left five temporary-register words
different. Its old diagnostic used synthetic callers and was correctly unclaimed.
The corresponding report proposed the real `menu_input_process` caller, but later
work deferred it because that caller has unrelated unresolved callee context.

This packet supplies the **complete actual caller body**, unchanged from
`src/blob/groups/codex_defaults_a16/group.c` at base commit
`6b2e9e506fe3d2267a710e41c85af5364ccd00c7`. The caller contains both genuine
native call sites, **0x800B6C34 and 0x800B6DD4**. The main-game J/JAL census read
at that base finds no other direct callers; this is not an indirect-call census.
The helper body is also unchanged from its archived source, including its
existing physical line containing `count++; if (...)`.

Keeping only the real caller externally visible allows the genuine helper to
remain internal and gives the native four-register temporary ring. No synthetic
wrapper, invented parameter, helper, call site, volatile, assembly, padding or
pressure expression is added. The first full A16-context compile matched; reducing
it to the two actual necessary functions preserved the same complete match.
No discarded static-function stub or inlined helper is required.

The matching source and recipe are in
`cloud/matches/dot_text_measure_b66b0_20261006/`. The bare source header is exactly
`/* flags: -g0 -O3 -mips2 -G 0 -non_shared */`. The unchanged canonical group
pipeline additionally passes its mandatory `-r4300_mul` backend workaround.
No raw-cc route or altered backend flag substitutes for that pipeline.

## Semantics

The helper receives a byte pointer and signed-halfword maximum. If the first
byte is 0xFF, it skips that tag and measures successive two-byte characters,
including the first all-zero pair when one is encountered. Otherwise it measures
ordinary bytes, including a terminating zero. A nonnegative maximum permits
**maximum + 1** characters to be examined, because this is a do/while operation.
A negative maximum searches until a terminator. The returned length is bytes,
including the tag in the paired encoding. The two-byte terminator requires both
bytes zero; the second byte is tested first with short circuiting.

This is a recovered game string format, not a claim of a standard Unicode encoding
or original source names. Null/invalid pointers, insufficient readable storage,
unterminated negative-limit inputs, enormous lengths causing signed-count overflow,
concurrency, gameplay reachability and hardware are outside the proof.

## Complete native and behavioral proof

`verify.py` compiles the group afresh and checks:

- Exact 152-byte ELF STT_FUNC extent and every target word, with zero target
  relocations and no owned data or literals. The unmodified object has 1,264 text
  bytes: helper 152, unclaimed caller 1,104, and eight separately checked zero
  alignment bytes.
- Independent GNU linking of the **whole unchanged object**. All 113 object
  relocations and 27 external bindings agree with the project relocator. All
  152 claimed linked bytes equal native bytes. The context is laid out after the
  helper for this proof only; no claim is made that this is its original address.
- 199,234 fixtures, totaling 398,468 native/GNU-linked interpreter executions
  and 199,234 unchanged-source C89 GCC cases with strict aliasing and
  undefined-behavior/bounds sanitizers. All 38 native instruction offsets execute,
  and both outcomes of all eight conditional branches execute.
- Every raw signed-halfword maximum, with noisy incoming upper argument bits;
  all 65,536 two-byte values; ordinary-byte edge cases; deterministic longer
  strings; 32,768-character boundaries; the real argument-home write, all stack
  canaries, input preservation and native-preserved registers.
- Four compiled wrong-source controls and four native negative controls fail.
  The integer interpreter rejects unsupported instructions, invalid accesses,
  branches in delay slots and incomplete instruction extents.

The hosted C narrowing check measures the verified low-halfword behavior of GCC
and IDO; out-of-range signed narrowing is not claimed to be universally portable
ISO C. The native emulator is a deliberately bounded interpreter, not an N64
emulator. Tests execute only the claimed helper. They do not execute or validate
the actual caller or its external services.

## Context honesty and portability

`menu_input_process` is an explicit **319/320-word NONMATCH** against the recorded
base native words, with a 1,104-byte ELF extent versus 1,280 native bytes. Its external private calling
conventions, original full translation-unit identity and complete behavior are not
proved. Its historical `sp170` assignments are retained solely to keep the full
production-context source body unchanged; they are not newly introduced storage
or register pressure. The caller's large real scratch buffer is inherited context.
No caller bytes receive credit.

The receipt binds only this packet's own source, group recipe, verifier and host
harness, plus the native helper bytes. It does not hash its test file, mutable
manifests, symbols, scorers, locks or production sources. Production context and
base eligibility, native caller census and unclaimed caller comparison are read
through `git show <base>:<path>`. The whole-object relocation context also uses
the recorded base symbol addresses. Later caller removal, renaming, native-context
or lock changes do not invalidate those historical checks. The claimed helper
continues to bind to the live `score.targets()[FN]` words. Fresh compiler results,
extents, relocations and behavior must still replay exactly.

## Reproduce

From a full-history checkout with the project IDO and GNU MIPS tools:

    python3 tools/cloud/score.py group cloud/matches/dot_text_measure_b66b0_20261006 --claims
    python3 cloud/work/frontier/dot_text_measure_b66b0_20261006/verify.py --check
    python3 -m pytest -q tests/conveyor/test_dot_text_measure_b66b0_20261006.py

The verifier accepts `--output PATH` to write a fresh receipt, and
`--history-repo PATH` for a source-only workspace using a separate local full-history
checkout. `--no-behavior` is explicitly byte-only and cannot be combined with
`--check`. Compiler-dependent pytest coverage skips cleanly when the pinned IDO
or GNU MIPS linker is absent. It also requires host GCC for the behavioral proof.

The parent must run the required full `tests/conveyor tests/cloud` matrix on
current master before publication. No aggregate-suite, source-image, compressed
stream, ROM, CI or production-acceptance result is asserted here.
