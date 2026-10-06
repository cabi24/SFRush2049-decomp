# Image B F938: complete private-child semantic reconstruction

**Research only. Complete source and bounded native behavior, not a native
private-ABI match or accepted-byte claim.**

Base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.
Native target: `[0x8038F938,0x8038FCD8)`, 928 bytes / 232 words.
Native SHA-256: `1b5eb1e66549eb1cc0fba50a3dc3e432e295fde04bf1777be2a5bcdfccd9e106`.

## Complete source and contracts

`setup.c` reconstructs the whole child: timed player transition, per-owner
scene transform and pitch, grow/hold/shrink scale states, deletion/recreation,
and packed color with unsigned alpha floor. The complete native body has a
56-byte frame and saves only `ra`. Inputs arrive through `s0` status, `s2`
player, `s3` vehicle and `s5` owner; it writes nonvolatile `s1/s3/s4/s6/f20`
without preserving them. Four explicit C inputs provide a semantic adapter
for this packet. They do not assert that the native function has an ordinary
standalone ABI or that its original formal ordering is recovered.

The genuine FCE0 call at `+0x1AC` makes status alias `player.status`. Its
vehicle and owner were selected from the player's signed owner byte. Only
owners 0..3 are admitted here: native setup initializes four 52-byte effects.
The model selector is unsigned and indexes a separately authenticated
13-float table. FCE0's exploratory owner cases do not establish a larger
F938 effect-array allocation.

`contracts.json` records layouts, private entry, historical helper provenance
and field narrowing. Important distinctions:

- Scene slots hold and compare full 32-bit indices against `-1`. Delete and
  color arguments narrow to signed16 only at the call. `0x1234FFFF` is not
  the empty sentinel, even though its signed16 argument is `-1`.
- Player alpha at `+0x3A1` is unsigned; 128/255 remain large values. The
  transition byte at `+0x3A2` is read signed and compared with 2.
- Color is a four-byte entry snapshot of `0xFFFFFFFF`. The optional alpha
  update replaces big-endian byte 3, clamped to at least 48. The final flag
  read uses `player.status`, preserving the native expression and alias fact.
- The existing scene-create wrapper's accepted `void` declaration is
  inconsistent with its native callers consuming `v0`. The underlying
  allocator returns a sign-extended16 scene index. This packet uses a local
  return-bearing declaration and changes no accepted source.
- Matrix copy, PitchUV, scene delete/create, and packed color are ordinary
  external services. None is replaced with optimizer-visible fake source.

No arcade original for this N64-specific child is established. The source
comes from the authenticated native control flow and layout evidence. No
inline assembly, volatile shaping, fake keepers, artificial pressure locals,
unused input parameters, declaration sweeps or guessed stub body is used.

## O3 and whole-ELF evidence

The source starts with the exact bare requested O3 header. The unchanged
canonical `score.compile_single` injects its required `-Wab,-r4300_mul` flag;
this is recorded as actual compiler provenance. All five helpers are external.
There is one full defined function, no hidden context body or keep list.

The candidate function is 1,044 bytes, with 12 separately checked zero text
alignment bytes and 32 bytes of owned rodata. The entire unmodified object
is GNU-linked at research addresses; every external binding, relocation and
allocated section is recorded. No executable neighbor is borrowed, no native
instruction slice is substituted, and no object instruction is patched.

The linked research-placement comparison differs in 228/232 native word
positions and has 27 nonzero words beyond the native extent. This is a broad
NONMATCH diagnostic, including candidate data at research placement. It is
not strict native-placement owned-data evidence and does not justify an
allocation-tuning loop. Full genuine closure and original visibility evidence
are still prerequisites for a meaningful private-ABI matching attempt.

The fail-closed interpreter compares native and compiled full-body memory
state and ordered call arguments/state digests while poisoning caller-save
registers. It runs 3,615 paired fixtures (7,230 executions), covering 231/232
native offsets and 260/261 candidate offsets. Native `+0x2E8` and candidate
`+0x358` are unreachable initial loop peel paths for a fixed three-element
row. They remain included in full instruction/extent authentication.

Fixtures cover all 32 low status-bit combinations plus unrelated high bits,
signed transition/blocked bytes, timer boundaries, model and owner bounds,
scale stages, unsigned alpha boundaries, full-word sentinel cases and signed
handle narrowing. Nine compiled wrong-source controls are rejected, alongside
unknown-opcode refusal. Adversarial callback mutations test later reads,
entry color snapshot and the post-delete `-1` store; these deliberately
broaden known helper effects and are not assertions about actual helper writes.

## Reproduce and portability

With the repository's pinned IDO and a MIPS GNU linker:

    python3 cloud/work/runtime_b_f938_setup_20261006/verify.py --check
    python3 -m pytest tests/cloud/test_runtime_b_f938_setup.py -q

A source-only overlay uses `--reference-root /path/to/repository`; pytest can
use `RUSH_REFERENCE_ROOT`. Historical assets/context are read through
`git show BASE:path`. Native selected words come from `score.targets()[FN]`.
Receipts bind this packet's own sources, complete compiled content, extents,
relocations, data and behavior, without hashes of live integration files or
live lock-state assertions. Tool provenance is recorded but excluded from
portable replay equality; actual compiled proof fields must still agree.
Tests skip compiler-dependent replay cleanly without IDO or the GNU linker.
The test itself is not hashed into its receipt. Use a unique `TMPDIR` per run;
normal temporary-directory cleanup is retained.

Before publication the coordinator must run the required current-master full
suite with and without IDO after independent frozen-source review. This
packet does not open a PR or update accepted/protected files by itself.

## Bounded proof limits and next action

- Real caller aliasing, four valid owner slots, model0..12, aligned valid
  initialized storage and disjoint player/vehicle/effect/scene regions.
- Zero and normal finite binary32 values with default rounding. Exact libm,
  NaN/infinity, denormal/overflow/underflow, exception/FCSR behavior, arbitrary
  invalid pointers or concurrent mutation are outside the proof.
- Matrix copy is modeled exactly. PitchUV uses the native row-update structure
  and rounded host sine/cosine as bounded outputs, not N64 libm equivalence.
  Color's selected word store is modeled exactly. Real scene allocator/deleter
  bodies and scene-graph capacity/lifetime are not executed.
- Extreme signed16 handles exercise the call boundary using synthetic mapped
  selected color words. They do not assert that those handles are valid in a
  running game. Creation tests do not establish capacity/failure recovery.
- Full FCE0/E114/DA78/D498 original TU/export visibility remains unresolved.
  This child supplies missing genuine source for that closure but makes no
  whole-context MATCH, image, compression, ROM or coverage claim.

Next: reconcile this complete body with the independently reconstructed real
FCE0 and E114 family, then establish actual closure visibility before one
justified whole-context O3 compile. Preserve existing research evidence.
