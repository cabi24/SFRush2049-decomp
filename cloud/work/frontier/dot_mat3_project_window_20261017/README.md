# ZOID_ProjectWindow: restore the genuine window getter

`brake_light_update` at `0x800A6244` is actually ZOID_ProjectWindow, a world-to-screen projection routine. This natural C candidate is **112/112 words, 448 bytes, strict MATCH**. The real BodyVector (`func_800A61B0`) and ZOID_GetWindow (`func_800A6078`) helpers also strictly match. The frame is the native **72 bytes**, without unused padding.

This is an isolated source/research packet. Accepted image/ROM credit is **zero**; whole-unit, image and ROM gates remain pending. The single new claim is the root; both already-locked standalone helpers are preserved as strict context, with no additional byte credit.

## What changed

The old tracked w9d/w14y draft directly indexed the window array and added an unused `s32 pad` to reproduce the frame. It remained 74/112 words different. Removing that unused local alone leaves an incorrect 64-byte frame and 86 differing words.

The matched MAT3 reconstruction calls the real ZOID_GetWindow helper. The N64 repository already has that genuine routine at `func_800A6078`: it returns the selected 0x48-byte window record. Restoring this call and removing the unused local closes the root exactly. Stock IDO inlines the getter, and this genuine call boundary naturally produces the right frame and allocation behavior. There is no fake helper or dummy caller.

The source-change verifier permits only the unused-local removal, the helper declaration/call, and moving the inverse-depth assignment after that side-effect-free getter. A positive control that leaves the inverse before the getter emits identical executable/relocation sections, establishing that the genuine call boundary is the relevant change.

The historical two-element screen-coordinate array remains a meaningful N64 reconstruction. MAT3's accessible reconstructed body uses scalar `sx`/`sy`; its original DWARF lists neither, so neither spelling is asserted as original N64 source. The scalar control fails at 64/112. The donor is guidance, not a substitute for native proof.

## Native behavior and donor differences

- Five arguments: signed 32-bit window index, point vector, 0x30-byte view matrix/translation record, optional body-space vector output, and signed-short screen-coordinate output. Return type is void.
- The point is translated relative to the view and transformed by BodyVector.
- Depth is clamped to the N64 **2.5f** minimum. The later MAT3 **0.01f** value is not transferred.
- The N64 vertical projection has no MAT3 **0.93333f** correction.
- Both screen coordinates saturate to [-32768, 32767], then truncate within range. The optional body-space output includes the clamped depth.

MAT3 provenance: `GameSource/src/zoidwindow.c`, ZOID_ProjectWindow at 0x8006385C, 388 bytes; source SHA256 `ce9be99cc01cacae7aa684ee42b476348ea48c585c32ad6fb7d48adc59bf9d25`. Its existing report records a matching 13-function unit. This packet does not claim a fresh MAT3 compile or identical historical source.

## Verification

With stock IDO 5.3 and MIPS binutils configured:

```
python3 cloud/work/frontier/dot_mat3_project_window_20261017/replay.py --check
python3 -m pytest -q tests/cloud/test_mat3_project_window.py
```

`replay.py --materialize <empty-directory>` writes the isolated group using unchanged canonical helper files. `tools/cloud/score.py group <directory> --claims` can then score its root claim directly. The replay always scores all three functions, including both helpers, with strict own-data/relocation checks and explicit ELF function-size checks of 448, 148 and 28 bytes.

Proof includes two fresh repeats, the identical reordered-getter control, and rejected mutations:

- Flatten the real getter: 86 differing words, frame64.
- Restore the historical direct-index/padding draft: 74 differing words.
- Use scalar screen coordinates: 64 differing words.
- Apply the wrong MAT3 depth clamp or vertical correction: rejected.
- Change the real window stride: both root and getter are rejected.

The finite host differential harness checks all four windows, translated/rotated views, depth boundaries, both signed-short saturation boundaries, and null/non-null optional output. Undefined-behavior sanitization is fatal, and a wrong-depth mutation must exit with the exact differential-mismatch code and message, rather than an arbitrary failure. These bounded tests supplement the exact native result; they do not assert behavior for arbitrary NaNs, invalid indices or overlapping output buffers.

Source, recipe, canonical context, target hashes, complete scores, executable/relocation section hashes and function boundaries are retained in `proof.json`. No ROM bytes, raw assembly dumps, objects, private traces, or credentials are included. Independent review is recorded separately.
