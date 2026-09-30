# Library hunt: third-party / SDK code in the game image

Round 3, 2026-09-30. Goal: find code in the inflated game image (0x80086A50..0x801249F0;
code ends at 0x8010FD60, data after) that comes from a known library, the way zlib
1.0.4 deflate was found. Method: (1) constant/table scan of the inflated image
(`libhunt/scan_constants.py`, `fscan`-style float scan), (2) per-function features
(`libhunt/func_features.py`: global refs, callees, byte ops, div/mult, jalr,
recursion) to find pure-computation functions, (3) reading the interesting ones.
Every match below was scored with `python3 tools/cloud/score.py fn ... --flags
"-g0 -O2 -mips2 -G 0 -non_shared"` (score.py now adds `-Wab,-r4300_mul` itself).

## Result in one paragraph

There is **no second big third-party library** in the game image. The only large
imported block is the zlib 1.0.4 deflate cluster (already matched). What exists
is a small **libc string/memory block** at 0x800A464C..0x800A48B8 (strstr, strncat,
strcpy, strcat, memmove) with strcpy/strncmp/strcmp/strncpy copies elsewhere, an
ANSI `rand()` (no callers: inlined at 34 sites), and a Midway/Atari-style **support
layer** (generic list container, heap/pool region lookup, 0xFF-tagged wide-string
routines). Static libs (gzip-style inflate with `huft_build`, LZSS, libm
`modf/fcvt/sinf/cosf`, libgcc 64-bit ops, libultra incl. PFS) are in the static
region, not in the game image. Nothing in the game image looks like libultra AL
(audio) code, libpng/jpeg/MPEG, or any hash/CRC.

## Negative results (so nobody repeats them)

- Constants absent from the whole inflated image (words and lui/ori pairs): CRC32
  (0xEDB88320, 0x77073096, 0x04C11DB7), MD5 (0xD76AA478), SHA-1 (0x67452301,
  0x5A827999), SHA-256 (0x428A2F98), Mersenne Twister (0x9908B0DF), FNV, golden
  ratio 0x9E3779B9, fast inverse sqrt 0x5F3759DF, Adler base 65521, Numerical Recipes
  LCS (1664525), Borland/`69069` LCGs, drand48. Only the ANSI LCG 1103515245 =
  0x41C64E6D is present.
- Float tables/constants: no sin/cos table, no `atan` minimax coefficients, no
  pi/180-based libm code beyond the game's own use (game calls static `sinf`/`cosf`).
  Float tables in the data section (0x801108C8..0x80124648) are game tuning
  (suspension, camera), plus one polynomial-looking row at 0x80123AC0 (1.5708, -0.6967,
  10.15, -39.69, 57.21, -27.37) that is game-side, not libm.
- Strings: no library identification strings (no copyright, version, "zlib", "adpcm"
  etc.). Only game format strings (`%d:%02d.%03d`, `CONTROLLER %d %s`, ...).
- Calls from the game image into static code use only libc/libm/libultra names
  already known (osJamMesg/RecvMesg, memcpy/memset, sinf/cosf, guLookAtF/guOrtho/
  guPerspective in `func_8009F058`, PFS and cont functions, sprintf, `inflate_decompress`,
  `lzss_decompress`). No AL* symbols, so no alSyn/alSeq copy in the game image.
- 113 functions of 20+ words have no data refs in the 0x8000-0x801F window (382 of 6+ words); I read the leaf ones
  by shape (list, heap, libc, wide-string, small vector helpers). Not every one was read: `func_800A1910`
  (75w, 24 byte ops), `func_800A1DD4`, `func_800B66B0`, `func_800ADCE0`, `func_800CDC3C` are unexamined.

## Candidates, ranked

Rank = chance a real, byte-exact match can be produced soon. "Done" = MATCH
reproduced by me in `cloud/work/bigfish/libhunt/libc_and_heap_matches.c` (one function scored per
run; not spliced, and NOT present in cloud/matches or src/blob).

| # | address | name | words | evidence | library / source | status |
|---|---|---|---:|---|---|---|
| 1 | 0x8008B2B4 | func_8008B2B4 | 12 | `seed*1103515245+12345, >>16 & 0x7FFF`, seed at 0x8011735C; no callers (inlined at 34 sites: 0x41C64E6D lui/ori across the image) | ANSI C `rand()` (K&R/ISO example implementation), seed must be `int` (signed: `sra`) | **MATCH** |
| 2 | 0x800950AC | func_800950AC | 29 | byte compare loop, n==0 -> 0, returns `*s1-*s2` | libc `strncmp` (BSD/SGI style) | **MATCH** |
| 3 | 0x80095F8C | func_80095F8C | 19 | walk list at 0x801527C8, `addr>=r[2] && addr<r[4]`, keep last hit | game "heap region lookup" (Midway mem module) | **MATCH** |
| 4 | 0x80095EF4 | func_80095EF4 | 38 | pool list at heap+0x18 (count/base/next), block list at heap+8 with tag byte at +0x15 | same heap module | **MATCH** (needs `p == 0 || p->base == 0` inside a `while(1)`; the `for` form folds `&heap->pool != 0` into `heap != -24`) |
| 5 | 0x800A44E8 | func_800A44E8 | 8 | `sb a1,0; sb a2,1; sw zero,4/12/8`; never called (probably inlined or unreachable) | list container `init(list, indirect, doubly)` | **MATCH** |
| 6 | 0x800A464C | func_800A464C | 32 | needle==empty returns s1; nested compare loop | libc `strstr` | already MATCH in cloud/matches (Lane B); the `ss.c` variant here is 14/32 off and not needed |
| 7 | 0x800A473C / 0x800A4770 | func_800A473C, func_800A4770 | 13 / 20 | strcpy, strcat | libc | already in cloud/matches |
| 8 | 0x8008AD04 | func_8008AD04 | 17 | `strcmp` | libc | already in cloud/matches |
| 9 | 0x800A46CC | func_800A46CC | 28 | strcat + bounded copy, `if(n==0)*d=0` | libc `strncat` (SGI variant: writes NUL at the n-th byte) | 10/28 words differ with `while (n-- != 0) { if ((*s1++=*s2++)==0) break; if (n==0) *s1=0; }` after a `while(*s1) s1++` scan; the target is loop-rotated twice (28 words vs 22 emitted), so it is a different loop form; `nearmiss/func_800A46CC_strncat.c` |
| 10 | 0x800A47C0 | func_800A47C0 | 63 | forward/backward word-aligned copy (`andi 3` checks, `sltiu n,4`), returns dst | libc `memmove` (SGI/GNU-style) | 15/63 words match; right structure (both branches, `a1=n` loop counter), 48 differ on register/count variable; `nearmiss/func_800A47C0_memmove.c`; callers: car_damage_visual, func_800CB748, func_800CB9D0, func_800CC50C |
| 11 | 0x80092DCC | func_80092DCC | 24 | strncpy with NUL padding (`sltiu/xori` boolean loop) | libc `strncpy` | 12/24 with `-Wo,-loopunroll,1`, otherwise IDO unrolls the pad loop by 4 (target is not unrolled): try -O1/-O2 unroll flags and a bool-var form; `nearmiss/func_80092DCC_strncpy.c` |
| 12 | 0x8009211C | func_8009211C | 87 | list remove; flag0=indirect (items are handles), flag1=doubly linked; count at +4, head +8, tail +0xC | Midway/Atari list container (source not identified; likely in the arcade tree) | 17/87 words differ (all t-register renumbering: target wastes two temporaries earlier), `nearmiss/func_8009211C_list_remove.c`. Head/prologue/loops all match now |
| 13 | 0x80091FBC | func_80091FBC | 88 | list insert-after (a0 list, a1 new, a2 prev), same layout | same list container | not attempted; same struct as #12, expect the same style of solution |
| 14 | 0x800BE744, 0x800BE4F0, 0x800BE6A4 | func_800BE744 (30w), func_800BE4F0 (109w), func_800BE6A4 (40w) | 179 | strlen/strcpy/strcat/strcmp analogues on strings whose first byte 0xFF marks 2-byte characters (`li t3,255`) | game text layer (arcade-derived) | m2c seeds compile: opcode LCS 89% (BE4F0), 85% (BE6A4); not attempted by hand; BE4F0 is called 12x from `control_settings` |
| 15 | 0x8008B2E4 | func_8008B2E4 | 18 | `frand(scale)`: `(float)rand15 * scale / 32768.0f` | game utility around #1 | 3/18 words differ: registers are `t0/t1` in the target vs `t9/t0` (a leaf with IPA-style temporaries, i.e. needs an IPA group with its callers); no static callers found (all inlined) |
| 16 | 0x800A80D0..0x800AAE68 | 19 zlib functions | 2,908+ | trees.c/deflate.c | zlib 1.0.4 | already spliced (`zlib_deflate`); see ../ipa-groups/ZLIB.md |

Numbers 1-5 are the "quick wins": five new exact matches, all standalone `-O2`, no IPA.

## Where a real library could still hide

- **A third library layer sits between game logic and the OS**: the list/heap/pool
  functions above are called from `audio_*`, `entity_*`, `car_*`, `menu_*` names.
  These heavily used helpers (func_80091FBC 88w, func_8009211C 87w, func_80095EF4 38w,
  func_80095F8C 19w, `audio_helper` 57w, func_80096B00 23w) look like a shared
  Midway/Atari-style utility layer (my guess, unverified); if the maintainers have
  `reference/repos/rushtherock` search it for `llist`/`heap`/`pool` modules and use the
  real source as the seed (they are ABI, single-function, no IPA).
- **Check `func_80092278`** (3 callers, in the same list region) and the hierarchy
  of list callers (`audio_fade_control`, `entity_flag_check`, ...) once #12/#13 are done.
- The remaining libc candidates by shape (but not seen): `strchr/strrchr` (static has
  strchr), `atoi`, `toupper`. None found; `fcvt`/`sprintf`/`modf` live in the static region.

## Reproduce

```
python3 cloud/work/bigfish/libhunt/scan_constants.py          # constants, lui/ori pairs (needs the inflated image, see scripts)
python3 cloud/work/bigfish/libhunt/func_features.py OUT.pkl   # per-function feature table
python3 tools/cloud/score.py fn cloud/work/bigfish/libhunt/libc_and_heap_matches.c func_800950AC --flags "-g0 -O2 -mips2 -G 0 -non_shared"
```
The scripts write/read the inflated image at a scratch path (`img.bin` produced
by `libhunt/m2c_seed_game.py`); edit the paths at the top before reuse.
