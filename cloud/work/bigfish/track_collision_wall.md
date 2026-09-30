# track_collision_wall (0x8009DD88) - bigfish scout

## Facts
- 678 words (blob_8009dd18.s), frame 280, saves ra, s0-s8, f20. Not spliced, not in cloud/matches.
- ABI with 7 arguments: a0-a3 plus three stack words (callers store `sw zero,16(sp)`, `sw t0,20(sp)`, `sw zero,24(sp)`); prologue stores a0/a1 to the caller home slots. Callers: `func_8009F058` (3 sites) and itself (recursive, 1 site). No non-ABI register read at entry: not IPA-dependent on the caller side. Callees: `func_8009D99C` x3, `track_collision_edge`, `func_8009D708`, `func_8009D45C`, `particle_system`, and itself; none inspected for IPA params (`func_8009D45C/D708/D99C` are close neighbours, likely one -O3 family with this function: verify with closure.py before trusting -O2).
- Name is a label: body emits RDP words (0xDE01.., 0xDC08.., 0x03000000/0x01000008 constants) so it is a wall/track display-list emitter, with a recursion.
- Globals: 43 distinct addresses (D_8011EF10/90, D_80124EF0.., D_8012E700, D_80150B70, D_8015B250/260, D_8015F740, D_80161430...). 298 loads/stores (most memory-dense of the five), 20 fp ops, 10 mul/div.
- Shape: recursive, irregular, macro-heavy. Not repetitive.
- No first-pass compile (no m2c for blob; not attempted).

## Feasibility: LOW
Recursion + 43 globals + 7-arg ABI + probable neighbour IPA. Effort 2-4 days.
