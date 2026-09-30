# cpak_init (tyre marks, not Controller Pak)

## Current status (Rescored 2026-09-30 (Round 2 addendum), master d0891f3.)

**Builds; no member matches (0/513 words).** Scored with `python3 cloud/work/tools/zbuild.py cloud/work/ipa-groups/cpak_init --as1=-r4300_mul`:

```
func_800AF8C0   13/113 words differ  size 113/113
save_validate  123/135 words differ  size 133/135
cpak_init      245/265 words differ  size 260/265
```

Blockers: register allocation and one scheduling slot in each member (see "Pass 3" at the end: `p` in `$f2` vs
`$f0` and an early `li 192`; `save_validate` loads `head->next` once not twice; `cpak_init`'s frame 216 vs 208).
Closure gap: not investigated. `closure.py` (approximate) lists `func_800EA3F4`, `func_800EB028`, `func_800EB90C`,
`vector_copy_scale`, `vector_normalize_length`; none is known to be the cause of the current differences. The
"Changes in pass 2" numbers below are history (superseded by the block above).

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

## Pass 3

```
func_800AF8C0   13/113 differ  size 113/113  (was 33/113, size 112)
save_validate  123/135 differ  size 133/135  (was 128/135, size 131)
cpak_init      245/265 differ  size 260/265  (unchanged in kind; follows save_validate)
```

- `func_800AF8C0`: `obj->f8 = *(f32 *) ((u32) &D_8002EB90[0]);` reproduces the ROM's `lui/addiu t7` + `lwc1 0(t7)`
  (it was the missing instruction; `[0]`, `*ptr` and a pointer local all fold the low half). A named
  `CarState *st` declared **after** the two vectors (`axle`, `across`) gives the ROM's register assignment
  (`a3 = wheel|1`, `t0 = wheel&2`, `a2 = st`) without moving the vectors. Left: `p` lands in `$f2` (ROM `$f0`)
  and the `li 192` is scheduled one slot earlier (13 words).
- `save_validate`: the scan cursor **is** `obj` (`far = obj = pool_head; while (obj->next) {...; obj = obj->next;}`);
  with that, `player/wheel/color/slot` get the ROM's `$s3/$s4/$s5/$s6` and the pool address `$s2`. The final
  `obj` after the display-list test is a second variable (`o2`, `$s7` in the ROM, via `move v0,zero/move v0,s0`).
  Left: the ROM loads `head->next` twice (guard and preload into `$a1`), ours once; `sra s3` sits in the `jal`
  delay slot in the ROM.
