# The zlib cluster (deflate side) in the game code

**Update: done.** Group [`zlib_deflate`](zlib_deflate/STATUS.md) matches all 18 zlib functions (2,908 words) from the zlib 1.0.4 source; the game's driver `car_angular_velocity_clamp` is context. The groups `car_cg_height_set`, `car_collision_update`, `car_crash_response` and `car_collision_init` are superseded by it. The layout notes below were confirmed, with two corrections found while matching: `IPos` is `unsigned short`, and `deflate_slow` has no `flush` parameter (see its STATUS.md).

Found 2026-09-29 by the cloud Lane A pass. Evidence: struct offsets and call
graph read from the target words; zlib 1.0.4 offsets measured by compiling
`offsetof` probes with IDO 5.3 against `tools/zlib-1.0.4/deflate.h`.

The game code contains zlib's **deflate** implementation (`trees.c` and
`deflate.c`), not just the boot-time inflater. The historical labels
(`car_*`, `func_800A*`) are wrong. Every function below was identified by
its call graph and struct accesses; `_tr_stored_block`/`bi_windup` are
confirmed by an exact match (group `car_collision_init`).

| address | label | zlib function | size | current group |
|---|---|---|---:|---|
| 0x800A80D0 | car_cg_height_set | `flush_pending` | 41 | car_cg_height_set |
| 0x800A8174 | func_800A8174 | `bi_windup` | 34 | car_collision_init (**MATCH**) |
| 0x800A81FC | func_800A81FC | `init_block` | 34 | car_collision_update |
| 0x800A8284 | func_800A8284 | `compress_block` | 317 | car_crash_response |
| 0x800A8778 | func_800A8778 | `send_tree` | 389 | car_crash_response |
| 0x800A8D9C | car_collision_init | `_tr_stored_block` (+ `copy_block` inlined) | 103 | car_collision_init (**MATCH**) |
| 0x800A8F38 | func_800A8F38 | `bi_reverse` | 11 | car_collision_update |
| 0x800A8F64 | car_collision_update | `gen_codes` | 60 | car_collision_update |
| 0x800A9054 | func_800A9054 | `gen_bitlen` | 142 | none |
| 0x800A928C | func_800A928C | `pqdownheap` | 65 | car_collision_update |
| 0x800A9390 | car_crash_detect | `build_tree` | 224 | car_collision_update |
| 0x800A9710 | func_800A9710 | `scan_tree` | 172 | car_crash_response |
| 0x800A99C8 | car_crash_response | `_tr_flush_block` (+ `build_bl_tree`, `send_all_trees` inlined) | 408 | car_crash_response |
| 0x800AA028 | func_800AA028 | `_tr_tally` | 127 | none |
| 0x800AA224 | func_800AA224 | `longest_match` | 138 | none |
| 0x800AA454 | car_reset_position | `fill_window` (+ `read_buf` inlined) | 173 | car_cg_height_set |
| 0x800AA708 | car_spawn_at_checkpoint | `deflate_slow` | 267 | car_cg_height_set |
| 0x800AAB3C | func_800AAB3C | `_tr_init` (+ `tr_static_init` inlined) | 203 | car_collision_update |
| 0x800AAE68 | car_angular_velocity_clamp | custom driver: init + `deflate_slow` + finish, not a zlib function | 201 | none |

Not yet placed: `func_800A7E10` (176 words, called from
`differential_output`) may be `adler32` or unrelated.

## The game's zlib is a trimmed 1.0.4

Offsets (game vs zlib 1.0.4 without `DEBUG`):

- `z_stream`: `next_out` 0xC, `avail_out` 0x10, `total_out` 0x14, `state`
  **0x18** (1.0.4: 0x1C), so `msg` is gone.
- `deflate_state` head: `strm` 0x0, `pending_buf` 0x4, `pending_out` 0x8,
  `pending` 0xC, one unidentified int at 0x10, `w_size` 0x14. 1.0.4 has
  `status` plus `noheader`, `data_type`/`method`, `last_flush` there: three of
  those four are gone (-0xC).
- `w_size` .. `level` are 1.0.4's order shifted by -0xC (`window` 0x20,
  `window_size` 0x24, `head` 0x2C, `hash_size` 0x34, `block_start` 0x44,
  `match_length` 0x48, `strstart` 0x54, `match_start` 0x58, `lookahead`
  0x5C, `prev_length` 0x60, `max_chain_length` 0x64, `max_lazy_match` 0x68,
  `level` 0x6C).
- `strategy` is gone: `good_match` 0x70, `nice_match` 0x74 (read by
  `longest_match`), `dyn_ltree` 0x78. Everything from `dyn_ltree` to the end is
  1.0.4 shifted by -0x10 (`dyn_dtree` 0x96C, `l_buf` 0x167C, `last_lit`
  0x1684, `d_buf` 0x1688, `compressed_len` 0x1694, `matches` 0x1698,
  `last_eob_len` 0x169C, `bi_buf` 0x16A0, `bi_valid` 0x16A4).
- Code follows the fields: `deflate_slow` has no `strategy` tests, and
  `_tr_flush_block` does not call `set_data_type`. `TRUNCATE_BLOCK` code is
  present in `_tr_tally`.

## Why the current groups cannot match separately

This is one IDO -O3 unit. The IPA register assignments (for example
`flush_pending` taking `strm` in `$s0`, `longest_match` taking `s` in `$a1`,
`fill_window` taking `s` in `$s3`) depend on every caller and callee in the
cluster. The generator split it into three groups (`car_cg_height_set`,
`car_collision_update`, `car_crash_response`) plus three ungrouped functions,
so no single group sees the whole call graph.

## Proposal

One group, `zlib_deflate`, whose `group.c` is zlib 1.0.4 `trees.c` +
`deflate.c` with the trimmed structs and removed branches above. Members:
all functions in the table; `keep`: the driver `car_angular_velocity_clamp`
(or hand-written C for it as a member). Provenance tier: library source.
`car_collision_init` shows the approach works: its two functions match from
unmodified zlib code. This replaces three generated groups, so it is a
maintainer decision.
