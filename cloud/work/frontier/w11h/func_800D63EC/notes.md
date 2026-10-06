# func_800D63EC (324 B) - 70/81 words; allocation-strategy residual

Packet sender: if `D_8011025C`, lock `D_80142728`, unlink the head of free list `D_80146170` (func_8009211C),
fill type/4 vectors from the four Vec3 arguments, append to `D_80146188` (func_80091FBC), ready = 1, unlock,
return packet; else return -1. best.c = PR a150 context body (codex_record_messages_a150), unchanged.

Retail keeps every value in caller-saved registers with save/restore around each call (a0-a2 copied to t0-t2,
packet in a1 spilled at 28/24, result homed at 44, no s-registers). Ours gives packet s0 (frame 32).
Trace (`ctrace.sh func_800D63EC d1`): packet web w4 save 9 x nocs 2 = 18; caller-saved cost 6.2, callee-saved
cost 4.2 -> s0. Retail therefore had callee-saved cost > 6.2 or a packet web of much lower value.
Not a single-caller inline issue: with `--internal` umerge inlines it into func_800D6530 (retail calls it by jal),
so it is kept. Tried exit-structure and `result` placement variants (7): no movement. Next: find what raises the
per-procedure callee-saved cost (compare with a matched function that shows the same t0-t2 argument copies).
