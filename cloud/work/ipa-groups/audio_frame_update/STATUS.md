# audio_frame_update

**BUILDS**; `func_800B0A88` is a strict MATCH (cloud pass 4), `func_800B08FC` is 4 words off. Scored with `-r4300_mul`:

| function | words | result |
|---|---|---|
| `func_800B08FC` | 99 | 4 differ (emits 99; was 24) |
| `func_800B0A88` | 112 | **MATCH** (was 57) |
| `audio_frame_update` | 150 | 29 differ (emits 152; was 94) |

There is no closure gap: the callers of both callees are `audio_frame_update`,
and `audio_frame_update` is only called from `music_control`.

## What was done

- **Bodies rewritten** with typed structures. The car record is
  `CarS` (`player_array[p]`, 0x3B8 bytes) whose `snd[]` array of 0x18-byte
  `SndSlot` starts at `+0x110`; index `0` is the master slot, `6..9` are the
  four channel slots, `16+i` and `18+i` are the two callee-owned slots
  (`func_800B0A88` uses `snd[16+idx]`, `func_800B08FC` uses `snd[18+idx]`).
- **Seed defects fixed.**
  - The seed stored `save_slot_valid`'s return value into nothing and wrote a
    pointer into `handle`; it is `slot->handle = save_slot_valid(...)`.
  - The callbacks are function addresses, not literals: `entity_anim_texture`
    (`0x80091874`), `buffer_swap` (`0x800924F4`), `&D_8008BEA4`,
    `anim_state_update`.
  - `save_slot_valid` takes ints (the ROM does `lw a2` with no `sll/sra`).
  - `D_801427C0` (`u16`) and `D_80111299` (`s8`) are arrays; `D_80156994` is `s8`.
  - IPA parameters are `s16` in order `(slot, idx)`: home slots `sw t0,40(sp);
    sw t3,44(sp)` show the order; both take an `sll/sra` at entry.
- The callee parameter registers now agree with the ROM (`t0,t3` and
  `s1,s2`), and `audio_frame_update` loads them as `move t0,...; move t3,zero`.
- `state_word_a & 8` is a local tested twice (`t == 0 || (t != 0 && ...)`),
  as in the ROM (the redundant second test is real).

## Cloud pass 4 (what worked)

- **Element pointer through a `(u32)` cast of a byte base.** The ROM computes
  `s0 = &player_array[slot] + 0x290/0x2C0 + idx*24` once and addresses fields at small
  offsets from it (`sw t9,20(s0)`, then `addiu s0,s0,704` before the call). It is
  reproduced by spelling every access as
  `(&((SndSlot *) ((u32) ((u8 *) &player_array[slot] + 0x2C0)))[idx])->field`
  (`0x290` and `snd[16+idx]` for `func_800B0A88`), including the early-exit store. The
  `(u32)` cast is what matters (without it the offsets are folded into the stores; with
  it only in the main path but not in the early exit, the register order still rotates).
  The `CarS`/`snd[]` typed accessors are gone from both functions.
- `func_800B0A88`: `h->b8` is a named `s32 b8` local loaded once after the early exit
  (the ROM loads it once into `$v0`), and the locals are declared `h, vec, k, b8`
  (declaration order moves `vec` from `sp+60` to `sp+56`: every permutation with `h`
  before `vec` matches, the others do not).
- `func_800B08FC`: `hd = E->handle;` as a named local before the model-index math (its
  `lh v1,6(s0)` then matches). Left: the temp that holds `&D_8012E708 + hd*0x44` is `$t9` in
  ours and `$v0` in the ROM (`lui v0,0x8013 ... lw v0,-6392(v0)`, 4 words). The forms
  `(&D_8012E708)[hd * 17]`, `*(f32 **)(...)[10]`, an `s32 mp`/`u8 *m` local for the
  pointer, casts of `hd`, and decl orders did not change it.

## Remaining blockers

- `func_800B08FC`: see above (4 words, one temp register).
- `audio_frame_update` (29 words): `slot*0x18` for the `D_8013FEF4` lookup is written
  `(u32) slot * 0x18` (a signed `slot * 0x18` shares the loop's hoisted `li 24` and
  becomes `multu`; the unsigned form expands to the ROM's `sll/subu/sll` chain; this
  took it from 94 to 29). Left: (1) the master-slot stores go through `addiu v0,a2,272`
  (`sw t8,20(v0); sh s3,8(v0)`) in the ROM and are folded into `a2` offsets here
  (the `(u32)`-cast element expression, a `SndSlot *m` local, `&car->snd[0]` did not
  reproduce it); (2) the ROM loads `D_801543CC` once into `$f0` before the loop, ours
  reloads it each iteration through a hoisted address in `$a1` (`static`, defined,
  `const`, and a float local did not change it), which also moves the loop counter
  registers (`a0`/`a1` vs `v1`/`a0`).
