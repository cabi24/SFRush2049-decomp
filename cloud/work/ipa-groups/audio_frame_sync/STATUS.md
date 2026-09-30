# audio_frame_sync (resource loader thread, not audio)

**BUILDS**; `func_800972C4` is a strict MATCH (22 words); the rest are not yet.
Scored with `-r4300_mul` (`zbuild.py`; context functions with `score.compare`):

| function | role | words | result |
|---|---|---|---|
| `func_800972C4` | member (small leaf, only callers are in the group) | 22 | **MATCH** |
| `fp_call_wrapper` | member | 26 | 2 differ |
| `brake_force_apply` | member (the loader thread loop) | 49 | 2 differ, or 14 (+6 extra) when IDO pads (see below) |
| `suspension_setup` | member | 60 | 37 differ (emits 55) |
| `func_80097164` | member | 86 | 49 differ (emits 85) |
| `audio_frame_sync` | member | 130 | 120 differ (emits 128) |
| `func_80096CA8` | member | 301 | 296 differ (emits 290) |
| `func_80097694` | context (closure gap: the locked leaf) | 65 | **MATCH** |
| `func_80096288` | context (closure gap) | 4 | 3 differ |
| `entity_lod_select` | context (closure gap, listed in `unprototyped`) | 179 | 179 differ, structure close (emits 177) |

## What this is

Loads a resource block and fixes up its internal pointers. `func_80096CA8`
(slot in `$fp`, `first` flag in `$t0`) finds the `OBHD`/`TXHD`/`PLHD`/`PTHD`/
`TXLD`/`IMAG` chunks with `lookup_with_output`, then adds the load base to the
offsets in the object (0x58), texture (0x24) and palette (0x18) records.
`entity_lod_select` is a display-list walker: a 10-entry pointer stack at
`sp+164`, opcodes `0xDB..0xE1` through a 7-entry jump table
(`0x80123A70`), and segment-relative rebasing of the second word.

## What was done

- **Closure gaps closed.**
  - `entity_lod_select`: hand-written (`context`, and in `keep`: the ROM saves
    `s0-s4` in its prologue, so it is not IPA-internal).
  - `func_80097694`: the `resource_slot_clear` group's locked leaf, copied in.
    It matches here as well, and it is why `suspension_setup` and
    `audio_frame_sync` keep values in `$a3/$t0` across the calls (its
    temporaries stay in `$t6-$t9`). Without it those functions spill.
  - `func_800972C4` (matches, promoted to member: its only callers are here)
    and `func_80096288` (an empty function; see blockers).
- **`func_80096CA8` rewritten** with typed records (`ResSlot`, `ObjRec`,
  `TexRec`, `PalRec`, `Tab`, `ParseCtx`) instead of the m2c seed; the `unset $t0`
  reads are gone (`first` is a real parameter). `func_80096C28` writes the base
  pointer 0x14 bytes below the pointer it is handed (M2C showed it as a stray
  `sp50`), so `ParseCtx.ctx` sits at `+0x14`.
- **Seed defects fixed.** `suspension_setup` read unset `$t0` (it is the second
  argument, stored to `ResSlot.type`); `func_80096288` takes three arguments;
  `D_8002EB70` is `volatile s16` (the ROM reloads it in the wait loop, and
  `lh`, not `lhu`).
- `entity_lod_select` signature `(u32 *dl, s32 base, s32 seg, s32 flag, u32 mask)`;
  `op`/`op2` are `u8` (the ROM does `andi 0xff` then a signed `slti`). Frame
  232: scalars above `stk[10]`, 25 words of unused locals below it (an unused
  array reserves frame space). Listing the three no-op cases 219..221 makes IDO
  emit the jump table.
- `func_800972C4` matched only after writing the assignment inside the
  condition: `if ((D_80156D38[i].f3 = D_801234AC[D_80156D38[i].type]) != 0)`.

## Remaining blockers

1. **`func_80096CA8` IPA parameters.** ROM: `slot` in `$s8`, `first` in `$t0`
   (spilled around every call, home slot `116(sp)`), the other nine values in
   `$s0-$s7`. Here IDO gives `first` the last saved register and leaves `slot`
   in memory (`lw t6,192(sp)`), so the register the callers pass `slot` in is
   also wrong: `func_80097164` (ROM `$s8`, here `$s5`), `brake_force_apply`,
   `suspension_setup` and `fp_call_wrapper` all inherit it. IDO ranks live
   ranges by weighted use; the ROM demotes `first` although it is tested in the
   inner loop, which I could not reproduce (tried named locals vs direct
   `D_801161F4[slot]`, a copy of `slot`, a `ctx` pointer local). Toy tests in
   scratch show `first` wins whenever it has more weighted uses than `slot`.
2. **`func_80096288`** (`beqz a2,+8; nop; jr ra; nop`): IDO -O3 removes every
   form of an empty `if` I tried (`if (c) {}`, `goto`, `switch`, `do/while`),
   so the function compiles to `jr ra`, is inlined, and `suspension_setup` loses
   the call. Something the original had (a debug macro?) kept the branch.
3. `brake_force_apply` is layout dependent: when its dead epilogue would start
   on a 32-byte boundary IDO inserts 7 zero words before it (49 words become
   56). That happened while the unit contained an extra `keep`d function; with
   the current members it depends on the object order. Emission order (Lane C
   item) decides it.
4. `entity_lod_select`: the ROM hoists 7 constants (`0xFFFFFF`, `0xFF000000`,
   `0xFF0000`, `221`, `4`, `1`, `222` in `$ra`); IDO hoists 9, and it keeps
   `op == 1`/`op == 4` outside the switch tail where the ROM shares it.
5. `audio_frame_sync` (130) was rewritten typed (`(kind, skip, async, flag, buf)`;
   `flag` is an `s32`, the `s8` conversion happens at the `func_80097694` call).
   Structure now matches; the ROM keeps the slot in `$a3` and the leaf result
   in `$v0`, spilling around calls, while IDO here keeps both in stack slots
   (`res` is read uninitialised when `skip != 0`, as in the ROM).
