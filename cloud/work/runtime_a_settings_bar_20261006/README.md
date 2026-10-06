# Image-A projected settings-bar callback

## Current receipt schema (2026-10-06)

Current receipts omit whole-file manifest, scorer/own-data tool, lock and redundant
production-context digests. The recorded BASE and `git show BASE:path` source reads
remain, as do native target authentication, own packet/verifier/C-harness bindings,
compiler executable identities, complete compiled extents, relocations, owned data
and behavioral evidence. Historical compatibility normalizers accept only their
explicitly listed legacy fields; unknown proof fields and changed invariants still
fail comparison. Descriptions of the earlier receipt schema below are historical.
This schema correction adds no matching or accepted bytes; fresh replay and the
aggregate test matrix are separate required checks.

**Complete NONMATCH: 4/193 words differ. Zero matching, accepted-byte, or ROM
coverage gain.** The full interval is `[0x803AE63C,0x803AE940)`, 772 bytes.
This is a frozen source/contract research packet, not a matching submission.

## Novelty and source contract

Fresh master `cd22879d40b3de443cfde047b86e75e159b6cec6`, current handoff, current
protected extents and open PRs through #157 were checked before reconstruction.
No earlier complete source or active claim for this image-qualified callback
was found. The separately active `A:803A3A6C` is not edited or claimed here.

The final packet was advanced to current master
`dea99f09ab19b1d3b324ed7097162f7b378e7096` and fully replayed. Its consumed
image-A/blob files and scorer were verified byte-for-byte against that commit.
Wave 8 did not add this callback or change its native/helper instruction bytes;
changed blob-region comments/manifests are reflected in the fresh receipt.
The final replay uses the current scorer's normal `owndata` dependency and
dataclass comparison fields. No protected file was edited.

The entry is independently marked `data_ref` and `prologue` by the image-A
extent manifest. No direct caller occurs in the protected game/image-A code.
Two genuine BLIT callback consumers were read in full: NewMultiBlit
(`sound_control`, `0x800B37E8`, 468 bytes) and the frame dispatcher
(`UpdateActiveObjects`, `0x800F733C`, 192 bytes). Their indirect calls at
`0x800B3948` and `0x800F7384` pass the BLIT in a0, use returned v0, and retain
only properly saved s-registers across the callback. The callback itself
leaves all s-registers untouched and homes its live caller-save registers.
There is no hidden incoming-register or extra-preservation contract in these
two consumers. Exact registration of this function in a particular descriptor,
and actual runtime reachability, remain unproved.

The sole input is a BLIT prefix. AnimID low four bits select the segment; its
**entire high sixteen bits** select the slot. `D_803B6B14` has the witnessed
64-byte slot stride; this callback reads slot+21: depth at +8, the projection
position at +48, and alpha at +60. These are bounded storage views, not a claim
to original type names or original table length. Hidden `0x80094F88` has a
signed-byte result, compares and updates Hide +26, calls UpdateBlit if changed,
then reloads Hide. The complete actual 60-byte Hidden body executes in tests.

Projection `brake_light_update` (`0x800A6244`, 448 bytes) receives player zero,
three floats at the selected position, context `D_80150B70`, a null optional
float-vector output, and a real two-halfword output at O32 stack+16. Its native
body writes exactly two halfwords. The context prefix contains a 3x3 rotation
and three-position floats; the per-player stride is 152 bytes. No larger
output array, invented formal, or helper replacement is used.

The callback checks its initial depth/alpha visibility; returns 1 if Hidden
does; otherwise projects and computes signed-halfword geometry. Quarter-height
and half-height divisions round toward zero. Alpha 255 becomes 254. A second
depth comparison chooses a texture half. Segment 1 selects another quarter;
markers 14/15/18/19 scale the right edge by four signed-byte settings with
divisors 3/4/5/2. It updates the BLIT and returns 1. Slot, marker and segment
are captured before helpers; geometry, depth, alpha and settings are observed
again afterward where native code does so.

Pinned arcade `historicalsource/rushtherock` commit
`845329d7b36f5a384c5625ed9a0aef584ab46139`, `game/select.c:AnimateBar`, was
inspected for ancestry. It supports the general BLIT/bar idiom but does not
supply this N64 settings callback. No exact donor is claimed.

## Bounded compiler result

The complete natural source initially scored 192/193 at O2 and 21/193 at O3.
Required workbench diagnosis found a 72-byte candidate frame versus native64
and a quarter-height carrier that also perturbed four registers. Replacing
that redundant named carrier with direct, genuinely consumed geometry
expressions yields the native64-byte frame and 4/193. The remaining differences
are exclusively the slot pointer's four stack saves/reloads at offsets
`+0x8c,+0x9c,+0xd0,+0xe4`: candidate stack+40, native stack+44.

Two limited controls were rejected: grouping the pointer before the real
output array gives 7/193; narrowing the named quarter-height local gives
136/193 plus one extra nonzero word. They are deterministic recipe controls
in `verify.py`, not broad spelling sweeps. Independent contract review found
no genuine source evidence for altering the remaining home. This packet is
frozen until such evidence exists. No pressure locals, padding arrays,
prototypes chosen for allocation, fake callers, assembly or compiler-policy
changes are used.

## Verification and limitations

- Complete 772-byte ELF function, 784-byte .text with twelve zero alignment
  bytes, no owned data, all 21 relocation sites and twelve address bindings.
- Explicit-address GNU whole-object link agrees with the project relocator,
  including exactly the same four nonmatching words; no unknown/masked site.
- 1,864 protected-native/GNU-linked/oracle fixtures, 3,728 machine executions.
  Every 193 callback instruction and all 15 real Hidden instructions execute.
  Tests check saved integer/FP registers, caller-clobber poisoning, mapped
  memory, write confinement, complete state snapshots and helper call traces.
- Four compiled wrong-source controls are rejected: alpha cap, first divisor,
  marker shift and return value. C89 32-bit structure-layout assertions pass.
- Current-base receipt replay and 71 scoped packet/scorer/guard/submission tests
  pass. Accepted-single/group aggregate scorer cases were intentionally excluded
  from this minimal checkout; no full-suite pass is claimed.

Backing, thresholds, setting values and helper mutations in the fixtures are
explicitly synthetic. The tested marker set extends beyond 255, but no original
table range or reachable descriptor value is inferred. Float comparisons use
finite synthetic values; real constant-pool values remain external anchors.
Projection is an output/pointer contract hook. Its numerical internals, overflow,
conversion validity, FCSR, traps and renderer execution are not proved. Hidden
executes real native code, while its UpdateBlit dependency is an effectful
contract hook. Narrow signed-halfword results describe target MIPS behavior,
not portable C behavior beyond representable values. There is no host-C runtime
or sanitizer pass claim, nor a full-suite, image, compression, ROM or hardware
claim.

## Reproduce

```
python3 cloud/work/runtime_a_settings_bar_20261006/verify.py --repo TRUSTED_REPO --tools-repo TRUSTED_REPO
python3 cloud/work/runtime_a_settings_bar_20261006/verify.py --repo TRUSTED_REPO --tools-repo TRUSTED_REPO --check
```

Use the ordinary documented IDO 5.3/binutils environment. The trusted checkout
supplies unchanged protected game/image-A targets and current scorer/toolchain.
The packet writes only its ignored build directory and receipt. All code/source
and artifact hashes are bound in `verification.json`; no raw native words,
disassembly, objects, ROM bytes, credentials or unrelated data are published.

## Integration-portable replay (2026-10-06)

Scorer, whole-manifest and accepted-context digests are historical provenance,
not live-tree requirements. The verifier normalizes only enumerated provenance
fields on both receipt sides. Packet source and verifier bindings, compiler
identity/actual flags, selected native bodies and addresses, complete emitted
extents, relocations, owned data and behavioral checks remain binding.
