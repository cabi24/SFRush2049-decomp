# Feature Specification: Game-Code Blob into the Cartridge (stage 2)

**Branch**: `009-blob-rom-splice` (008 image rebuild merged onto the 007 promotions line)
**Created**: 2026-09-24 · **Status**: In progress

## Goal

Build the cartridge's compressed game-code blob from sources, so that every
function spliced into the game-code image (008) is cartridge coverage under
the full-ROM SHA-1 gate. Today 56 spliced functions count for nothing in the
ROM because the ROM build copies the original compressed bytes from the
extracted `assets/us/data.bin`.

## Facts this rests on

- The blob is raw DEFLATE, 326,180 bytes at ROM `0xB0CB10`, inside the
  `data` bin segment (`0x10000`–`0xC00000`, `assets/us/data.bin`).
- **zlib 1.0.4**, `deflateInit2(9, Z_DEFLATED, -15, 8, Z_DEFAULT_STRATEGY)`,
  reproduces it byte for byte (`specs/008-blob-image-rebuild/research/
  compressor-identified.md`). Every later zlib is 140 bytes long.
- 008 links the 647,072-byte image from sources, byte-identical, 56 spliced.

## Design

1. **Vendored compressor.** zlib 1.0.4 source in `tools/zlib-1.0.4/` (zlib
   licence) plus `tools/deflate104/deflate104.c`, a CLI with the parameters
   fixed. Built on demand as a static archive. **Build-only:** a 1996 release
   with known CVEs, used only to compress our own image, never untrusted input.
2. **The blob is an input to the ROM build, not a copy of it.** The
   `data.o` rule composes `data.bin[:slot] + game_code.deflate +
   data.bin[slot+326180:]`. The original compressed bytes are never read.
   A missing `game_code.deflate` fails the build — there is no fallback.
3. **Where it runs.** The image needs the layout map and cached spliced
   objects, which live on the Pi, so the Pi builds image → deflate and syncs
   it to the builder with the Makefile; the builder runs the ROM build and
   `make test`.
4. **Length invariant.** The compressed blob must be exactly 326,180 bytes;
   anything else would shift the rest of the ROM and is refused before any
   build, naming the length.

## Gates

- **Pre-flight (Pi, seconds):** image byte-identical to `game_code.bin`
  (008 gate) and compressed output byte-identical to the ROM's stream.
- **Authoritative (builder):** full-ROM SHA-1 from `make test`.
- **Drill:** one byte of `game_code.deflate` altered must fail the ROM SHA-1.
  Without this the gate could be vacuous — identical bytes flow through
  either path — so the drill is what proves the ROM's bytes come from the
  pipeline.

## Success criteria

- **SC-001**: the ROM builds SHA-1 exact with the blob composed from
  `build/blob/game_code.deflate`, and the original stream is not read.
- **SC-002**: the drill fails the ROM SHA-1; restoring passes it.
- **SC-003**: `make progress` counts spliced game functions as cartridge
  coverage once SC-001 holds, reported alongside static coverage.
- **SC-004**: a missing or wrong-length blob fails loudly before a build.
- **SC-005**: suite green; existing locks and ROM hash unchanged.

## Out of scope

Changing the blob's length or layout; building the image on the builder
(it needs the Pi's database); promoting unmatched functions.
