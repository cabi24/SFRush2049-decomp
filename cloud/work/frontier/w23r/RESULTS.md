# w23r: recomp Phase 3 (per-call capture, input scripts, static binding, OSThread word mask) — tooling lane, worktree branch w23r

Status: done (2026-10-12). Not a matching lane: no src/, asm/, lock or coverage change.
Host: rocky, `~/rush2049/recomp/w23r` (copy of this branch's tracked files + rsync of tools/recomp; nothing
ROM-derived committed). Design and commands: `tools/recomp/README.md` "Phase 3"; plan section 11; how a lane
reads a verdict: WAVE_BRIEF "Behavioural check".

## Verdicts (1800 frames each; stock side = capture build of the same set; `runs/final/*.txt` on rocky)

| Set / script | First line of `oracle.py run` (+ per-call line) |
|---|---|
| `matched10` / race | `EQUIVALENT over 1800 frames (judged: all frames, 2 baseline run(s))`; `per-call: all 333252 captured calls identical (arguments, returns, effects)` |
| `matched10` / race_track2 | EQUIVALENT; `all 374162 captured calls identical` |
| `matched10` / two_player | EQUIVALENT; `all 418288 captured calls identical` |
| `matched10` / audio_options | EQUIVALENT; `all 201059 captured calls identical` |
| `bug` (render_helper `>` -> `<`) / race | `FIRST DIFFERING CALL: seq 2 = render_helper (bug_render_helper) call 2, frame 2: effect` (13 words vs 1: D_80149B88 0->0x10, D_8012E608 0->0x10, D_8012E678 0->0xFFFF, gDisplayListPtr 0x80127330->0x80127350, 8 display-list words at 0x80127330 `ee000000 ffff0000 e2001d00 ...`); frames: DIFFERENT at frame 3 |
| `draft_engine_sound_sync` (w21h best.c) / race | `FIRST DIFFERING CALL: seq 1410 = engine_sound_sync (draft_engine_sound_sync) call 1410, frame 705: effect` (80 words vs 78; 20 differ: the +4 float of 12-byte records D_80142DD8+0x4..+0xE8, stock 1.0/-2.0 alternating, draft -2.0/-1.0) |
| `draft_engine_sound_sync_probe` (+4 callee probes) / race | `FIRST DIFFERING CALL: seq 1444 = entity_transform_calc (probe_entity_transform_calc) call 1, frame 705: args`, `inside: engine_sound_sync (draft_engine_sound_sync) call 1410 (seq 1441)`, `f22: stock 3F800000, replaced 00000000`, `f28: stock 3F800000, replaced BF800000` |
| w23f `draft_engine_sound_sync_ipa` (`w23f/engine_sound_sync/oracle.c`, sha1 07301561) / race, race_track2, two_player, audio_options | `EQUIVALENT over 1800 frames`; `all 3599 captured calls identical` on each; with `--probe-callees` (race): `all 157251 captured calls identical` |
| `draft_func_800D7634` (w21h best.c) / audio_options | `EQUIVALENT over 1800 frames`; `all 435 captured calls identical`; with `--probe-callees`: `all 8942 captured calls identical` |
| `draft_func_800D7634` / race | `NOT EXERCISED` |
| `statics` (matched func_800E7710) / race | `EQUIVALENT`; 1 call identical (Phase 2: DIFFERENT at frame 1, its private static) |
| all 689 matched `src/blob` game files (exploration, not committed) / four scripts | `NOT EXERCISED over 1800 frames` = no judged frame difference, 340/346/342/340 of 689 called; per-call (race): `all 4241687 captured calls identical` |

### engine_sound_sync (for w23f)
The standalone draft's first wrong call is its first call of entity_transform_calc (0x80091CA4) in its
call 1410 (frame 705, first race frame doing work): retail passes the four values in callee-saved
f22/f24/f26/f28 (target.s 0x80098F30-0x80098F40; the callee compares each with the -2.0 sentinel in f20,
0x80091CC8 `c.eq.s $f22,$f20`); retail call 1: f22=1.0 f24=0 f26=0 f28=1.0; the draft leaves f22=0,
f28=-1.0 there (it passes o32 a1-a3/sp+16; retail a1-a3 hold stale array pointers). w23f's oracle source
(the same body with static copies of the IPA callees) is EQUIVALENT on all four scripts: the body's logic
is behaviourally right; only the call convention of a standalone compile was wrong. CLAIMS note format:
`correct, not matched (oracle EQUIVALENT 1800 frames x4 scripts, 3599 calls identical; w23f oracle.c with
static IPA callee copies)` — the coordinator decides whether an oracle source counts.

### func_800D7634
EQUIVALENT under `audio_options` (435 calls, frames 1201-1635, rows changed with up/down/left/right, B exits),
callees identical too (8,942 captured calls with `--probe-callees`). Earlier today it looked DIFFERENT
(speed_set call 17, frame 1485: message counter gRenderSync+0xC one short): that was the Phase 2 legacy-name
bug below (its post_msg copies call game `osJamMesg`), not the draft.

## Design (what was built)

- **Capture** (`runtime/src/capture.cpp`, host `--call-log`, `--call-detail SEQ`): entry hook records the
  prototype's argument locations, exit the return and the effect; write tracking by page (mprotect 8 MiB
  read-only at each entry, SIGSEGV copies each page before its first write for every open window, compare
  at exit). Effect excludes the thread's stack below the outermost replaced function's entry sp, other
  threads' stacks, OSThread host words. 10x run time (15 s for 1800 frames, 0.3 M calls; 3 min for 4.2 M).
- **Stock capture build** (`gen_config.py --capture-set SET`): hook = bypass check, count, enter, re-enter the
  recompiled original, exit. Replaced build: a body's `jal` to itself is retargeted to the original entry so
  recursion is captured identically (model_transform_setup: Phase 2 counted 76682 calls, the real 76684).
- **Probes**: entries with `"mode": "capture"` (prototype or `capture_args` registers, `capture_ret`);
  `--probe-callees` (both builds) probes every retail `jal` target, skipping functions the replaced source
  defines and runtime-reimplemented libultra; a probe also hooks a replacement's own copy of the function.
- **oracle.py**: `compare_calls` (first differing record: args / return / effect / sequence / length; refuses
  logs of different sets, layouts or capture versions), `effect_diff` via a `--call-detail` rerun, `calls`
  and `script` subcommands, `--script`, `compare --calls`; NOT EXERCISED ignores probes.
- **Scripts** `tools/recomp/scripts/{race,race_track2,two_player,audio_options}.press`; host `--press` items
  take `@PORT`; controllers connected = ports used. How each was found (no pixels): probe call counts
  (func_800D7634 = pause, DOWN x3, A; DOWN x4 reaches physics_sym), the set of streams inflated after image B
  (track selector at 18.5 s cycles them), `active_player_count` 0x8014A108 and display-list counts (2P).
- **Static binding** (`replace.static_bindings`): local writable statics patched to retail addresses from the
  replaced function's own retail HI16/LO16 words (opcode-checked), `D_` names, or entry `statics`.
- **Fixes found on the way**: game/overlay replacements resolve libultra through `game_static_legacy.us.txt`
  (as `blob_tu.static_symbols`), loads into $zero nopped in the image, word diffs skip the thread words.
- **OSThread mask**: `offsetof(OSThread, context)` = +0x20 (8-byte host pointer); two stock runs differ in
  exactly those words of 11 threads; masked word by word, so all 2048 pages are judged.
- Tests: `tests/recomp/test_phase3.py` (capture layouts, hooks, header tables, callee probes, self-call
  retarget, zero loads, legacy names, call-log diff incl. version/layout refusal, effect diff, CLI, thread
  words, script parser and committed scripts, static binding on synthetic objects incl. shared HI16).

## Limits

- A draft calling IPA-convention callees (frontier `register_params`) is judged wrong at that call unless
  built as an oracle source with static copies of those callees (w23f). No automatic detection: `ipa.py`
  reads derived asm whose FP instructions are `.word`.
- Effects written into callers' stack frames below the outermost replaced function are not judged;
  writes by another thread while a window is open count for that window (both builds alike).
- Scripts were found without pixels; "second track" and "two players" rest on the RAM/stream evidence above.

## Integration notes

Merge branch w23r as is (tools/recomp/**, tests/recomp/**, plan section 11, WAVE_BRIEF "Behavioural check",
this file). No CLAIMS line for the drafts from this lane; the two EQUIVALENT verdicts (func_800D7634 under
audio_options; engine_sound_sync via w23f's oracle source) are for w23f / the coordinator to record.
w23f's manifest entry (`w23f/engine_sound_sync/replacements_entry.json`) was run from an uncommitted manifest
on rocky; it is not added to replacements.json here because its source is outside this branch.

## Checks

- `python3 -m pytest tests/recomp -q -p no:cacheprovider` on the Pi worktree: `.s....................................                                   [100%]`.
- `make test-fast JOBS=4` on the Pi worktree (submodules via --reference): tests/conveyor 1512 passed / 808
  skipped, tests/cloud 770 passed / 150 skipped, exit 0.
- rocky: nothing of this lane left running (uptime 2 h, no reboot during the lane); runs trimmed to 2.6 GB.

## Permission denials

None.
