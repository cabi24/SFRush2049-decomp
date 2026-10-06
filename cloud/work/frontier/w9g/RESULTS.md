# w9g — provisional burn-down (wave 9)

Assignment: close provisional entries `func_800D8078`, `drone_throttle_calc`, `func_800E4300`, `func_800E451C`,
`func_800DED78`, `audio_priority_find` by matching their real callers. (`speed_set`, `camera_track_spline` are lane
w9b's; `func_80087110`, `stat_race_update`, `func_800FE5B0` were not touched.)

**Outcome: no provisional entry can be closed this wave.** The only cheap caller (`audio_mixer_main`, 732 bytes)
stays 31/183 words off; every other entry waits on a caller of 2.3–3.9 KB that itself has unmatched blockers.
Nothing was spliced, committed or added to `cloud/matches/`.

## Table

| Entry | Real caller(s) still unmatched | State of the caller | Can close? |
|---|---|---|---|
| `audio_priority_find` (432 B) | `audio_mixer_main` (732 B, kept) | **31/183 words**, `-g0 -O3 -mips2 -G 0 -non_shared`, same residual as w2b/w3b/w4b | no |
| `func_800DED78` (488 B, f18 reg-param) | `mode_select_handler` (2,976 B, L2, internal: s1 reg-param) | blocked by unmatched `best_times_display`; unit also needs `func_800E05F0` | no (not attempted) |
| `func_800E4300` (540 B) | `func_800E451C` (provisional itself) → `func_800E4B58` (2,268 B, L5) + `func_800E398C` (2,396 B) | PR #128 packet: E451C 365, E398C 592, E4B58 521 differing positions; E4B58 blocked by `state_utility` | no (not attempted) |
| `func_800E451C` (1,588 B) | `func_800E4B58` | as above | no (not attempted) |
| `func_800D8078` (220 B) | `func_800D91A0` (3,860 B, L8) | 6 unmatched blockers: `drone_target_update`, `func_800B4FB0`, `func_800D816C`, `func_800D9058`, `render_replay_ui`, `render_results_screen` | no (not attempted) |
| `drone_throttle_calc` (1,024 B) | `func_800D91A0` | as above | no (not attempted) |

Cheapest-first ordering used: `audio_mixer_main` is the only caller under 1 KB and the only one with no unmatched
blockers, so the whole budget went there.

## audio_mixer_main — 31/183 (no movement)

Commands and exact output (builder scratch `~/rush2049/scratch/frontier/w9g`, Pi repo root):

```
$ IDO_DIR=… python3 tools/cloud/score.py group cand/frontier_route_cross      # = cloud/work/frontier/w2b/groups/frontier_route_cross
Members:
audio_priority_find:
  MATCH
Context (informational; excluded from exit status):
audio_mixer_main:
  31/183 words differ

$ python3 -m tools.conveyor.pipeline.blob_unit --tag w9g score audio_mixer_main audio_priority_find \
    --with cloud/work/frontier/w9g/audio_mixer_main/best.c --internal audio_priority_find --keep audio_mixer_main
  FAIL audio_mixer_main: 31 of 183 words differ
  EQUAL audio_priority_find: 108 words (internal, c_best.c)
```

`best.c` = w2b's group source plus a `u32` typedef (code unchanged). Proc ordinal of `audio_mixer_main` in the
current unit is **537** (was 480 in w4b's notes); traced-uopt colouring for the baseline (`tools/tr9.sh … 537`)
is unchanged from w4b: `k + 1` is web w80 (type 4, save 10, tot 40, 4 blocks) → s2.

Residual (unchanged): in loop 2 retail computes `k + 1` three times (`addiu t9,s0,1` for the test,
`addiu v0,s0,1` in the else arm, `addiu s0,s0,1` as the increment); ours keeps one copy in s2, which shifts
`done`, the two float-total addresses and the constant 80 up one s-register.

**New finding (narrows the hypothesis space):** the increment is *not* what creates the shared web. The diagnostic
variant `d_k2` (test and arm use `k + 2`, so the increment cannot share the expression) still CSEs test and arm into
one temp (`addiu v1,s1,2; bnel v1,v0; move v0,v1`) and scores 24/183, with the s-registers then nearly retail
(k/pointer swapped). So in retail **the test and the arm are themselves different ucode expressions** to uopt, not
just the increment. w4b's conclusion ("the candidate is never created") holds, and the cause has to separate the
test from the arm as well.

Tried this wave (≈35 variants, all in the unit; lists in `audio_mixer_main/variants/`):
- unsigned spellings in the test, the arm, the increment (`1U`, `(u32)k + 1`, `k = (u32)k + 1`), `u32 next`: 31,
  same code (uopt does not distinguish signed and unsigned adds);
- increment spellings `k += 1`, `k = k + 1`, `k = 1 + k`, `k -= -1`, `++k` crossed with `1 + k`, `k - -1`: 31;
- pointer comparison `&D[k + 1] == &D[count]`: 61;
- parallel counter `n` (in test only / test and arm): 42 / 62;
- static helpers, inlined: whole segment-length body `seglen(s32)` 176, `seglen(s16)` 59, `nextcp(s32)` 31,
  `nextcp2(cp, count)` 65;
- goto loop: 176.

Best next hypothesis: something gives the test's `k + 1` and the arm's `k + 1` different operands in ucode (two C
objects that uopt cannot prove equal, e.g. a value that passes through memory or through an inlined helper's
parameter that is *not* copy-propagated). The deleted static at the stub `func_800BA2B0` (just before
`func_800BA2B8`, caller-less) is in this TU and is the best candidate for that helper; w3b's helper tries only
covered `get_next_checkpoint` shapes. A uopt PRE trace (not just the colouring trace) would settle it.

## Tools (in `tools/`)

- `am.sh CAND.c [dump]` — unit score of `audio_mixer_main` + internal `audio_priority_find` (tag w9g), optional
  disassembly of our body.
- `var.py BASE.c VARFILE` — replaces loop 2 (from `done = 0;` to the function's closing brace) with each variant
  (`%%% name` separators, optional `@DECL` line) and scores it.
- `tr9.sh CAND.c LABEL [PROC]` — colouring trace with a private copy of w3a's traced uopt in the w9g builder scratch.

## Generalisable

- When a residual looks like "one CSE web too many", test whether the *loop increment* is the cause by changing the
  constant in the other occurrences (`k + 2`): here it showed the test/arm pair is CSE'd on its own, which rules out
  every increment-spelling variant tried in earlier waves.
- Proc ordinals in the unit drift as functions land; re-derive them by diffing `procindex` decision counts of two
  traces that differ only in the target function.
- Provisional entries whose callers are ≥2 KB with their own unmatched blockers (D91A0, E4B58/E398C,
  mode_select_handler) are not burn-down work: they close only as part of the main frontier order.
