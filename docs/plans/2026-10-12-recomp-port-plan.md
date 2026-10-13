# Static recompilation and PC port: plan (2026-10-12)

**Purpose.** Run Rush 2049 from its own code on a PC by static recompilation, first as a **tool for the
decompilation** (function replacement, memory inspection, differential testing of candidate C), second as
a playable port. The byte-identical ROM remains the project's goal; this effort must never absorb the
matching work, and it runs on the owner's machines because it needs the ROM.

## 1. Why this helps the decompilation

- **Non-matching C gets a home.** Behaviourally correct drafts that are not byte-exact (camera_play_script at
  27 rows, the boot-tail near-misses with scheduling-only residuals, Astra's 2-word overlay residuals) can run
  in the port now. "Near-miss" becomes a shippable state instead of a dead end.
- **Structure recovery.** The hardest open game functions are blocked by unknown structure, not registers
  (entity_spawn_init's missing early value, func_800D91A0's record layout, the Visual/Model structs). Watching
  the real code run with real data is the fastest way to recover layouts; Astra's host semantic harnesses are a
  cloud approximation of this.
- **Differential testing.** Recompiled function vs candidate C on the same captured inputs, automatically.
  Catches wrong reconstructions that happen to look close, and lets a lane stop at "correct" when the owner
  decides that is enough for the port.
- **The expensive inputs already exist**: function extents and symbols for all four code populations
  (`asm/us/blob`, `asm/us/boot_tail`, `asm/us/ovl_a`, `asm/us/ovl_b`, `extents.json` / `symbols.json`), the
  libultra identification on the static side (`symbol_addrs.us.txt`, w15g/w16g label tables), the overlay load
  scheme (two deflate streams, `0xB5C534` and `0xB6FEC4`, both loaded at `0x8038A400`, pointers `D_8002B020` /
  `D_8002B024`), the game image stream (`0xB0CB10` → `0x80086A50`) and the vendored zlib 1.0.4 that reproduces
  every stream (`tools/conveyor/pipeline/ovl_image.py`, `blob_rom`).

## 2. Non-goals (for the spike)

Renderer fidelity, widescreen, input remapping, audio quality, mod support, distribution. Those are the port
proper (Phase 4) and start only after the tool stages work.

## 3. Approach

Use an existing N64 static recompiler rather than writing one: **N64Recomp** (Mr-Wiseguy; the
Zelda64Recomp / `N64ModernRuntime` + RT64 stack) is the reference. Verify its current input format against its
README before writing the generator; the notes below describe what it needs in general terms:

- a description of each code section (ROM offset or raw bytes, vram, size) and every function in it (name,
  vram, size), plus the symbols the code references;
- which functions are libultra and replaced by runtime shims;
- relocatable/overlay sections, so code loaded at runtime (our images A and B) is recompiled as its own set and
  dispatched by load state.

Our game image is a compressed stream inflated at boot, not a plain ROM segment, so the generator feeds the
recompiler the **inflated** images (produced from the ROM on the fly, never committed) at their vram, exactly as
`blob_layout` / `ovl_layout` do.

The ROM, the inflated images and any recompiled output derived from them are **private inputs**: generated into
`build/recomp/` (git-ignored), never committed, same rule as `baserom.us.z64`. Committed: the generator, the
shim list, the replacement manifest, the harness, tests and docs.

Host: **rocky** (x86-64, 20 cores, 38 GB, idle; already a CI-equivalent host). Work in
`~/rush2049/recomp/` there; the ROM copied from the Pi by hand, not by a script.

## 4. Phases, acceptance, time boxes

### Phase 0: setup (half a day)
- Build N64Recomp and its runtime on rocky; record exact versions/commits in `tools/recomp/VERSIONS.md`.
- `tools/recomp/gen_config.py`: derive the recompiler's input from our committed inventories (four populations,
  the static libultra names, the overlay pair) and the ROM. Tests: deterministic output from fixture
  inventories; refuses a missing/wrong-hash ROM.
- Acceptance: the recompiler consumes the generated config and emits C for all 1216 + 668 + 193 + 49 functions
  without hand edits to its config.

### Phase 1: boot (1–2 days)
- Runtime shims for the libultra surface the game uses (threads, messages, PI/SI/VI/AI, controller, Pfs); the
  static side's identification list is the shim list.
- Overlay loader hook: the function that reads `D_8002B020` / `D_8002B024`, inflates and jumps; the runtime must
  select the recompiled set for image A or B. (`ovl_image` has the constants and the exact inflate.)
- Acceptance: title/attract and the front end run; a race starts. Record what does not work; do not fix
  renderer issues beyond "renders at all".

### Phase 2: function replacement (1 day)
- `tools/recomp/replacements.json`: function → C source (a `src/blob/*.c`, `src/ovl_*/`, `src/rom` body, or a
  `cloud/work/**/best.c` draft) with the population and a note. A build flag swaps the recompiled block for the
  C body compiled for the host (same `-D` surface Astra's host harnesses use; the arcade-derived headers in
  `include/`).
- Prove with ten already-matched functions (behaviour identical by construction) and one disclosed draft.
- Acceptance: identical observable behaviour over a fixed input script (see Phase 3 capture) with and without
  the swap, for the ten; the draft runs.

### Phase 3: differential harness (1–2 days) — the decompilation payoff
- Capture at function entry/exit in the recompiled build: arguments, the memory the function reads and writes
  (page-granular diff), return value; triggered by a function list and an input script (deterministic replay).
- Replay: compile a candidate C body for the host, run it against the captured inputs, diff outputs and memory
  writes. Report per candidate like the scorer does (`EQUIVALENT` / first differing field).
- Pilot on three open near-misses: world_effect_update (12 words, two block boundaries), func_80010E80 (frame
  off), one of Astra's #311 overlay functions. Acceptance: the harness says EQUIVALENT for a known-matched
  function, flags an injected bug, and gives a verdict for each pilot.
- Wire-up: a `cloud/work/frontier/WAVE_BRIEF.md` section ("Behavioural check") so lanes can ask for a verdict;
  a `CLAIMS.md` state "correct, not matched" with the harness verdict path.

### Phase 4: the port proper (open-ended, owner's pace)
RT64/renderer settings, input, audio, save paths, packaging. Only after Phase 3 is in use by the waves.

## 5. Interaction with the matching effort

- The coordinator's waves continue unchanged. The spike's mechanical parts (generator, shim list, harness code)
  can be lanes in worktrees like other tooling; Phase 1/2 debugging needs the ROM and rocky and is the owner's
  or the coordinator's work on the LAN.
- Astra cannot run any of this (no ROM in the cloud) but can consume harness verdicts and provide candidates;
  the brief's packet protocol gains one optional field: the harness verdict for a non-matching body.
- A function replaced in the port by a non-matching body stays **unmatched** in every coverage number; the four
  coverage rows are unchanged by this effort.

## 6. Risks and open questions

- **Recompiler fit**: the game image and overlays are runtime-loaded; if the recompiler cannot take raw images
  at a vram, write a small adapter or fall back to an emulator-side harness (a scriptable emulator with memory
  hooks gives Phase 3 without Phase 1/2). Decide after Phase 0.
- **Two images at one address**: dispatch by load state; both sets recompiled; a function name is only unique
  per image (same rule as the scorer's `--targets`).
- **Host compile of game C**: the matched sources use N64 types and IDO-isms (`-Xcpluscomm`, `M2C_FIELD`
  views); Astra's packets already compile bodies on the host with UBSan, so the surface exists but is per-packet
  today. Phase 2 generalises it.
- **Owner decisions**: whether recompiled C derived from the ROM may ever be committed (recommended: no, keep
  it generated); whether "correct, not matched" bodies may be promoted into the port build by default; whether
  the port gets its own repo once Phase 4 starts.
- **Legal**: the user supplies the ROM; nothing ROM-derived is distributed. Same model as Zelda64Recomp.

## 7. First concrete steps

1. On rocky: clone/build N64Recomp and the runtime; copy the ROM by hand; `tools/recomp/VERSIONS.md`.
2. `tools/recomp/gen_config.py` from `asm/us/*/extents.json` + `symbols.json` + the stream constants.
3. Boot the static code and the game image; then the overlay hook.
4. Function replacement with ten matched functions; then the differential harness on world_effect_update.

## 8. Phase 0 findings (lane w20r, 2026-10-11)

Detail, formats and commands: `tools/recomp/README.md`; versions: `tools/recomp/VERSIONS.md`.

- **Acceptance met with committed, reasoned exceptions.** `tools/recomp/gen_config.py` (stdlib,
  deterministic, refuses a missing/wrong-SHA-1 ROM) writes one N64Recomp symbol-file config for all
  four populations; N64Recomp (ffb39cda) consumes it with no hand edits and exits 0 in 0.2 s on rocky.
  Emitted: game **1216/1216**, image A **193/193**, image B **49/49**, static **578/668** + 10
  provisional. The 90 static functions not emitted are by design: 47 on N64Recomp's
  reimplemented list and 39 on its ignored list (libultra; matched by our canonical names), and 4
  we ignore (`__fixdfdi`/`__fixsfdi`: `trunc.l.*` unsupported; `__osSendInterrupt`: branches into
  another function; `osCauseGet`: mfc0 Cause). 4 more are emitted as stubs (cache / TLB ops).
  All 41 emitted C files compile with clang 21 against the runtime's `recomp.h`.
- **Recompiler fit (risk in §6): no adapter beyond a synthetic ROM.** N64Recomp cannot take a raw
  image at a vram, but its symbol-file mode reads each section from `rom` offsets of one ROM file,
  so the generator appends the inflated game image and images A/B to an unchanged copy of the ROM
  (`build/recomp/rom/rush2049.recomp.z64`; the static side keeps ROM 0x1000). No ELF synthesis.
- **Two images at one vram:** two *non-relocatable* sections at 0x8038A400 in the same run.
  In-image calls bind statically; the one call from outside into both images
  (`input_aux_handler` -> 0x8038A400) is emitted as a runtime lookup, i.e. dispatch by whatever
  librecomp has `load_overlays`-ed. Relocatable sections would make N64Recomp refuse such calls.
  Overlay functions carry an `ovl_a_`/`ovl_b_` name prefix.
- **Static side: symbol-file mode, not the ELF** (one run cannot mix modes; the libultra handling is
  by name either way).
- **Inventory gaps the recompiler exposed** (no coverage change; recomp-only):
  (1) hand-written codec kernels past the static `text_end` 0x800277D0 (the open BT08
  census-boundary targets 0x800277D0, 0x800278C4, 0x80027B44, 0x8002C5E0 and their `bal`/`jalr`
  helpers): 10 provisional extents in `tools/recomp/extra_functions.json`; they read PC with a
  never-taken `bltzal $zero`, rewritten to `lui/ori ra` by instruction patches; (2) a game function
  at 0x80104704 inside an 860-byte opaque run (called from `func_80105480`), which N64Recomp
  recovered as `static_1_80104704`.
- **One N64Recomp bug worked around:** a load into `$zero` (`hud_render` 0x800CB408) is emitted as
  `0 = MEM_W(...)`; the generator nops such loads automatically.
- **Phase 1 needs:** build N64ModernRuntime (+ RT64) against the same N64Recomp commit (the runtime
  pins 81213c18); an overlay hook at the routine that inflates the D_8002B020/D_8002B024 streams that
  calls `load_overlays(0xCA0000 or 0xCD0000, 0x8038A400, size)` (and `unload_overlays`), and the same
  for the game image (`load_overlays(0xC00000, 0x80086A50, 647072)`) after boot inflates it;
  host versions of `__fixdfdi`, `__fixsfdi`, `osCauseGet` and the 20 ignored libultra internals our
  code still calls (list in the README), or ignoring their Pfs/controller-pak callers; a decision on
  the provisional kernels (they use `jalr` into computed addresses, so their extents must be right
  for lookups to succeed at runtime).

## 9. Phase 1 findings (lane w21r, 2026-10-11)

Detail and commands: `tools/recomp/README.md` "Phase 1", `tools/recomp/BOOT_LOG.md`; versions and
the N64Recomp pin decision: `tools/recomp/VERSIONS.md`.

- **How far it boots (rocky, headless):** entrypoint -> `main` -> idle/game/scheduler/audio/render
  threads -> game image inflated and loaded -> image A (front end) loaded -> front-end main loop
  (30 gfx tasks/s, audio tasks at 60/s, 57 asset inflates, stable for 90 s). With scripted
  START/A presses, image B (race) is loaded 23 s in and display lists grow to ~14 KB (a 3D scene);
  stable for 120 s at 30 frames/s. Not shown: pixels. The host uses a null renderer (RT64 not
  built: no SDL2/window on rocky); "draws" = display lists submitted and buffers swapped. Whether
  the image-B run is a player race or an attract demo is not confirmed without pixels (no input ->
  no image B in 90 s, which points to the race).
- **Runtime:** N64ModernRuntime cdf5abbd (its pinned N64Recomp 81213c18 headers) links unchanged
  with C emitted by N64Recomp ffb39cda: the two commits between them only reorder
  `--dump-context` output. Host = `tools/recomp/runtime` (CMake subproject build, ~15 s with 20
  jobs).
- **The blocker was naming, not the recompiler.** N64Recomp/N64ModernRuntime replace libultra
  by name; the static inventory misnames ~40 libultra functions (ultralib VERSION_J: swapped
  `osSendMesg`/`osJamMesg`, `osViSetMode` = `osViSwapBuffer`, `osSetEventMesg` =
  `osCreateViManager`, the Pfs/Motor family shifted by one, ...). `tools/recomp/identity.json`
  renames 43 functions for the recompiler only (no inventory or coverage change); the static
  inventory names are worth correcting in the project itself (separate decision).
- **Overlay/game-image loading:** one `[[patches.hook]]` at `inflate_entry` entry calls
  `load_overlays`/`unload_overlays` by stream ROM offset; the recompiled inflate itself runs
  (the provisional kernel extents are right: image A and B inflate bit-exact, the game image
  differs only in 7 data words the game has written by the time it is checked).
- **Runtime gaps found in N64ModernRuntime:** no `osPiReadIo_recomp` (reimplemented name);
  `osStopThread` on another thread is an assert/no-op (the game stops and restarts threads; a
  re-start then corrupts the run queue); `thread_queue_remove` only removes a queue head; an idle
  `for (;;) {}` needs `pause_self` (cooperative scheduling). All handled in the host, none
  patched upstream.
- **Stubbed per function (host side):** `__fixdfdi`, `__fixsfdi`, `osCauseGet`, `osPiReadIo`,
  `osStopThread` (host C); inert: `__osSendInterrupt`, `__osCleanupThread`, `__osSetCompare`,
  `__osSet/ResetGlobalIntMask`, `__osViSwapContext`, `__osSpDeviceBusy`, `__osSpSetStatus`,
  `__osEPiRawReadIo/WriteIo`, `__osSiDeviceBusy`, `__osSiGetAccess/RelAccess`,
  `__osSiRawStartDma`, `__osPackReadData`, `__osContAddressCrc`; no-pak: `__osContRamRead/Write`,
  `__osPfsRWInode`, `__osPfsSelectBank`. Audio RSP tasks complete without microcode (silence).
  No inert shim was reached in 120 s.
- **Phase 2 (function replacement) needs:** (1) a build mode that compiles matched C for a
  chosen function set on the host and links it instead of the recompiled function: N64Recomp's
  `[patches]` `ignored` + a host `extern "C" void NAME(uint8_t*, recomp_context*)` wrapper that
  unpacks o32 arguments from `ctx` (the `runtime_contract` + `check_contract.py` machinery
  already enforces "every dropped name is defined"); (2) an rdram view for host-compiled game C
  (pointers are N64 addresses; the runtime stores rdram word-swapped, so struct field access
  needs MEM_* accessors or a byte-order-aware translation; this is the main cost); (3) a frame
  oracle: deterministic input script (`--press`) + per-frame state hashes (rdram regions, display
  list bytes) to compare recompiled vs replaced runs; the null renderer already sees every
  display list; (4) optionally RT64 on a machine with a display for visual checks.

## 10. Phase 2 findings (lane w22r, 2026-10-12)

Detail and commands: `tools/recomp/README.md` "Phase 2"; lane notes: `cloud/work/frontier/w22r/RESULTS.md`.

- **Acceptance met.** Ten matched functions replaced (8 compiled by IDO, 2 host-native) -> EQUIVALENT
  over a 60 s (1800-frame) frame-clocked script, each replaced function called 3 to 138,946 times; an
  injected bug (render_helper `>` flipped) -> DIFFERENT at frame 3 with the flag word and the extra
  display list named; the draft `w21h/engine_sound_sync/best.c` runs and is DIFFERENT at frame 706 (its
  first race frames: effect/voice counts D_8011024C, D_80110288 and a free list differ), so it is not
  "correct, not matched" yet; `w21h/func_800D7634/best.c` is not reached by the script.
- **Replacement design: compile the C for the N64, not the host.** The matched sources use struct views,
  32-bit pointers in rdram and globals; the runtime keeps rdram word-swapped. Rather than an accessor
  layer, `replace.py` compiles the unchanged source with the project's IDO, links it with `ld.lld` at
  0x80800000 against the inventory's addresses and hands it to N64Recomp as a fifth section; an entry
  hook delegates the original function to it. Exact N64 semantics, any population, no source change;
  678 of 689 matched `src/blob` files built this way as they are in the first pass (10 of the 11
  failures were `#pragma intrinsic(fabsf)` declarations, which the builder now ignores; one calls
  `strcpy`, which has no inventory address). A host-native mode with generated o32 wrappers exists for leaf bodies that touch
  aligned words through pointer arguments (proved on two); it is the port's long-term path but needs an
  accessor layer or a source transformation for everything else.
- **Determinism needed lockstep, not just a frame clock.** ultramodern delivers VI/AI on a 60 Hz wall
  clock and SP/DP from host threads; the game's front-end animations follow VI phase, so two identical
  runs diverged (display lists) by frame 93-1232. Delivering VI/AI from the idle thread, completing RSP
  tasks synchronously, modelling the framebuffer swap and virtualising osGetCount/osGetTime (link
  `--wrap`, no runtime patch) gives identical display lists in all 1800 frames across runs, at 1.3 s per
  run (vs 60 s real time).
- **Limits.** One input script (front end + one minute of a race); thread stacks are masked; three pages
  of ultramodern host state are never judged; a function-local static of a replaced body lives in the
  replacement image (the all-678 run shows the one case where that is visible: func_800E7710 sends its
  static's address as the SI message). Native mode: see README.
- **Phase 3 needs:** per-function capture (arguments, pages read/written, return) at entry/exit of the
  replaced function in both builds, so a DIFFERENT can be attributed to the first differing call rather
  than the first differing frame; more input scripts (menus: audio options, multiplayer, every track);
  a word-level mask for the host-pointer fields in OSThread instead of whole pages; data-placement aware
  binding of function-local statics (use the linked game image's map). **Phase 4:** RT64 in place of the
  null renderer keeps lockstep available as a test mode only.

## 11. Phase 3 findings (lane w23r, 2026-10-12)

Detail and commands: `tools/recomp/README.md` "Phase 3"; lane notes: `cloud/work/frontier/w23r/RESULTS.md`;
how a lane reads a verdict: `cloud/work/frontier/WAVE_BRIEF.md` "Behavioural check".

- **Per-call verdicts.** Both builds log every call of the replaced functions: entry arguments (from the
  prototype, or explicit registers), return registers, and the call's effect on rdram (words changed
  between entry and exit) found by page write tracking (`mprotect` + SIGSEGV pre-write copies; 10x run
  time). The stock side is a capture build of the same set whose hooks re-enter the original. `oracle.py`
  names the first differing call (function, its call number, frame, field, enclosing replaced call) and
  lists the effect's differing words. Injected bug -> render_helper call 2 (frame 2); engine_sound_sync
  draft -> its call 1410 (frame 705), and with callee probes -> its first entity_transform_calc call:
  retail passes the four values in callee-saved f22-f28 (IDO -O3 IPA), the standalone draft passes them
  o32. With w23f's oracle source (static copies of the IPA callees) the same body is EQUIVALENT on all four
  scripts (3,599 calls identical): the draft's logic is right, the standalone compile was not.
- **Scripts**: race, race_track2 (track selector), two_player (player-count selector + controller 2),
  audio_options (pause -> audio options). func_800D7634's draft is EQUIVALENT under audio_options (435 calls).
  Menu layout found without pixels by probe call counts and RAM evidence (inflated stream sets,
  active_player_count).
- **Static binding** from the retail image (the replaced function's own HI16/LO16 pairs): func_800E7710's
  false difference is gone. A second Phase 2 bug surfaced on the way: game bodies must call libultra by the
  game-side legacy names (game `osJamMesg` = static osSendMesg); Phase 2 bound them by static names, which
  was the all-matched divergence and func_800D7634's apparent DIFFERENT. Now all 689 matched game files
  replaced at once: no judged frame difference on four scripts, 4.24 M captured calls identical (race).
- **OSThread**: only `context` (8-byte host pointer at +0x20) differs between runs; masking those words
  makes all 2048 pages judged every frame.
- **Limits**: a draft calling IPA-convention callees needs an oracle source (no automatic detection yet:
  `ipa.py` reads derived asm whose FP ops are `.word`); probes on functions a replacement inlines are
  skipped by `--probe-callees` but not for explicit probes; effects into callers' stack frames below the
  outermost replaced function are not judged; capture costs 10x.
- **Phase 4 needs**: RT64 (pixels) in place of the null renderer, keeping lockstep as a test mode, so menu
  scripts can be written and checked visually; an IPA-aware replacement build (compile a draft inside its
  whole-program unit, or generate register-passing shims from the frontier's `register_params`); scripts
  for every track/mode reached by probe evidence; capture cost (write tracking per window costs two
  8 MiB mprotects per call) if it is to run on every lane request.


## 12. Phase 4 findings (lane w24r, 2026-10-12)

Detail and commands: `tools/recomp/README.md` "Phase 4"; lane notes: `cloud/work/frontier/w24r/RESULTS.md`;
versions and the package pins: `tools/recomp/VERSIONS.md`.

- **IPA-aware replacements (§11 "Phase 4 needs", item 2): done as register shims, not whole-unit compiles.**
  The fifth-section design compiles one TU, so it never carries a callee's IDO -O3 convention (proved: the w21h
  engine_sound_sync draft with no IPA table reproduces the frame-705 call difference). `ipa_callees.json` +
  replace.py now give such callees generated o32->IPA shims (saving every callee-saved register involved) or link
  the inlined body of a `jr ra` stub; callees detected by `ipa_members.json`/`frontier.json` but not described
  are refused unless `--allow-ipa` (flag name shared with w24t's detection lane). The w21h draft, unchanged, is
  EQUIVALENT on all four scripts (3599 calls identical): the "correct, not matched" verdict w23f needed an oracle
  source for is now produced by the standard build.
- **RT64 as a test mode (item 1): built, not yet seen.** `RUSH_RENDERER=rt64` (RT64 23cab603, the Zelda64Recomp
  pin) adds a window, RT64 and live input; null stays the default and the oracle reference; lockstep works with
  either renderer (RT64 mirror + 60 VI/s pacing). Configures/compiles/links on rocky; the first pixels come from
  the gameplay-test package on the owner's Windows 11 laptop.
- **Portability:** call interception by compile-time renaming instead of `ld --wrap`; per-call capture gated
  (`RUSH_CAPTURE`, POSIX); host sources parse against Windows headers. Windows build/run unverified by us.
- **Scripts (item 3):** the track selector cycles four tracks (race, race_track2/3/4 cover all); no attract demo
  race in 5 minutes of idle front end (`attract.press`).
- **Gameplay-test package** (owner decision 2026-10-12): `package.py` -> `rush2049-recomp-test-v<N>.tar.gz`;
  every later recomp phase regenerates it, bumps N (`VERSIONS.md` package-pins) and adds a CHANGES section.
- **Package v1 laptop report (2026-10-12, Windows 11, RTX 3050, clang-cl 19; `tools/recomp/reports/v1/`):** the
  Windows build works end to end (A5 must target `N64RecompCLI`, not `N64Recomp`). **Under RT64 the game renders
  correctly in lockstep (`--clock frames`)**: title, every front-end menu, four tracks, 1P and 2P split screen,
  pause/audio/video menus, 30 gfx tasks/s, 0.36 host cores; the owner raced a lap with an Xbox 360 pad ("the game
  seems whole, and working"); no crash in any run. **Wall clock + RT64 hangs at boot every time (13/13, D3D12 and
  Vulkan)**: `game_loop` 0x800FD500 is `if (state_word_a != state_word_b) { while (__osScTaskCount) ; ... }` — the
  mode-switch drain spins on the scheduler's task count (0x8002EB70, the +0x290 slot w24s found) without a libultra
  call, and ultramodern switches threads only inside libultra calls, so RT64's asynchronous DP completion never
  gets to run `__scHandleRSP`. Lockstep completes SP/DP synchronously (lockstep.cpp), the null renderer is
  effectively synchronous, so neither hangs. Fix = a `before_vram` hook at 0x800FD500 calling `pause_self`
  (same recipe as `rush_hook_park_idle`). RT64 lockstep self-check: DIFFERENT at frame 2 in 71 pages =
  0x807DD000–0x807FFFFF (framebuffer write-back, expected) and 0x8017A000–0x8019DFFF (36 pages right after
  `gBssEnd`, unnamed; name via the task's `output_buff`/z-buffer and mask). Doc/privacy fixes for v2: librecomp's
  `select_rom` copies the ROM into every `--home` dir (T9 "zip run\" would ship it; the tester excluded it by
  hand); T6 has rows/values keys swapped; A5 target name; attract and stunt untested.
- **Phase 5 needs:** the tester's report (does RT64 draw Rush 2049? which GBI/ucode; input mapping; FPS); audio
  (recompile the audio RSP microcode with RSPRecomp, as Zelda64Recomp does for aspMain); a Windows build check on
  real hardware (CI or the laptop) feeding fixes back; Windows per-call capture (VirtualProtect + vectored
  exception handler) if lanes need it there; IPA table entries for every other `register_params` callee a draft
  calls (the build now refuses them by name); cheaper capture (only re-protect pages dirtied since the last
  window instead of two 8 MiB mprotects per call); inbound IPA shims exercised on a real replaced IPA callee.


## 13. Phase 5 findings (lane w25r, 2026-10-12)

Detail and commands: `tools/recomp/README.md` "Phase 5"; lane notes: `cloud/work/frontier/w25r/RESULTS.md`;
tester input: `tools/recomp/reports/v1/REPORT.md`; package v2: `tools/recomp/VERSIONS.md`, `package/CHANGES.md`.

- **Wall-clock boot hang: fixed by yielding in libultra-free busy-waits.** `game_loop` 0x800FD500 spins on
  `__osScTaskCount` (D_8002EB70); ultramodern switches threads only inside libultra calls, so RT64's late DP
  completion never reached the scheduler thread. A scan of all four populations found 14 such game loops (7 more
  drains of the same counter, a retrace wait in dma_wait_complete, two in wheel_params_set, one in
  func_800E7038, two in the static sound code); all get `rush_hook_spin` (one `yield_self_1ms` turn, no-op in
  lockstep). Reproduced and verified without a GPU: `--null-dl-delay` (null renderer) and RT64 itself on Xvfb +
  lavapipe in a container on rocky (`gfx_tasks=2` without the yield, boot + menus + race with it). Three sites
  are hit in a scripted race (0x800FD500 at boot, 0x8038BA20 in the front end, 0x80096184 at the race start).
- **RT64 self-check explained.** The second differing range (36 pages after gBssEnd) is the z-buffer
  (G_SETZIMG 0x8017AEC0, 320x220x2); the first is framebuffer 1 (0x807DDA00; framebuffer 0 at 0x807BB3C0). Named
  in `runtime_buffers.json` (datasyms only). `oracle.py compare --mask-renderer` / host `--mask-rdp-images` judge
  the rest: RT64 lockstep race vs null `EQUIVALENT except renderer-written pages over 1800 frames`. Found on the
  way: lockstep + RT64 could crash (the game rebuilt a display list RT64 was still reading); now the game thread
  waits for the renderer to take each mirrored list.
- **Audio.** Rush's audio ucode is its own per-voice synthesiser, not aspMain; RSPRecomp handles it with no
  extra branch targets once the text size stops at the code end (0xF40 bytes at IMEM 0x1080). Audio tasks now
  run it (wall: runtime task thread; lockstep: synchronously). A second gap: the game feeds the AI one 192-frame
  buffer per AI interrupt (115/s), ultramodern sends one AI message per VI; paced AI messages make the game
  produce 22.05 kHz of audio (headless 45 s: 1.98 M samples, 84 % non-zero; SDL disk driver: continuous output).
  Lockstep verdicts unchanged in kind: stock self-check and draft_engine_sound_sync on race, race_track2,
  two_player, audio_options all EQUIVALENT (3599 calls identical each) with the ucode running.
- **Attract mode exists** (corrects §12): idle, the game waits on the boot notice NO CONTROLLER PAK DETECTED /
  CONTINUE WITHOUT SAVING (silent); after one START it cycles the title (5 s) and demo races (~30 s) on the four
  tracks in order. `attract.press` presses that START; stunt mode verified with pixels (RT64 on lavapipe:
  stunt arena, score box, 4:40 timer).
- **Package v2** (process unchanged): A5 builds `N64RecompCLI` + `RSPRecomp`, A6 runs RSPRecomp; the runtime
  deletes librecomp's ROM copy from `--home` once loaded; TEST_PROCESS: wall clock by default (`--clock frames`
  fallback), T6 keys fixed, T2 5-minute attract, T6b stunt (`stunt.press`: SELECT MODE DOWN x4 loads race data
  0x812BE0), T7a pad mapping (`--log-input`), sound in every step, T8 with `--mask-renderer`.
- **Phase 6 needs**: the v2 tester report (wall clock on a real GPU, sound by ear: pitch, sync, crackle; stunt
  and attract pixels; pad table); audio pacing on a real device (the SDL-queue feedback thresholds are untested
  on hardware); whether any other game thread starves in wall clock (the census covers loops without calls, not
  long call-free computations); a Windows CI or laptop build check of the RSPRecomp step and `-march=nehalem`
  under clang-cl; saves/controller pak; the other `register_params` IPA callees; cheaper capture.

## 14. Package v2 laptop report (2026-10-12, Windows 11 25H2, RTX 3050, clang-cl 19.1.5; `tools/recomp/reports/v2/`)

- **Both v1 blockers are closed on real hardware.** 17 wall-clock windowed runs plus the T8 RT64 lockstep run: 0 hangs,
  0 crashes, 30 gfx tasks/s (worst second 27). Four of the 14 spin hooks fired: 0x800FD500 (boot, every run),
  0x8038BA20 (front end), 0x8038B4E8 (track selector, each time the track changed; not in CHANGES.md's examples),
  0x80096184 (race start). `--clock frames` was never needed outside T8.
- **Sound judged good by ear**: title, menu, countdown, engine following speed, race music, split-screen, stunt; stereo,
  in sync, no pops. The T3 recording is real time (173.8 s of audio in a 175 s run) with gaps only on the silent boot
  notice and the pause screen. All four audio options measurably change the output (effects 0 and music 0 each drop
  ~2.5 dBFS, BATTLE2 is a different tune, MONO makes L and R sample-identical). So the "lockstep makes ~82 % of expected
  audio" open item is a lockstep/headless question, not a wall-clock one.
- **T8 on a real GPU**: a vs b `EQUIVALENT over 1800 frames`; a vs c `DIFFERENT at frame 2: 71 pages` from 0x8017A000;
  with `--mask-renderer` `EQUIVALENT except renderer-written pages`, 105 pages masked. Same verdicts as rocky's
  lavapipe run, so the runtime_buffers.json mask is right for RT64 on D3D12 too.
- **Coverage by a human**: boot notice (A also passes it), title, attract (8 demo cycles, credits demo from ~223 s),
  1P single race with pause/resume, tracks 2–4, 2P split screen (pad as player 2), audio options, stunt (score,
  bonus text, controls page), pad + keyboard tables all correct. Not yet played: Practice, Ghost Race, Circuit,
  Battle, other cars, BACKWARD, RECORDS, T7 free play (owner skipped).
- **Small findings for Phase 6**: `controller -> player N` printed twice per pad (open + device-added reopen);
  left-stick dead zone steps 0 → 0.15 with no rescaling; Back and L3/R3 unmapped (by design, document it);
  Vulkan path boots and plays too (`RT64 setup: ok, graphics API Vulkan`); RSPRecomp and `-march=nehalem` under
  clang-cl verified (A5/A6/A8 clean, 645 steps, 18 benign warnings); N64Recomp prints `Ambiguous jal target
  0x8038A400 in function input_aux_handler` (info only). Load: 0.55 cores, 452 MB working set, 12.5 % of the 3050.
- **Phase 6 therefore**: gameplay is no longer the gate. Open items are saves/controller pak (the boot notice is the
  first thing every player sees), the untested modes above, controller polish (dead-zone rescale, duplicate line),
  the remaining `register_params` IPA callees, cheaper capture, and the lockstep audio shortfall. The differential
  harness (Phase 3) is now trustworthy on a real GPU with `--mask-renderer`.

## 15. Phase 6 findings (lane w26r, 2026-10-12)

Detail and commands: `tools/recomp/README.md` "Phase 6"; lane notes: `cloud/work/frontier/w26r/RESULTS.md`;
package v3: `tools/recomp/VERSIONS.md`, `package/CHANGES.md`.

- **Saves: the controller pak works.** The runtime's osPfs* are "no pak" stubs; the host now implements them
  (`runtime/src/pak.cpp`, calls renamed at compile time) over a 32 KiB `.mpk` image in `--home` (lockstep: a fresh
  in-memory image, never written). The game uses no EEPROM/SRAM/flash. The boot notice is gone (Midway logo ->
  title -> attract by itself); PLAYER 1 offers CREATE PLAYER (ENTER YOUR NAME) -> note "RUSH2049"+name, 9 pages,
  statistics written at race start and on quit; a restart lists the player (PLAYER 1, RECORDS, "AAAA PAUSED").
  Verified headless lockstep and wall clock, RT64 lockstep pixels, RT64 wall clock (loads the saved note). With a
  controller pak the game shows "INSERT RUMBLE PAK NOW" at race start (pak swap prompt; faithful).
- **Scripts declare the pak** (`pak on|off`): all pre-Phase-6 scripts say `pak off` and keep their frames and
  verdicts; 11 pak-flow scripts cover practice, ghost race, circuit, backward, another car, records, battle (2P),
  saves/reload, the default race and attract. All reach their mode (RT64 pixels) without a hang; the wall-clock
  sweep (null renderer, 25 ms DL delay, 16 scripts) hits only known busy-wait sites, and a new run-burst census
  (longest game-thread run between message calls or yields) is <= 1.1 ms after boot: no starving computation.
- **Lockstep audio "82 %" was a measurement artifact** (the last per-second status line before the frame stop);
  the output is 100.0 % of 22050 Hz (WAV 59.986 s for 60.00 virtual s); lockstep stops now print it.
- **Lockstep + RT64 could still crash** when a software GPU took > 10 s for a frame (the timeout went on while
  RT64 read the list); the game thread now waits without a timeout.
- **Pad**: one `controller ->` line per pad; dead zone rescaled; Back/Guide/L3/R3 unmapped on purpose.
- **Cheaper capture**: lazy re-protection, 333k-call capture 21.0 s -> 15.5 s, identical call log.
- **ess draft regression**: EQUIVALENT + all calls identical on race, race_track2, two_player, audio_options
  and the pak-flow saves (8999 calls) and battle (5399 calls) scripts.
- **Package v3** built and sanity-tested from a fresh extract (INSTALL B, RT64 in the container).
- **Phase 7 needs**: the v3 tester report (saves by hand, the new modes by ear and eye, the dead zone); IPA
  table entries for the remaining `register_params` callees (each needs its prototype and register mapping from
  a call site; not automatable soundly); rumble (the game supports a rumble pak; the host has no rumble output
  and reports a controller pak); a second pak / players 2-4 paks (only controller 1 has one); GAME OVER menu
  input in scripts (DOWN did not move it in w26r's scripts); capture cost beyond the page faults (the per-call
  log formatting); Windows per-call capture.

## 16. Phase 7: decomp feedback (lane w27r, 2026-10-12)

Owner decision 2026-10-12: recomp time goes to decompilation feedback, not port polish; no laptop test is waited
for. Detail and commands: `tools/recomp/README.md` sections "draft_check", "regmap_verify", "Call census"; lane
notes: `cloud/work/frontier/w27r/RESULTS.md`; package v4: `tools/recomp/VERSIONS.md`, `package/CHANGES.md`.

- **draft_check**: one command (`python3 -m tools.recomp.draft_check [--remote rocky] NAME draft.c`) builds the
  retail capture side and the draft side of one function (replace.py `ido` mode, IPA shims, `allow_ipa`),
  incrementally per function (generated files synced by checksum into a persistent build tree: ~30 s first, ~3 s
  after), runs the census-chosen lockstep scripts in parallel with the retail logs cached per (binary, script),
  and prints EQUIVALENT over N calls / K scripts (worded as behaviour only, NOT a match), the first divergence
  (script, frame, call number, argument / return / memory words, retail vs draft) or NEVER CALLED. Works from
  the Pi (draft, its local headers and include/ copied to rocky's inbox; service tree `~/rush2049/recomp/dc/repo`).
  Register-parameter functions need `--params` (inbound shim + capture of those registers); callees that write
  callee-saved registers unsaved get automatic save shims (found on the first real draft: func_800D7634 calls
  credits_screen and drone_pathfind_main o32 otherwise).
- **regmap_verify**: live-in registers of the retail body (ipa_detect dataflow, now with the o32 argument
  registers and forwarded arguments), every static `jal` site with the instruction that set each register, and a
  capture of every register at every call with the call site from `$ra` (N64Recomp does not write `$ra` on a
  call; `gen_config --extra-hooks` sets it at each static site). First use: func_800A1DD4 (w27e's claim `a0,t3`)
  is refuted: a1 (buffer pointer) and a2 (size) are live-in too, t3 = 0/1 by caller.
- **Census**: 167 of 218 unmatched game functions reached by the 19 scripts; 51 (52,660 B) never: results and
  end-of-race screens, high-score entry, replay save, controller/save settings, attract demo/video handlers, much of
  0x8010A000-0x8010F000. `stunt_end.press` (stunt until the timer ends) reaches two more.
- **Capture**: up to 48 locations per call (was 12); `ra` and `at` are capture registers.
- **Package v4**: no gameplay change (same pins); the new tools and script.
- **Phase 8 needs**: scripts that finish a race (results, high score entry, replay save prompt: steering by
  script or a cheat/time-limit lever) and reach the options screens (controller settings, save settings) and the
  attract video/demo handlers; draft_check for drafts that are a whole IPA unit (several functions replaced at
  once, each with its own convention); overlay functions (both images at one vram: per-mode scripts); a
  `--probe-callees` option in draft_check to localise a divergence inside the draft; a cache-cleanup policy for
  `~/rush2049/recomp/dc` (trees ~250 MB per function); refreshing the service tree when wave27 moves (the
  coordinator's rsync line in the README); the v3/v4 tester report when the owner has time.

## 17. Phase 8: decomp feedback, round two (lane w28r, 2026-10-12)

Owner decisions 2026-10-12: recomp time goes only to decompilation feedback; laptop/gameplay tests gate nothing
(no package this phase: no gameplay change). Detail and commands: `tools/recomp/README.md` "Phase 8" and
"script_probe"; lane notes: `cloud/work/frontier/w28r/RESULTS.md`.

- **No more hand-written `--callee` for the known conventions**: `ipa_callees.json` entries for slot_state_setup
  (s2 / s0,s1,s3), sound_update_channel (t0), func_800B6748 (a0-a3,t0-t2), func_80096CA8 (s8,t0 / s0-s7),
  func_80097164 (s8), func_800A1DD4 (a0,a1,a2,t3), each with its evidence.
- **Interprocedural live-in**: a register parameter can stay live across a call to a callee that does not touch
  it (sound_update_channel reads t0 after its first call); regmap_verify now follows static callees, and the
  five known mappings are CONSISTENT (sound_update_channel was wrongly REFUTED before).
- **`--probe-callees`** derives a callee's convention (live-in set, kinds from a capture, unsaved callee-saved
  registers) and prints it in `--callee` syntax; parameter order and return type stay the lane's call (the call
  sites' setter order is scheduled, not prototype order). draft_check suggests one for every register-parameter
  callee it had to call o32.
- **`draft_check --unit`**: a cluster draft (one TU) replaces all its members at once in one chunk; calls between
  members stay inside the unit; both sides record only the outermost member call per thread; the first
  divergence names the member. w27e pak cluster: 4 members EQUIVALENT over 684 calls once func_800A1C6C's
  mapping was read off the first divergence (handle=a0, address=t2, bytes=a1, offset=t1).
- **script_probe + host `--text-log`**: the strings the game queues for drawing (menu_input_process) per frame,
  plus which probed functions a script reaches: menus, results and name entry readable headless, so scripts can
  be written without RT64 screenshots.
- **dc cleanup policy** (`--gc`, automatic past 16 trees): keep the 12 most recently used function trees, the
  newest retail cache per function, 5 runs per function or 3 days.
