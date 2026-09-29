# cpak_init (tyre marks, not Controller Pak)

**BUILDS** (cloud pass 2, 2026-09-29), hand-written from the assembly; not
matching yet. Scores with the game's `-r4300_mul` assembler flag
([../../R4300_MUL.md](../../R4300_MUL.md)), `cloud/work/tools/zbuild.py`:

```
cpak_init       245/265   (size 259/265)       was 244/265, size 254
func_800AF8C0    33/113   (size 112/113)       was  39/113
save_validate   128/135   (size 131/135)       was 129/135
```

## Changes in pass 2

- `save_validate`'s signature is now `(WheelSlot *slot, s16 player, s32 wheel,
  u8 *color)` (the same order as `func_800AF8C0`): the ROM stores `player` at
  its home slot `sp+44` (second parameter), and the IPA registers then follow
  (`player $s3, wheel $s4, color $s5, slot $s6`).
- `save_validate`'s free-list scan is `for (; p->next != 0; p = p->next)`: the
  pool list ends in a sentinel and the last node is never a candidate (the
  ROM tests `p->next` before the first iteration).
- `func_800AF8C0` frame: **each referenced named scalar local costs a stack
  slot**, and locals are laid out in declaration order (first declared =
  highest address). Removing `a`/`b` (`wheel | 1`, `wheel & 2` inline), the
  `lift` local (reuse `p`) and the `car` local (`player_array[player]`
  inline) gives the ROM's 104-byte frame, and declaring the scalars before
  `axle`/`across` puts the arrays at `sp+64/76`. Only relocation-independent
  difference left: the ROM materialises `D_8002EB90` with `lui/addiu` and
  loads through the register (`lwc1 $f8,0(t7)`); ours folds the low half into
  the load. `[0]`, `*(f32 *)&`, `*ptr`, a pointer local: no change.

## What this is

Per-wheel tyre marks: four `WheelSlot`s (0x5C bytes) per player at
`D_80155290`, each holding a pooled display object from `D_80155220`.

| label | role |
|---|---|
| `cpak_init(s16 player)` | per wheel: start, extend or end a mark depending on contact, terrain and surface flags |
| `func_800AF8C0(slot, player, wheel, color)` | place the mark's leading edge across the axle (wheels `wheel\|1` and `wheel&2`), push it to the display list |
| `save_validate(player, wheel, color, slot)` | take an object from the pool, stealing the one farthest from the camera (`D_80150B94`) if empty; start the mark |

IPA registers in the ROM: `func_800AF8C0` takes `slot` in `$s0`, `player`
in `$t1` (an `s16` stored to the second parameter's home slot, so
`player` is the second parameter), `wheel` in `$s4`, `color` in `$s5`;
`save_validate` takes `player` in `$s3`, `wheel` in `$s4`, `color` in
`$s5`, `slot` in `$s6` and passes them on.

## Known remaining differences

- `func_800AF8C0`: the `D_8002EB90` materialisation above (33 differing words
  are that plus register naming after it).
- `save_validate` (`s0=obj, s1=far, s2=pool` in the ROM, `s7=obj, s1=pool`
  here): the ROM does not merge the two definitions of `obj` (the first call's
  result and the retry after stealing) into one web, so `obj` shares `$s0`
  with the list cursor; ours keeps one web alive across the scan.
- `cpak_init` (259 vs 265 words): a large IPA caller; the ROM's frame is 216
  bytes, ours 208, and it spills a different set of live values around the
  `save_validate`/`func_800AF8C0` calls. Not tried further.

## Relocations (for review; resolved by the strict scorer)

`player_array`, `D_8014A250`, `D_80155290`, `active_player_count`,
`state_word_a`, `D_8011743C`, `D_80117438`, `D_8011AD8C`, `D_8011AD90`,
`D_801497F8`, `D_80123C08`, `D_80123C0C`, `D_8002EB90`, `D_80155220`,
`D_80150B94`, `D_80161378`; calls `func_8008E3C0`, `func_800AFA84`,
`func_8008D0C0`, `func_800A78BC`, `func_8008C074`, `vector_copy_scale`,
`func_800AF844`.
