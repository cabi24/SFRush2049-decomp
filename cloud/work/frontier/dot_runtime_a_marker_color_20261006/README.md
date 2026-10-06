# Image-A projected marker color callback

Complete **816-byte / 204-word MATCH** for image A: `func_803A3A6C`,
`[0x803A3A6C,0x803A3D9C)`. Ordinary standalone IDO 5.3 **O3** source.
This is a candidate body, not newly accepted cartridge coverage.

## Identity and source

Current publication base: `dea99f09ab19b1d3b324ed7097162f7b378e7096`.
Initial reconstruction base: `cd22879d40b3de443cfde047b86e75e159b6cec6`, checked against master,
current handoff, the protected runtime-image inventory, source archive and
open PRs through #155 before claiming this fresh callback. Image A loads at
`0x8038A400`, image SHA-256
`0d6702c320df84cc6cbc3c5967dde08d44b6e476e110667fe2a43dc2dd536667`.
The exact protected target body hash is in `verification.json`.

The wave-8 integration advanced master during publication preflight. Its
changes to protected blob-region files only add accepted-source comments;
all native words, our target, four helper/caller bodies, image-A files and
current cloud tools are unchanged. This target/source is not integrated or
claimed by that commit. We refreshed the 10-file packet onto the new base
and repeated independent review before publication. Candidate C is unchanged.
Manifest and scorer hashes remain recorded as dated provenance; `--check`
does not treat those three broad metadata maps or the recorded host-GCC/GNU-linker
version strings as immutable source evidence. Different host tool versions
must still pass the actual unchanged-source sanitizer and full GNU link replay.
Current manifest integrity is still enforced when read, and exact native
bodies, external anchors, complete ELF/GNU equality, source/proof/compiler
hashes and behavior results remain binding. Focused negative tests check
that only these provenance fields are ignored, not the actual evidence gates;
the full replay test changes both recorded version strings and still runs
all executable, ELF, address, relocation, layout, native and behavior checks.

The complete N64-specific source is
`cloud/matches/ovl_a/func_803A3A6C.c`. This is a native-authoritative
reconstruction, not an exact arcade donor. Read-only comparison with
[the pinned arcade select.c](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/game/select.c)
(AnimateCar/AnimateBar, around lines 2340–2425) supplies BLIT-family ancestry
only. It does not establish the N64 source spelling or translation unit.

The callback decodes player from bits 0..3 and marker from bits 16..23.
It hides and disables itself when player >= the signed player count, and
hides without disabling when the selected player's signed flag is nonzero.
Otherwise it applies the strict external depth-window / zero-alpha test
through the actual Hidden helper. Visible markers project a world-space
vector to signed screen coordinates, center by signed width/height division
toward zero, reload alpha, select one of three car-dependent color indices,
write RGBA, point the BLIT at that color and replace its high AnimID nibble.
It calls UpdateBlit and returns one on every defined path.

## Witnessed layouts and ABI

- Native player stride is 2816 bytes and slot stride is 64. `MenuSlot` is a
  typed union view: depth at +8, alpha at +60. Projection reads three floats
  at slot +19 / float +12, equivalent to the witnessed +1264-byte address.
  The 44-slot declaration expresses that stride; synthetic backing does not
  prove the original allocation's global bounds or original type names.
- The view/context stride is 152 bytes. The complete projection helper reads
  its 3x3 rotation matrix and position vector at +36..44. The remaining 104
  bytes stay opaque.
- BLIT +14/+16 are signed coordinates, +20/+22 dimensions, +24 alpha,
  +26 signed Hide, +40 callback, +44 AnimID. The C record is only a 48-byte
  accessed-prefix view. Real engine BLIT objects are larger: UpdateBlit also
  reads +52, outside this prefix.
- The complete `sound_control` / NewMultiBlit caller is independently bound
  at `[0x800B37E8,0x800B39BC)`. It loads descriptor callback +28 at
  `0x800B3900`; masks and stores descriptor AnimID at BLIT +44 at
  `0x800B390C..0x800B3918`; and passes a0=BLIT through the indirect call at
  `0x800B3948`, storing callback +40 in its delay slot. The caller preserves
  state in s-registers across that call. The exact callback has no private
  saved-register clobbers or raw private live-ins. This is not inferred from
  its shortness or its name.
- The native Hidden helper is `[0x80094F88,0x80094FC4)`: compare signed Hide,
  store changed byte, call UpdateBlit, reload signed Hide. Projection at
  `[0x800A6244,0x800A6404)` uses five ordinary O32 arguments and writes exactly
  two signed halfwords through the fifth argument. Its historical
  `brake_light_update` name is misleading. UpdateBlit at
  `[0x80094EC8,0x80094F88)` is a separately bound renderer boundary.

The descriptor table that points specifically to this callback is not part
of the available protected data. Its `data_ref` extent evidence is retained,
but descriptor reachability and the original marker values are not proved.

## Bounded compiler work

The initial complete typed reconstruction used named color-source and
color-destination aliases. O2 differed at 171/204 words. O3 matched the
entire instruction sequence except 27 stack-frame/home operands: an
80-byte frame versus native 72. Workbench diagnosis classified the residual
before any refinement. Repeating the genuine indexed color expressions
removed those duplicate aliases, and declaring the actually used slot
pointer beside the decoded fields put its local home before palette and
position locals. The complete ordinary O3 body then matched.

The initial O1/O2/O3 controls remain in `controls/initial.c`, freshly compiled
with full ELF extents and strict scores on each replay. No fictitious live
values, pressure locals, unused padding, volatile qualifier, private ABI
parameter, assembly, stand-in callee or optimizer policy change was used.
The result does not claim recovery of original declaration spelling.

## Verification

`verify.py` compiles with the current master scorer and `owndata.py` in
separate isolated namespaces for image A and main-game helpers. It checks:

- Complete ELF function and text: 816 bytes, zero alignment tail, zero owned
  data; all 33 relocations and all 15 external address anchors resolved.
- An independent ELF reader and GNU whole-object link, explicitly placing
  `.text` at `0x803A3A6C`; byte equality against the protected native body and
  canonical relocation path, not an address-aligned extracted window.
- 32-bit field/stride layout assertions; complete helper/constructor bodies
  hashed against protected manifests; source, proof and compiler hashes.
- 3,203 producer fixtures through protected native, project-relocated and
  GNU-linked MIPS, a separate behavioral oracle, and the unchanged C89
  source under GCC UBSan/bounds and strict aliasing. All 204 callback words
  and all 15 actual Hidden words execute. The taken third-marker comparison leading to
  uninitialized palette/channel is outside the defined-domain branch set.
- Ordered snapshots, projection arguments/vector contents, full RGBA backing,
  relevant reloads, saved GPR/FPRs, mapped-memory bounds and surrounding stack
  canaries. Every projection/UpdateBlit hook destroys caller-save GPR/FPRs.
  The complete protected Hidden body executes, including its nested call.
- Eight compiled wrong-contract source mutants rejected by strict scoring
  and hosted behavior/argument validation. Unknown native instructions and
  invalid extents fail closed in the focused tests.

The receipt is canonical JSON. The portable equality receipt excludes raw
path-dependent ELF/debug hashes; the local forensic object/linked hashes are
written only to ignored build output. Tool versions/hashes are included.
There is no claim that a clean hosted CI run or these tests prove a ROM.

## Valid domain and limitations

Projection/color-reaching descriptors require player 0..3, marker 7..9,
selected car index 0..12 and selected palette index 0..15, with valid,
disjoint backing. These table sizes are bounded test storage, not inferred
retail resource lengths. Early-return tests additionally cover player 4/15,
signed16 player-count endpoints and several other hidden-only marker values.
For other visible markers the native body reads uninitialized channel/palette
stack values at `0x803A3D08`; the C likewise leaves those locals undefined.
It is not made safer by inventing a default branch that the native lacks.

The two external float-pool addresses are fully bound, but their retail
contents are unavailable. Tests use synthetic finite thresholds/depths,
including signed zero, boundaries and reversed windows. No retail constant,
NaN, infinity, FCSR exception/rounding or trap behavior is asserted.
Integer-to-s16 narrowing follows the target/compiler's implementation-defined
wrapping; out-of-range signed narrowing is not called universally portable C.

Projection and UpdateBlit are explicit adversarial O32 contract hooks. They
can change geometry, AnimID, selected car and alpha to verify captured-index
versus reloaded-value behavior; they do not execute the full renderer or
projection math. Exact helper bodies are contextual provenance, not extra
matching claims. No full-image splice, compression, ROM hash, original TU,
hardware, gameplay, concurrency or accepted-coverage gate was run.

## Reproduce

With the documented IDO/binutils environment:

```
python3 cloud/work/frontier/dot_runtime_a_marker_color_20261006/verify.py
python3 cloud/work/frontier/dot_runtime_a_marker_color_20261006/verify.py --check
python3 -m unittest discover -s tests/cloud -p 'test_runtime_a_marker_color.py' -v
```

A minimal checkout may pass `--repo /path/to/trusted/targets` and
`--tools-repo /path/to/current/tools` as read-only inputs. The source and
receipt stay in this packet's own root; replay works from an unrelated cwd.
Only source, tests and proof notes are published. No ROM bytes, raw assembly,
objects, credentials or unrelated data are included. Independent review must
freeze this exact source/evidence tree before draft publication. Merging and
full acceptance remain with the independent checker; no CI watcher is added.
