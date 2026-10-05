# object_render (0x80087A08..0x8008A148, 10,048 bytes) — structure

The name is a historical label. The function is **"select texture image"**: an RDP texture-state cache in
the same family as `func_80086A50` / `func_8008705C` / `func_800878E0` / `func_80087110`
(`src/blob/groups/gfx_modes/`). It is not a display-list interpreter, has no jump table, no float literals
and owns no `.rodata`. It is not built from inlined statics and has no stub neighbours; its size is 22
expansions of the SDK texture-load macros (7 commands each).

```c
void object_render(void *img, u16 type, u16 siz, u16 width, u16 height,
                   u16 uls, u16 ult, u16 lrs, u16 lrt, u16 pal, s32 tile);
```

| arg | stack | meaning |
|---|---|---|
| img | 632 | texel pointer (modified in place in block mode) |
| type | 638 | 0 RGBA, 2 CI, 5 CI + second 4b tile, 4 I, 3 IA |
| siz | 642 | G_IM_SIZ_* (0 4b, 1 8b, 2 16b, 3 32b) |
| width, height | 646, 650 | texels |
| uls, ult, lrs, lrt | 654, 658, 662, 666 | sub-rectangle |
| pal | 670 | palette number for the render tile |
| tile | 672 | 0 = block load, non-zero = tile load; overwritten with the rectangle key |

Callers (4 sites): `Input_ProcessGameplayPad` (2), `audio_doppler_calc` (2) — both labels are also
historical. A typical call passes `tex->0x18, type, siz, tex->0x10 (u16 width), tex->0x12 (u16 height),
0, 0, width-1, height-1, 0, 0`.

Frame: 632 bytes; saves s0, s1, ra. Locals `s32 mode` (sp+628), `u16 masks` (626), `u16 maskt` (624, never
stored); every macro's block-scoped `Gfx *_g` owns one slot below that (which is why tile-mode PipeSync
`_g`s appear at sp+600, 544, 484, 360, 244, 188); sp+36..92 are uopt spill homes.

## Sections

| Range | What |
|---|---|
| 0x80087A08–0x80087A30 | prologue; `s1 = width`, `s0 = tile` |
| 0x80087A34–0x80087AF4 | `tile == 0`: `img +=` `ult*width` texels (×4, ×2, ×1, /2 by `siz` 3, 2, 1, other); `height = lrt - ult + 1` |
| 0x80087AF8–0x80087B5C | `tile != 0`: `tile = ((s64)uls << 48) + ((s64)ult << 32) + ((s64)lrs << 16) + lrt` (three `__ll_lshift` calls, truncated to 32 bits) |
| 0x80087B60–0x80087C04 | render mode from `type` and `D_8012E608`: type 0/2 → 1 if any of bits 0x20, 0x10, 0x8000 else 0; type 5 → 4; type 4/3 → 3 if bit 0 else 2 |
| 0x80087C04–0x80087C38 | `if (type == 3) func_800878E0(0x20); if (mode != D_8014A248) func_80086A50(mode);` |
| 0x80087C3C–0x80087C7C | cache test: `if (img == D_8012E684 && D_8012E688 == tile) return;` (64-bit compare) |
| 0x80087C78–0x80087CC8 | masks: tile mode `func_80087804(uls - lrs)`, `func_80087804(ult - lrt)`; block mode `func_80087804(width)`, `func_80087804(height)` |
| 0x80087CCC–0x80088490 | **type 0 (RGBA)**: TLUT off if `D_8012E680 != 0`; siz 2: 16b tile (…7EC0) / block (…80DC); else 32b tile (…826C) / block (…8490) |
| 0x80088494–0x80088D4C | **type 2 or 5 (CI)**: TLUT RGBA16 if `D_8012E680 != 1`; siz 1: 8b tile (…8690) / block (…88BC), then for type 5 a second tile (0x800888C0–0x80088978: `gDPSetTile(CI, 4b, (width+7)>>3, 0, tile 1, pal, CLAMP, maskt, 0, CLAMP, masks+1, 15)` + `gDPSetTileSize(1, …)`); else 4b tile (…8B24) / block (…8D4C) |
| 0x80088D50–0x8008954C | **type 4 (I)**: TLUT off; siz 1: 8b tile / block; else 4b tile / block |
| 0x80089550–0x8008A110 | **type 3 (IA)**: TLUT off; siz 2: 16b tile / block; siz 1: 8b tile / block; else 4b tile / block |
| 0x8008A114–0x8008A130 | `D_8012E684 = img; D_8012E688 = tile;` |
| 0x8008A134–0x8008A144 | epilogue |

Tile loads are the SDK `gDPLoadTextureTile` / `gDPLoadTextureTile_4b` verbatim. Block loads are
`gDPLoadTextureBlock` / `_4b` with one change: `gDPSetTileSize(RENDERTILE, uls<<2, ult<<2,
(uls+width-1)<<2, (ult+height-1)<<2)` instead of `0, 0, (width-1)<<2, (height-1)<<2`. All use
`G_TX_CLAMP` for cms/cmt and shift 0.

## Globals

| Address | Type | Meaning |
|---|---|---|
| D_80149438 | `Gfx *` | display-list write pointer |
| D_8012E608 | `u32` | render-state flag word (bits 0x1, 0x10, 0x20, 0x8000 read here) |
| D_8014A248 | `s32` | current render mode (set by func_80086A50) |
| D_8012E680 | `s32` | cached TLUT mode: 0 = G_TT_NONE, 1 = G_TT_RGBA16 (`sound_init` resets it to -1) |
| D_8012E684 | `void *` | cached texture image pointer |
| D_8012E688 | `s64` | cached rectangle key (0 for a block load). The shared declarations currently say `extern s32 D_8012E688; extern s32 D_8012E68C;` — that is one 64-bit object (`sound_init` also stores it as a 64-bit zero with one `lui`) |
| 0x8000D994 | function | IDO `__ll_lshift(long long, long long)`; labelled `__ashldi3` in `symbol_addrs.us.txt` |
