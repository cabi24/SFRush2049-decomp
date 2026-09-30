# entity_lookup (entity handle table, message records, camera slot)

**4 of 8 members MATCH** (round 5, hand-written; `claims` lists only these):
`func_80091BA8`, `func_800BF01C`, `scheduler_recv`, `results_time_display`.

| function | words | result |
|---|---|---|
| `func_80091BA8(h)` | 21 | **MATCH** (`if (D[h & mask].id != h) return 0; return &D[h & mask];`, array form twice) |
| `func_800BF01C(c)` | 2 | **MATCH** (empty; the dead `if (0) switch` keeps it from being inlined) |
| `scheduler_recv(h)` (`keep`) | 40 | **MATCH** (`Msg *m = 0;` declared before `Entity *e;`: the spill slot is the highest local) |
| `results_time_display(h)` (`keep`) | 41 | **MATCH** (`if (e) { if 1||3 r=1; else if (state != 2) r=0; else r=check } else r=0`) |
| `func_80091B00()` | 42 | 11 words: store order only |
| `results_screen_update`, `leaderboard_update`, `camera_clip_planes` (`keep`) | 32/32/55 | blocked on `func_800BF01C` (below) |

## Blockers

- `func_800BF01C` (`jr ra; nop`) is called with the argument in `a0` (`jal; lw a0,64(v1)`) and the callers
  keep `a3`/`a1` alive across it. Any non-inlinable empty stub I wrote (dead switch, dead recursion) makes
  IPA pass the argument through `sw t6,0(sp)` instead, and an inlined stub deletes the call. When the
  parameter is used live the argument does go in `a0`. Same problem as `func_80096288` in
  `audio_frame_sync/STATUS.md`. Until that is solved the three callers cannot match.
- `func_80091B00` (first free `Msg`, 4x unrolled by `for (i = 0; i < 128; i++)` on `D_80142DD8[128]`):
  all words match except that retail places `li -1` after the `sb` of `used`, mine before it, in all four
  unrolled copies. Source order, constant forms (`0xFFFF`, `~0`, pointer local) and field types did not change it.
- Types: `Msg` is 0x18 bytes (`s16 id; s8 type; s8 used; Entity *ent`), `Entity` is 0x44 bytes
  (`id` at 0xC, `state` at 0x10, byte counter at 0x1A, `link` at 0x3C, camera slot pointer at 0x40).
  `D_80110244` is the entity table, `D_80146104` the index mask, `D_80142728`/`D_801427A8` message queues.

Note (round 5): claims trimmed to members not already spliced or in cloud/matches/ (func_800BF01C and results_time_display are already in src/blob/; func_800BF01C cannot be spliced with a stub-dependent caller set).
