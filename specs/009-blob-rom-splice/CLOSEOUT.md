# 009 Game-Code Blob into the Cartridge — close-out (2026-09-24)

| SC | Verdict | Evidence |
|----|---------|----------|
| SC-001 ROM builds SHA-1 exact from the source-built blob | **MET** | `blob_rom rom`: `MAKE=0 TEST=0`, `ROM matches!` on watchman2 |
| SC-002 one flipped bit fails the ROM SHA-1; restore passes | **MET** | drill: `ROM does NOT match` from `verify` (a hash mismatch, not a build error); restored `ROM matches!` |
| SC-003 spliced game functions counted as cartridge coverage | **MET** | `make progress`: static 23/230, game 56/912, **79 functions linked from C** |
| SC-004 missing or wrong-length blob fails loudly | **MET** | `No rule to make target 'build/blob/game_code.deflate'`; `compose_data` refuses any length ≠ 326,180 (tested) |
| SC-005 suite green; locks and ROM hash unchanged | **MET** | full suite green; 26 locks intact (pre-commit hook) |

## What changed

The ROM build no longer copies the game-code blob out of the extracted data.
It composes the data segment around a blob compressed — with vendored zlib
1.0.4 at the cartridge's exact parameters — from the image *linked from
sources*. The 56 functions spliced into that image in 008 therefore stopped
being evidence and became cartridge coverage, against a game-code image ten
times the size of the static area the project had been measuring.

## Things worth knowing

- **Nearly repeated the 004 vacuous gate.** The first version of the
  builder check piped `make test` through `tail` and read `$?` — tail's
  status, always 0. Caught by reading `verify` before the first run; exit
  codes are now captured before any pipe and success also requires the
  positive `ROM matches!` line.
- **The drill has to fail for the right reason.** A broken build also
  "fails". The drill now requires `ROM does NOT match`; the first run's
  summary hid the reason and was re-run to show it.
- Only the deflate half of zlib 1.0.4 is vendored; the inflate side, where
  most of old zlib's CVE history lives, is not.

## Honest limits

- Game-code coverage is 0.64% of the image. The path is complete; the volume
  depends entirely on matching more functions.
- The blob is produced on the Pi because the layout map comes from the Pi's
  database, so the builder cannot rebuild the ROM from a bare checkout.
  Committing the layout map would remove that dependency.
- Blob length is fixed at 326,180 bytes. That holds as long as the image is
  byte-identical, which every splice guarantees; a non-matching change to the
  game code would need the ROM layout to move, which is out of scope.
