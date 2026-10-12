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

RT64 itself on rocky (no GPU/display): the RT64 host runs in a fedora:41 container on Xvfb with Mesa lavapipe
(software Vulkan; `~/rush2049/recomp/w25r/xvfb/{Dockerfile,run.sh}`). Wall clock, 30 s: `busy-wait at 0x800FD500:
yielding`, 767 gfx tasks (`--no-spin-yield`: **gfx_tasks=2**, the laptop hang reproduced with RT64); race script
80 s: race loads at 26 s, 1611 gfx tasks (lavapipe is CPU rendering).

## 2. RT64 lockstep self-check: the second range is the z-buffer

The host now logs the render targets the display lists set (`# rdp_image F KIND+ADDR/WxH/BPP`: G_SETCIMG /
G_SETZIMG, width, G_SETSCISSOR height) and the gfx OSTask buffers (`# gfx_task`). Every frame of race, two_player,
attract and audio_options uses the same four: color 0x8017AEC0 (the z-buffer's clear), **Z 0x8017AEC0** 320x220x2
(0x8017AEC0-0x8019D4BF: right after gBssEnd 0x8017A640, ends just before the data inflated at 0x8019D500), color
0x807BB3C0 and 0x807DDA00 (double-buffered 320x220x2; VI origin 0x007DDC80 = +1 line). The v1 71 pages = 36 pages
0x8017A000-0x8019DFFF (z-buffer) + 35 pages 0x807DD000-0x807FFFFF (framebuffer 1). gfx task: ucode 0x80028CC0,
dram_stack 0x8007BA50/1024, output_buff 0x8007CA50..0x80084A50, yield 0x8007BE50/3072. Named in
`tools/recomp/runtime_buffers.json` -> datasyms `.absolute` (rush_z_buffer, rush_framebuffer_0/1,
rush_gfx_dram_stack/yield_buffer/output_buffer; recomp-only names, not matching evidence).

Masks: `oracle.py compare --mask-renderer` (page-granular, any logs; drops page 0x8017A000 whole, which also holds
game .bss) and host `--mask-rdp-images` (precise: image bytes zeroed in the page hashes, header `# mask rdp_images`,
every compared run must use it). Null-renderer verdicts unchanged (no mask unless asked).

Second RT64 bug found on the way: lockstep + RT64 crashed (SIGSEGV in RT64::Interpreter::processDisplayLists) at the
race start: lockstep sends SP/DP to the game at once, so the game rebuilt the display list while RT64 still read it
(slow software GPU widens the window; latent on the laptop). Now the game thread waits (host-side) until the renderer
has interpreted the mirrored list.

rocky, race script, 1800 frames, lockstep (a, b: null; c: RT64 on lavapipe):

    a vs b:                   EQUIVALENT over 1800 frames (judged: all frames, 1 baseline run(s))
    a vs c:                   DIFFERENT at frame 2: 71 rdram page(s)   (= the laptop's T8)
    a vs c --mask-renderer:   EQUIVALENT except renderer-written pages over 1800 frames (judged: all frames, 1 baseline run(s))
                                renderer mask: 105 page(s) not judged (RDP color/depth images: rush_framebuffer_0, rush_framebuffer_1, rush_z_buffer)
    am vs cm (--mask-rdp-images on both): EQUIVALENT except renderer-written pages over 1800 frames (judged: all frames, 1 baseline run(s))

## 4. Audio: the audio microcode recompiled with RSPRecomp

First audio task (host log): `ucode_boot 0x80027DD0/208 ucode 0x8002A050/4096 ucode_data 0x8002E210/2048 dram_stack 0
output 0 data 0x801A9260/1 yield 0`. Text and data are in the boot segment (ROM 0x2AC50 / 0x2EE10). **Not the
libultra aspMain**: no acmd command table; the code walks per-voice records in DMEM (fields +0x2C, +0x3C, +0x40...),
indirect jumps are only `jr $ra/$30/$8` on saved return addresses (RSPRecomp collects those itself). The declared
4096 bytes hold data after IMEM 0x1FC0 (two words decode as invalid instructions), so the config stops at the code
end: `text_offset 0x2AC50, text_size 0xF40, text_address 0x04001080` (gen_config `AUDIO_UCODE` ->
`rush2049.rsp_audio.toml`; `RSPRecomp rush2049.rsp_audio.toml` -> `rsp/rush_audio_ucode.cpp`, 2578 lines; CMake
compiles it with `-march=nehalem` when present, `RUSH_HAVE_AUDIO_UCODE`). No extra branch targets, no unsupported
instructions, no "Unhandled jump target" in any run.

Second audio bug: the game's AI thread (static func_80010A40) hands the AI one 0x300-byte buffer (192 frames) per
AI message, as the console's AI interrupt fires once per buffer (22050/192 = 115/s); ultramodern sends AI once per
VI (60/s), so the game produced half its audio. Now AI messages are paced at frequency/frames-per-buffer (wall: a
host thread corrected by the SDL queue; lockstep: integer accumulation per virtual VI). Output: SDL2 queue-mode
device (L/R swap as Zelda64Recomp, SDL_AudioStream 22050 -> device rate), `--no-sound`, `--volume`, `--wav PATH`.
New status fields `audio_nonzero=`, `audio_peak=`.

| run (rocky) | audio_samples | audio_nonzero |
|---|---:|---:|
| headless wall 20 s, `--no-audio-ucode` (v1 behaviour) | 880,896 | **0** |
| headless wall 45 s race, ucode, AI per VI (before pacing) | 1,036,416 (11.5 kHz worth) | 890,039 |
| headless wall 45 s race, ucode + AI pacing | 1,984,128 (22.05 kHz worth) | 1,672,584 |
| RT64 (lavapipe) wall 45 s race, SDL `disk` audio driver | 1,991,040 | 1,770,888; device stream 45.1 s, 0 silent frames after 10 s |

First non-silent buffer at t=5.1 s (title). Spectrogram of the WAV: rhythmic broadband hits, harmonic lines and
sweeps (music + effects), not noise; LR correlation 0.9+ in menus, independent in the race. Pitch/tempo by ear:
tester.

Lockstep (audio ucode now runs synchronously in osSpTaskStartGo; AI paced): stock self-check race `EQUIVALENT over
1800 frames (judged: all frames, 1 baseline run(s))`; draft_engine_sound_sync capture vs replaced:

    race:          EQUIVALENT over 1800 frames (judged: all frames, 2 baseline run(s)) / all 3599 captured calls identical
    race_track2:   EQUIVALENT over 1800 frames (judged: all frames, 2 baseline run(s)) / all 3599 captured calls identical
    two_player:    EQUIVALENT over 1800 frames (judged: all frames, 2 baseline run(s)) / all 3599 captured calls identical
    audio_options: EQUIVALENT over 1800 frames (judged: all frames, 2 baseline run(s)) / all 3599 captured calls identical
