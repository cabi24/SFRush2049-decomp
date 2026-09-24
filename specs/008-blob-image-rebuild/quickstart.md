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

*(actuals follow)*
