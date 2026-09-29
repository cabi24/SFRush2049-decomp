# audio_frame_update

**BUILDS**; no member is a strict MATCH yet. Scored with `-r4300_mul`:

| function | words | result |
|---|---|---|
| `func_800B08FC` | 99 | 24 differ (emits 99) |
| `func_800B0A88` | 112 | 57 differ (emits 112) |
| `audio_frame_update` | 150 | 94 differ (emits 152) |

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

## Remaining blockers

- `func_800B08FC`: the ROM's `s0` is the element base (`car + idx*0x18`) with
  stores at `+0x2D4/+0x2C8/+0x2CC`, then `addiu s0,s0,704` before the call.
  The typed-array form reproduces the offsets but computes the handle address
  after the call. Forms that do reproduce the `+704` (byte pointers) change
  the IPA parameter to `$t1`; the ROM's `$t3` needs `t1` and `t2` taken by the
  index and the hud pointer, and `v0/v1` by `car` and `idx*0x18`.
- `func_800B0A88`: ROM `s0` is the element pointer computed once
  (`&car->snd[16+idx]`); here IDO folds the offset into the stores.
- `audio_frame_update`: `slot*0x18` is a shift/subtract in the ROM but
  `li t0,24; multu` here (the loop's constant `24` gets hoisted and reused);
  and the two 8-halfword copies start from different registers.
