# Recomp boot log (Phase 1)

Exact commands and what each run reached. Host: rocky (see `VERSIONS.md`). Everything under
`build/recomp/` is private (ROM-derived) and never committed.

## Commands

```bash
# on rocky, in a copy of the w21r worktree (tracked files only)
cd ~/rush2049/recomp/w21r
python3 tools/recomp/gen_config.py --rom ~/rush2049/recomp/rom/baserom.us.z64 \
        --n64recomp-src ~/rush2049/recomp/src/N64Recomp
(cd build/recomp && rm -rf RecompiledFuncs && ~/rush2049/recomp/build/N64Recomp/N64Recomp rush2049.recomp.toml)
CC=clang CXX=clang++ cmake -S tools/recomp/runtime -B build/recomp/host -G Ninja \
      -DN64MODERNRUNTIME_DIR=$HOME/rush2049/recomp/src/N64ModernRuntime -DRECOMP_OUT=$PWD/build/recomp
ninja -C build/recomp/host
build/recomp/host/rush2049_recomp --rom build/recomp/rom/rush2049.recomp.z64 \
      --home build/recomp/home --seconds 30 [--press 8:START,12:A] [--fb-dump build/recomp/fb]
```

`cmake` runs `tools/recomp/runtime/check_contract.py` first: every shim/hook the generator
recorded in `manifest.json` `runtime_contract` must be defined by the host or the runtime.
Crash triage: `gdb -batch -ex run -ex bt --args build/recomp/host/rush2049_recomp ...`.

## Runs

Scripted input used for the race runs (`--press`, seconds since start):
`5:START,8:START,11:A,14:A,17:A,20:START,23:A,26:A,29:A,32:A,35:START,38:A,41:A,44:A,47:A,50:A`.
Input never needed more than this: image B is loaded 23.3 s in, after the 20:START/23:A presses.

| # | Change before the run | Reached | Stopped at |
|---|---|---|---|
| 1 | first link (no identity map) | entrypoint -> `main_recomp` | SIGSEGV in `osPiReadWord` (recompiled `osPiRawReadIo`: reads PI_STATUS 0xA4600010): `main` calls the inventory's `osPiRawReadWord`, which is really `osPiReadIo` |
| 2 | `identity.json` first cut (16 renames) | game image inflated + `load_overlays` | SIGSEGV in `osSetGlobalIntMask` (really `osSetIntMask`, reads MI_INTR_MASK) from `osScAddClient` |
| 3 | full identity map (43 renames from the libultra audit), `__osContAddressCrc` inert | game image, data inflates, image A loaded, all threads created | `std::runtime_error("No threads left to run!")`: the idle thread's hook parked it in a blocking `osRecvMesg` with nothing runnable |
| 4 | idle park = ultramodern `pause_self` | 2 gfx tasks, 1 audio task, then a hang | run queue self-loop: scheduler thread (id 4) inserted twice. Cause: the game stops and restarts other threads; the runtime's `osStopThread` only stops the caller (no-op otherwise), so `osStartThread` queued an already-queued thread |
| 5 | `osStopThread_rx` shim (unlinks another thread) | front-end main loop: 30 gfx tasks/s, 60 audio tasks/s, 57 asset inflates, VI 60 Hz, stable 90 s | (no input) stays in the front end, display lists 344 B |
| 6 | + scripted input | image B (race) loaded at 23.3 s; display lists grow to 14 KB (3D scene); stable 120 s at 30 frames/s | null renderer: nothing is rasterised (framebuffer stays clear) |
| 7 | + inflate verification | game image 161761/161768 words equal the compiled image (7 data words already written by the game), image A 48532/48532, image B 10972/10972 | same as 6 |

Notes:
- `fb_nonzero` stays at 4 words: the null renderer executes no display lists, and the game
  does not draw with the CPU. "Draws" therefore means "submits display lists and swaps
  buffers": gfx task count, display-list sizes and VI origin changes are the evidence.
- Audio RSP tasks complete immediately (no recompiled audio microcode); the AI buffer
  queue receives silence-length sample counts (`audio_samples`), so the audio thread runs.
- Shims reached at runtime in every run: `osPiReadIo_recomp` (main reads the ROM header
  via PI), `osStopThread_rx`. No INERT/NOPACK shim was reached in 120 s of front end + race.
