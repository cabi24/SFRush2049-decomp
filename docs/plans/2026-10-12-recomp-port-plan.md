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
