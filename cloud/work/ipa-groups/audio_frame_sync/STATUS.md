# audio_frame_sync (resource loader thread, not audio)

**1 of 7 members MATCH** (`func_800972C4`, 22 of 674 member words), rescored
2026-09-30 (`zbuild.py --as1=-r4300_mul`; context via `score.compare`). Not spliced.

| function | role | target words | result |
|---|---|---|---|
| `func_800972C4` | member (leaf, callers only in group) | 22 | **MATCH** |
| `fp_call_wrapper` | member, `keep` | 26 | 2 differ |
| `brake_force_apply` | member (loader thread loop), `keep` | 49 | 14 differ, 6 extra words (layout padding, below) |
| `suspension_setup` | member, `keep` | 60 | 37 differ (emits 55) |
| `func_80097164` | member | 86 | 49 differ (emits 85) |
| `audio_frame_sync` | member, `keep` | 130 | 120 differ (emits 128) |
| `func_80096CA8` | member | 301 | 296 differ (emits 290) |
| `func_80097694` | context (locked leaf) | 65 | **MATCH** |
| `func_80096288` | context | 4 | 3 differ (emits 2) |
| `entity_lod_select` | context, `keep` | 179 | 179 differ (emits 177) |

## What this is

Loads a resource block and fixes up its internal pointers. `func_80096CA8`
(slot in `$fp`, `first` flag in `$t0`) finds the `OBHD`/`TXHD`/`PLHD`/`PTHD`/
`TXLD`/`IMAG` chunks with `lookup_with_output`, then adds the load base to
offsets in the object (0x58), texture (0x24) and palette (0x18) records.
`entity_lod_select` is a display-list walker: 10-entry pointer stack at `sp+164`,
opcodes `0xDB..0xE1` through a 7-entry jump table (`0x80123A70`), segment-relative
rebasing of the second word.

## Closure

No gap left (all hand-written): `entity_lod_select` (in `keep`: ROM saves
`s0-s4`), `func_80097694` (`resource_slot_clear` group's locked leaf, copied in;
its temporaries in `$t6-$t9` are why `suspension_setup`/`audio_frame_sync` keep
values in `$a3/$t0` instead of spilling), `func_800972C4`, `func_80096288`.

## Techniques that worked

- Typed records (`ResSlot`, `ObjRec`, `TexRec`, `PalRec`, `Tab`, `ParseCtx`).
  `func_80096C28` writes the base pointer 0x14 below the pointer it is handed,
  so `ParseCtx.ctx` sits at `+0x14`.
- `suspension_setup` second argument is stored to `ResSlot.type`;
  `D_8002EB70` is `volatile s16` (`lh`, reloaded in the wait loop).
- `entity_lod_select(u32 *dl, s32 base, s32 seg, s32 flag, u32 mask)`; `op`/`op2`
  are `u8`. Frame 232: scalars above `stk[10]`, 25 words of unused locals below
  (unused arrays reserve frame). Listing the no-op cases 219..221 makes IDO emit
  the jump table.
- `func_800972C4` needed the assignment inside the condition:
  `if ((D_80156D38[i].f3 = D_801234AC[D_80156D38[i].type]) != 0)`.
- `audio_frame_sync(kind, skip, async, flag, buf)`, `flag` `s32`, the `s8`
  conversion happens at the `func_80097694` call; `res` is read uninitialised
  when `skip != 0`, as in the ROM.

## Remaining blockers

1. **`func_80096CA8` IPA parameters.** ROM: `slot` in `$s8`, `first` in `$t0`
   (spilled around calls, home slot `116(sp)`), nine other values in `$s0-$s7`.
   IDO gives `first` the last saved register and leaves `slot` in memory.
   The ROM demotes `first` although it is tested in the inner loop; not
   reproduced (named locals, copies of `slot`, `ctx` pointer local). Callers
   `func_80097164` (ROM `$s8`, here `$s5`), `brake_force_apply`,
   `suspension_setup`, `fp_call_wrapper` inherit the wrong register.
2. **`func_80096288`** (`beqz a2,+8; nop; jr ra; nop`): IDO -O3 deletes every
   empty-`if` form tried, so it inlines to `jr ra` and `suspension_setup`
   loses the call. Something in the original (debug macro?) kept the branch.
3. `brake_force_apply` is layout dependent: when its dead epilogue would start
   on a 32-byte boundary IDO inserts 7 zero words (49 becomes 56). Depends on
   object emission order.
4. `entity_lod_select`: ROM hoists 7 constants (`0xFFFFFF`, `0xFF000000`,
   `0xFF0000`, `221`, `4`, `1`, `222` in `$ra`); IDO hoists 9 and keeps
   `op == 1`/`op == 4` outside the shared switch tail.
5. `audio_frame_sync`: ROM keeps slot in `$a3` and the leaf result in `$v0`,
   spilling around calls; IDO keeps both in stack slots.
