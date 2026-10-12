# Image A C93C object initializer: 936-byte strict match

Deliverable: this group directory, with C93C as its only member and claim.
The full stock IDO 5.3 body at `[0x8038C93C,0x8038CCE4)` matches all
234 words and owns 32 authenticated literal bytes at `0x803B930C..0x803B932C`.
**936 new candidate code bytes; 0 preserved/inherited code bytes; 0 accepted
image or cartridge bytes.** Literal bytes are not added to the code count.
No existing locked body is defined or migrated. Independent merging and
image/ROM admission remain separate.

## Source change and evidence

Baseline master: `1a2e0f005`. The existing complete option-scene group in
`../dot_runtime_a_option_scene_20261006` reproduced its 14/234-word residual.
The parallel B560 initializer lane traced the corresponding FP-web choice
and established that the apparent external float anchors are owned literals.
This packet independently authenticated C93C's eight values in the current
tracked compressed asset. The values are the negative quarter-turn, the two
independent 55.2 Y constants, 75.1, the half-turn, 87.1, -75.1 and -87.1.

Replacing those anchored loads with literals and the named Y snapshots with
direct position-field writes closes the complete body. The two Y pairs use
`55.2f` and `55.20f`: equal float values but distinct IDO constant webs and
separate retail literal slots. The no-longer-used Y local was removed; the
native 72-byte frame remains exact. No unused padding, fabricated pressure,
false prototypes, added formal parameters, synthetic callers, extra keep
roots, instruction patching or altered compiler policy is used.

The initializer traverses all 48 witnessed 64-byte object slots. Missing
handles create menu objects using their name and matrix; slots 42 through 47
then receive the native texture/transform setup and state flags are set.
No whole-function arcade donor or original source spelling is established.

## Genuine caller and private ABI

The genuine complete DFEC caller and its ACB0/B834 predicates are unchanged
research context. DFEC has exactly one direct C93C call in both native and
compiled code. C93C takes no source arguments; its 72-byte frame saves ra
but deliberately uses integer and FP callee-saved registers without preserving
them. It is a private-group body, not a standalone O32 replacement.

Keeping the genuine DFEC root makes stock IDO save s0–s7, fp and ra plus
f20/f22/f24/f26 around the private context. Native DFEC itself belongs to a
larger private unit and is not matched by this packet. Its outer caller is
not reconstructed here. No context body is an installation claim:

- ACB0: 97/104 differing words, three nonzero excess words
- B834: 45/63 differing words
- DFEC: 380/380 differing words, 38 nonzero excess words

The main-image helpers remain external, including the actually returning
sign-extension wrapper. Source compatibility and a genuine call edge do not
establish complete caller behavior or the original whole-TU boundary.

## Reproduction

Run the unchanged canonical scorer through an authenticated in-memory image
binding (all members, no `--claims` and no `--allow-unverified`):

    python3 cloud/work/frontier/dot_runtime_a_c93c_closure_20261012/reproduce.py

Exact member output:

    Members:
    func_8038C93C:
      MATCH
        own .rodata verified at 0x803B930C..0x803B932C

The wrapper also prints every nonmatching context diagnostic. It is needed
because the repository has no standalone `ovl_a_data` artifact; it feeds the
canonical owned-data verifier the authenticated existing image in memory.
It changes neither scorer rules nor any protected inputs and publishes no
extracted bytes. Running bare `score.py` without an image binding reports
unverified owned-data references rather than a strict match.

Full cheap gate:

    python3 cloud/work/frontier/dot_runtime_a_c93c_closure_20261012/verify.py
    REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/conveyor/test_runtime_a_c93c_closure.py -q

Input-only gate requires neither compiler nor GNU MIPS tools:

    python3 cloud/work/frontier/dot_runtime_a_c93c_closure_20261012/verify.py --inputs-only

The single regression follows shared missing-tool skips and fails those skips
under REQUIRE_TOOLCHAIN=1. The verifier remains active under Python `-O`.
This overlay group uses stock IDO 5.3. No static ROM TU is compiled.

## Proof scope and controls

The input-bound receipt binds the group C and recipe, verifier and ELF reader,
image identity, selected complete native bodies/extents and referenced symbols.
It does not freeze shared tools, accepted locks or assembly-file text. It reads
the current tracked asset directly; no historical nonancestor checkout is used.

A separate ELF reader proves the entire 936-byte symbol and absence of nonzero
excess. A GNU whole-object link resolves all 88 relocations in C93C at the
native address; all 234 words equal retail. The full 32-byte owned .rodata
section equals the authenticated image before and after GNU linking. Context
is placed at a diagnostic base solely for this check; neither its layout nor
its bytes are claimed native. Unknown allocated data and residual relocations
fail closed.

Two fresh stock-compiler negative controls are rejected:
- A wrong literal keeps a zero text score but fails owned-data equality
- Merging the two Y literal webs gives 48 differing words and fails owned data

Native/compiled direct-call counts and the compiled root's private-register
preservation are checked separately. No service internals, unrestricted
behavioral equivalence, whole-game execution, image splice, compression,
full-ROM SHA-1, hardware or gameplay validation is claimed.

## Independent review and local completion

The paired renderer lane independently reviewed the source/recipe delta and
replayed stock/GNU, Python `-O` and the packet pytest on the frozen source.
It confirmed 936 complete bytes, 32 owned literal bytes, unchanged genuine
contexts and no locked migration or protected edits. Its one reporting finding
was corrected: `--inputs-only` now expressly says the compiler/GNU checks were
not run. The final packet regression passes in 0.21 seconds; the no-IDO arm
skips, and REQUIRE_TOOLCHAIN=1 converts that missing-tool skip to a failure.
C89 32-bit syntax and protected-path guard also pass. Hosted CI is reported in
the PR separately, tied to its exact published head.

A subsequent publication refresh incorporates master `3ef036222` unchanged.
The only merge conflict was the adjacent CLAIMS row: current master's renamed
tooling row is retained and this packet's overlay claim is appended. The full
source/recipe/receipt are unchanged and their strict replay passes on that base.
