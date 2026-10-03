# ROM stream census (R16 / S13 lead) — 2026-10-03

**Status:** READY-RESEARCH output. Read-only scan of `baserom.us.z64`
(sha1 `3f99351d…`). No ROM bytes are published here: only offsets, lengths and
counts. Base: master `f82c5204`.

## Method

`scan_deflate.py` walks ROM `[0x10000, 0xB80000)` trying a raw-deflate
(`wbits=-15`) decode at each offset, and jumps past every stream that reaches
end-of-stream with at least 512 decompressed bytes. Hits with
`decompressed ≈ compressed − 5` are single stored blocks matched by chance
(LEN/NLEN collisions in high-entropy data) and are discarded: kept rows require
`decompressed > 1.15 × compressed` and `compressed ≥ 256`. Result:
`streams.json` (119 streams). Runtime on the Pi: about 10 minutes.

## Findings

| ROM range | Content | Evidence |
|---|---|---|
| `0x00000–0x01000` | header + IPL3 | splat |
| `0x01000–0x10000` | counted static code (230 fns, 61,440 B) | `layout.coverage()` |
| `0x10000–~0x2D200` | **more uncompressed MIPS code + data, outside every denominator** | 541 `jr ra` words, against 250 in `0x1000–0x10000`; `0x10000` begins with `jal 0x8000D2B0`-style calls into static code |
| `~0x2D200–0x32F1F0` | uncompressed data, about 3 MB, entropy ≈ 7.0–7.4 bits/byte | likely raw textures / ADPCM sample data; 16-bit RGBA-looking runs near `0x310000`. Unclassified. |
| `0x32F1F0–0xB0CB10` | 116 raw-deflate streams (about 7.7 MB compressed → 15 MB) | `streams.json`; 8 gaps larger than 16 B, the largest about 174 KB at `0x399363` and `0x41484B` (other format or uncompressed) |
| `0xB0CB10` (326,180 → 647,072) | game code image | `blob_rom.ROM_OFFSET` |
| `0xB5C534` (80,272 → 194,128) | **second code image** (195 `jr ra`) | ROM word at `0x2BC20` (= `D_8002B020`) holds `0x00B5C534` |
| `0xB6FEC4` (23,639 → 43,888) | **third code image** (49 `jr ra`) | ROM word at `0x2BC24` (= `D_8002B024`) holds `0x00B6FEC4` |
| `~0xB75B1B–0xC00000` | padding | entropy scan |

### R16 consequence

The R16 card names two loaders that fill `0x8038A400..0x803BB380`
(200,576 B) from the ROM-pointer globals `D_8002B024`/`D_8002B020`. Those
globals point exactly at the two code images above, and both images fit the
window (194,128 B and 43,888 B). An `0x8038xxxx` service address is therefore
ambiguous until it is tied to one of the two images. Contract requests should
name `image=B5C534|B6FEC4` plus the address. Which image is resident for which
game state remains open; that is the loader-flag work in R16.

### Denominator consequence (P01)

If `0x10000–~0x2D200` is confirmed as boot-segment code, the static
denominator (`0x1000–0x10000`) understates static code by roughly 2× by
return count. Before any metric changes, confirm the extent, its vram mapping
(`rom − 0x1000 + 0x80000400` is assumed from the entry segment) and whether
any of it is data that happens to contain `0x03E00008`.

## Next actions

1. P01/S12: disassemble `0x10000–0x2D200` under the entry-segment mapping;
   establish function boundaries and the code/data split.
2. R16: disassemble both runtime images at `0x8038A400` and match
   the blocked `8038D798`, `8038D3A4`, `8038F454`, `80391490` and `8039133C`
   service addresses to a specific image.
3. S13: classify the 116 asset streams by decompressed header shape (several
   share leading words, for example `000c0000` and `00150000`), and find the
   loader's ROM table that indexes them.
