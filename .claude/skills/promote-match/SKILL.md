---
name: promote-match
description: Integrate verified Rush 2049 C matches into static ROM segments or the compressed game image and verify cartridge identity.
---

# Put a match into the cartridge

Run from the repository root. Establish the target population and recorded
true-zero evidence, then use the corresponding path below. Read
[builder operations](../../../docs/BUILDING.md#watchman2-builder) before syncing
or building remotely. Respect the requested target scope; batch commands affect
all eligible matches. Static promotion commits on success and rolls back on failure.

## Static boot and library code

Follow [static promotion](../../../tools/conveyor/README.md#promotion-splicing-matches-become-rom-004):

1. Verify/lock the candidate with its proven flagset and inspect existing locks.
2. Derive the layout and convert the required segment to a ROM-aligned C TU if
   needed. Conversion must preserve ROM bytes before introducing matched C.
3. Run `python3 -m tools.conveyor.pipeline.promote run <seg>:<fn> --from <path> --via-builder`.
   The workflow requires a clean tree, rebuilds on the builder, checks the full-ROM
   SHA-1, and migrates the lock to the ROM TU. Resolve a refusal through its stated
   remedy; do not silently bypass evidence or discard unrelated changes.

## Extracted game code

Follow [image splicing](../../../tools/conveyor/README.md#game-code-image-rebuild-008-stage-1)
and [cartridge composition](../../../tools/conveyor/README.md#game-code-blob-into-the-cartridge-009-stage-2).
Static `lock`/`promote` commands intentionally reject this population.

1. Derive `blob_layout` and generate `blob_tu` inputs. The layout must tile the
   image without overlaps or holes, preserving opaque data regions.
2. Use `blob_splice` for the requested matched target(s), with the exact flagset
   that scored zero. `splice --all-matched` is the batch form. The image gate must
   remain byte-identical to `build/game_code.bin`; preserve function extents despite
   IDO object padding. Splice locks live in `blob_matched.lock.json`.
3. Run `python3 -m tools.conveyor.pipeline.blob_rom rom` on the Pi. It builds and
   compresses the linked image, syncs the required blob to the builder, and runs
   the ROM build plus `make test`. The required `build/blob/game_code.deflate` uses
   vendored zlib 1.0.4 with the documented exact parameters; no extracted-byte fallback.

## Evidence to report

Report the target(s), flags, image gate where applicable, built-ROM hash result,
and derived coverage from `make progress`. Keep static and game denominators
separate. If a gate fails, report the failure and resolve it before claiming
cartridge coverage. Re-run the deliberate failure drill when changing the gate
itself; routine source promotion uses the existing gates.
