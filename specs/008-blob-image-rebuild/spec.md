# Feature Specification: Game-Code Image Rebuild (blob stage 1)

**Feature Branch**: `008-blob-image-rebuild` (off `007-population-closure`)
**Created**: 2026-09-24
**Status**: Draft
**Input**: User description: "Spec stage 1 of the blob rebuild path: link the
decompressed game-code image from C behind a byte-identity gate, so matched
game functions become measured coverage. Out of scope: reproducing the
compressed stream (stage 2), promotion into the cartridge, m2c internals."

## Overview

Track B can now match game code — 76 functions are byte-identical as of
2026-09-24 — and **none of it counts for anything**. The matches are
evidence-only behind the 005 firewall because the game code does not exist in
the ROM as code: it is a raw DEFLATE stream at ROM `0xB0CB10` that the game
inflates at boot to `0x80086A50`. There is no build path that turns verified
C back into that image, so matched functions cannot be linked, cannot be
gated, and cannot be counted.

This feature builds that path for the **image**, and stops there. It links
the 647,072-byte decompressed game-code image from translation units — all
passthrough at first, spliced with verified C one function at a time — behind
a gate that the linked image must equal `build/game_code.bin` byte for byte.
It deliberately does **not** attempt to reproduce the compressed stream; that
is stage 2, tracked separately, and the two are independent (measured:
stock zlib `-9` comes within 140 bytes of the original's 326,180 but shares
no bitstream, so the original encoder is zlib-like and unidentified).

The mechanism is the one that already works. Feature 004 does exactly this
for the static ROM: derive a layout map, convert a splat `asm` subsegment to
a ROM-aligned `c` TU of all-passthrough `GLOBAL_ASM` (byte-identical by
construction), then splice verified C over one passthrough at a time behind a
full-ROM SHA-1 gate. This feature is that pattern pointed at the image
instead of the cartridge, with image byte-identity as the gate.

### Why it is worth doing before stage 2

The image is **ten times** the code we currently track: 647,072 bytes versus
the 61,440-byte static range where today's 19 promoted functions live. Stage
1 alone converts every current and future game-code match into a measured,
gated number, and it is not blocked on the compression research — which may
take an afternoon or may never land.

## Measured starting position (2026-09-24)

| fact | value |
|---|---|
| image | 647,072 bytes, `0x80086A50`–`0x801249F0` |
| compressed stream | raw DEFLATE, 326,180 bytes at ROM `0xB0CB10`, verified to inflate to exactly this image |
| gate-passed functions | 912, covering 514,324 bytes (**79.5%**) |
| interior gaps | 213 runs, 47,612 bytes (data, jump tables, unregistered code) |
| tail after the last function | `0x8010FD60`–`0x801249F0`, 85,136 bytes (data) |
| byte-identical matches today | 76 functions, 5,820 bytes (**0.90%** of the image) |

Note the image ends at `0x801249F0`; CLAUDE.md's `0x80124AF0` is off by 256
bytes and should be corrected.

## User Scenarios & Testing

### User Story 1 — the image builds byte-identically from TUs (P1) 🎯 MVP

A maintainer runs the blob build and gets an image identical to the one the
ROM inflates, assembled from checked-in translation units rather than one
opaque blob.

**Independent test**: `blob build` emits an image whose sha256 equals
`build/game_code.bin`'s, from TUs that are all passthrough.

### User Story 2 — a verified function links as C (P2)

A maintainer splices one of the 76 matched functions into its TU; the image
still hashes identically, and coverage reports one more function linked.

**Independent test**: splice, rebuild, image sha unchanged, coverage +1
function / +N bytes; revert restores the previous state exactly.

### User Story 3 — coverage is reported honestly (P3)

`make progress` (or the blob equivalent) reports image coverage separately
from ROM coverage, and never implies the cartridge hashes when it does not.

**Independent test**: with N functions spliced, the report shows image
coverage N and states the cartridge still embeds the original stream.

### Edge Cases

- **Unregistered bytes**: 47,612 bytes of interior gaps and 85,136 bytes of
  tail data have no function rows. They stay verbatim `.incbin` — the TU
  covers them without claiming to understand them.
- **A function whose extent is wrong**: if a spliced function's bytes do not
  land exactly, the image gate fails and the splice is rolled back. The gate
  is authoritative; extents are not trusted just because they passed the
  005 scan.
- **`extent_conflict` rows (375)**: these are suffixes of other functions;
  they are never independently spliceable and must not appear in the map.
- **Data referenced by spliced C**: a matched function may reference a data
  symbol whose address falls in an `.incbin` region. The link must resolve it
  to the right address without the region being carved up.
- **Compiler flags per TU**: the static side needed per-file `-O1`/`-O2`
  overrides (004's `opt_overrides.mk`). The blob is likely uniform `-O2`, but
  the map must carry a per-TU flagset rather than assume.

## Requirements

### Functional Requirements

- **FR-001**: A derived, deterministic **image layout map** MUST describe the
  647,072-byte image as an ordered list of TU-sized regions, each carrying its
  vram range, the functions inside it, and its flagset. Regenerating it from
  the same inputs MUST produce byte-identical output.
- **FR-002**: The map MUST cover the image completely — every byte belongs to
  exactly one region, and any byte not inside a gate-passed function extent
  is marked as opaque data.
- **FR-003**: Generated TUs MUST be byte-identical by construction: all
  function bodies enter as `GLOBAL_ASM` passthroughs and all non-function
  bytes as `.incbin` of the extracted image.
- **FR-004**: `blob build` MUST link the TUs at `0x80086A50` and emit an image
  file plus its sha256.
- **FR-005**: The build MUST **fail loudly** unless the linked image is byte
  identical to `build/game_code.bin`. No partial-credit pass, no warning-only
  path. (The 004 SC-003 drill found a hash gate that had been vacuous for
  months; this gate must be drilled the same way before it is trusted.)
- **FR-006**: Splicing a verified function MUST replace exactly one
  passthrough with its C body, rebuild, and commit only if the image hash is
  unchanged — otherwise roll back completely, leaving no partial edit.
- **FR-007**: Splices MUST be recorded with provenance (target, score,
  flagset, toolkit sha, source hash) in the same lock discipline as
  `matched.lock.json`, so a body cannot silently drift.
- **FR-008**: Coverage reporting MUST state image coverage (functions and
  bytes linked from C out of 647,072) **separately** from ROM coverage, and
  MUST state plainly that the cartridge still embeds the original compressed
  stream.
- **FR-009**: The 005 promotion firewall stays intact for the *cartridge*:
  nothing in this feature promotes extracted evidence into the ROM build or
  into `matched.lock.json`'s ROM-TU paths.
- **FR-010**: If the image cannot be rebuilt byte-identically, the run MUST
  stop and report the first differing offset with the region and function that
  owns it, rather than proceeding with a partial image.

### Non-Goals

- Reproducing the compressed stream, or making the cartridge SHA-1 match with
  a rebuilt blob (stage 2).
- Renaming, re-typing or improving the decompiled C.
- Changing how functions are matched, scored or searched.

## Success Criteria

- **SC-001**: `blob build` produces an image byte-identical to
  `build/game_code.bin` from all-passthrough TUs, reproducibly.
- **SC-002**: At least **50** of the 76 currently matched functions are
  spliced and still produce a byte-identical image; each splice is recorded
  with provenance. (Not all 76: some may sit in `extent_conflict` regions or
  reference data the link cannot yet resolve — those are reported, not forced.)
- **SC-003**: A deliberate corruption drill — one spliced body altered by one
  instruction — makes the build fail with the offending offset named. The gate
  is proven, not assumed.
- **SC-004**: Coverage reporting shows image coverage and ROM coverage as
  distinct numbers, with the cartridge caveat stated in the output.
- **SC-005**: Rebuilding after a revert restores the exact prior image hash;
  the process is reversible.
- **SC-006**: Full local suite green; no change to ROM coverage, the ROM
  hash, or `matched.lock.json`'s existing entries.

## Assumptions

- `build/game_code.bin` is authoritative for the image and is reproducible
  from the ROM (`tools/extract_game_code.py`); its sha is recorded in the map.
- The image is position-dependent code at a fixed base with no load-time
  relocation beyond what the instructions encode — supported by the fact that
  raw-word target objects matched ROM words exactly for 891 functions.
- IDO with the confirmed flagsets is the right compiler for this code; the 76
  matches were produced with it.
- The builder (watchman2) remains the only machine that can run IDO and
  therefore the blob build.
