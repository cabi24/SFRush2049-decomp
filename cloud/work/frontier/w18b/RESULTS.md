# w18b — runtime-image stream round-trip and ROM compose (2026-10-11)

Verdict: **the path is possible.** Both runtime-image streams reproduce byte for byte,
and composing them into the ROM gives `ROM matches!`; a one-byte image change fails
the ROM hash.

## Stream reproduction (Pi)

Both streams reproduce exactly from their inflated images with vendored zlib 1.0.4 at
the game blob's parameters (level 9, windowBits -15, memLevel 8, default strategy,
single `deflate(Z_FINISH)`, i.e. `tools/deflate104` unchanged). A sweep of 1,788 (A) /
1,794 (B) parameter sets (level 0-9 x windowBits -15..-8 x memLevel 1-9 x strategy 0-2)
matched only that one set.

| image | pointer | ROM stream | stream B | ends at | followed by | image B | data.bin slot |
|---|---|---|---|---|---|---|---|
| game | (blob_rom) | 0xB0CB10 | 326,180 | 0xB5C534 | image A's stream | 647,072 | 0xAE4740 |
| A | 0x8002B020 | 0xB5C534 | 80,272 | 0xB6FEC4 | image B's stream | 194,128 | 0xB34164 |
| B | 0x8002B024 | 0xB6FEC4 | 23,639 | 0xB75B1B | zero fill to 0xC00000 (end of ROM) | 43,888 | 0xB47AF4 |

Stream SHA-256: A `ebd4b2c1…805710a`, B `0e1292c1…885592e7` (full values in
`ovl_image.STREAMS`). The boot segment holds only start addresses (D_8002B028.. are 0);
the three streams are contiguous, so each slot is fixed-length and a rebuilt image
must compress to exactly the slot length (refused otherwise). A rebuilt image that
compresses to a different length would need neighbour streams moved and the pointer
words rewritten: out of scope, and it cannot happen for byte-identical images.

## ROM compose (watchman2, scratch `~/rush2049/scratch/frontier/w18b/rom`, -j2)

Scratch = git-tracked files of this worktree + baserom + toolkit 796ae99a ido (5.3,
identifies as ido5.3) + `compilers install ido7.1` + `build/blob/game_code.deflate`
copied from the Pi main checkout's blob_rom output (verified == ROM stream).

- Baseline `make COMPILER=ido -j2 && make test` (no OVL_COMPOSE): ROM matches!
- `ovl_rom rom --drill --builder-repo ~/rush2049/scratch/frontier/w18b/rom --jobs 2`:
  identity case (retail images as stand-ins) **SHA-1 EXACT**; drill A (image byte
  +0x17B28 flipped, stream length kept) `ROM does NOT match`; drill B (+0x55B8) `ROM
  does NOT match`; restored **SHA-1 EXACT**. Whole run 14 s.
- After the Makefile change, a plain build without OVL_COMPOSE still matches.

## Commits (branch worktree-agent-a80a1876ba239a91b)

1. `ovl_image`: constants, `inflate(image, rom=None)`, `deflate(data)`, `prove`,
   `check`, `extract`; tests `tests/conveyor/test_ovl_image.py`.
2. `ovl_rom produce|rom [--drill]`, `compose_data.py --extra STREAM:SLOT:LEN`
   (overlap refused), Makefile `OVL_COMPOSE=1` (opt-in; no rule makes the streams),
   `blob_rom.SYNC_FILES` + `builder_rom_build(repo, make_args, log)` (defaults
   unchanged); tests `tests/conveyor/test_ovl_rom_compose.py`.
3. `ovl_image check` CLI tests + tools/conveyor/README.md section after 009.

## Interface for w18a (layout/splice)

```python
from tools.conveyor.pipeline import ovl_image
ovl_image.STREAMS["a"|"b"]   # Stream(image, pointer, rom_offset, length, image_size,
                             #        image_sha256, stream_sha256, followed_by)
ovl_image.BASE               # 0x8038A400
ovl_image.inflate("a")       # retail image bytes (gated on slot end + SHA-256)
ovl_image.deflate(data)      # exact cartridge compression (deflate104)
```
Hand the rebuilt image to `ovl_rom produce|rom --image-file a=PATH --image-file b=PATH`.
`ovl_rom verify` (the earlier C-splice prototype) is untouched.

## Commands for the coordinator

```bash
python3 -m tools.conveyor.pipeline.ovl_image check          # Pi, needs baserom + gcc
python3 -m pytest -q tests/conveyor/test_ovl_image.py tests/conveyor/test_ovl_rom_compose.py
python3 -m tools.conveyor.pipeline.ovl_rom produce          # retail stand-in
python3 -m tools.conveyor.pipeline.ovl_rom rom --drill      # default builder repo, -j16
```
`rom` needs `build/blob/game_code.deflate` locally (run `blob_rom produce` first) and
syncs it with the streams. Production use requires w18a's rebuilt images; until then
OVL_COMPOSE stays off in normal builds (making it unconditional, like GAME_BLOB, is
the coordinator's call once splice lands).

## Not reproduced / caveats

- Nothing failed to reproduce.
- Full `tests/conveyor` run in this worktree: failures/errors in 13 unrelated files
  (missing untracked `src.scorer`, missing `build/game_code.bin`); none touch these
  modules. The new and adjacent suites (ovl_image, ovl_rom_compose, blob_rom,
  ovl_targets) pass.
- `make test` is now invoked with the same make args as the build (`-j16` for
  blob_rom, `-jN OVL_COMPOSE=1` for ovl_rom), so a test-triggered rebuild could not
  silently drop the composition.
