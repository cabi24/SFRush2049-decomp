# zlib_deflate

**Spliced into the ROM** (2026-09-29, `src/blob/groups/zlib_deflate`): image gate
and full ROM SHA-1 exact. `blob_group` places the unit's own `.data`
(`static_init_done`) at the address the relocation sites agree on
(0x8011EA10) and checks its bytes against the image.

**18/18 members MATCH**, 2,908 words, rescored 2026-09-30 with
`zbuild.py --as1=-r4300_mul` (and the strict cloud scorer). `_tr_init`
(`func_800AAB3C`) matches with 4 section-relative relocations unverified (see
below), so `score.py group` needs `--allow-unverified`. Context
`car_angular_velocity_clamp` (the game's driver, 201 words) is 194 differ, emits 206.

Replaces the generated groups `car_cg_height_set`, `car_collision_update`
and `car_crash_response`, and the three ungrouped zlib functions. The
`car_collision_init` group's two members are included here too.

## Source

`group.c` is **generated**: `python3 gen/gen.py` splices exact line ranges
from `tools/zlib-1.0.4/trees.c` and `deflate.c` and applies the edits in
`gen/edits.py`, each of which must match exactly once. Hand-written parts
are only `gen/header.h` (types, the game's struct layout, name map),
`gen/tables.h` (table addresses) and `gen/driver.c` (the game's own
driver). Edit those, not `group.c`. Provenance: **library source** (public
zlib, Gailly/Adler licence in `tools/zlib-1.0.4/README`), not m2c or arcade.

| member | zlib function |
|---|---|
| car_cg_height_set | flush_pending |
| func_800A8174 | bi_windup |
| func_800A81FC | init_block |
| func_800A8284 | compress_block |
| func_800A8778 | send_tree |
| car_collision_init | _tr_stored_block (copy_block inlined) |
| func_800A8F38 | bi_reverse |
| car_collision_update | gen_codes |
| func_800A9054 | gen_bitlen |
| func_800A928C | pqdownheap |
| car_crash_detect | build_tree |
| func_800A9710 | scan_tree |
| car_crash_response | _tr_flush_block (build_bl_tree, send_all_trees inlined) |
| func_800AA028 | _tr_tally |
| func_800AA224 | longest_match |
| car_reset_position | fill_window (read_buf inlined) |
| car_spawn_at_checkpoint | deflate_slow |
| func_800AAB3C | _tr_init (tr_static_init inlined) |
| car_angular_velocity_clamp (context) | not zlib: the game's one-shot compressor |

## What the game changed in zlib 1.0.x

All confirmed by the matches; all in `gen/header.h` / `gen/edits.py`:

1. **Struct layout.** `z_stream` has no `msg` and nothing after `state`;
   `deflate_state` drops `status`, two of `noheader`/`data_type`+`method`/
   `last_flush` (one int remains at 0x10, unused here), and `strategy`.
2. **`IPos` is `unsigned short`** (1.0.4: `unsigned`). Shows up in
   `longest_match` as 16-bit masking of `cur_match` and the distance limit.
3. **No `strategy` tests** in `deflate_slow`; **no `set_data_type`** in
   `_tr_flush_block`; **no `adler32`** in `read_buf`.
4. **`deflate_slow(s)` has no `flush` parameter:** the `Z_NO_FLUSH` early
   return is gone, the final `FLUSH_BLOCK` uses `eof = 1`, and it returns
   `block_done` (1).
5. **`configuration_table` entries have no `func` pointer** (8-byte
   entries); the driver applies them before `deflateReset`, so `lm_init`
   does not.
6. **`tr_static_init` has 8 more bytes of stack** than 1.0.4. The cause is
   unknown; `int pad[2]` reproduces it. **FAKE**, flagged in the source.

Everything else is zlib 1.0.4 as vendored (trees.c is identical in 1.0.4
and 1.0.5; 1.0.2's deflate_slow returns plain ints, which the game does not
use).

## Build model (what made it match)

- zlib's `local` functions are written as plain globals (`#define local`),
  which `uld -kp` then internalizes. As `static`, IDO -O3 inlined almost
  all of them (the game keeps them out of line).
- **Keep list = zlib's global API plus the driver**: `_tr_init`,
  `_tr_stored_block`, `_tr_flush_block`, `_tr_tally`, `deflate_slow`,
  `car_angular_velocity_clamp`. Those keep the standard ABI, exactly as in
  the ROM; the internal ones get IPA registers (`flush_pending` takes
  `strm` in `$s0`, `longest_match` takes `s` in `$a1`, `fill_window` `s`
  in `$s3`) and IDO reproduced every one of them.
- No stand-in callers were needed.

## Tables

Declared `extern` at their game addresses (`gen/tables.h`), found by
matching relocation sites to target words. The `.data` ones sit in exactly
zlib's source order: `extra_lbits` 0x8011E888, `extra_dbits` 0x8011E8FC,
`extra_blbits` 0x8011E974, `bl_order` 0x8011E9C0, `static_l_desc`
0x8011E9D4, `static_d_desc` 0x8011E9E8, `static_bl_desc` 0x8011E9FC,
then `static_init_done` 0x8011EA10; `configuration_table` 0x8011E838.
`.bss`: `static_ltree` 0x801249F0, `static_dtree` 0x80124E70,
`dist_code` 0x80152260, `length_code` 0x80152468, `base_length`
0x80152578, `base_dist` 0x80152600.

## The one unverified relocation

`static_init_done` must be a **function-local `static`** in
`tr_static_init`, as in zlib: declared `extern` (at any scope), IDO keeps
its address in a register and the code no longer matches. A local static
lives in this unit's `.data`, so the cloud scorer reports its 4
relocations as section-relative/unverified, and a splice would need the
object's `.data` placed at 0x8011EA10 (the "relocations in `.data`" item
S4 left open). The words themselves match.

## The driver (context)

`car_angular_velocity_clamp(next_in, avail_in, next_out, avail_out, level,
windowBits, memLevel)` sets up a `z_stream` on its stack, runs
`deflateInit2_`/`deflateReset`/`lm_init` inline, calls `deflate_slow`
once, frees the five buffers under a message-queue lock
(`osRecvMesg(&D_80152770)` / `audio_reverb_update(?, ptr, 0)` /
`osJamMesg`), and returns `total_out`. `audio_dma_sync(0, size)` is the
allocator. `gen/driver.c` is hand-written from the assembly.

Not matched yet (194/201 words differ, emits 206). `gen/driver.c` mirrors zlib: an `__inline`
`deflate_init` (`deflateInit2_` minus checks), `deflateReset`/`lm_init`
written out, and `deflateEnd` as five `TRY_FREE`s through an `__inline`
free helper. Findings so far:

- The game **inlines a free helper at all five sites**: each freed pointer
  is parked in its own stack slot (160, 136, 112, 88), and `$s0`/`$s1` are
  saved but never used. IDO only inlines a helper at five sites here when
  it is marked `__inline`; left to its heuristic it inlines one site, not
  five.
- **The free function's first argument is never set** in the ROM (no load,
  no move into `$a0`). An uninitialized local in the inlined helper is still
  loaded from its stack home by IDO, `register` or not. Unresolved.
- Remaining shape differences: the game's frame is 200 bytes (`z_stream` at
  `sp+172`, 24-byte spacing between the free slots), ours 136; the game
  keeps `s` in `$s2`, ours in `$s0`.

Being context, it does not affect the members: every member it calls is in
the keep list and has the standard ABI.

## Relocations (all resolved by the strict scorer, except as noted)

Calls: `memcpy`, `memset` (driver), `audio_dma_sync`,
`audio_reverb_update`, `osRecvMesg`, `osJamMesg` (driver), and calls
between members. Data: the tables above, `D_80152770` (driver).
`static_init_done`: unverified, see above.
