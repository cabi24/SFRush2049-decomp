# Quickstart: Game-Code Image Rebuild (008, stage 1)

Everything through the gate runs on the Pi — no IDO, no builder. That is a
deliberate consequence of the contract amendment below.

```bash
python3 -m tools.conveyor.pipeline.blob_layout derive     # build/blob_layout.json
python3 -m tools.conveyor.pipeline.blob_tu generate       # asm/us/blob/*.s + src/blob/blob.ld
python3 -m tools.conveyor.pipeline.blob_build build       # the gate
```

## Contract amendment (2026-09-24)

The contract specified 004's `#pragma GLOBAL_ASM` C translation units. In
implementation that pattern proved to be shaped by splat driving the
cartridge build. For a standalone image, one named section per entry plus an
explicit linker script is simpler, needs no asm-processor, and — the deciding
point — lets the all-passthrough build run on the Pi with no IDO, so the
byte-identity gate could be proven before the builder was involved at all.
Splicing still swaps in an IDO-compiled object per function; only the
container changed. Contract §7-§9 should be read with that substitution.

## 1. Map (T001-T004)

Actual (2026-09-24): image 647,072 bytes at `0x80086A50`, sha256
`bf7da3fa6283…`, **18 regions**, 912 functions covering 514,324 bytes
(79.5%), 214 opaque runs covering 132,748 bytes (20.5%). Largest opaque runs:
the 85,136-byte tail at `0x8010FD60`, then 13,288 at `0x8010C7CC`. Two
derivations produced byte-identical JSON (`3a474a384c4062ba…`) — the map
carries no timestamp by design.

Preflight confirmed the compressed stream is raw DEFLATE, 326,180 bytes at
ROM `0xB0CB10`, inflating to exactly this image.

## 2. Units (T005-T007)

Actual: 18 region files (2.8 MB of `.word` passthroughs and `.incbin`
slices) plus `src/blob/blob.ld`. Both are generated artifacts and
`.gitignore`d — they rebuild from the ROM-derived image and the map in under
a second. Spliced C bodies will be tracked.

## 3. The gate (T008, SC-001)

Actual: **PASS on the first run.** The linked image is byte-identical to
`build/game_code.bin` — 647,072 bytes, sha256 `bf7da3fa6283…` — and the whole
build takes **0.3 s**.

```
blob build OK — image matches (647072 bytes)
  sha256 bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d
```

## 4. Corruption drill (T013, SC-003)

Run before trusting the gate, because 004's ROM gate was vacuous for months.
One instruction word inside `func_80095EC0` was changed to `0xDEADBEEF`:

```
blob build FAILED — image is not byte-identical
first difference at image offset 62576 (vram 0x80095EC0)
  region:   blob_800959dc
  owner:    func_80095EC0
  expected: 00053082
  built:    deadbeef
exit=1
```

Correct offset, correct region, correct owning function, non-zero exit. The
word was restored and the build passes again.

## 5. Splice (T009-T012)

Actual (2026-09-24): **56 of 76 spliced**, image byte-identical throughout —
SC-002 (≥50) met, SC-005 verified by reverting and re-splicing one function.
Image coverage **56/912 functions, 4,168/647,072 bytes (0.64%)**.

Getting there took six corrections, every one of them caught by the gate
rather than by inspection. Recorded because each is a trap for the next
person:

| # | symptom | cause | fix |
|---|---|---|---|
| 1 | `ld` **segfaults**, no message | IDO objects carry `.reginfo` (LINK_ONCE), `.options`, `.mdebug` | strip them at objcopy time, where the failure is attributable |
| 2 | `undefined reference to 'pad_config'` | data globals live inside opaque runs; nothing declares them | `PROVIDE` the merged symbol table in the script |
| 3 | one bad body aborted the whole run | `BuildError` escaping the loop | a body that cannot link is a refusal for *that* function |
| 4 | diff exactly at a function's end | **IDO pads `.text` to 16 bytes**: a 72-byte function occupies 80 in its object and the padding overwrites its neighbour; truncating the section loses its relocations | link each function ALONE at its image address, then cut to the extent — spliced C contributes *bytes*, not a section |
| 5 | diff at offset 0 of a body that scored 0 | spliced at `-O2` while the match was at `-O1` | use the flagset the sweep recorded as scoring 0, grouped per batch |
| 6 | 37 × `undefined reference to 'func_…'` | a function linked alone cannot see its siblings | `PROVIDE` every function address from the map too (4,346 symbols total) |

Run-by-run: 6 → 34 → **56**.

### Remaining 20 refusals

- **12 link failures**: unresolved callees that are not in the map —
  `entity_flags_apply`, `UpdateActiveObjects`, `AdjustSpeed`,
  `Input_ApplyPadConfig`, `func_803914b4`. These are the `extent_conflict`
  suffix rows and the out-of-image `0x8038xxxx` class, neither of which has a
  real address to provide. Closing them needs the 007 residuals, not this
  feature.
- **8 image differences**, including `func_80095EC0` and `func_800C8738` —
  the first two functions ever matched. Their stored score of 0 came from a
  raw-word target *before* the reloc-aware rebuild, so a body that matched a
  relocation-blind comparison need not match in place. The gate is right to
  refuse them; they are re-scoring candidates, not regressions.
