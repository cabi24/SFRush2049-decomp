# highscore_entry_anim (suspicious: not a work item)

Investigated 2026-09-29; not forced.

**Finding: this is not a function start. It is the tail of an unregistered
function in the opaque run before it (a head defect).**

- No callers by `jal`, and no `lui/addiu` pair builds its address anywhere
  in the game code, so it is not a function-pointer target either.
- It has no prologue: the first instruction is `jal dispatch_handler`, it
  reads `24(sp)` and `$s3`/`$s8` that it never sets, and it uses
  `$s0` without saving it.
- In `asm/us/blob/blob_8010221c.s` it is immediately preceded by an opaque
  `.incbin` run of 10,300 bytes (0x8010221C-0x80104A58). The function that
  owns this code starts somewhere in that run.

The words just before 0x80104A58 are in `build/game_code.bin`, which the
cloud does not have, so the real start could not be located from here.
Suggest an extent repair on the maintainer side (scan back from 0x80104A58
for the `addiu sp,sp,-N` prologue that matches the stack slots used here).
