# controller_poll (0x800C95DC, 232 words) -- not matched

`best.c` is the body in `groups/controller_poll/group.c`. It emits 234 words; 124 rows differ in an
instruction-aligned diff (the previous draft: 209 words, 212 rows). The official scorer prints
`225/232 words differ (2 extra words ...)` because two extra prologue words shift everything.

## Settled

- **It is a kept (ABI) function.** With a stand-in caller and not in `keep` it comes out 209 words; in `keep`
  the size and the whole skip path are right (232 words in that variant). `func_800C9590` stays internal with
  its real callers (`controller_poll` x2, `func_800E7134`).
- **The tick counter is volatile and lives in a block.** Retail keeps `0x8002E8E8` in `$a3` and reads the
  frame counter as `lw 636(a3)` at every use (twice in one block). `game_loop_tick` (0x8002EB64) as a plain
  extern gives a folded `lui/lw` and one cached copy. `((SysBlk *) &D_8002E8E8)->tick` with a `volatile s32`
  field at +636 gives the base register and the reloads. The neighbours `D_8002EB90/EB94` (the `f32` clock and
  frame time read with an unfolded `lui/addiu; lwc1 0(reg)` in `func_800AF8C0` and `camera_scene_manager`) are
  very likely `volatile` fields of the same block (+0x2A8, +0x2AC); the `(u32)` laundering used for them today
  is a stand-in for that.
- `x`/`y` locals for the two scaled axes (both stores come after the second call), `repeat = D_80123F94`
  loaded before the loop (`$f20`), `held` read back through `(u32 *)` right after the store to `D_80156978[i]`.
- The order of the ten `lui` in the loop preheader is the order of first use in the source: skip loop
  `gPrevPressed, gStick, D_80143A00, D_80156978, D_80156998`, then `raw, cal, gHeld, gConnected, D_8002AFB4`.

## Open (register allocation only)

Retail gives the sixteen available registers (`$s0-$s8`, `$a2,$a3,$t1-$t5`) to: the ten array pointers,
`&D_80156944`, `&D_80149784`, the block base, `i`, the constant 32 and `connected`. Ours also gives one to the
message-queue address (`$s0`, used by the three `osRecvMesg/osJamMesg` calls) and one to `&D_8015694C`, and
ends up with `connected` in `$ra`. Retail rematerialises the queue address at each call and `&D_8015694C`
inside the loop. Until the queue address stops being a CSE candidate the rest cannot settle.

Tried: `(u32)` laundering of the queue address at each call (still one register, plus a spill), a named
pointer local for the block (constant-propagated away), the laundered block pointer (base recomputed at every
use), `func_800C9590(raw, range, calib)` parameter order (callee still matches; argument load order unchanged).

Best next hypothesis: the lock/unlock calls are small wrappers that other code also uses (the plan notes
`MP_TargetSpeed` is a message-queue lock wrapper); if `controller_poll` calls kept wrappers the queue address
is not in this function at all -- but retail has direct `jal osRecvMesg/osJamMesg` here, so the wrappers
would have to be inlined, which leaves the address in place. More likely the queue is a member of a larger
object accessed through a pointer global in two of the three calls.
