# car_collision_init

**MATCH (cloud, provisional until FIX-PLAN F2):** both members.

```
$ python3 tools/cloud/score.py group cloud/work/ipa-groups/car_collision_init
car_collision_init:
  MATCH
func_800A8174:
  MATCH
```

## Identification

These two are not game logic. They are zlib's deflate bit writer from
`trees.c`:

| label | zlib function | notes |
|---|---|---|
| `car_collision_init` (0x800A8D9C) | `_tr_stored_block(s, buf, stored_len, eof)` | `copy_block()` is inlined into it under -O3 |
| `func_800A8174` (0x800A8174) | `bi_windup(s)` | kept out of line by `__standin_func_800A8174` |

`group.c` is the zlib 1.0.4 source (`tools/zlib-1.0.4/trees.c`, macros
`send_bits`/`put_byte`/`put_short` copied verbatim), not the m2c seed.
Provenance tier: **library source** (public zlib), not arcade or m2c.

The game's `deflate_state` matches 1.0.4 without `DEBUG` at the tail
(`compressed_len` 0x1694, `matches` 0x1698, `last_eob_len` 0x169C, `bi_buf`
0x16A0, `bi_valid` 0x16A4), but the head is 4 bytes shorter: `pending_buf` is
at 0x4 and `pending` at 0xC (1.0.4 has `status` at 0x4). Either an older zlib
or a trimmed copy. Only the fields these functions touch are declared.

Both functions use the standard ABI (`$a0`-`$a3`). The seed's
`M2C_ERROR(Read from unset register $t0/$a1)` was m2c losing track of the
`move t0,a0` / `move t1,a1` copies, not IPA registers.

## Relocations (for review until F2 lands)

- `car_collision_init`: one `jal` to `func_800A8174` (bi_windup). No data
  relocations.
- `func_800A8174`: none.

## Follow-up

The whole range around 0x800A7xxx-0x800A9xxx looks like zlib `trees.c` /
`deflate.c` (emitted in -O3 order, not file order). Groups
`car_cg_height_set`, `car_collision_update` and `car_crash_response` sit in
that range and are likely matchable from the same source.

The seed m2c output is kept at the previous commit of this file.

**Superseded by [../zlib_deflate](../zlib_deflate/STATUS.md)**, which matches every member of this group together with the rest of the zlib unit. Do not splice from this directory.
