# Image-A availability and cleanup: bounded NONMATCH research

No matching claim, accepted-byte gain, integration or ROM-coverage gain.
Protected baseline: `390d40f6f`. Image A is loaded at `0x8038A400`, image
SHA-256 `0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667`.
Image B is never used. Extents, symbols and native code remain unchanged.

## Findings

- A:803932E4: 168 bytes, newly reconstructed complete cleanup body. It walks
  **23** 64-byte records at 803B5F28 through (exclusive) 803B64E8. Field +4
  is compared against -1; other values are narrowed to signed16 and passed to
  the helper at 80090254, then replaced with -1. It clears 803BA7D0, conditionally
  consumes/clears pointers at 803B6530 and 803B652C, calls 800B557C, and clears
  byte 803B6528. Native code clobbers s0–s2 without saving them. Actual callers
  are A:8039338C at80393B14 and A:80394058 at80394114. Legacy symbol names
  `sound_call_minimal`, `sound_handles_array_clear`, `sound_stop`, and
  `ambient_sounds_clear` identify address bindings, not newly established
  sound-domain semantics. Standalone O2/O3:40/42,6 excess words.
- A:803930B8:264 bytes, newly reconstructed complete four-slot count producer.
  It emits -4 for an inactive signed byte at the slot's 16-byte record,
  -1/-2/-3 for the three witnessed helper outcomes, or counts list objects
  whose unsigned-byte slot field matches. Native code clobbers s0–s7/fp without
  saves. Callers:931C0 at931EC and94058 at940B0. Prior flags research documented
  the producer contract but supplied no source. Standalone O2/O3:65/66,17 excess
  words. Structs express observed offsets, not recovered original types.
- A:80392F28:220 bytes, existing archived complete source reproduced at2/55
  for both O2 and O3. Real callers:931C0 at932B4 and94058 at94264/94338.
  Independent whole-object GNU link confirms exactly the two differing word
  offsets +0x84/+0xAC. Complete ELF function220 bytes, .text224 bytes with
  four zero alignment bytes. Only the register carrying constant5 changes
  (native t6, candidate t0). The workbench on the linked object reports one
  two-site register web and **no supported source lever**: ownership between
  UOPT reservation and UGEN demand is unknown. The old packet's temp-ring
  attribution is therefore retained only as an earlier hypothesis. No spelling
  sweep or unsupported pressure edit was made.
- A:803931C0:276 bytes, archived complete mode dispatcher reproduced at17/69
  at O2/O3. Real callers:9338C at93A14/93ACC,93B3C at93B54,94058 at940F0/944E0.
  It calls the complete930B8/93004/92F28 family. The naturally connected O3
  single-root group inlines all three callees, leaving eight-byte retained
  stubs and making931C0 69/69 with144 excess nonzero words. The next grounded
  avenue is reconstruction of the genuine second large caller94058 (or9338C
  for cleanup), not synthetic callers or arbitrary keep-list expansion.

80393004 is already matched elsewhere. Its source is included solely as genuine
call-family context; this packet does not reclaim it. Sources selector.c and
mode.c preserve archived sources from cloud/work/r16_runtime_images/near_miss.
The count/availability-row source in group.c preserves
cloud/matches/ovl_a/func_80393004.c. group.json retains only actual931C0 root.

## Verification and limits

`reproduce.py --check` passes: eight standalone compiler/scorer results, four
natural-group results, input hashes and independent selector ELF/GNU-link
checks. `--diagnose` reruns the workbench without publishing raw listings.
Both optimization baselines use stock IDO5.3 and ordinary flags; no compiler,
scorer, target, ABI, instrumentation or keep-list policy modification.

Standalone cleanup and expanded group have scorer unpaired-HI16 errors because
comparison truncates oversized bodies before a companion relocation. These
are explicitly retained errors, never waived. Only selector's full linked
extent has independent GNU verification; no equivalent link or semantic
execution claim is made for the other bodies. Helper behavior, list ownership,
valid runtime bounds, private caller-register contracts and runtime reachability
are not proven by compilation. Linked-list counting assumes valid terminating
nodes and nonoverflowing counts. Source storage descriptions do not establish
original source recovery. The arcade reference checkout is absent here; no
arcade donor or ancestry claim is made.

The cleanup's unknown fields and producer's prefix members are bounded storage
views for real offsets, not artificial allocation locals. No helper stand-in,
fake keeper, register-pressure variable or nonstock compiler control is used.
No runtime emulation, host equivalence, sanitizer, image/compression/ROM hash,
hardware, or full-suite gate ran. This is additive research only.

## Replay

With standard IDO_DIR and MIPS GNU binutils configured, from any directory:

    python3 /path/to/repo/cloud/work/frontier/dot_runtime_a_availability_wave_20261011/reproduce.py --check
    python3 /path/to/repo/cloud/work/frontier/dot_runtime_a_availability_wave_20261011/reproduce.py --diagnose

All generated assembly, binary and ELF artifacts remain temporary and are
removed. The receipt publishes hashes, scores and offset metadata only.

A second reviewer statically checked both new bodies against the protected
native code: record counts/offsets, signedness, helper gates, list traversal,
stores and call order agree. This adds no execution or matching claim.
The assertion-based replay explicitly rejects optimized Python.
