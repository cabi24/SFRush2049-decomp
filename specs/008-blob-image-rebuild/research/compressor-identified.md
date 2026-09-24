# The blob compressor is zlib 1.0.4 (2026-09-24)

Stage 2 — reproducing the cartridge's compressed game-code stream — was
specified as open-ended research with an honest fallback. It took one
afternoon of the cheap decisive test.

## Result

**zlib 1.0.4, `deflateInit2(level=9, method=Z_DEFLATED, windowBits=-15,
memLevel=8, strategy=Z_DEFAULT_STRATEGY)` reproduces the stream byte for
byte.**

```
original : 326180 bytes  sha256 9e632d21b7d0d293563755d9f2033557cb1a4d91d7df6e8d41e68d8afe3e5f7d
rebuilt  : 326180 bytes  sha256 9e632d21b7d0d293563755d9f2033557cb1a4d91d7df6e8d41e68d8afe3e5f7d
BYTE-IDENTICAL: True
```

The rebuilt stream also round-trips: inflating it yields
`build/game_code.bin` exactly.

## Why it took a specific version

Every other build tested lands 140 bytes long and shares no bitstream:

| zlib | result |
|---|---|
| **1.0.4** (1996) | **exact, level 9 / memLevel 8** |
| 1.1.3, 1.1.4 | closest 326,320 (+140) |
| 1.2.0, 1.2.3 | closest 326,320 (+140) |
| modern (3.x via Python) | closest 326,320 (+140) |

zlib changed `deflate_slow`'s lazy-match heuristics after 1.0.x, so
everything from 1.1 onward makes slightly different match choices and
produces a different (marginally longer) stream. "Nearly the same size" was
never going to be good enough for a hash, but it was the clue that the
encoder was zlib and not something else.

Note memLevel **8**, not 9 — the default, not the maximum. Level 9 with
memLevel 9 is what modern zlib gets closest with, which is a red herring.

## What this unlocks

The cartridge can now be rebuilt from the game-code image: link the image
(008 stage 1), compress it with zlib 1.0.4 at these settings, splice the
result into the ROM at `0xB0CB10`, and the full-ROM SHA-1 gate applies. Every
function spliced into the image becomes real cartridge coverage — 56 today,
and the 647,072-byte image is ten times the static code area the project has
been measuring against.

## Reproducing

```bash
curl -sL -o z.tgz https://zlib.net/fossils/zlib-1.0.4.tar.gz && tar xzf z.tgz
cd zlib-1.0.4 && ./configure --static && make libz.a
# deflateInit2(&s, 9, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY)
```

zlib 1.0.4 is a 1996 release with known CVEs; it is used here **only** as an
offline byte-reproduction oracle for a 1999 ROM, never to process untrusted
input. It should be vendored and built as a static archive for the build,
not installed system-wide.
