# zlib 1.0.4 — vendored, deflate side only, build-time oracle

**Why this exact version:** it is the encoder that produced the game-code
blob in the San Francisco Rush 2049 (US) cartridge. With
`deflateInit2(level=9, Z_DEFLATED, windowBits=-15, memLevel=8,
Z_DEFAULT_STRATEGY)` it reproduces the 326,180-byte raw DEFLATE stream at ROM
`0xB0CB10` byte for byte. Every later release (1.1.3 through modern) makes
different lazy-match choices and lands 140 bytes long. Evidence:
`specs/008-blob-image-rebuild/research/compressor-identified.md`.

**Source:** https://zlib.net/fossils/zlib-1.0.4.tar.gz
sha256 `e5c260cd3db1370fb3e0c193e9cbd9f127a9bd055d622b3fb55b82747f6e5b24`,
fetched 2026-09-24. Files copied unmodified.

**What is here:** only what deflate needs — `deflate.c/.h`, `trees.c`,
`zutil.c/.h`, `adler32.c`, `zlib.h`, `zconf.h` — plus the upstream README
(zlib licence). The inflate side is deliberately NOT vendored: we only ever
compress, and most of old zlib's CVE history lives in inflate.

**Security:** zlib 1.0.4 is a 1996 release with known vulnerabilities. It is
used strictly as an offline build tool that compresses this project's own
decompressed image. Never link it into anything that processes untrusted
input, and never install it system-wide.
