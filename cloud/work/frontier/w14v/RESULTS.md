# Wave 14, lane w14v: func_800988D8 and engine_sound_sync

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w14v` (synced from the Pi). Nothing spliced, committed or pushed.
No match. Neither function is a candidate for `cloud/matches/`, and no group dir is claimed.

| Function | Bytes | State | Flags | Best |
|---|---:|---|---|---|
| `func_800988D8` | 380 | 91 of 95 words differ in the group context (real-caller stand-in is the unmatched owner-group `func_80098FB8`); 94 of 95 in the whole-program unit | `-g0 -O3 -mips2 -G 0 -non_shared` | `func_800988D8/best_g5.c` (= `t/g5/v0000.c`) |
| `engine_sound_sync` | 1236 | not matched; m2c fails (see below) | - | none |

## func_800988D8

Scorer lines (exact):
```
vbatch (group ctx, owner group.c with func_800988D8 replaced, keep func_80098FB8,func_80091FBC,func_8009211C):
strict  91  mnem-missing  23  aligned-missing  57  size +9  v0000.c
blob_unit --tag w14v score func_800988D8 --with func_800988D8/best_g5.c --neighbours:
  FAIL func_800988D8: 94 of 95 words differ; compiled body is 118 words, target 95
  locked bodies that differ in this unit: 0
  blob_unit score: 0/1 equal
diagnose one func_800988D8 --source best_g5.c --flags "-g0 -O3 -mips2 -G 0 -non_shared":
  verdict mixed(structural:93, register:29), frame_delta -32, lever drop-a-declared-local
```

### What the retail code says (from `tdis`)
- Frame is 24 bytes, saves only `ra`. s0, s3-s7 are used but not saved: the function is internal and the
  unit's real caller (`func_80098FB8` @0x80098FB8, `jal 0x988d8` at 0x80098fd8, unmatched) keeps them live.
- Body is a `while (s3)` over the list at `D_80149868`. The `b14 != 0` test and the `-1` float test share one
  `scheduler_recv(w56); entity_flag_check(s3)` block (`block_9` in m2c terms).
- Constants and addresses are hoisted into s4 (-1), s6 (`&D_80152748`), s7 (2); 14400.0f lives in f20.

### Blockers
1. Unsaved s-registers need the real caller in the unit. With no caller (`--internal`) the function is
   dead code (2 words). With a stand-in caller (`__standin_func_80098FB8` calling it twice, `--keep`) the
   prologue matches only when the caller is the owner-group `func_80098FB8` body (`ipa-groups/codex_effect_owner_b132/group.c`),
   which is itself a frozen nonmatch (B132). So any match here is provisional on that caller.
2. Frame: ours is 32 bytes larger than retail (diagnose `frame_delta -32`). Nine named locals are declared.
   The lever `drop-a-declared-local` was not yet tried; the t5 round tried inline expressions and it was worse (94).
3. Block layout: retail branches `bnez t6` to the shared block_9 placed after the loop body. A plain goto
   form (shared `block_9:`/`next_9:` labels) scored 103, worse.

### Rounds (vbatch, all scored)
| Round | Template | Variants | Best strict | Notes |
|---|---|---:|---:|---|
| 1 | standalone, 9 choice points (`t/b1`, `t/t2.c`) | 300 | 97 | standalone compile cannot reproduce unsaved s-regs |
| 2 | standalone, 13 choice points (`t/b2`) | 300 | 94 | **contaminated**: nested choice point (`m1`/`km1`) produced `if (k_m1)`; discard |
| 3 | owner group ctx, `t/g3` | 300 | 85 | **invalid**: same nested-choice bug, and a choice with an empty option deleted `scheduler_recv` (`best_g.c`). Discard |
| 4 | owner group ctx, ternary float test (`t/g4`) | 16 | 94 | valid, worse |
| 5 | owner group ctx, clean 8 choice points (`t/g5`) | 256 | 91 | valid; every option ties (control v0000 = 91) |
| 6 | goto form (`t/goto_plain.c`, `g7/`) | 1 | 103 | valid, worse |

Round 5 is the honest best. Its choice points (declaration order, `D < f` orientation, `!= 0` orientation, `2 ==`,
`-1 ==`, `4 <=`) are semantically neutral and did not move the count.

### Next hypothesis
Fix the frame first (the diagnose lever): drop the `temp_f14`/`var_f12` locals and use one `f32` local, or drop
`temp_v0_2`. Then aim at the hoisting of `-1`, `2`, `&D_80152748` (s4/s6/s7) with a register-priority probe
(`tools/trace/force.sh`), which needs the trace toolkit installed in the scratch (not done in this lane).

## engine_sound_sync (1236 B, 309 words)
Not matched. `mkasm.py` produced `engine_sound_sync/ess.s` with all labels defined. `tools/mips_to_c/m2c.py -t mips-ido-c -f engine_sound_sync`
fails with `Cannot find branch target L80098f5c` (label is at 0x80098f5c, the `lui a0,0x8014` line after the `bnezl s4` at 0x80098f54).
Unresolved; no draft written. Its unit neighbours are audio_effect_setup (provisional), entity_state_check (unlocked), entity_transform_calc and func_80098A54.

## Integration notes
- Lane files: `func_800988D8/t/` holds the templates and per-round `score.txt`. `func_800988D8/g2` and `t/b2` came from the contaminated round 2/3 templates; ignore them.
- Nothing to splice. No `cloud/matches/` file. No group dir claimed.
- `audio_effect_setup` provisional status is unchanged: this lane did not prove its body with a real caller.
