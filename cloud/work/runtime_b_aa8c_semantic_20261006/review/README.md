# Independent full AA8C semantic-C review

Verdict: PASS as a complete physical-address callback model under explicit
external-service contracts. No source-body correction was required. This is
neither an original-C object/domain proof nor a MIPS matching candidate.
Reviewed base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.

## Evidence

- The final producer receipt was independently reproduced from a foreign working
  directory with an absent IDO path: 1,759 native/host-C pairs, 2,027 reachable
  words, 112 branch outcomes, 64 direct call sites, and 12 negative controls.
- The independent challenge runner executes the actual compiled `semantic.c`
  and its host bus adapter, rather than interpreting a duplicate Python model.
  It tests 2,766 cases with both C99 O0 and C99 O3 plus undefined-behavior
  sanitization, with floating contraction disabled: 5,532 additional pairs.
- Independent cases comprise 936 all-model selector cases, 48 partially
  overlapping source-transform layouts, 256 deterministic randomized finite
  arithmetic/state cases, 800 service-position mutation cases, 140 entry guards,
  512 cleanup masks, eight full-word-sentinel/narrowing cases, 24 alpha cases,
  18 real-RNG-grid edge cases, and 24 timer float boundary cases.
- Comparison checks ordered nonstack memory stores as well as final mapped
  bytes, ordered service traces, retained scene state, RNG consumption and
  physical offset/color read order. Sibling-player states deliberately differ
  to expose accidental replacement of the retained initial player pointer.
- Each service position is independently challenged with a changed descriptor
  owner, changed live state/model/color, replacement of the scene's stored source
  pointer, and writes into source-transform contents. These are adversarial
  ordering probes, not claims about the real services mutating descriptor state.
- Eight wrong C variants are rejected: resource selection, sentinel width,
  extra effect ID, trigger signedness, initial texture selection, erased mode-8
  reads, removed second live-cache test, and refreshed initial player pointer.
- A separately compiled double-product mutation is rejected on the first
  randomized transform fixture, demonstrating sensitivity to per-operation f32
  rounding rather than only algebraic equivalence.
- An independent delay-slot-aware conservative CFG includes the authenticated
  nine-way switch table and finds exactly two structurally unreachable words:
  AB8C and C488. No native/image/disassembly bytes are included in this packet.

The broad independent cases cover 2,020 words and 111 branch outcomes by
themselves; the separately reproduced producer suite supplies the complete
2,027-word reachable-closure coverage. Coverage is not an exhaustive gameplay
or Cartesian-product proof.

## Review of effects and boundaries

The separate native service-effect audit supports the important interface
choices: retained allocation pointers, position-before-matrix writes, sequential
copy, signed-low-half setter/removal handles, full-word visibility handles,
low-half resource IDs and full signed-word texture slots. This review uses the
corrected RollUV guard and s32 texture-slot adapter. Shared deterministic
service models remain a limitation; actual N64 trig, allocator graphs, native
RNG state, FCSR/exceptions and arbitrary scene-handle validity are not integrated
into this root replay.

Original source-object identities remain unresolved. In particular, real mode
8 reads independently used data beyond eight offset rows, registration permits
owners 4/5 despite the known four-record attachment storage, and the global
model-selector bound is unproved. No invented ninth float row, merged 17-ID
object, keeper, fake original callee, exported private ABI, or ordinary-IDO
matching closure was accepted. The semantic source and actual private cleanup
bodies remain intact.

Host O3 is an additional semantic/compiler check, not the canonical IDO O3
matching pipeline. No IDO call, full target compilation, strict equality claim,
production edit, protected-tool change, lock update, publication or CI monitoring
was performed by this review.

## Reproduction

Requires a host C compiler with UBSan support and the same dependencies needed
by the canonical target reader. No IDO or MIPS linker is needed.

    TMPDIR=/writable/temporary/directory python3 verify_independent.py \
      --packet /path/to/full-aa8c-packet \
      --reference-root /path/to/SFRush2049-decomp \
      --output /tmp/aa8c-independent.json

    python3 /path/to/full-aa8c-packet/verify.py \
      --reference-root /path/to/SFRush2049-decomp --check

The rounding control is independently reproducible:

    TMPDIR=/writable/temporary/directory python3 verify_rounding.py \
      --packet /path/to/full-aa8c-packet \
      --reference-root /path/to/SFRush2049-decomp \
      --output /tmp/aa8c-rounding.json

Deliverable files are this README, `verify_independent.py`, `final-review.json`,
`verify_rounding.py`, and `rounding-control.json`. The receipt binds the review runner, the actual semantic-C
and bus/fixture/verifier sources, authenticated native targets, and base commit.
It excludes pytest files and mutable production, manifest, scorer, context and
lock hashes. The review does not need to publish its temporary packet copy,
compiled libraries, logs, stale intermediate receipts or native assets.
