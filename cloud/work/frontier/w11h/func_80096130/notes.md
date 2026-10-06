# func_80096130 (264 B) - 21/66 words, structure and frame exact; one colouring tie + one as1 pick

Slot release: if `D_80156D38[index].allocation` is set, wait for `D_8002EB70` (volatile s16) to clear, free the
block through the heap-free helper `func_800960CC` (lock / `audio_reverb_update(addr, 0)` / unlock, defined for
real in `src/blob/groups/codex_heap_release_a25/wheel_params_set.c`, inlined by umerge in the unit), clear the
slot and the three 8-byte records `D_801161F4/D_80151AE8/D_80138670[index]`.

best.c = `c6.c`: the busy wait is an inlined static (`dma_wait`), which supplies the missing 8 frame bytes
(frame 64 -> 72; homes 56 (inlined `address`), 36, 32 now equal retail). The same wait loop appears in
suspension_setup, tire_compound_set, world_collision_response and game_loop, so a shared helper/macro is plausible;
retail has no caller-less stub next to this function, so a definitive static was not found.

Unit: `blob_unit --tag w11h score func_80096130 --with .../best.c --neighbours` ->
`FAIL func_80096130: 21 of 66 words differ` / `locked bodies that differ in this unit: 0`.

Residual (traced, `tools/trace/ctrace.sh` + `force.sh`):
1. Colouring: web 5 (`&D_80156D38[index]`, expression) and web 7 (`allocation`) tie at save 1.0; web 5 goes
   first (lower number), has totalsave 3.0 == bestcost 3.0 for v1 and **splits**; web 7 then takes v1. Retail
   has slot = v1, allocation = a3. Forcing `p1:w5=c2,p1:w7=c6` gives retail registers everywhere and leaves 9 rows.
2. Those 9 rows are one as1 placement: retail keeps `sw zero,12(v1)` (slot->allocation = 0) before
   `lw v0,72(sp)`; ours sinks it into the first memset's delay slot.
Tried ~20 source variants (allocation local [frame 80/72 wrong home 64], index-form access, laundering, helper
splits `slot_free`/`slot_clear`): none changes the w5 split. Next hypothesis: one more use of the slot address
(or one fewer call crossed) so web 5 colours instead of splitting; the as1 pick may then follow.
