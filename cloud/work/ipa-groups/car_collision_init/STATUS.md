# car_collision_init

**2 of 2 members MATCH** (137 words), rescored 2026-09-30 with
`zbuild.py --as1=-r4300_mul`. **Superseded by [../zlib_deflate](../zlib_deflate/STATUS.md)**,
which matches both members together with the rest of the zlib unit and is
already spliced (`src/blob/groups/zlib_deflate`). Do not splice from this
directory; it is redundant and can be deleted.

| function | zlib function | target words | result |
|---|---|---|---|
| `car_collision_init` (0x800A8D9C, `keep`) | `_tr_stored_block` (`copy_block` inlined) | 103 | **MATCH** (zbuild prints 104/103: pad nop) |
| `func_800A8174` | `bi_windup` (out of line via stand-in) | 34 | **MATCH** |

## Identification

Not game logic: zlib's deflate bit writer from `trees.c`. `group.c` is zlib
1.0.4 (`tools/zlib-1.0.4/trees.c`, `send_bits`/`put_byte`/`put_short` copied
verbatim). Provenance tier: library source. Both functions use the standard ABI
(the seed's `M2C_ERROR(unset $t0/$a1)` was m2c losing `move t0,a0`/`move t1,a1`).

The game's `deflate_state` matches 1.0.4 without `DEBUG` at the tail
(`compressed_len` 0x1694, `matches` 0x1698, `last_eob_len` 0x169C, `bi_buf`
0x16A0, `bi_valid` 0x16A4) but the head is 4 bytes shorter (`pending_buf` at 0x4,
`pending` at 0xC; 1.0.4 has `status` at 0x4).

Relocations: `car_collision_init` has one `jal` to `func_800A8174`; no data.
