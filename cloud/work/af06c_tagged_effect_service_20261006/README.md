# AF06C tagged-input effect service

## Result

Complete typed semantic C now covers main-image `save_write_data` at
`800AF06C` (1,200 bytes) and its genuine private `func_80090308` body
(1,128 bytes). The historical save/serialization name is misleading.
This is a **nonmatching research packet**, not production replacement C.

All production evidence is read with `git show` at base
`6b2e9e506fe3d2267a710e41c85af5364ccd00c7`. No changing live source, lock,
manifest, scorer, or data-index hashes are acceptance predicates. Only this
packet's source/verifiers and complete authenticated native bodies are bound.

Evidence, in order:

1. Completed viewport service-contract audit and actual `FA9B4` caller were
   checked before writing source. `FA9B4` passes a local signed-short pointer,
   mode 1, scale 1.0f in a2, and sound 0. It is not a serialization call.
2. Before target compilation: 28 ILP32 layout checks passed with host GCC
   `-m32 -fsyntax-only`; the complete unchanged typed source passed C89 checks.
3. 1,008 paired native/host fixtures passed, comparing all modeled final storage
   bytes and ordered external calls. The native runner executes both real
   bodies and verifies the ordinary outer saved-register boundary.
4. Independent source/native checks passed 420 fixtures across host O0/O2,
   reached 571 native instruction addresses and 49 branch outcomes, and rejected
   six deliberately wrong source controls. A separate native-only sweep covered
   another 864 fixtures. See `independent-review.json` and `independent/`.
5. One coordinated target build used unchanged `score.compile_single` with the
   exact bare-header O3 flags. Mandatory `-Wab,-r4300_mul` was injected by the
   canonical route and recorded honestly. No second target build or shaping
   sweep was performed.
6. That single linked output passed the same 1,008 paired native fixtures.
   This preserves useful executable semantic evidence without claiming a match.

The O3 build naturally inlines private 90308 and emits one named function:

- `save_write_data`: object offset 8, 2,096 bytes, versus native 1,200
- `func_80090308`: naturally absent as a separate emitted function
- `.text`: 2,112 bytes total, including an unlabelled compiler-generated
  8-byte return prefix and an 8-byte alignment tail
- Canonical comparison: 298/300 native target words differ, with 220 nonzero
  extra words beyond the native extent
- No unresolved or unverified relocations, scorer errors, or own-data failures;
  there are zero emitted owned-data references

The prefix is a compiler artifact, not handwritten source or an added keeper.
Every emitted function and allocated section is recorded in `baseline.json`.
Private90308 was included because its real native boundary is nonstandard,
not to manufacture register pressure. The compiler's natural inline decision
is retained. Original translation-unit membership and a matching build recipe
remain unproved; no source/ABI alterations were made to force the old layout.

## Source and ABI

The ordinary outer signature is:

`void save_write_data(void *input, s32 mode, f32 scale, s32 sound)`.

The initial global shortcut is evaluated before mode. If enabled, `input`
is read as a signed short even when mode is zero. Outside that shortcut,
nonzero mode reads a short player/effect index, while zero mode reads three
floats. The input is never written through or retained. Nodes store copied
indices/scene data and fixed callbacks. Stored callback declarations include
both real arguments: node pointer and signed-short update mode.

The genuine private child receives the copied index in the caller's outgoing
SP+0 word and reads its low half at incoming SP+2 on big-endian O32. It freely
clobbers nonvolatile registers. The native AF06C outer root preserves all
ordinary s0..s8 and f20..f31 state around it. Its complete C is therefore a
static same-unit helper, not an invented ordinary external declaration.

The independently audited 90284 allocator remains an ordinary no-input external
returning a 24-byte node or null. It is modeled faithfully in fixtures but was
not added to the candidate translation unit for allocation shaping. Matrix-copy,
scene removal/creation, event and sound declarations follow real helper bodies.
The test harness's callback-identity placeholders are never executed and are
not part of the target source or its build.

Typed layouts: node 24; transform 48; player 952; simple effect 60; debris 84;
private group 400; scene 68 bytes. Unidentified storage fields preserve genuine
native offsets, not artificial stack or register padding.

The private child has a native initial load from `8011B550` whose entire copied
word is replaced by four RGBA byte stores before consumption. The semantic C
omits this dead read. It also omits a provably false sound-id sentinel check.
Neither is reintroduced as a source-shaping device. The real mutable color
at `8011B554`, in contrast, is snapshotted before extra-node allocation and is
retained across that boundary.

## Behavioral coverage and limits

Checks cover mode/index/position branches; mode-independent shortcut; count and
signed-byte conditions; both allocation failures; existing scene removal;
ring selection and wrap; callback-time ring/head/input/player/global mutations;
private four-debris setup and RNG state; captured float constants; rechecked
private extra/event conditions, including event after allocation failure;
optional sound, enable state and inclusive 0.5/1.0 thresholds at neighboring
binary32 values; copied callback/index state; and outer register preservation.

Required domain: initialized aligned storage; in-range player/effect/scene/ring
indices; nonaliasing input and global objects; valid pool nodes and at most the
fixture's bounded allocation count; finite normal-or-zero binary32 under default
rounding. Native successful scene indices are required where the code indexes
scene storage. No invented negative/null scene guard is added. These fixtures
model ordinary scene/audio/event boundaries, rather than implementing the full
scene graph, audio engine, callback lifetimes, or gameplay. NaN, subnormal,
invalid-conversion, FCSR/exception, concurrency and arbitrary-corruption behavior
are not established.

The host adapter includes `candidate.c` unchanged. It only translates native
big-endian storage and O32 pointers to host layout at external boundaries.
Native caller-saved integer and f0..f19 registers, LO and condition state are
poisoned at those boundaries. HI is not modeled because neither selected body
reads it. The linked-output runner adds only needed ordinary MIPS operations
and entry-address support to the fail-closed engine.

## Reproduction

Set `TMPDIR` to a writable workspace directory. All generated temporary objects
stay there. Native words and the historical asset are read in memory and are
not deliverables.

- `python3 packet/verify.py --reference-root /path/to/repository`
- `python3 packet/independent/audit_contracts.py --reference-root /path/to/repository`
- See `independent/README.md` for the independent challenge CLI.
- `cc -m32 -std=c89 -pedantic -Wall -Wextra -Werror -fsyntax-only packet/check_layout.c`
- `RUSH_REFERENCE_ROOT=/path/to/repository python3 -m pytest packet/test_packet.py -q`

For a separately authorized fresh diagnostic, `build_once.py` accepts a
reference root, optional tool root, and fresh work directory; it guards missing
IDO/linker support and records a marker before its single target invocation.
Do not rerun the current one-shot baseline or vary flags to chase the score.
The build uses the unchanged canonical tools from the chosen repository.
Existing ELF replay uses:

`python3 packet/verify_linked.py --reference-root /path/to/repository --elf /path/to/candidate.elf`

Compare future receipts only on proved source/native identities, emitted words
and extents, relocations, owned data, and bounded behavior. Absolute diagnostic
paths, manifest hashes, live lock status, compiler metadata and integration
state are not equality gates. The pytest file is intentionally not receipt-hashed.

## Scope and stopping point

No caller source, production source, protected tool, lock, or integration file
was edited. No external writes, publication, blocked retries or CI watching
occurred. No aggregate test pass, strict match, accepted lock, original TU,
image/ROM identity or merge-readiness claim is made. The research task stops at
complete semantic source, independent bounded checks, and the honest single-O3
nonmatch. Further matching work requires new evidence, not artificial source
pressure or ABI changes.
