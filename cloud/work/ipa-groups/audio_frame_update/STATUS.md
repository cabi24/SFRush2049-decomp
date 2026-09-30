# audio_frame_update

**1 of 3 members MATCH** (`func_800B0A88`, 112 of 361 words), rescored
2026-09-30 (`zbuild.py --as1=-r4300_mul`). Not spliced.

| function | target words | result |
|---|---|---|
| `func_800B0A88` | 112 | **MATCH** |
| `func_800B08FC` | 99 | 4 differ (emits 99) |
| `audio_frame_update` (`keep`) | 150 | 29 differ (emits 152) |

**Closure: complete.** Callers of both callees are `audio_frame_update`, which
is only called from `music_control`.

## Structure

Car record `CarS` (`player_array[p]`, 0x3B8 bytes) with a `snd[]` array of
0x18-byte `SndSlot` at `+0x110`: `0` master slot, `6..9` channel slots,
`16+i` (`func_800B0A88`) and `18+i` (`func_800B08FC`) callee-owned slots.
IPA parameters are `s16 (slot, idx)` (home slots `sw t0,40(sp); sw t3,44(sp)`,
both `sll/sra` at entry); callee registers agree with the ROM (`t0,t3`, `s1,s2`).

## Techniques that worked

- Seed fixes: `slot->handle = save_slot_valid(...)`; callbacks are function
  addresses (`entity_anim_texture` 0x80091874, `buffer_swap` 0x800924F4,
  `&D_8008BEA4`, `anim_state_update`); `save_slot_valid` takes ints;
  `D_801427C0` (`u16`) and `D_80111299` (`s8`) are arrays, `D_80156994` is `s8`;
  `state_word_a & 8` is a local tested twice (the redundant test is real).
- **Element pointer through a `(u32)` cast of a byte base.** The ROM computes
  `s0 = &player_array[slot] + 0x290/0x2C0 + idx*24` once and uses small
  offsets from it. Spell every access as
  `(&((SndSlot *) ((u32) ((u8 *) &player_array[slot] + 0x2C0)))[idx])->field`
  (`0x290` and `snd[16+idx]` for `func_800B0A88`), including the early-exit
  store. Without the cast offsets fold into the stores.
- `func_800B0A88`: `h->b8` is a named `s32 b8` loaded once after the early
  exit; locals declared `h, vec, k, b8` (every order with `h` before `vec` matches).
- `func_800B08FC`: `hd = E->handle;` as a named local before the model-index math.
- `(u32) slot * 0x18` for the `D_8013FEF4` lookup: a signed multiply shares the
  loop's hoisted `li 24` and becomes `multu`; the unsigned form gives the ROM's
  `sll/subu/sll` chain (94 -> 29 differing words).

## Remaining blockers

- `func_800B08FC` (4 words): the temp holding `&D_8012E708 + hd*0x44` is `$t9`
  here, `$v0` in the ROM. Tried `(&D_8012E708)[hd * 17]`, `*(f32 **)(...)[10]`,
  `s32 mp`/`u8 *m` locals, casts of `hd`, declaration orders.
- `audio_frame_update` (29 words): (1) master-slot stores go through
  `addiu v0,a2,272` in the ROM but fold into `a2` offsets here (`(u32)`
  element expression, `SndSlot *m`, `&car->snd[0]` did not help); (2) the ROM
  loads `D_801543CC` once into `$f0` before the loop, ours reloads each
  iteration via a hoisted address in `$a1` (static/defined/const/float local
  had no effect), which also shifts the loop counter registers.
