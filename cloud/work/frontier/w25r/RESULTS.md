# w25r: recomp Phase 5 (wall-clock busy-waits, RT64 self-check mask, package v2 fixes, audio ucode) — tooling lane, branch w25r

Status: in progress (2026-10-12). Not a matching lane: no src/, asm/, lock or coverage change.
Host: rocky, `~/rush2049/recomp/w25r` (clean copy of this branch's tracked files + w24r's `b.sh`/`sanity`; nothing
ROM-derived committed). Inputs: the v1 laptop report `tools/recomp/reports/v1/REPORT.md`.

## 1. Wall-clock boot hang: busy-wait hooks

Cause (v1 report, confirmed): `game_loop` 0x800FD500 `bnel t8,zero,. / lh t8,0(v0)` spins on `__osScTaskCount`
(D_8002EB70, scheduler +0x288) with no libultra call; RT64's DP completion is an external message that only a
libultra call (or the idle thread) delivers, so the scheduler thread never runs `__scHandleRSP`/`RDP`.

Reproduced headless on rocky: `--null-dl-delay MS` (new, test only) makes the null renderer take MS ms per display
list on the renderer thread, as RT64 does. `--no-spin-yield` (new, diagnostic) disables the fix.

| null renderer, wall clock, 15 s | gfx_tasks | spin_yields |
|---|---:|---:|
| delay 0 | 450 | 0 |
| delay 0, --no-spin-yield | 449 | 0 |
| delay 20 ms | 449 | 41 |
| delay 20 ms, --no-spin-yield | **2** (the laptop hang) | 0 |
| delay 40 ms | 299 | 78 |
| delay 40 ms, --no-spin-yield | **2** | 0 |

Wall clock + delay 25 ms + race / two_player / audio_options scripts (75 s): race loads (`load ovl_b`), 2249 gfx
tasks; three sites yield: 0x800FD500 (boot), 0x8038BA20 (front end, image A, 20 s), 0x80096184 (race start, 23 s).

Fix: `rush_hook_spin(rdram, ctx, site)` (runtime/src/hooks.cpp) at 14 sites (patches.json `"site_arg": true`):
`yield_self_1ms` (ultramodern: wait <= 1 ms for one external message, deliver it, swap to a woken higher-priority
thread) — the one-turn form of `rush_hook_park_idle`'s `pause_self`. No-op with `--clock frames`.

Busy-wait census (scanner: every backward branch whose loop has no call/jump, loads only from loop-invariant
addresses; static, game, image A, image B; 22 loops, of which 8 are libultra hardware-register polls N64Recomp
does not emit (osDpSetNextBuffer, osViInit, osPiRawReadIo, osPiRawStartDma, osEPiRawStartDma, __osEPiRaw*Io) and
14 are game busy-waits, all hooked):

| site | function | waits on |
|---|---|---|
| 0x800FD500 | game_loop | D_8002EB70 (__osScTaskCount) != 0 |
| 0x80096184 | func_80096130 | D_8002EB70 != 0 |
| 0x800A5094 | suspension_setup | D_8002EB70 != 0 |
| 0x800A5C2C | tire_compound_set | D_8002EB70 != 0 |
| 0x8038B3DC | ovl_a func_8038B388 | D_8002EB70 != 0 |
| 0x8038B4E8 | ovl_a func_8038B3EC | D_8002EB70 != 0 |
| 0x8038BA20, 0x8038BA84 | ovl_a func_8038B940 | D_8002EB70 != 0 |
| 0x8008AB3C | dma_wait_complete | scheduler +0x274 (retrace counter, 0x8002EB5C) unchanged |
| 0x800A4BA0 | wheel_params_set | D_8011028C != 0 |
| 0x800A4BC8 | wheel_params_set | 0x80146168 != 0 |
| 0x800E7114 | func_800E7038 | D_80111950 == 0x80 |
| 0x80011868 | func_80011848 (static) | 0x800382CC != 0 |
| 0x8001416C | func_80014140 (static) | 0x80038362 > 0 |

Oracle (lockstep, null renderer, rocky): stock self-check (race, a vs b) `EQUIVALENT over 1800 frames (judged: all
frames, 1 baseline run(s))`; `draft_engine_sound_sync` (capture build vs replaced build):

    race:          EQUIVALENT over 1800 frames (judged: all frames, 2 baseline run(s)) / per-call: all 3599 captured calls identical
    race_track2:   EQUIVALENT over 1800 frames (judged: all frames, 2 baseline run(s)) / per-call: all 3599 captured calls identical
    two_player:    EQUIVALENT over 1800 frames (judged: all frames, 2 baseline run(s)) / per-call: all 3599 captured calls identical
    audio_options: EQUIVALENT over 1800 frames (judged: all frames, 2 baseline run(s)) / per-call: all 3599 captured calls identical

Also: wave 24 (w24s) merged viStub into viDeadlinePassed, so the static inventory has 667 functions; gen_config's
expected count was still 668 (the ROM end-to-end test failed on wave25 before this lane). Function count now 2135.
