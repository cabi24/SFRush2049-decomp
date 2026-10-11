# w18b — runtime-image stream round-trip and ROM compose (in progress)

## Stream reproduction (Pi, 2026-10-11)

Both runtime-image streams reproduce byte for byte from their inflated images with
vendored zlib 1.0.4 at the game blob's parameters (level 9, windowBits -15, memLevel 8,
default strategy). A sweep of 1,788 (A) / 1,794 (B) parameter sets (level 0-9,
windowBits -15..-8, memLevel 1-9, strategy 0-2) matched only that one set.

| image | pointer | ROM stream | stream bytes | ends at | followed by | image bytes |
|---|---|---|---|---|---|---|
| game | (blob_rom) | 0xB0CB10 | 326,180 | 0xB5C534 | image A's stream | 647,072 |
| A | 0x8002B020 | 0xB5C534 | 80,272 | 0xB6FEC4 | image B's stream | 194,128 |
| B | 0x8002B024 | 0xB6FEC4 | 23,639 | 0xB75B1B | zero fill to 0xC00000 (end of ROM) | 43,888 |

The three streams are contiguous; any length change would shift a neighbour, so each
slot is fixed-length (compose_data refuses other lengths).
