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
