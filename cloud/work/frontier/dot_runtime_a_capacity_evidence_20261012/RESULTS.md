# Image A projected-options callback: independently bounded capacity closure

Base: `b37643eec4841043dd5b8157fd8309af92e9b12e`.
Population: image A. Stock IDO 5.3, `-g0 -O3 -mips2 -G 0 -non_shared`.

## Deliverable and accounting

The group at `groups/dot_runtime_a_seven_option_group_20261007` extends the
installed group of the same name with `func_803A6B50`, complete native extent
`0x803A6B50..0x803A6F14`: **964 fresh exact candidate bytes, 241/241 words**.
Existing `func_80399394` remains **88/88 exact bytes**, with zero duplicate
credit. Both are members and claims. Both real original context bodies and
the eligibility helper are source-identical to the installed group. The
existing root and new callback are kept. No group migration is needed.

New accepted image bytes and ROM coverage: **zero pending the independent
checker**. No production source, lock, generated target or build gate changes.
Keep the PR a draft; independent checker owns installation, ROM checks and merge.

Authenticated canonical command, no `--claims` or unverified waiver:

```sh
python3 cloud/work/frontier/dot_runtime_a_capacity_evidence_20261012/verify.py --score
```

Member output:

```text
Members:
func_80399394:
  MATCH
func_803A6B50:
  MATCH
    own .rodata verified at 0x803B9820..0x803B9834
```

The original two contexts are informational and remain NONMATCH. Their bytes
are not claimed or installed. A group success does not claim their code.

## Independent writer evidence before capacity closure

DDA8 #337 supplied the method, not a bound for this separate callback. A6B50's
inherited text[50] was an explicitly unproven reconstruction. This packet first
authenticates both actual formats as `%d %s`, then follows the native language
loader and all six supported language resources. No format or asset content is
copied into the published receipt; only addresses, lengths and hashes are saved.

- The original asset SHA-256, its loader pointer, image-A SHA-256, and entire
  game-image SHA-256 are checked before interpreting bytes.
- The initial selector at `D_8002E860` is 5. `func_800A5158` loads its resource
  through `audio_frame_sync`; `func_800A4E58` parses the six counted sections
  and relocates pointer tables into `D_8017A4E0` (the historical symbol
  `countdown_state`). `func_800A510C` swaps the resource when the language changes.
- The native options handler `func_800DB1E0` wraps the selector over 0..5;
  game target words for that handler and the loader chain are input-bound.
  Saved configuration can also select a language. The supported-domain proof
  assumes a valid language selection and valid unmodified loaded tables. It
  does not claim to sanitize corrupt saves, arbitrary pointer mutation or a
  language resource outside these six authentic resources.
- Loader table `D_8011B5BC` selects streams at `0x2F4E0`, `0x30A60`, `0x32030`,
  `0x335F0`, `0x34C80`, `0x36300`; `D_80123564` selects codec 2 for all six.
  Each stream is bounded, decoded and hashed independently. The parser follows
  the consumer's four-byte count and even-u16 padding. All selected pointers
  must be within their authenticated resource and NUL-terminated.
- The singular arm always formats integer 1 with `labels[29]`, at most 7 bytes.
  Its complete output is at most 10 bytes including space and NUL. The plural
  arm uses `indexed[indices[20]]`; that index is 167 for every supported resource,
  and the string is at most 9 bytes. The entire signed16 count domain takes at
  most 6 decimal bytes, including a minus sign. Thus its complete output is at
  most **6 + 1 + 9 + 1 =17 bytes**. This is independent of any desired frame.
- `fcvt_wrapper` forwards o32 varargs to the installed formatter in
  `src/rom/lib_34a0.c`. The actual decimal/string paths have no requested width
  or precision here; `%s` uses the terminated length. `PRINT` and `FLUSH` in
  `include/static_formatter_context.h` copy that length and append one NUL.
  The two mutually exclusive formatter calls are the only users of this local
  text buffer; its pointer is not returned, stored or passed to another reader.

`capacity.py` rejects unsupported resource IDs and a corrupted plural selector.
The host check runs the **actual new callback** over all 393,216 combinations of
six authentic strings and every signed16 count. It checks exactly one writer
call, actual format/argument selection, every byte including NUL and a maximum
of 17 bytes. Formatting and other services are host hooks; eligibility returns
false and projection is disabled. This isolates the writer path and is not a
full gameplay or native/helper behavior-equivalence test.

The language source resources are not modified by this work. The inherited
packet's larger language structure remains an accessed-field view, not a claim
that it is the original complete source typedef.

## Full native stack census and trace

The full native frame is 144 bytes. All stack-relative memory operations and
address-taking instructions are enumerated in the receipt, not just the two
frame adjustments. Native point is `[88,92)` and text starts 92. Outgoing argument
homes are 0/4/16; saved registers occupy 32..60; the incoming unused callback
parameter is stored at 144 in the caller argument home, beyond this frame.

Reconstructed text[36] occupies `[92,128)`, comfortably beyond the proven17-byte
writer requirement. The declared scalar-home reconstruction is slot 128..132,
title 132..136, index 136..140, y 140..142, x 142..144, with no overlaps. These scalar
homes are inferred from source allocation; unlike the emitted point/text and
saved-register accesses, they are not independently live native references.
**The exact original text capacity remains unknown.**

Three bounded stock compiler diagnostics, with whole-object equality between
ordinary and diagnostic builds:

| Declaration | M/T/P | Frame | Difference |
|---|---|---|---|
| inherited text[50] |92/20/4|160|3/241|
| independently bounded text[36] |76/20/4|144|0/241|
| text[32] negative control |72/20/4|136|11/241|

The32-byte diagnostic shifts the point/text homes down 4 as well as changing
frame/argument-home words. It is rejected, despite being large enough for the
writer. The accepted source uses both independent requirements: sufficient
writer capacity and the full nonoverlapping native stack layout. The sole
executable source delta from #330 is text[50] to text[36]; there are no padding
arrays, dummy locals, artificial reads, false signatures, forced registers or
compiler modifications.

Fresh workbench diagnosis of the inherited source identifies the +16-byte frame
and stack-home lever with identical register-class lanes. Comparing its
relocatable object to a fixed native target also reports relocation-layout
noise; canonical scoring and the separate GNU link resolve that noise and
show exactly the original three frame-related words.

## Additional proof and scope

- Input-bound receipt pins owned sources/verifiers and normalized selected
  target words. It does not pin mutable tools, locks, generated assembly text,
  Makefile or non-ancestor historical source commits.
- GNU MIPS as/ld relocates both complete STT_FUNC extents at native addresses,
  checking full words, symbol addresses/sizes and the complete 20-byte owned
  angle pool at `0x803B9820`. No word or frame immediate is masked in that proof.
- The actual installer packet conversion and relocation reproduce both full
  bodies. Missing data and independent mutation of all five angle literals
  fail closed in the scorer; each corrupt literal also fails the installer.
- IDO C89 compile-time assertions check scalar/pointer/short sizes, the genuine
  by-value Color4 ABI, Slot64 size and position offset 48. No static ROM TU is
  rebuilt; if future work rebuilds one, its compiler must come from `tu_cc`.
- The inherited50-byte and rejected32-byte controls keep the helper exact.
- No raw ROM bytes, resource payloads, assembly dumps, objects or credentials
  are published. No full image splice, compression, ROM hash, hardware or
  unrestricted memory-safety result is claimed.

A7180 remains unchanged at its inherited frame residual. Its more complicated
record-name and localized concatenation writer paths need separate bounds;
this packet does not transfer the A6B50 bound to them or run capacity probes.

## Reproduction

```sh
python3 cloud/work/frontier/dot_runtime_a_capacity_evidence_20261012/verify.py --deep
python3 -O cloud/work/frontier/dot_runtime_a_capacity_evidence_20261012/verify.py --deep
python3 cloud/work/frontier/dot_runtime_a_capacity_evidence_20261012/trace.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_capacity_evidence.py -o addopts='' -q
```

One routine packet test passes in 0.45s with stock IDO. It authenticates the
resources and inputs and recompiles the complete group. GNU/installer, exhaustive
host formatting and diagnostic controls are in the optional deep replay.
Independent review and final-head hosted CI results are recorded separately.
