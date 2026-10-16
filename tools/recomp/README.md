# tools/recomp: static recompilation (Phase 0 config, Phase 1 host, Phases 2-4 replacement, oracle, package, Phase 5 wall clock + audio, Phase 6 controller pak, Phases 7-12 decomp feedback)

Generates an [N64Recomp](https://github.com/N64Recomp/N64Recomp) configuration for Rush 2049 (US)
from the project's committed inventories and the private ROM. Plan:
`docs/plans/2026-10-12-recomp-port-plan.md` (Phase 0 findings at the end). Versions: `VERSIONS.md`.

Everything generated is **private** (derived from the ROM) and goes to `build/recomp/` (git-ignored
via `build/`). Committed here: the generator, its two curated inputs, the output checker, docs.

| File | What |
|---|---|
| `gen_config.py` | inventories + ROM -> `build/recomp/{rush2049.recomp.toml, rush2049.syms.toml, rush2049.datasyms.toml, rom/rush2049.recomp.z64, images/*.bin, manifest.json}` |
| `extra_functions.json` | PROVISIONAL extents for 10 boot-segment CPU routines past the static inventory (recompiler only) |
| `patches.json` | functions N64Recomp refuses and what the config does about each (stub / ignored / renamed / instruction patch), with reasons |
| `check_output.py` | per-section emitted counts, recompiler-invented statics, called-but-undefined names (the shim list) |
| `identity.json` | Phase 1: recompiler-only names for 43 static libultra functions whose inventory name is not their identity (evidence per entry) |
| `runtime/` | Phase 1 host: CMake project linking N64Recomp's output with N64ModernRuntime, null renderer, hooks, shims, `check_contract.py` |
| `BOOT_LOG.md` | Phase 1 commands and what each run reached |
| `replacements.json` | Phase 2: function -> C body (population, source, mode, state, note) and named sets |
| `replace.py` | Phase 2: builds replacements (IDO -> `.repl` section, or native host wrappers) for `gen_config.py --replace-set` |
| `oracle.py` | Phase 2: compares per-frame oracle logs (stock vs replaced): EQUIVALENT / DIFFERENT at frame F / NOT EXERCISED; Phase 3: the first differing CALL (`calls`, `run`), input scripts (`--script`, `script`) |
| `scripts/*.press` | Phase 3: input scripts for the oracle (one line each below) |
| `runtime/src/capture.cpp` | Phase 3: per-call capture (`--call-log`, `--call-detail`) |
| `ipa_callees.json` | Phase 4: how a standalone replacement calls IDO -O3 IPA-convention callees (register shims, inlined-stub bodies) |
| `runtime/src/rt64_renderer.cpp`, `sdl_frontend.cpp` | Phase 4: RT64 renderer + SDL2 window/input (CMake `RUSH_RENDERER=rt64`) |
| `make_rom.py` | Phase 4: synthetic ROM from the retail ROM + a config's `manifest.json` (stdlib; the package's ROM step) |
| `package.py`, `package/` | Phase 4: gameplay-test package `rush2049-recomp-test-v<N>.tar.gz` and its tester documents |
| `runtime/src/pak.cpp` | Phase 6: controller pak (osPfs* over a 32 KiB `.mpk` image under `--home`) |
| `runtime_buffers.json` | Phase 5: recomp-only datasym names for the z-buffer, framebuffers and gfx task buffers |
| `draft_check.py` | Phase 7: one-command behavioural check of a draft against retail (section "draft_check") |
| `regmap_verify.py` | Phase 7: register-parameter mapping verifier for internal callees (section "regmap_verify") |
| `census.py`, `census/` | Phase 7: which scripts reach which unmatched game functions (section "Call census"); Phase 11: `--overlays` (`census/overlays.json`, `OVERLAYS.md`: image A/B functions per script) |
| `script_probe.py` | Phase 8: what a script reaches and which strings the game draws (section "script_probe") |
| `runtime/src/fault.cpp` | Phase 9: `[rush] FAULT` report when the host faults (section "Phase 9") |
| `static_layout.py` | Phase 10: where IDO put a draft's statics (no symbols), for `--statics` by name (section "Phase 10") |
| `reports/vN/` | tester reports of package vN (text and logs only; screenshots stay on rocky) |

## Commands (rocky)

```bash
cd ~/rush2049/recomp/repo
python3 tools/recomp/gen_config.py --rom ~/rush2049/recomp/rom/baserom.us.z64 \
        --n64recomp-src ~/rush2049/recomp/src/N64Recomp
cd build/recomp && ~/rush2049/recomp/build/N64Recomp/N64Recomp rush2049.recomp.toml
cd ../.. && python3 tools/recomp/check_output.py --json build/recomp/check.json
# compile check (no link): every emitted file, runtime header only
cd build/recomp && mkdir -p cc_check && ls RecompiledFuncs/funcs_*.c | xargs -P 20 -I{} sh -c \
  'clang -std=c11 -O2 -c -Wno-error=implicit-function-declaration \
     -I ~/rush2049/recomp/src/N64ModernRuntime/N64Recomp/include {} -o cc_check/$(basename {} .c).o'
```

`gen_config.py` refuses a missing ROM or one whose SHA-1 is not `us.sha1`; `--n64recomp-src` is
required because N64Recomp's built-in name lists decide which names the config may touch. Output is
byte-identical across runs and hosts (Pi Python 3.11, rocky 3.12). Tests:
`python3 -m pytest tests/recomp -q -p no:cacheprovider` (fixtures, no ROM; the end-to-end test runs
only when `baserom.us.z64` or `$RUSH2049_ROM` is present, else skips "private input").

## N64Recomp input formats (from its source, commit ffb39cda)

**Config toml** (`src/config.cpp`). `[input]`: `entrypoint` (vram; optional), `output_func_path`
(required), exactly one of `elf_path` or `symbols_file_path`, `rom_file_path` (required with a
symbols file), `relocatable_sections_path` (text file, one section name per line, `*` = empty slot;
only shapes the `overlay_sections_by_index` table), `functions_per_output_file` (default 50; with 1
each function gets its own file *without* `#include "funcs.h"`, so cross-calls do not compile:
use >1), `manual_funcs` (`{name, section, vram, size}`, ELF mode only), `function_sizes`
(`{name, size}`, ELF mode only), `use_absolute_symbols`, `unpaired_lo16_warnings`, `use_mdebug`
(+ `mdebug_file_mappings`), `recomp_include`, `trace_mode`, `single_file_output`,
`func_reference_syms_file` / `data_reference_syms_files` / `allow_exports` / `strict_patch_mode`
(ELF-mode mod/patch builds only), `output_binary_path`. `[patches]`: `stubs` (names: body emitted
empty), `ignored` (names: not emitted, callers still call by name), `renamed` (names: emitted as
`NAME_recomp`), `[[patches.instruction]]` `{func, vram, value}` (one word replaced before
recompiling), `[[patches.hook]]` `{func, before_vram?, text}` (C text inserted).

**Symbol file** (`Context::from_symbol_file`): `[[section]]` with `name`, `rom`, `vram`, `size`,
optional `got_address`, `functions = [{name, vram, size}]`, optional `relocs = [{vram,
target_vram, type}]` (HI16/LO16/26/32; *the presence of a `relocs` key marks the section
relocatable*). Function words are read from the ROM file at `section.rom + (vram - section.vram)`
as raw big-endian words. In this mode N64Recomp applies its own name lists
(`src/symbol_lists.cpp`) before `[patches]`: `reimplemented_funcs` (116 names; renamed
`NAME_recomp`, not emitted, declared in funcs.h, the runtime implements them), `ignored_funcs`
(358; renamed, not emitted, *not* declared), `renamed_funcs` (83; emitted as `NAME_recomp`).
Consequence: those names must not appear in `[patches]` (the lookup by the original name fails),
and an instruction patch in a built-in-renamed function must name `NAME_recomp`. Data symbols are
not read in this mode (HI16/LO16 pairs are recompiled as plain address arithmetic into `rdram`).

**ELF mode** (`src/elf.cpp`): functions = STT_FUNC symbols in executable sections (sizes from the
symbol table or `function_sizes`), relocations from `.rel.*` for sections named in
`relocatable_sections_path`, data symbols for reference. Fits a linked ELF of the whole program.

**Data reference symbol file** (`read_data_reference_syms`): `[[section]]` `{name, rom?, vram,
size, symbols = [{name, vram}]}`; ELF mode only. We still emit one (`rush2049.datasyms.toml`) for
Phase 1 debugging and Phase 3 naming.

**Zelda64Recomp** (`us.rev1.toml`, dev branch) uses the same symbol-file mode on a *decompressed*
ROM (`mm.us.rev1.rom_uncompressed.z64`), a `relocatable_sections_path` list for its overlays (with
real relocs in its syms toml), a few `stubs`, one `ignored`, and `[[patches.instruction]]` fixes.

## Decisions

- **One symbol-file run for all four populations**, four sections: `.static` (boot segment, ROM
  0x1000-0x2F4E0 -> 0x80000400, 668 inventory + 10 provisional functions), `.game` (0x80086A50,
  647,072 B, 1,216), `.ovl_a` (0x8038A400, 194,128 B, 193), `.ovl_b` (0x8038A400, 43,888 B, 49).
  One context lets every cross-population `jal` resolve. Sections span the whole image (code and
  data) so jump tables, which N64Recomp reads through the function's own ROM mapping, are reachable.
- **Static side uses the symbol-file mode, not the ELF.** `build/us/rush2049.us.elf` would cover
  only the boot segment (one run cannot mix ELF and symbol file), and the libultra handling is by
  name in both modes, so the symbol file loses nothing: our canonical libultra names hit N64Recomp's
  lists directly (47 reimplemented, 39 more ignored, 23 renamed; `manifest.json` lists them).
- **Adapter: a synthetic ROM.** N64Recomp reads functions from one ROM file at section offsets and
  cannot take a raw image at a vram. `rom/rush2049.recomp.z64` = the retail ROM unchanged (so
  `.static` keeps ROM 0x1000 and the runtime's normal ROM DMA still works) + the inflated game
  image at 0xC00000, image A at 0xCA0000, image B at 0xCD0000 (64 KiB aligned). Same idea as
  Zelda64Recomp's decompressed ROM; no ELF synthesis needed.
- **Two images at one vram: two NON-relocatable sections at 0x8038A400**, not relocatable sections
  and not separate runs. Calls inside an image resolve to that image (same section); a call from
  outside to an address both images define is emitted as a runtime lookup (`LOOKUP_FUNC`), which
  librecomp answers from whichever section was last `load_overlays`-ed: dispatch by load state.
  A `relocs` key would make them relocatable, and N64Recomp then refuses calls into them from
  other sections (it only accepts non-relocatable candidates), so they stay non-relocatable.
  Overlay function names get an `ovl_a_`/`ovl_b_` prefix (both images start with `func_8038A400`;
  N64Recomp emits one C function per name).
- **Refusals are handled in committed inputs, never by editing generated files**:
  `patches.json` (4 stubs, 4 ignored, 5 renamed, 12 instruction words) and `extra_functions.json`
  (10 provisional routines). One rule is automatic: a load into `$zero` (N64Recomp emits
  `0 = MEM_W(...)`) becomes a nop; it fires once (`hud_render` 0x800CB408).

## Acceptance result (rocky, 2026-10-11)

N64Recomp exits 0; `Function count: 2136`; 0.2 s. Emitted (`check_output.py`):

| Section | In symbol file | Emitted | Not emitted (why) |
|---|---:|---:|---|
| `.game` | 1216 | **1216** | - |
| `.ovl_a` | 193 | **193** | - |
| `.ovl_b` | 49 | **49** | - |
| `.static` | 668 + 10 provisional | 578 + 10 | 47 reimplemented + 39 ignored by N64Recomp's lists; 4 ignored by us (`__fixdfdi`, `__fixsfdi`: trunc.l.d/s unsupported; `__osSendInterrupt`: branches into another function; `osCauseGet`: mfc0 Cause) |

Plus one function N64Recomp found itself: `static_1_80104704` (called from `func_80105480`; it sits
in an 860-byte opaque run 0x801046FC-0x80104A58 of the game inventory, i.e. a game function the
inventory misses). The 578 static functions include 4 stubs (empty bodies: `osInvalICache` (inventory `osInvalICache_full` before w22g),
`func_8002C5E0` (cache ops), `__osTlbFlush`, `__osTlbInit` (mfc0 EntryHi)), 23 + 5 emitted as
`NAME_recomp`, and `entrypoint` as `recomp_entrypoint`.

What the recompiler refused before the patches (each found by running it; it stops at the first):
`cache` (osInvalICache_full, func_8002C5E0), `trunc.l.d`/`trunc.l.s` (__fixdfdi/__fixsfdi), mfc0
cop0 10/13 (__osTlbFlush, __osTlbInit, osCauseGet), "Unhandled branch" (__osSendInterrupt), and
`scd` in a static it invented at 0x800277D0: the boot segment has hand-written codec kernels past
the static inventory (`text_end` 0x800277D0; the open BT08 census-boundary question) that use a
never-taken `bltzal $zero` to read PC; N64Recomp treats that as a call into the inline table.
Fixed by the provisional extents and by rewriting each `bltzal zero; nop` pair to `lui ra; ori ra`
(same `ra`, no call). No jump-table detection failures, no overlapping-section errors.

Recompiler messages (whole run): `[Info] Ambiguous jal target 0x8038A400 in function
input_aux_handler, falling back to function lookup` (the only direct game -> overlay call; it is the
designed dispatch), `[Info] Indirect tail call in recomp_entrypoint`, 5 `Tail call` infos (the
codec kernels' shared tail; `static_1_80104704` -> `highscore_entry_anim`). No warnings.

Compile check: all 41 emitted files compile with clang 21 (`-std=c11 -O2`, 2.6 s with 20 jobs) given
`-Wno-error=implicit-function-declaration`; without it they fail only on 20 undeclared callees, all
libultra internals on N64Recomp's *ignored* list that our non-ignored code still calls
(`__osPiRawStartDma` 6 callers, `__osContRamRead` 5, `__osPfsRWInode` 4, `__osPfsSelectBank` 4,
`osYieldThread` 4, `__osContRamWrite` 3, `__osSpDeviceBusy` 3, `__osSpSetStatus` 3, `__osSetCompare`
2, `__osSiRawStartDma` 2, `__osViSwapContext` 2, and 9 with one caller: `__osCleanupThread`,
`__osEPiRawReadIo`, `__osEPiRawWriteIo`, `__osPackReadData`, `__osResetGlobalIntMask`,
`__osSetGlobalIntMask`, `__osSiDeviceBusy`, `__osSiGetAccess`, `__osSiRelAccess`). Phase 1 either
provides these or ignores their callers (most are Pfs/controller-pak paths the runtime replaces).
`recomp_overlays.inl` and `lookup.cpp` compile against librecomp's headers. Other warnings: 4
`-Wpointer-sign`, 1 `-Wtautological-compare`.

## Limits found

- Data symbols are not consumed in symbol-file mode (the emitted C carries addresses, not names).
  `rush2049.datasyms.toml` keeps them (1,885 placed, 1,504 absolute; one conflicting name, `gstate`, at two addresses across the files,
  recorded in `manifest.json`).
- The 10 provisional extents and the `static_1_80104704` gap are inventory gaps, not recompiler
  limits; they do not change coverage numbers and must not be used for matching.
- `functions_per_output_file = 1` is unusable without a custom `recomp_include` (funcs.h is not
  included); we use 50.
- N64ModernRuntime pins N64Recomp at 81213c18, two commits behind the generator we ran (ffb39cda);
  Phase 1 should build the runtime against the same N64Recomp or confirm the emitted ABI matches.

## Phase 1: the host (lane w21r, 2026-10-11)

Commands: `BOOT_LOG.md`. Result: boots to the front end and, with scripted input, loads the race
image (B) and runs at 30 frames/s for 120 s; nothing is rasterised (null renderer).

- **Generator additions** (`gen_config.py`): `patches.json` `"hook"` -> `[[patches.hook]]`
  (C call inserted at function entry or `before_vram`); `identity.json` renames static functions
  in the symbol file only; `manifest.json` `runtime_contract` = `shims` (every name the emitted
  code or the section table references that N64Recomp does not emit: built-in reimplemented
  names in the run and called built-in ignored names as `NAME_recomp`, our `ignored` by name)
  + `hooks` + `images`; `rush2049_images.h` = compressed stream -> recompiled section table.
- **Why identity.json**: N64Recomp replaces libultra by NAME. Lane w21r found 43 static
  libultra functions named wrongly in the inventory (ultralib VERSION_J): e.g. `osJamMesg`/
  `osSendMesg` were swapped, `osViSetMode` was `osViSwapBuffer`. Without the right names the
  host replaces the wrong functions and the real ones poke hardware registers (boot segfaults
  in `osPiRawReadIo` in `main`). Lane w22g renamed them in the inventory itself
  (docs/SYMBOL_MISATTRIBUTION.md); identity.json keeps only the 11 whose recompiler name must
  differ from the canonical one: names ending `_rx` are deliberate non-canonical names for
  functions that must stay emitted (or be shimmed), and the PI raw DMA functions keep their
  pre-J names (`osPiRawStartDma`, `osEPiRawStartDma`) for N64Recomp's lists.
- **Hooks**: `rush_hook_inflate` at `inflate_entry` entry maps the stream ROM offset to the
  recompiled section and calls `unload_overlays`/`load_overlays` (game image 0xB0CB10 ->
  0x80086A50; A 0xB5C534 / B 0xB6FEC4 -> 0x8038A400, unloading whatever is there). The
  recompiled inflate (hand-asm kernels with the provisional extents) runs and is verified: at
  the next inflate the image in rdram is compared with the compiled one (A and B exact; game
  image 7 data words already written). `rush_hook_park_idle` replaces the idle thread's
  `for (;;) {}` with ultramodern's `pause_self` (cooperative scheduler).
- **Shims** (`runtime/src/shims.cpp`): HOST `__fixdfdi`, `__fixsfdi`, `osCauseGet`,
  `osPiReadIo_recomp` (reimplemented name the runtime lacks), `osStopThread_rx` (stopping
  another thread; the runtime only stops the caller); INERT/NOPACK for 19 libultra internals
  whose callers the runtime replaces (none reached in 120 s). `check_contract.py` (run by CMake)
  fails the build if a contract name is defined neither here nor in N64ModernRuntime.

## Phase 2: function replacement and the frame oracle (lane w22r, 2026-10-12)

**Replacement** (`replace.py`, `replacements.json`, `gen_config.py --replace-set SET | --replace ID,...`).
The original function stays in the recompiled set; a `[[patches.hook]]` at its entry counts the call
(`rush_repl_count`), calls the replacement and returns, so direct calls, `LOOKUP_FUNC` and the section
table all reach it. Two modes:

- `ido` (default): the source is compiled for the N64 by the IDO the project matches with (5.3 for
  game/overlays, 7.1 for the boot segment; flags from its `/* flags: */` header), linked by `ld.lld`
  (one object per chunk, `--gc-sections` from the function) at `0x80800000+` against the inventory's
  addresses, and appended to the synthetic ROM as a fifth N64Recomp section `.repl` (functions named
  `repl__<id>__<name>`). The runtime copies the image into rdram above the game's 8 MiB (librecomp
  commits 512 MiB; the game sees `osMemSize` 8 MiB) on the first replaced call. The body therefore runs
  with exact N64 memory semantics: no source change, struct views, 32-bit pointers, byte order and
  globals all as on the console. Globals the source *defines* are bound to the game's copy (a linker
  script assignment overrides the object's definition); function-local statics get a private copy
  (`private_data` in the manifest when global and unknown). Undefined symbols resolve by inventory
  name or by an address-derived name (`D_`/`func_XXXXXXXX`); others fail the build (IDO's
  `#pragma intrinsic` names with no relocation are ignored).
- `native`: the source is compiled for the host (clang, `-funsigned-char -fwrapv`) with the function
  renamed by `#define`, behind a generated `native__<id>(uint8_t* rdram, recomp_context* ctx)` that
  unpacks o32 arguments from the prototype (a0-a3 by 4-byte slot, 8-byte types in aligned pairs,
  slots >= 4 at `sp+16+`, f12/f14 only for leading float arguments) and stores the return (v0, v0:v1,
  f0). Pointer arguments become host pointers into rdram. **Limits** (why `ido` is the default):
  rdram is word-swapped (`MEM_B` = addr^3, `MEM_H` = addr^2), host pointers are 8 bytes and the game's
  globals and callees are not host symbols, so only leaf bodies that touch aligned 32-bit words through
  their pointer arguments are correct. The generator refuses pointers to 1/2/8-byte scalars, structs
  and pointers to pointers; a body naming a game global or callee fails to link. Pointers held in
  rdram structs are the hard case and are not attempted. Host libm calls (sinf, ...) would not be the
  game's.

**Frame oracle** (host `--clock frames --oracle-log PATH --frames N`, `oracle.py`). Frame = one graphics
task in the scheduler's `__scExec` (hook before `osSpTaskStartGo`, on a game thread with every other one
suspended). Per frame: hash of every display list walked from the task (F3DEX2/F3DEX `G_DL` calls and
branches, segments tracked), hash of each 4 KiB page of the 8 MiB (changed pages only), thread stacks
(`osCreateThread`) masked into a separate hash. `--clock frames` also makes the run deterministic
(`runtime/src/lockstep.cpp`, all via `-Wl,--wrap`): VI and AI messages are delivered by the idle thread
(which runs only when every other game thread is blocked) instead of ultramodern's 60 Hz wall-clock
thread, `osSpTaskStartGo` completes synchronously and sends SP/DP from the caller, current/next
framebuffer are modelled, `osGetCount`/`osGetTime` are virtual. Result: three stock runs give identical
display lists in all 1800 frames; three pages differ every frame (ultramodern host state in the OSThread
structs, 0x80034000-0x80035FFF and 0x80038000) and are left unjudged. 1800 frames (60 s of game time)
take 1.3 s. Without lockstep, two runs diverged by frame 93-1232 (wall-clock VI phase).
`oracle.py run` runs the stock build twice and the replaced build once, judges a page or display list
only where the stock runs agree, and on a page difference reruns both to that frame with `--dump-at`
and lists the first differing words with data symbols (stacks and unjudged pages skipped).

```bash
# rocky, in a copy of this tree; TK53/TK71 = the IDO 5.3 / 7.1 toolkit dirs
python3 tools/recomp/gen_config.py --rom ROM --n64recomp-src SRC --out build/recomp-SET \
        --replace-set SET --ido53 TK53 --ido71 TK71
(cd build/recomp-SET && ~/rush2049/recomp/build/N64Recomp/N64Recomp rush2049.recomp.toml)
CC=clang CXX=clang++ cmake -S tools/recomp/runtime -B build/recomp-SET/host -G Ninja \
      -DN64MODERNRUNTIME_DIR=$HOME/rush2049/recomp/src/N64ModernRuntime -DRECOMP_OUT=$PWD/build/recomp-SET
ninja -C build/recomp-SET/host
python3 tools/recomp/oracle.py run --stock-host build/recomp/host/rush2049_recomp \
        --rom-stock build/recomp/rom/rush2049.recomp.z64 --repl-host build/recomp-SET/host/rush2049_recomp \
        --rom-repl build/recomp-SET/rom/rush2049.recomp.z64 --out runs/SET --frames 1800 \
        --datasyms build/recomp/rush2049.datasyms.toml
```

Results (rocky, 2026-10-12; default script = BOOT_LOG's race presses, frame-clocked):

| Set | Verdict |
|---|---|
| `matched10` (8 ido + 2 native) | EQUIVALENT over 1800 frames; every function called (3 to 138,946 times) |
| `bug` (render_helper, `>` flipped to `<`) | DIFFERENT at frame 3: display lists + 4 pages; words D_80149B88 0 -> 0x10, D_8012E608, an extra display list |
| `draft_engine_sound_sync` (w21h best.c, 8 rows off) | DIFFERENT at frame 706 (6 frames into the race): D_8011024C 0xC -> 0x2, D_80110288 0x14 -> 0xA, the free-list links at 0x80142C64+ |
| `draft_func_800D7634` (w21h best.c) | NOT EXERCISED (the script never opens the audio-options screen) |
| all 678 buildable matched `src/blob` functions at once | DIFFERENT at frame 1, only func_800E7710's function-local static (its address is sent as an SI message: placement, not behaviour) |

## Phase 3: per-call verdicts, input scripts, static binding (lane w23r, 2026-10-12)

**Per-call capture.** Every replaced function's entry hook now also opens and closes a capture window
(`rush_cap_enter` / `rush_cap_exit`, `runtime/src/capture.cpp`). The stock side is a separate build,
`gen_config.py --capture-set SET`: the same functions in the same order, not replaced; each hook arms a
one-shot bypass and re-enters the recompiled original, so recursion inside the body is captured again
(the replaced build retargets a body's `jal` to itself onto the original entry for the same reason:
`self_calls_retargeted` in the manifest). With `--call-log PATH` the host writes per call

    c <seq> <frame> <fn> <args>            entry: the registers/stack words the prototype uses
    C <seq> <frame> <fn> r=<ret> w=<n>:<hash>   exit: return register(s) + the call's effect

`seq` = global entry order, `fn` = index into the "# fn" header (id, mode, function, labels). Arguments
come from the C prototype (`replace.capture_layout`: o32 a0-a3 / f12,f14 for leading floats / `sp+16..`;
return v0, v0:v1, f0); entries may override with `capture_args` (register names, e.g. a callee that IDO
-O3 passes values in callee-saved f22-f28) and `capture_ret`. The **effect** is every N64 word whose
value differs between entry and exit, found by **write tracking by page**: at entry the 8 MiB are
`mprotect`ed read-only; the first write to each 4 KiB page faults, the SIGSEGV handler copies the page's
pre-write contents for every open window and unprotects it; at exit the copies are compared word by word
(host pages are 4 KiB and rdram is word-swapped within words, so a host word is the N64 word at the
same offset). Not part of the effect: the calling thread's stack below the sp at entry of the outermost
replaced function open on it (dead frames; their layout differs whenever the code does), other threads'
stacks, the OSThread host words (below). Writes by another thread while a window is open count for that
window (both builds alike). Cost: 10x (1800 frames: 15 s instead of 1.5 s) for ~0.3 M captured calls.
`--call-detail SEQ` adds `W seq addr old new` lines for that call.

**Probes.** A `replacements.json` entry with `"mode": "capture"` (state `probe`) is recorded but not
replaced: put a replaced body's callees in its set to see where it first departs (prototype or
`capture_args` required). `gen_config.py --probe-callees` (give it to BOTH builds) adds a probe, without
arguments, for every function the retail body calls with `jal`; a probe on a function the replacement
object defines itself (a draft's own helper copy) also hooks that copy, under the same index. Probes on
functions the runtime reimplements (osSendMesg ...) are dropped from both builds (`probes_dropped`).

**Verdict** (`oracle.py run` with the stock capture build as `--stock-host`, or `oracle.py calls
STOCK.calls REPL.calls`): the call logs are compared record by record; the first difference is reported
before the frame verdict:

    FIRST DIFFERING CALL: seq 1444 = entity_transform_calc (probe_entity_transform_calc) call 1, frame 705: args
      inside: engine_sound_sync (draft_engine_sound_sync) call 1410 (seq 1441)
      f22: stock 3F800000, replaced 00000000

`what` is `args` (named by the prototype labels), `return`, `effect` (then both builds are rerun to that
frame with `--call-detail` and the differing words are listed with data symbols, old->new per side),
`sequence` (one side calls something the other does not), or `length`. Logs from builds of different
sets, layouts or capture versions are refused. No difference: `per-call: all N captured calls identical`.

**Input scripts** (`--script NAME`, `tools/recomp/scripts/NAME.press`; `oracle.py script NAME` prints the
host `--press` string). Format: `TIME BUTTON[@PORT] [DURATION]` per line (game seconds = frames/30, port
1-4, default 1 and 0.2 s), `frames N`, `#` comments. The host connects as many controllers as the script
uses (`T:BUTTON@PORT` in `--press`).

- `race.press`: the Phase 1/2 race script (front end, then a minute of the default track).
- `race_track2.press`: one RIGHT on the track selector (18.5 s): the six race-data streams after image B
  differ (first 0x4D2FA0 instead of 0x43FE80; RIGHT x2 and LEFT give two more), i.e. another track.
- `two_player.press`: RIGHT on the player-count selector (9.5 s) and controller 2 repeating every press;
  `active_player_count` (0x8014A108) = 2, the race loads, 460 display lists per frame instead of 357.
- `audio_options.press`: pause during the race (36 s), DOWN x3, A: the audio-options screen; its rows are
  changed and left with B. func_800D7634 runs 435 times (frames 1201-1635).

**Static binding.** A replaced body's function-local statics (local symbols in .data/.bss/.sdata/.sbss)
are patched to their retail addresses after linking (`replace.static_bindings`): from the replaced
function's own HI16/LO16 pair decoded from the retail image at the same offset (only where both
instruction words equal the object's outside the immediate: matched code), from an address name
(`D_XXXXXXXX`), or from an entry's `"statics": {name: address}` (drafts). The manifest lists
`statics_bound` / `statics_private`. func_800E7710's SI message (0x80150F58) is now EQUIVALENT (set
`statics`). Also fixed while doing this: game and overlay bodies resolve libultra calls by the game-side
legacy names (`game_static_legacy.us.txt`, e.g. game `osJamMesg` = static osSendMesg), as the game link
does; Phase 2 linked them by the static names, which sent func_800D6E00's message with the wrong function
(the all-matched divergence) and made the func_800D7634 draft look DIFFERENT. Since lanes w23g/w23m
renamed every game source (src/blob, cloud/matches, the w21h/w23f draft sources) to the static names,
that table has no entries: `game_legacy_names()` returns `{}` and game bodies bind by the inventory
names directly (a draft written against the old game names must go through
`static_rename.renamed_game_source()` first). Loads into `$zero` in a
replacement are nopped as in the retail sections.

**OSThread word mask.** N64ModernRuntime's OSThread keeps an 8-byte host pointer (`context`) at
`offsetof(OSThread, context)` = +0x20; two stock runs' rdram differ in exactly those two words of the 11
threads. They are now masked word by word (log `T` lines, `# thread_host_words`), so all 2048 pages are
judged at every frame (Phase 2 left three pages unjudged).

```bash
# rocky; SET from replacements.json; the stock side is the capture build of the same set
python3 tools/recomp/gen_config.py ... --out build/cap-SET --capture-set SET [--probe-callees]
python3 tools/recomp/gen_config.py ... --out build/repl-SET --replace-set SET [--probe-callees]
(N64Recomp + cmake/ninja for each, as in Phase 2)
python3 tools/recomp/oracle.py run --stock-host build/cap-SET/host/rush2049_recomp \
        --rom-stock build/cap-SET/rom/rush2049.recomp.z64 --repl-host build/repl-SET/host/rush2049_recomp \
        --rom-repl build/repl-SET/rom/rush2049.recomp.z64 --out runs/SET --script audio_options \
        --datasyms build/cap-SET/rush2049.datasyms.toml
python3 tools/recomp/oracle.py calls runs/SET/stock.calls runs/SET/repl.calls   # re-read a verdict
```

Results (rocky, 2026-10-12; stock side = the capture build of the same set):

| Set / script | Verdict (first line; per-call line) |
|---|---|
| `matched10` / race, race_track2, two_player, audio_options | EQUIVALENT over 1800 frames each; all 333,252 / 374,162 / 418,288 / 201,059 captured calls identical |
| `bug` / race | FIRST DIFFERING CALL: seq 2 = render_helper call 2, frame 2: effect (13 words vs 1: D_80149B88 0->0x10, D_8012E608, D_8012E678, gDisplayListPtr +0x20, 8 display-list words at 0x80127330) |
| `draft_engine_sound_sync` / race | FIRST DIFFERING CALL: seq 1410 = engine_sound_sync call 1410, frame 705: effect (80 words vs 78; the +4 floats of the 12-byte records at D_80142DD8: stock 1.0/-2.0, draft -2.0/-1.0) |
| `draft_engine_sound_sync_probe` / race | FIRST DIFFERING CALL: seq 1444 = entity_transform_calc call 1, frame 705: args, inside engine_sound_sync call 1410: f22 stock 3F800000 replaced 00000000, f28 stock 3F800000 replaced BF800000 (IPA convention: retail passes the values in f22-f28) |
| w23f `draft_engine_sound_sync_ipa` (oracle.c: the same body + static copies of the IPA callees) / all four scripts | EQUIVALENT; all 3,599 calls identical (with `--probe-callees`: 157,251) |
| `draft_func_800D7634` / audio_options | EQUIVALENT over 1800 frames; all 435 calls identical (with `--probe-callees`: 8,942) |
| `draft_func_800D7634` / race | NOT EXERCISED |
| `statics` (func_800E7710) / race | EQUIVALENT (Phase 2: DIFFERENT at frame 1, its private static) |
| all 689 matched `src/blob` game files / four scripts | no judged frame difference (NOT EXERCISED: 340-346 of 689 called); per-call (race): all 4,241,687 captured calls identical |

## Phase 4: IPA-aware replacements, RT64, Windows, the gameplay-test package (lane w24r, 2026-10-12)

Lane notes: `cloud/work/frontier/w24r/RESULTS.md`; plan section 12.

**IPA-aware replacement build** (`replace.py` "IPA conventions", `ipa_callees.json`, `gen_config.py --allow-ipa
--ipa-table --ipa-members`). The game is IDO -O3 whole-program code: a callee may take parameters in non-ABI
registers (entity_transform_calc: x,y,z,w in f22-f28), write callee-saved registers unsaved (audio_effect_setup:
s1/s2), or exist only as a `jr ra` stub because its body was inlined everywhere (func_80091AF0/AF8, the slot-queue
lock). A replacement compiled on its own calls them o32 (the w22r frame-706 verdict). The fifth-section design does
not carry the convention by itself: replace.py compiles one TU, so the callee is never in the unit (proved: the w21h
draft built with `--ipa-table none` (and `--allow-ipa` or its entry flag) gives the Phase 3 `FIRST DIFFERING CALL: seq 1410 ... frame 705`).
Now, for each callee the object calls:

- `ipa_callees.json` `params` (+ `prototype`, `clobbers`): replace.py appends a shim (generated MIPS words) to the
  replacement's chunk and retargets the body's `jal callee` to it. The shim saves ra and every callee-saved
  register it writes or the callee clobbers, moves the o32 arguments (a0-a3, f12/f14, caller stack) into the
  listed registers, calls the retail callee, restores. An inbound shim (the replaced function is itself an IPA
  callee) does the reverse and also preserves caller-save registers; the entry hook calls it.
- `body`: the C source (a matched stub body) is compiled with IDO and linked into the replacement, so the call
  reaches the inlined code instead of the retail stub.
- Detection and refusal (`replace.ipa_hazards`; detector lane w24t): replace.py lists the functions the object
  reaches by `jal` to an undefined symbol and reads each one's RETAIL body (`tools/recomp/ipa_detect.py`, stdlib:
  a must-write dataflow over the body, delay slots and branch-likely modelled) for registers read before any
  write that o32 does not pass in (anything but a0-a3, f12, f14, sp, ra, gp; prologue saves are not reads;
  caller-save registers count as written after a call). It needs no `build/` inputs, so it works in a fresh
  clone; `build/ipa_members.json` `callees` (ipa.py) and `build/frontier.json` facts (`register_params`,
  `unsaved_callee_regs`), when present, add names (`load_ipa_detection`, `--ipa-members`). A callee the source
  defines itself (w23f's oracle form, static copies) is not a hazard. A replacement calling a detected callee is
  refused (`ReplaceError`, naming callee and registers) unless `--allow-ipa` or the entry's `"allow_ipa": true`;
  allowed, it is built with the table's shims / bodies for described callees and plain o32 calls for the rest,
  with one `replace: WARNING <id>: calls IPA register-parameter callees (...); built with --allow-ipa: ...` line
  on stderr. A replaced function that is itself detected needs an inbound table entry or `--allow-ipa`. Calls to
  retail 2-word `jr ra` stubs are listed as notes. The manifest records `ipa_callees` (detected, with reasons),
  `ipa_shims` (call sites per callee), `ipa_bodies`, `ipa_inbound`, `ipa_notes`; gen_config prints the shim, body
  and note lines. Calibration of the detector on the game image (2026-10-12): 91 functions flagged,
  entity_transform_calc = f22,f24,f26,f28, audio_effect_setup = s0, 0 of the 688 standalone locks; 91 of
  build/ipa_members.json's 94 callees (missed: func_80105EA8 t9, lap_count_select t8, split_time_display t4,
  which ipa.py reports through paths the dataflow judges written; `--ipa-members` still adds them).
  `draft_engine_sound_sync` carries `"allow_ipa": true` (it calls entity_transform_calc and audio_effect_setup).

Result (rocky; stock side = capture build): `draft_engine_sound_sync` (w21h best.c, unchanged) is EQUIVALENT on
race, race_track2, two_player and audio_options, 3599 captured calls identical each (`--probe-callees`: callees'
calls identical too); shims alone (no lock bodies) are also EQUIVALENT (the lock's recv/send pair leaves no effect).

**Portable interception.** lockstep.cpp/oracle.cpp no longer rely on GNU ld `--wrap`: CMake renames the nine
intercepted runtime calls in the recompiled code (`RUSH_WRAPPED`: `-DNAME=__wrap_NAME`), `src/rush_wrap.h` maps
`__real_NAME` back. Same verdicts on Linux; links with MSVC-style linkers.

**Renderer option.** `cmake ... -DRUSH_RENDERER=rt64 -DRT64_DIR=<rt64 at the VERSIONS pin>` adds RT64 (HLE, the
API Zelda64Recomp uses), an SDL2 window and live controller/keyboard input; `--renderer null` still selects the
headless renderer in that binary (`null` stays the CMake default and the oracle's reference). Host options:
`--renderer null|rt64`, `--gfx-api auto|d3d12|vulkan`, `--fullscreen`, `--rt64-dev`, `--keyboard-alone`,
`--press-file scripts/NAME.press`, `--seconds 0` (unlimited; rt64 default). Lockstep with RT64 (`--clock frames
--renderer rt64`) mirrors each graphics task to the renderer (the runtime's SP/DP completions go to a private
dummy queue at 0x80F00000) and paces virtual VIs at 60/s; verdicts are defined on the null renderer. Built on
rocky (configures, compiles, links; never run there: no display). `RUSH_CAPTURE` (default OFF on Windows) gates
the POSIX per-call capture; without it `--call-log` exits with a message and the hooks stay transparent.

**Windows.** Not built by the coordinator (no Windows host). The host sources parse against Windows headers
(clang `--target=x86_64-w64-mingw32` in a fedora:41 container, `-DRUSH_HAVE_RT64 -DRUSH_CAPTURE=0`); the
package's INSTALL.md gives the clang-cl + Ninja path Zelda64Recomp uses, with RT64's bundled SDL2 2.26.3.

**Scripts.** `race_track3.press` (RIGHT x2) and `race_track4.press` (LEFT x1): the track selector cycles four
tracks (first race-data stream after image B: 0x43FE80, 0x4D2FA0, 0x555850, 0x5D6160; RIGHT x4 = default,
LEFT x1 = RIGHT x3). `attract.press`: 9000 frames without input; only image A, no race data inflated (no attract
demo race in 5 minutes of game time; Phase 5: the game was waiting on the boot notice, see "Attract mode").

**Gameplay-test package** (`package.py`; process for every later phase):

1. Bump `package_version` in `VERSIONS.md` (`package-pins` block, also the pins) and add a `## vN` section to
   `package/CHANGES.md` (what the tester will notice).
2. On rocky: stock config (`gen_config.py` without `--replace*`), N64Recomp, the RT64 host
   (`-DRUSH_RENDERER=rt64`), then
   `python3 tools/recomp/package.py --n64recomp SRC/N64Recomp --runtime SRC/N64ModernRuntime --config build/stock
   --binary build/stock/host-rt64/rush2049_recomp --out ~/rush2049/recomp/dist --repo-commit $(git rev-parse HEAD)`.
   `--with-rom ROM` only for the owner's private copy. The archive is deterministic; `--verify TARBALL` rechecks it.
3. Sanity test: extract into a fresh directory, follow INSTALL.md's Linux path to a headless boot and the T8
   self-check (lane w24r's script: `~/rush2049/recomp/w24r/sanity/run.sh`).
4. Copy the tarball without ROM to the Pi `build/recomp/dist/` (git-ignored). Never commit a tarball or ROM.

Contents: N64Recomp and N64ModernRuntime source snapshots (no .git), `tools/recomp` (no package tooling), the
stock config (`rush2049.recomp.toml`, `*.syms.toml`, `manifest.json`, generated headers: names and addresses, no
ROM bytes and no recompiled C), the prebuilt Linux RT64 host (it contains the translated game code: private like
the ROM), INSTALL/TEST_PROCESS/REPORT_TEMPLATE/CHANGES/README, `VERSION.txt`, `MANIFEST.sha256`. RT64 is cloned
by the tester at the pinned commit. `make_rom.py` rebuilds the synthetic ROM from the tester's ROM and refuses
anything not byte-identical to the manifest's.

## Phase 5: wall-clock busy-waits, RT64 self-check mask, audio microcode, package v2 (lane w25r, 2026-10-12)

Lane notes: `cloud/work/frontier/w25r/RESULTS.md`; plan section 13; tester report v1: `reports/v1/REPORT.md`.

**Busy-wait hooks** (`patches.json` hooks with `"site_arg": true` -> `rush_hook_spin(rdram, ctx, site)`,
`runtime/src/hooks.cpp`). The game polls memory in 14 loops with no call inside (found by a scan of every
backward branch whose body has no call and loads only loop-invariant addresses, in all four populations; 8 more
are libultra hardware polls N64Recomp does not emit). ultramodern switches game threads only inside libultra
calls, so such a loop starves the thread that would change the variable: with RT64 the DP completion arrives
later than with the null renderer and `game_loop` 0x800FD500 (`while (__osScTaskCount != 0)`, D_8002EB70) hung
every wall-clock boot on the laptop. The hook does one `yield_self_1ms` turn (deliver one external message,
run a woken higher-priority thread) per iteration; it is a no-op with `--clock frames`. Sites: D_8002EB70 drains
in game_loop, func_80096130, suspension_setup, tire_compound_set and image A 0x8038B3DC/B4E8/BA20/BA84; the
retrace-counter wait in dma_wait_complete 0x8008AB3C; wheel_params_set 0x800A4BA0/BC8; func_800E7038
0x800E7114; static func_80011848 0x80011868 and func_80014140 0x8001416C. Repro without a GPU:
`--null-dl-delay 20` (the null renderer takes 20 ms per display list) hangs at `gfx_tasks=2` with
`--no-spin-yield` and boots without it. Status field `spin_yields=`.

**RT64 on rocky**: a fedora:41 container with Xvfb and Mesa lavapipe runs the RT64 host (software Vulkan;
`~/rush2049/recomp/w25r/xvfb/`), SDL's `disk` audio driver records the audio device stream. Not a GPU, but it
reproduced the laptop hang and found the next bug: lockstep + RT64 sent SP/DP to the game before RT64 had read
the mirrored display list (SIGSEGV in RT64's interpreter at the race start); the game thread now waits
(host-side) for the renderer's `send_dl` to return.

**RT64 self-check mask.** The oracle log has `# rdp_image F KIND+ADDR/WxH/BPP` lines (G_SETCIMG/G_SETZIMG and the
largest G_SETSCISSOR) and `# gfx_task` (the OSTask buffers). All scripts use z-buffer 0x8017AEC0 (320x220x2,
right after gBssEnd) and framebuffers 0x807BB3C0 / 0x807DDA00; the v1 a-vs-c 71 pages are the z-buffer (36) and
framebuffer 1 (35). `runtime_buffers.json` names them (`rush_z_buffer`, `rush_framebuffer_0/1`,
`rush_gfx_*`) in the datasyms. `oracle.py compare --mask-renderer` leaves pages covered by those images unjudged
(page granular); host `--mask-rdp-images` zeroes the image bytes in the page hashes (precise; every compared
run needs it; header `# mask rdp_images`). Both report `EQUIVALENT except renderer-written pages`. Verdicts of
replacement lanes stay defined on the null renderer without a mask.

**Audio.** The first M_AUDTASK names the ucode: text 0x8002A050 (ROM 0x2AC50), data 0x8002E210, boot 0x80027DD0.
It is not libultra's aspMain (no command table; a per-voice synthesis loop). `gen_config.py` writes
`rush2049.rsp_audio.toml` (`AUDIO_UCODE`: 0xF40 bytes at IMEM 0x1080; the declared 4096 bytes end in data);
`RSPRecomp rush2049.rsp_audio.toml` (in the config dir) writes `rsp/rush_audio_ucode.cpp`; CMake compiles it
when present (`-march=nehalem`, `RUSH_HAVE_AUDIO_UCODE`) and `get_rsp_microcode` returns it for audio tasks;
lockstep runs it synchronously in `osSpTaskStartGo`. The game's AI thread (static func_80010A40) gives the AI one
192-frame buffer per AI message (the console's per-buffer interrupt, 115/s at 22050 Hz); ultramodern sends one
per VI, so the game made half its audio. `lockstep.cpp` now paces AI messages at frequency / frames per buffer
(wall: host thread corrected by the SDL queue; lockstep: integer accumulation per virtual VI). Output: SDL2
queue device in the rt64 build (`sdl_frontend.cpp`; `--no-sound`, `--volume`), `--wav PATH` in any build,
status fields `audio_nonzero=` / `audio_peak=`, `--no-audio-ucode` / `--ai-per-vi` for the v1 behaviour.

**Privacy.** librecomp's `select_rom` copies the ROM into `--home`; the host deletes the copy in the game's
`on_init_callback` (after `load_stored_rom` has read it) and at exit (`--keep-rom-copy` keeps it).

**Attract mode** (corrects Phase 4): with no input the game never leaves the boot notice (NO CONTROLLER PAK
DETECTED / CONTINUE WITHOUT SAVING) and stays silent; after one START (`attract.press` now presses it at 5 s) it
cycles the title (5 s) and a demo race (image B, ~30 s) on tracks 1, 2, 3, 4 (race data 0x43FE80, 0x4D2FA0,
0x555850, 0x5D6160).

**Other host options**: `--log-input` (button/stick changes per player: the pad check), `--no-spin-yield`,
`--null-dl-delay MS`, `--mask-rdp-images`. Script `stunt.press`: SELECT MODE DOWN x4 (STUNT): race data 0x812BE0
instead of the default track's 0x43FE80.

```bash
# rocky, ~/rush2049/recomp/w25r: stock config + both hosts with the audio ucode
./b.sh stock      # gen_config, N64Recomp, RSPRecomp, null host
cmake build/stock/host-rt64 && ninja -C build/stock/host-rt64      # (first time: cmake -S ... -DRUSH_RENDERER=rt64)
xvfb/run.sh runs/x.txt --home runs/h --seconds 30                   # RT64 on Xvfb + lavapipe, wall clock
python3 tools/recomp/oracle.py compare --mask-renderer runs/self/a.log runs/self/c.log
```

## Phase 6: controller pak, pad polish, mode scripts, cheaper capture, package v3 (lane w26r, 2026-10-12)

Lane notes: `cloud/work/frontier/w26r/RESULTS.md`; plan section 15; tester report v2: `reports/v2/REPORT.md`.

**Controller pak** (`runtime/src/pak.cpp`). N64ModernRuntime cdf5abbd answers every `osPfs*` call with
PFS_ERR_NOPACK (librecomp/src/pak.cpp), hence the boot notice NO CONTROLLER PAK DETECTED. The game uses the pak
only (no EEPROM, SRAM or flash call in the recompiled code); it calls osPfsInitPak, FreeBlocks, AllocateFile,
DeleteFile, FileState, ReadWriteFile and RepairId (plus osMotorInit for rumble). CMake renames those calls in
the recompiled code to `rush_osPfs*_recomp` (`RUSH_PAK_RENAMED`, the RUSH_WRAPPED mechanism), which implement
libultra's semantics and return codes over a 32 KiB image in the standard `.mpk` layout (ID block with
checksums and copies, inode table + backup with its checksum, 16 notes, 123 data pages) so emulators can open
it. Controller 1 reports a pak that is not a rumble pak (osContGetQuery status CONT_CARD_ON, osMotorInit
PFS_ERR_DEVICE, as a real controller pak). Modes: wall clock `--home/rush2049.mpk`, created formatted, rewritten
(temp file + rename) after every allocate/delete/write; `--clock frames` a fresh formatted image in memory,
never written (or `--pak FILE` read, `--pak-write` to write it back: save tests); `--no-pak` = v1/v2. Log lines
`pak: ...` (allocate, delete, every read/write), oracle events `E <frame> pak ...`, status field `pak_writes=`.
What the game does with it: START GAME -> PLAYER 1 (JUST PLAY, CONTROLLER PAK n, the players, CREATE PLAYER,
DELETE PLAYER); CREATE PLAYER -> ENTER YOUR NAME -> note "RUSH2049"+name (company 0x3544, game 0x4E525545,
9 pages = the notice's "1 NOTE AND 9 PAGES"), 2304 bytes written; race start writes 64+32 bytes, quitting a race
128 bytes (statistics); boot and player selection read the 2304 bytes. With a controller pak the game shows
"INSERT RUMBLE PAK NOW" for the first seconds of a race (it invites a pak swap; expected).

**Input scripts and the pak.** A `.press` script may say `pak on|off` (host `--press-file` and oracle.py honour
it; `--pak`/`--no-pak` win). Every script written before Phase 6 says `pak off` (the notice flow; its frames and
verdicts are unchanged); `oracle.py run` with `--press` or the default press string runs `--no-pak`. New
pak-flow scripts (lockstep frames: title ~3 s; 4 START -> START GAME; 7 A -> PLAYER 1; 10 A -> SELECT MODE;
13/16 A -> SELECT TRACK; 19 A -> car; 22 A -> race):
`race_pak.press` (default race, image B at frame 580), `attract_pak.press` (no input: demos from frame 242,
tracks 1-4), `practice.press`, `ghost_race.press`, `circuit.press` (BEGINNER CIRCUIT 8 RACES), `backward.press`
(BACKWARD ON), `other_car.press` (RIGHT x2 on CAR), `records.press`, `battle.press` (2P, BATTLE 1 arena),
`saves.press` (CREATE PLAYER "AAAA", race, quit: 5 pak writes) and `saves_reload.press` (selects AAAA; run it
with the pak `saves.press` wrote).

**Pad polish** (`sdl_frontend.cpp`): `controller -> player N` printed once per pad (SDL's device-added event for
pads present at start re-opened them and printed again); stick/trigger dead zone 0.15 rescaled linearly from
its edge; Back, Guide, L3/R3 unmapped on purpose (commented).

**Lockstep + RT64**: the game thread waits for the renderer to take a mirrored display list however long it
takes (warning every 10 s). The old 10 s timeout went on, and on an overloaded software GPU RT64 then read a
half-rebuilt list ("Unable to find a matching GBI", SIGSEGV; three crashes in w26r's parallel runs, none after).

**Lockstep audio**: the "~82 %" of Phase 5 was the last once-per-second status line before the frame stop; the
run's output is 100.0 % (race.press: 1,322,688 frames in 3600 virtual VIs, w25r's binary and this one alike;
WAV 59.986 s). A lockstep `--frames` stop now prints the `final` status line and
`lockstep: N virtual VIs (S s), F audio frames = P% of 22050 Hz`.

**Run-burst census** (`hooks.cpp`; osRecvMesg/osSendMesg added to RUSH_WRAPPED): the longest time a game thread
runs between message calls (or busy-wait yields), after the first 2 s, as `max_burst_ms=` and a `longest
game-thread run ...` line from 4 ms. Wall clock, null renderer, `--null-dl-delay 25`, every script: at most
1.1 ms over 16 scripts (race_track2), 0.3-0.8 ms elsewhere. No long call-free computation starves another
thread on this host.

**Cheaper capture** (`capture.cpp`): rdram is made read-only once; each window re-protects only the pages
written since the last one (runs of pages per mprotect) instead of protecting 8 MiB at entry and unprotecting
it at the last exit. matched10 capture set, race script, 333,252 calls: 21.0 s -> 15.5 s (no capture: 2.7 s),
call log byte-identical.

Other: `--fb-every N` (framebuffer dump period), RT64 pixel runs `rtp.sh`-style: `xvfb/run.sh` with
`--clock frames --press-file ... --fb-dump DIR` (lavapipe is too slow for wall-clock scripts: ~3 gfx tasks/s).

## draft_check: does a draft behave like retail? (Phase 7, lane w27r, 2026-10-12)

For matching lanes. One command builds the recomp with your draft in place of the retail function, runs the
lockstep input scripts headless and compares every call of the function with retail:

```bash
# on the Pi, from the repo root (until wave27 has the module, call it by path from the w27r worktree):
python3 .claude/worktrees/w27r/tools/recomp/draft_check.py --remote rocky NAME path/to/draft.c
# once merged:      python3 -m tools.recomp.draft_check --remote rocky NAME path/to/draft.c
# on rocky itself:  cd ~/rush2049/recomp/dc/repo && python3 -m tools.recomp.draft_check NAME /path/draft.c
options: --scripts race,two_player | all | reached   (default: the 6 census scripts with most calls of NAME)
         --params handle=a0,mark=t3          (NAME takes IDO -O3 register parameters: every prototype parameter)
         --callee 'void f(s32 h, f32 x)=a0,f22[/s1,s2]'   (a register-parameter callee the draft calls)
         --statics name=0x80150F58           (bind a function-local static to its retail address; Phase 10:
                                              initialised ones too, FUNCTION:NAME when two share a name)
         --frames N, --jobs N, --json PATH, --keep
         --unit A,B,C draft.c                (Phase 8: one TU, every member replaced; --params A:p=r,...)
         x --probe-callees F,G [--static-only]   (Phase 8: derive F's register convention, print --callee)
         x --gc [--dry-run] [--keep-trees N] [--gc-days D]   (Phase 8: clean ~/rush2049/recomp/dc)
         --lane LANE [--scripts ...] [--remote rocky]   (Phase 9: every cloud/work/frontier/LANE/<NAME>/best.c,
                                                  one table; per-draft options in <NAME>/dc.args)
```

Output (exit status): `VERDICT: EQUIVALENT over N calls / K scripts (...)` (0); `VERDICT: DIVERGES` (1) after a
`FIRST DIVERGENCE: script S, frame F, NAME (draft) call #C (global seq Q): arguments differ at entry | return value
differs | memory written differs | call sequence differs | the draft FAULTED (host signal; retail did not)` (Phase 9:
then the fault's N64 address, PC + instruction, registers and N64 call chain), then the differing values as `retail X | draft Y`, and
for memory every differing word with its data symbol (`word 0x80149B88: retail unchanged | draft 0->0x10
D_80149B88+0x0`); `VERDICT: NEVER CALLED: function NAME never called in these scripts` (3); build errors (the IDO or
linker message) exit 2. Per script one line (`race EQUIVALENT 21344 calls of render_helper (1800 frames, 7 s)`).

**EQUIVALENT is not a match.** It says the draft's logic is right on every exercised call (arguments, return
value, every N64 memory word it wrote, and every judged frame); only compiler shaping is left. It is never coverage
and never goes in cloud/matches (score.py / blob_unit decide matching). A divergence names the first differing call:
read the arguments of that call (the inputs that took the draft down another path) and the words it wrote.

What it does (rocky, `~/rush2049/recomp/dc`): a temporary replacements.json with one entry `draft` (`ido` mode, the
draft's `/* flags: */` header, `allow_ipa`), built twice per function in `dc/trees/<checkout>/NAME/{cap,repl}`:
`gen_config --capture-set` (retail with the capture hook) and `--replace-set` (the draft), N64Recomp/RSPRecomp into a
stage directory, synced into the build tree by checksum so ninja recompiles only what changed (first build of a
function ~30 s, later ones ~3 s). Then `oracle.py run --reuse-stock` per script, in parallel; the retail logs (two
runs + call log) are cached per (retail binary + ROM hash, script text, frames) in `dc/cache/NAME/`, so a repeat
check of the same function costs one draft run per script (race: ~4 s). Run directories: `dc/runs/NAME/<time>-<pid>/`
(`summary.json`, `runs/<script>/oracle.txt` and `verdict.json`, build logs). `--remote HOST` copies the draft, the
files it includes with `#include "..."` and the current checkout's include/ to `dc/inbox/<user-NAME-time-pid>/`
and runs the same command in the service tree `dc/repo` there (a copy of the w27r tree; the coordinator refreshes
it with `git ls-files | rsync -a --files-from=- . rocky:rush2049/recomp/dc/repo/` plus build/frontier.json and
build/ipa_members.json).

IPA (whole-program IDO -O3) conventions:
- NAME with `register_params` in build/frontier.json facts is refused without `--params` (or a committed
  `ipa_callees.json` entry): the hook then reaches the draft through an inbound shim, and both sides capture those
  registers instead of o32 ones. Prove a mapping with the verifier below before trusting a verdict.
- callees with register parameters: shims from `ipa_callees.json` (+ `--callee`); others are called o32 and listed
  in a `replace: WARNING` line (a divergence at or after such a call may be the harness).
- callees that write callee-saved registers without saving them (facts `unsaved_callee_regs`, e.g. credits_screen,
  drone_pathfind_main) get an automatic shim that passes a0-a3 and 8 stack words through and saves those registers.
- inlined `jr ra` stubs: bodies from `ipa_callees.json` (func_80091AF0/AF8, the slot-queue lock).

Checked (rocky): matched render_helper EQUIVALENT (race/two_player/attract 60,802 calls); the w22r injected bug
DIVERGES at race frame 2 call #2 (13 words written instead of 1); w21h engine_sound_sync EQUIVALENT (race, battle;
shims for entity_transform_calc/audio_effect_setup); w21h func_800D7634 NEVER CALLED on race, EQUIVALENT on
audio_options (435 calls, clobber shims for credits_screen/drone_pathfind_main); audio_effect_setup (inbound s0)
EQUIVALENT. Test: `tests/recomp/test_draft_check.py` (the end-to-end good/broken pair runs on rocky only).

## regmap_verify: is this register mapping right? (Phase 7, lane w27r)

For a function IDO -O3 compiled with register parameters (frontier facts `register_params`), before writing
its prototype or passing `--params` to draft_check:

```bash
python3 .claude/worktrees/w27r/tools/recomp/regmap_verify.py --remote rocky NAME 'a0=pak:ptr,t3=mark:int'
# once merged: python3 -m tools.recomp.regmap_verify --remote rocky NAME '...'   (--scripts, --static-only, --json)
```

Mapping items `REG=LABEL[:KIND]` (KIND ptr | int | float | any). Evidence printed: (1) the retail body's live-in
registers (read before written on some path; ipa_detect dataflow with a0-a3/f12/f14 included) and the argument
registers it only forwards to a callee; (2) every `jal NAME` in the game image with, per register, the instruction
that set it before the call (delay slot first, then a backward scan of the caller); (3) a capture build with a
probe on NAME recording all integer registers and the even FP registers at every call, the call site taken from
`$ra` (the recompiled code does not write `$ra` on a call, so `gen_config --extra-hooks` sets it at each static
`jal` site; jalr sites show as unnamed), per site the number of distinct values, range and kind of each register.
`VERDICT: CONSISTENT` = the claimed registers are exactly the live-in set and each has the claimed kind at every
captured call; `REFUTED` lists the problems (claimed but never read, live-in but unclaimed, wrong kind); exit 0 / 1,
3 when no script calls NAME (static evidence only). Example (func_800A1DD4, w27e's pak cluster, claim `a0,t3`):
REFUTED: a1 (buffer pointers 0x80218AA0..) and a2 (sizes 0x10..0x900) are live-in too; t3 = 0 from
track_data_decompress, 1 from slot_state_lookup; a0 = 0x80144D5C at both. entity_transform_calc
`f22,f24,f26,f28` (+ a0 forwarded) CONSISTENT over 51,092 calls at 4 sites.

## Call census (Phase 7, lane w27r)

`python3 -m tools.recomp.census` (rocky, ~2 min): one capture build with a probe on each unmatched game function
(asm/us/blob functions not in blob_matched.lock.json; `--functions all|NAME,...`), `$ra` set at every static `jal`
site (`--extra-hooks`), every script run headless with `--call-log`. Writes `census/<script>.json` (per function:
calls, first/last frame, call sites with counts, a0-a3 and frontier register-parameter registers: min, max,
distinct count, kind, the values when <= 8), `census/census.json` (function -> {script: calls}; draft_check's
default scripts) and `census/README.md` (the unreached table). Register values are what the caller left at entry:
meaningful only for registers that are parameters. 2026-10-12 (19 scripts): **167 of 218 unmatched functions
reached (178,776 of 231,436 B); 51 never reached (52,660 B)**, mostly end-of-race/results screens, high-score entry,
replay save, controller/save settings, the attract demo/video handlers and part of the 0x8010A000-0x8010F000 block.
New script `stunt_end.press` (stunt mode played until its 4:40 timer ends, 11,000 frames) reaches
func_80101D84 and func_80102448 (from frame 10,010: the end-of-stunt screens). A ghost race held to 9,600 frames
reached nothing new (not kept). Refresh the census after adding a script or when the lock changes.

## RDRAM accessors (optional native path; Astra PR #325, imported by w22p, 2026-10-12)

`rdram.h` and `game_rdram.h` are the accessor layer the `native` mode above lacks. They are standalone
C11/C++11 headers: nothing in `replace.py` or the runtime uses them yet, and the default replacement path
stays `ido`. A native replacement that uses them keeps every guest pointer as a `rush_addr` (`uint32_t`)
token and reads/writes guest memory only through checked accessors; an unchanged matched source does
not become safe by including them.

- Layout: N64Recomp 81213c18's word-swapped rdram (`MEM_B` = addr^3, `MEM_H` = addr^2, `MEM_W`
  unswizzled, `LD`/`SD` high word first). `rush_rdram memory = {rdram, allocated_size}`.
- `rush_rdram_read_KIND(&memory, addr, &out)` / `rush_rdram_write_KIND(&memory, addr, v)` for
  `u8/u16/u32/u64/s8/s16/s32/s64/f32/f64/addr`; return `bool`, KSEG0 inside the allocation only, any
  alignment, no partial write on failure. `rush_addr_from_gpr` accepts zero- or sign-extended o32 values.
- `game_rdram.h`: a `rush_rdram_view {memory, base}` and `rush_TYPE_read_FIELD` / `rush_TYPE_write_FIELD`
  for every named non-padding field of `include/game_types.h`. `tests/recomp/test_rdram.py` recomputes the
  o32 offsets from the header with pycparser, so a `game_types.h` change that moves a field fails the test
  until the table is reconciled.

```bash
python3 -m pytest tests/recomp/test_rdram.py -q -p no:cacheprovider
RUSH_N64RECOMP_INCLUDE=/path/to/N64Recomp/include python3 -m pytest tests/recomp/test_rdram.py -q  # upstream MEM_* cross-check
```

Not imported from #325: `gen_wrapper.py` / `o32_abi.h` (a second o32 wrapper generator next to
`replace.py`'s `render_wrapper`; its guest-pointer adapter mode belongs in `replace.py` as an option of
`native` mode when a replacement first uses the accessors) and `.github/workflows/recomp-fixtures.yml`
(GitHub CI scope is the owner's call; `make test-fast` runs only `tests/conveyor tests/cloud`, so run `python3 -m pytest tests/recomp -q -p no:cacheprovider` after recomp changes).

## Phase 8: decomp feedback, round two (lane w28r, 2026-10-12)

Lane notes: `cloud/work/frontier/w28r/RESULTS.md`; plan section 17. Not a matching lane; no gameplay change (no
package).

**Register-parameter callees without `--callee`.** `ipa_callees.json` now describes the callees lanes passed by
hand in wave 27 (each entry's `evidence` says where the mapping comes from): `slot_state_setup` (s2; writes
s0,s1,s3 unsaved), `sound_update_channel` (t0), `func_800B6748` (a0-a3 + t0-t2 for parameters 5-7),
`func_80096CA8` (s8,t0; s0-s7 unsaved), `func_80097164` (s8), `func_800A1DD4` (a0,a1,a2,t3). A draft that calls
them gets the shims automatically; a draft OF one of them gets its inbound shim and register capture without
`--params` (the table entry is used as NAME's convention).

**Interprocedural live-in** (`ipa_detect.register_params(..., call_effects=)`, `regmap_verify.Interproc`). IDO -O3
keeps a register parameter alive across a call to a callee that does not touch it: `sound_update_channel` reads
`force` in t0 after its first call, so the intra-procedural detector (calls clobber every caller-save register)
missed it and regmap_verify REFUTED the lanes' (correct) claim. regmap_verify now follows static `jal` callees:
a register a callee neither reads nor writes survives the call, one it reads is read at the call (jalr, calls out
of the game image and recursion keep the old default). All five known mappings (slot_state_setup s2,
sound_update_channel t0, func_800A1DD4 a0,a1,a2,t3, entity_transform_calc a0,f22-f28, audio_effect_setup s0) are
CONSISTENT. replace.py's hazard detection (w24t) is unchanged.

**`--probe-callees`** (`draft_check x --probe-callees F[,G] [--static-only] [--scripts S]`, or `regmap_verify F
--derive`): derives F's convention from the retail body and its call sites and prints it in `--callee` syntax:
parameters = the interprocedural live-in set (a0-a3, then other GPRs, then FP, each by register number), kinds
from a capture run (or the instructions that set them at the static sites with `--static-only`), clobbers =
callee-saved registers F (or a callee, transitively) writes without a prologue save (+ frontier facts). The
parameter ORDER is a guess (the call sites' setter order is not usable: the scheduler reorders it), and so are
the names and the return type: match them to the draft's prototype. Example: `--callee 's32
func_800B6748(void *p0, ..., void *p6)=a0,a1,a2,a3,t0,t1,t2'` (2,024 race calls, all pointers). A normal
draft_check run also prints a `suggested ... --callee` line for each register-parameter callee it had to call
o32. Trap seen on the pak cluster: func_800A1C6C's live-in set is a0,a1,t1,t2 but the prototype order (handle,
address, bytes, offset) maps to a0, t2, a1, t1 (found from the first divergence's written words; EQUIVALENT
with `--params func_800A1C6C:handle=a0,address=t2,bytes=a1,offset=t1`).

**`draft_check --unit A,B,C draft.c`.** The draft is one translation unit that defines every member; replace.py
compiles and links it ONCE as one chunk (`-u` per member; an entry `"unit": [...]`, `"unit_capture"` for the
non-first members' capture registers) and every member's entry hook calls into that chunk (member records
`draft__<member>`; each with its own inbound shim from the table or `--params MEMBER:p=r,...`, its own static
binding and self-call retargeting). Calls between members stay inside the chunk, in the unit's own conventions,
so they pass no hook; to keep the logs comparable the generated hooks of a unit's members record a call only
when no other member call is active on the same thread (`rush_unit_enter`/`rush_unit_exit`, runtime
capture.cpp), on BOTH sides. The verdict counts calls per member; the first divergence names the member
(`FIRST DIVERGING MEMBER: func_800A1C6C (unit ...)`); a member called only from inside the unit shows 0 outer
calls (NOTE line). Default scripts: the census scripts with the most calls of any member. Checked: w27e cluster
func_800A1BB4 + func_800A1DD4 EQUIVALENT (saves: 3 + 9 calls); slot_state_lookup + func_800A1DD4 +
func_800A1C6C + func_800A1BB4 EQUIVALENT over 684 calls (with the mapping above); test
`tests/cloud/test_recomp_phase8.py` (end-to-end pair with an injected bug on rocky).

**Cleanup policy** (`draft_check x --gc [--dry-run]`; also automatic after every draft_check run once there are
more than 16 function build trees). Under `~/rush2049/recomp/dc`: build trees (`trees/<checkout>/<NAME>`,
~250 MB each): the 12 most recently used are kept (`$RUSH_DC_KEEP`, `--keep-trees`; last use = the mtime of the
tree's build lock), never one being built; retail log caches (`cache/<NAME>`): dropped with their tree, else only
the newest retail-binary key is kept; run dirs (`runs/<NAME>/*`): the newest 5 per function and anything younger
than 3 days (`$RUSH_DC_DAYS`, `--gc-days`); `verify/`, `probe/`, `census/` runs and `inbox/` drafts older than 3
days. A collected tree costs one first build (~30-50 s) when that function is checked again. Dry run at
2026-10-12: 45 entries, 9.7 GB of 20.4 GB.

## script_probe: what does a script reach, what is on screen? (Phase 8, lane w28r)

```bash
python3 tools/recomp/script_probe.py --remote rocky path/to/new.press [more.press ...] --text
# options: --functions unreached (default: the census's never-reached list) | unmatched | NAME,...
#          --text-from F --text-to F --all-text (HUD digits too) --frames N --pak FILE --pak-write FILE --json PATH
```

One capture build (cached tree `@probe-<set>`) with a probe on each chosen function and a text hook: an
`--extra-hooks` entry in the game's string-queue function `menu_input_process` (0x800B6994, a0 = string, a1 =
length or < 0) calls the host's `rush_text_log`, and the host option `--text-log PATH` writes `<frame>\t<text>`
whenever a string appears that the previous frame did not draw. Each script runs headless (1,200 frames < 1 s,
7,000 frames ~6 s); printed per script: every probed function reached (first frame, calls) and the text timeline,
e.g. `3676  PLAYER 1 | 6TH PLACE | TIME 1:36.533`, `3742  ENTER YOUR NAME | A | B | ...`, `4200  GAME OVER | LAP
TIMES | RACE AGAIN | NEW TRACK | NEW CAR | QUIT`. It replaces RT64 screenshots for script writing. Menu traps:
a 0.5 s press auto-repeats (use the default 0.2 s, ~1 s apart); SELECT TRACK DOWN + LEFT x2 = 1 lap (default 3);
with the accelerator held the default car finishes a 1-lap race on track 1 (6th, ~frame 3676).

**Phase 8 scripts** (lane w28r; pak flow; frames in brackets): `options_menus.press` [2490] front-end OPTIONS / AUDIO /
VIDEO, every row changed; `pause_menus.press` [4320] pause -> OPTIONS, CONTROLS (every row), AUDIO, VIDEO, RUMBLE PAK,
RESTART; `controls_frontend.press` [1740] PLAYER 1 -> CONTROLS editor (RIGHT on the row before A, else A starts JUST
PLAY); `race_finish.press` [4800] 1-lap race to the finish, ENTER YOUR NAME, BEST LAPS, GAME OVER -> RACE AGAIN;
`two_player_finish.press` [4800] the same for 2P; `player_finish.press` [5100] CREATE PLAYER "AAAA" then a 1-lap finish as
that pak player (stat writes, BEST AVERAGES), QUIT; `stunt_short.press` [3600] stunt with MINUTES 1 (ends at frame
2810; keep stunt_end for the four functions only it reaches). Traps: a held press counts on release; LAPS is
fixed at 3 in ghost and circuit; the GAME OVER menu takes 0.2 s DOWN presses (A on LAP TIMES toggles the lap list,
which is why w26r thought DOWN was dead).

**Census refresh (2026-10-12, 26 scripts, 206 unmatched): 183 reached (197,184 B), 23 never (22,768 B)** (was
167 of 218 / 51). Still unreached, with what is known (census/README.md has the table): func_80103D28,
func_80104B14, func_800CC50C, func_800CC040 (ghost-save branch: D_80140B20 = 4 after a ghost race FINISHED with a
ghost recorded; gas-only 3-lap ghost races end DNF against the 3:40 limit); func_80102980, func_80101904 (the
game-mode-6 case of func_80102F30/func_8010221C: probably battle results; scripted battles never score);
func_8010C974 (WPR_MINE weapon callback, table 0x80118BB0: needs a mine pickup in battle); control_settings,
func_800DD0C0, attract_mode_handler, attract_demo_handler, attract_video_handler (callees of control_settings,
which only image A reaches: the "attract" labels are wrong); draw_speedometer, draw_ui_element, func_800C7EC8,
drone_set_catchup, AdjustSpeed, menu_back, func_800CBF2C (gated inside func_800C813C's per-object loop / props
render); func_800F0F44; world_effect_update, fp_call_wrapper, object_bytes23_sum (no jal, pointer or lui/addiu
reference in any image: dead or computed-jalr only).

## Phase 9: host faults, static helpers, scene levers (lane w29r, 2026-10-13)

Lane notes: `cloud/work/frontier/w29r/RESULTS.md`; plan section 18. Not a matching lane; no gameplay change (no
package).

**The -11 crashes of wave 28.** Two causes. (1) Host bug, fixed: IDO -O3 writes no symbol for a `static` function,
and replace.py retargeted a draft's `jal callee` to the callee's IPA shim only inside NAMED functions; a static
helper's calls stayed o32. w28c's func_8010AEAC draft (`dc_static.c`: printOption/animButtonText as static helpers)
called slot_state_setup (s2 parameter, clobbers s0,s1,s3) directly, s3 held the gfx message queue, and the next
osSendMesg faulted. `replace.code_ranges`: every code range (shim retargeting, `$zero`-load nops, self-call
retargets outside a unit) is the named functions plus the object's whole `.text`. The draft is now EQUIVALENT
(options_menus 960 + pause_menus 689 calls). (2) Draft bug, now reported: w28b's func_800C813C draft read the
per-pak file list head at +8 of D_80144D68's 16-byte entries (retail `lw s2, 0x4D68(s2)`: +0) and dereferenced
0x01010000 (a TLB exception on the console too).

**Fault report** (`runtime/src/fault.cpp`). A SIGSEGV/SIGBUS/SIGFPE/SIGILL/SIGABRT handler (capture's write-tracking
handler chains to it for faults outside rdram) prints `[rush] FAULT` lines on stderr: signal, read/write, the N64
address (host address - rdram + 0x80000000), frame, thread; the replaced/probed call open on that thread (every
generated hook passes through `rush_cap_enter/exit`, capture or not); host pc + backtrace; that call's recomp
context registers. It flushes the call log and the oracle log, then dies as before. `oracle.py run` tolerates a
signal death of the REPLACED host only (stock runs must succeed), symbolises the pc and backtrace with addr2line,
maps each recompiled-C line to the `// 0xADDR: insn` comment N64Recomp wrote above it, compares the call logs up to
the fault (an earlier differing call is still reported first), and returns DIFFERENT with `crash`. draft_check:

    saves          DIVERGES     3 calls of func_800C813C - draft FAULTED at frame 4  (4500 frames, 8 s)
    FIRST DIVERGENCE: script saves, frame 4, func_800C813C (draft) call #3 (global seq 3): the draft FAULTED (host signal; retail did not)
      fault: SIGSEGV, N64 read of 0x01010000 at frame 4 (thread game7)
      N64 PC 0x8080013C `lw $v0, 0x0($s2)` = func_800C813C+0x13C (draft)
      registers: v0=FFFFFFFF s2=01010000 sp=80031C90 ra=00000000 (thread context; may lag the host registers)
      N64 call chain: func_800C813C (draft) <- func_800C813C <- input_aux_handler @0x800FD408 <- game_loop @0x800FD60C <- ...
      faulting call: func_800C813C (draft) call #3 (global seq 3, entered at frame 4)

The PC is in the draft's chunk (0x80800000+): `func_800C813C+0x13C` is the offset in the draft's compiled body
(`mips-linux-gnu-objdump -d` of the IDO object, or the recompiled `RecompiledFuncs/funcs_*.c` in the build tree).

**Argument home slots (call log v3).** The 16 bytes above the outermost replaced call's entry sp (o32: the caller's
slots for a0-a3, which the callee may store its register arguments into) are no longer part of a call's effect.
w28b's func_800C813C (race) and func_800CD104 (saves) "one extra own-stack word" divergences were the draft homing
a0 there: both EQUIVALENT now.

**Scene levers and chained paks** (scripts; every harness: oracle.py, draft_check, census, script_probe,
regmap_verify):
- `poke TIME ADDR VALUE [BYTES]`: host `--poke FRAME:ADDR:VALUE[:BYTES][,...]` writes the value (1/2/4 bytes, hex)
  when that graphics frame starts, on the submitting game thread with the others suspended, before the frame's
  hashes (event `E F poke ...` in the oracle log). Both sides of a comparison get the same pokes.
- `pak from SCRIPT`: start from the pak image SCRIPT leaves. The harness runs SCRIPT once on the stock host
  (`--pak IMG --pak-write`, recursively for its own `pak from`) and caches the image per script chain
  (`pak_<script>_<sha12>.mpk` in the run dir); every compared run then reads it (lockstep: never written back).
  `script_probe --pak-write FILE` now also writes a fresh image (it used to pass `--pak-write` only with `--pak`).

Phase 9 scripts (frames in brackets): `player_reload.press` [1500] saves_reload with `pak from saves` (saves_reload
itself stays "not runnable alone"); boot reads player AAAA: func_800C7EC8, drone_set_catchup, draw_speedometer.
`ghost_save.press` [5000] AAAA ghost race made 1-lap by `poke 28 80152735 1 1` (the race's lap count byte; 3 in
ghost/default races, 1 after LAPS 1; found by dump diff), finish 3775, BEST GHOSTS 3836, GAME OVER 4736, RACE AGAIN
4860: func_800CC50C, func_800CC040, func_800CBF2C, menu_back (+ the player read). `battle_end.press` [1800] the 2P
battle with its points target byte 0x80142513 poked to 0 at 24 s: GAME OVER scoreboard at 795, A on SCOREBOARD at
855: func_80101904, func_80102980. Not found: a pak GHOST file (the 1-lap ghost enters BEST GHOSTS and the player
record, 128 B, but no ghost note is written: GC updateHighScoreScreen saves only when gGhostToSave is set; lap
counter pokes 0x8014A647/0x8014A980/0x801E0ECB/0x801F6178 did not shorten a 3-lap race), so the SaveServer ghost
branch (draw_ui_element), the ghost-save menu (func_800F0F44 = GC _FindGhosts), func_80103D28 (DrawUnlocked: needs
an unlock) and func_80104B14 stay unreached.

**`draft_check --lane LANE`** (summary mode): runs draft_check on every `cloud/work/frontier/LANE/<NAME>/best.c`
(or a directory path) one after the other, NAME = the directory; a `dc.args` file next to best.c holds that draft's
extra options in shell syntax (`--unit A,B,C` replaces NAME, `--params`, `--callee`, `--scripts`; `#` comments);
every other option on the command line (`--scripts`, `--remote rocky`, `--jobs`) applies to each draft. Prints one
line per draft as it finishes, then a Markdown table (function, verdict, first divergence or EQUIVALENT line) and
the verdict counts; `--json` writes the rows. Each draft's own run directory keeps its details.

**`--gc` policy check** (rocky, 2026-10-13, after this lane's runs: 15.6 GB in dc): `--gc --dry-run` would free 0.4 GB
(trees #13/#14 by last use, one superseded retail cache); the automatic collection (past 16 trees) keeps the dc
directory bounded. `--gc` no longer needs a NAME word.

**Census refresh** (2026-10-13, 30 scripts, 192 unmatched at wave29 = 8c74d8b6): **178 reached (194,028 B), 14
never (14,912 B)** (was 183 of 206 / 23 at w28r; 14 functions were matched in wave 28). Newly reached: func_800C7EC8,
drone_set_catchup, draw_speedometer, func_800CC50C, func_800CC040, func_800CBF2C, menu_back, func_80101904,
func_80102980. Still never: func_8010C974 (mine pickup in battle), control_settings, func_800DD0C0 and the three
"attract_*" handlers (image A only), func_80103D28, func_80104B14, func_800F0F44, draw_ui_element (need a pak ghost
file or an unlock, above), AdjustSpeed (pak file delete with retry: needs a full or failing pak), world_effect_update,
fp_call_wrapper, object_bytes23_sum (no static reference).


## Phase 10: ghost files, unlocks, pak faults, initialised statics, renames (lane w30r, 2026-10-13)

Lane notes: `cloud/work/frontier/w30r/RESULTS.md`; plan section 19. Not a matching lane; no gameplay change (no
package).

**Why no script wrote a ghost file.** The high-score state machine is `finish_state_normal` 0x800F64D4 (GC
highscorescr.c updateHighScoreScreen; state word gHighScoreState 0x80140B20, SetHighScoreState takes its state in
s2). In state 3 (after the name entry) a ghost race with a recorded ghost (the N64 gGhostToSave = 0x80140800)
compresses it (func_800CC50C = CompressGhost) and then: if the byte **0x801148E4** (the N64 gSaveGhostFiles) is set,
goes to state 4 = SAVE GHOST (FindGhosts, func_80104B14, func_800F1930); else calls func_800CC040 = SaveGhost with
limbo = 1 (the ghost stays in memory: what w29r's ghost_save saw). Only func_800CCEFC sets 0x801148E4: when the
player's id string equals the developer id `000F1CC5 EJK` (0x801211CC), together with 0x801164A1 and the halfwords
0x801164C0.. (unlock everything). GC is the same (`_ReadPlayer`: gSaveGhostFiles only for that id). Retail players
never save ghost files; the scripts poke the flag.

**Unlocks.** `_InitScreen` (track_texture_load 0x800F5F90) snapshots the per-player unlock tables (0x80150DD8.. ->
0x801426A0..), recomputes them (net_state_validate 0x800F2A28 = GC CalcUnlockedStuff) and compares (func_800A1910);
any difference = SetHighScoreState(1) = ControlUnlocked (func_800F2718) -> DrawUnlocked (func_80103D28). With the
developer halfword 0x801164C0 = 1 CalcUnlockedStuff unlocks every car, so the first race end shows CONGRATULATIONS!
YOU JUST UNLOCKED: LOCUST LX | GX-2 | MINI XS | VENOM | CRUSHER | EURO LX | PANTHER.

**Pak faults** (`runtime/src/pak.cpp`, host `--pak-fail OP:N[:ERR][,...]`, script line `pak fail OP N [ERR]`): the
Nth call (1-based, counted per OP over the run) of osPfsDeleteFile (`delete`), osPfsReadWriteFile (`read`,
`write`), osPfsAllocateFile (`alloc`) or osPfsFileState (`state`) returns ERR (default 4 = PFS_ERR_CONTRFAIL) and
changes nothing; log `pak: injected failure: ...`, oracle event `E F pak fail ...`. Deterministic in lockstep, so
both sides of a comparison fail the same call. Every harness passes it (`oracle.lever_args`: oracle.py run,
draft_check, census, script_probe, regmap_verify, `pak from` producers). A failed delete in
pak_delete_file_retry (0x800A2680, = PS_DeleteFile) opens the game's pak error dialog CONTROLLER 1 | UNKNOWN
ERROR ... | CONTINUE WITHOUT SAVING | RETRY, drawn by **control_settings and control_cell_text2** (the first scripts
that reach them: they are the pak-error/notice menu cells, not only image A's control-settings screen); A = RETRY,
and the second delete succeeds.

Phase 10 scripts (frames in brackets; all pak flow):
- `ghost_pak.press` [5400] ghost_save's AAAA 1-lap ghost race + `poke 28 801148E4 1 1`: SAVE GHOST 3836
  (FindGhosts, func_80104B14), DOWN + A = SAVE: GHOST SAVED ON CONTROLLER PAK 1 (3990, note "RUSH2049AAAA1F",
  256 B), BEST GHOSTS 4080, GAME OVER 4980. Gas released at 127 s (a held A swallows menu presses).
- `ghost_reload.press` [1500] `pak from ghost_pak`: the boot reads the ghost file (draw_ui_element =
  _CheckAndRepairGhost, frame 4), the car screen lists GHOST 1 TIME 1:38.858 and loads it (func_800CBF2C,
  menu_back, 600). SELECT MODE now starts on GHOST RACE (the player's last mode): no DOWN presses.
- `ghost_resave.press` [5400] ghost_pak again from its own pak: SaveGhost replaces the old file of the same
  track/player, i.e. deletes it first: pak_delete_file_retry (3990).
- `ghost_delete_fail.press` [4300] ghost_resave + `pak fail delete 1`: the error dialog (3991), RETRY, saved (4080).
- `unlock.press` [4800] race_finish + `poke 30 801164C0 1 2`: CONGRATULATIONS! 3742 (func_80103D28 98 calls,
  func_800F2718, func_80105480), then race_finish's name entry and menus.

**Initialised function-local statics (`--statics`).** IDO writes NO symbol for a function-local static, initialised
or not (nor for a file-scope `static`); the code reaches them through the `.data`/`.bss` section symbol + offset,
so Phase 3's name lookup found nothing to bind in a draft whose code differs from retail (w29d SaveServer:
sInitCountdown/sLastPak/sReadingPak stayed private, the lane needed a `dc.c` with externs). `static_layout.py`
recovers the layout from the draft as IDO's own preprocessor sees it (`cc -E`): every object definition in source
order (file-scope objects and the `static` objects in function bodies), initialised -> `.data`, uninitialised static
-> `.bss`, each aligned to max(4, natural) (checked on rocky with IDO 5.3 objects: chars 4 bytes apart, a double at
8, unreferenced statics dropped without a gap). It is rejected (statics stay private, a `NOTE static layout:`
line says why) when an object symbol is not where the layout puts it, the layout passes the section end, an
integer initialiser differs from the section bytes, or a size is unknown (struct statics: later objects of that
section unplaced). `--statics NAME=0xADDR` (or `FUNCTION:NAME=0xADDR`) now binds them; with a `--unit` draft the
names apply to every member; the NOTE line names each private static. Checked (rocky): w29d's SaveServer best.c
(no dc.c) `--statics sInitCountdown=0x8011742C,sLastPak=0x80117430,sReadingPak=0x80117434`: `VERDICT: EQUIVALENT
over 3490 calls / 3 scripts (saves 1568, ghost_reload 825, ghost_resave 1097)` - the ghost branch included; without
`--statics`: DIVERGES at saves frame 2 (`word 0x8011742C: retail 03000000->02000000 | draft unchanged`).

**Renamed functions** (wave 29, w29n). Three things kept the old names: (1) `build/frontier.json` (a build product
scanned before the rename): its `register_params`/`unsaved_callee_regs` facts were looked up by name, so 22 renamed
functions lost them (no refusal for a register-parameter draft such as SetupWaveModel, and no clobber shim around
calls to drawMain, PrintScores, TitlePane, control_cell_text...). `replace.frontier_facts` now keys the facts by
today's name through each fact's `address` (draft_check, regmap_verify, replace.load_ipa_detection; also
build/ipa_members.json names). (2) Lane directories and their `dc.args` (`w29d/func_800C813C/` holds SaveServer
now): `draft_check --lane` maps NAME, `--unit` members, `--params MEMBER:` prefixes and `--callee` prototype names
through `replace.stale_function_names()` (frontier fact addresses, every accepted function row of
`cloud/work/frontier/*/renames.json`, and `func_XXXXXXXX` names that have a real name now) and prints a `NOTE ...
renamed since the draft was saved` line. (3) The committed census: w29n's text replacement had also renamed the
SCRIPT `race_finish` to `animPlace` in `census/*.json` (draft_check's default script choice then named a script that
does not exist); the census refresh below regenerated every file.

**Prototypes with function-pointer parameters** (`s32 (*poll)(void)`, control_settings): `find_prototype` /
`parse_prototype` accept them (one word, captured as a pointer); draft_check used to say `best.c has no definition`.

**Rename check** (`--lane` over the 31 drafts w29n renamed, plus w29d/w29a/most of w29b): no divergence caused by
the rename; new first verdicts on reached-for-the-first-time functions (draw_ui_element, func_80103D28 EQUIVALENT;
w28b pak_delete_file_retry and the w29c/w30d control_settings drafts FAULT on the new paths). Table:
`cloud/work/frontier/w30r/RESULTS.md`.

**Census refresh** (2026-10-13, 34 scripts, 166 unmatched at wave30 f0249900, after w30i's installs and w30n's
func_80104704 registration): **159 reached (178,368 B), 7 never (5,260 B)** (first run at 1b34f797: 168 of 175; w29r:
178 of 192 / 14). Newly reached: func_80104B14 (ghost_pak, ghost_resave, ghost_delete_fail),
draw_ui_element (ghost_reload, ghost_resave, ghost_delete_fail), func_80103D28 (unlock), pak_delete_file_retry
(ghost_resave, ghost_delete_fail), control_settings and control_cell_text2 (ghost_delete_fail: the pak error
dialog). Still never: func_8010C974 (_AnimateMine: a mine pickup in battle), func_800DD0C0, control_cell_text3,
control_cell_text (control_settings' other cell kinds: the error dialog draws only control_cell_text2; probably
image A's controller-settings screen, unverified), world_effect_update, fp_call_wrapper, object_bytes23_sum (no static
reference: dead or computed-jalr only).


## Phase 11: battle mines, overlay reach, pak dialogs, stack arguments, all lanes (lane w31r, 2026-10-14)

Lane notes: `cloud/work/frontier/w31r/RESULTS.md`; plan section 20. Not a matching lane; no gameplay change (no
package).

**Battle mines** (script `battle_mine.press` [1500]). Player 1's weapon slot is the halfword car+0x384 of gGameCar
0x80152818 (stride 0x3B8): type byte, ammo byte. func_8010D3C0 (= GC _StartWeaponIcon, the pick-up) stores it
(`sb a0,0x384(v0)`, `sb v1,0x385(v0)`, ammo from the table 0x80121D60); GC tCarData.battle.weapon {type, ammoCnt} is
the same pair. battle.press + `poke 26 80152B9C 0309 2` (type 3 = mine, 9 rounds) + Z (B, L, R do not fire) drops a
mine: the WPR_MINE target (targetInfo entry 0x80118BB0: name WPR_MINE / WPR_MINEBASE, start func_8010C7F4 =
_StartMine, animate func_8010C974 = _AnimateMine) from frame ~839, 573 calls of func_8010C974 and 2 of _StartMine.
Types 0-2 and 4-7 do not reach it. Side result for the overlay lane, `battle_weapons.press` [1800]: type 0 + Z
reaches image B's func_8038A95C (= GC _RemoveCannMFlash), type 1 func_8038AA14 (_RemoveGattMFlash), type 7
func_8038D498.

**Overlay reach: the host runs image A and B code.** The game loads both images through `inflate_entry`
(`rush_hook_inflate` -> `load_overlays`; both at 0x8038A400, a call from outside dispatches by load state), so any
script that opens a screen runs that image's recompiled functions. `script_probe --functions` and the census now
probe overlay functions: items `ovl_a` / `ovl_b` (a whole image) and `ovl_a:NAME` (records name them
`ovl_a_NAME`; replacements.json entries with `"population": "ovl_a"`). `python3 -m tools.recomp.census --overlays`
probes all 242 overlay functions over every script and writes `census/overlays.json` + `census/OVERLAYS.md` only
(the game census files are left alone; no call sites: the `$ra` hooks cover the game image). Image A is the front
end (title, PLAYER, mode/track/car selection; it stays loaded through a normal race: 69 image-A functions in
`race`), image B the race-time image (1 function in a normal race, 23-28 in battle: the weapons).

**The "image A only" functions were not image A only.** control_cell_text, control_cell_text3 and func_800DD0C0 are
cell kinds of control_settings, the modal controller-pak dialog: save_settings (0x800DE20C, game image) runs it for
every pak error, and image A's player screen (func_803990D0, a second caller at 0x803991B4) only when its own flag
0x803B46B4 is set (not reached by any script). A failing osPfsReadWriteFile/osPfsAllocateFile during CREATE PLAYER
(saves.press, frame 720) shows the dialog for the PFS error code (`pak fail write 1 ERR`; read/delete/state faults
in that flow show none):

| ERR | dialog (frame 721) | cell drawn by |
|---|---|---|
| 1 NOPACK | NO CONTROLLER PAK CAN BE FOUND ... PRESS THE A BUTTON TO CONTINUE | func_800DD0C0 |
| 2 NEW_PACK | THIS IS A DIFFERENT CONTROLLER PAK ... CONTINUE WITHOUT SAVING / RETRY | control_cell_text2 |
| 3 INCONSISTENT | THIS CONTROLLER PAK IS CORRUPTED. PRESS THE A BUTTON TO FIX THE DATA. | control_cell_text |
| 4 CONTRFAIL, 5 INVALID | UNKNOWN ERROR ... CONTINUE WITHOUT SAVING / RETRY | control_cell_text2 |
| 6 BAD_DATA | THIS NOTE HAS BEEN CORRUPTED ... FIX THE DATA | control_cell_text |
| 7 DATA_FULL, 8 DIR_FULL | THERE IS NOT ENOUGH SPACE ON THIS CONTROLLER PAK ... | control_cell_text |
| 9 EXIST | THERE IS ALREADY A NOTE WITH THAT NAME ... CONTINUE WITHOUT SAVING / RETRY | control_cell_text2 |
| 10 ID_FATAL, 11 DEVICE | UNSUPPORTED DEVICE ... ATTEMPT TO REPAIR? CONTINUE WITHOUT SAVING / REPAIR / RETRY | control_cell_text3 |
| 12 (other) | UNKNOWN ERROR ... PRESS THE A BUTTON TO CONTINUE | control_cell_text |

Scripts (saves.press up to the CREATE PLAYER START, then A at 27 s; [1100]): `pak_nopak.press` (ERR 1,
func_800DD0C0), `pak_repair.press` (ERR 10, control_cell_text3, DOWN DOWN UP before A), `pak_corrupt.press` (ERR 3,
control_cell_text). Observed, not pursued: `pak fail read 1 3` on player_reload (the boot read of player AAAA) makes
the STOCK host fault at frame 120 (N64 read of 0x00000000; a TLB exception on the console too, if it is the game's).

**Stack arguments are reads** (`ipa_detect`). A store of a callee-saved register to the stack counted as a prologue
save, never as a read; retail control_cell_text passes its register parameter s7 on as a stack argument (`jal
camera_auto_follow; sw s7,24(sp)`), so s7 was missing from its live-in set (regmap_verify REFUTED a correct claim,
--derive omitted it). Now such a store is a save only if the body reloads the register from that slot, writes the
register somewhere, or makes no call; over the 1,216 game functions this changes exactly control_cell_text (s5,s6 ->
s5,s6,s7) and control_cell_text2 (s5,s6,s7 -> s5-s8). The frontier facts (`build/frontier.json`, tools/conveyor's
scanner) list the old sets for both.

**Inbound shim, stack parameter at the same offset** (`replace.ipa_shim`). A retail stack parameter at sp+N whose o32
slot is also sp+N (5th/6th parameter) was a "move" from `sp+16` to `sp+16` and skipped, though the source is the
caller's frame and the destination the shim's outgoing area: control_cell_text3's s2/s3 reached the draft as garbage
(draft FAULTED in state_utility). Sources are now named `csp+N` internally.

**Three callee conventions and matched clobberers** (found by the all-lanes run; every one was a harness gap that
made a correct draft FAULT):
- `audio_reverb_update` takes (address, tag) in **a1, a2** under IPA (regmap_verify `a1=address:ptr,a2=tag:int`
  CONSISTENT: the live-in set is exactly a1,a2; the matched callers in codex_heap_release_a25 / frontier_score_insert
  call it `audio_reverb_update(address, 0)`). Not an o32 hazard ipa_detect can see (a1/a2 are argument registers),
  so drafts passed the address in a0 and faulted in the callee. w30r read this as a w28b draft bug; it was not:
  with the `ipa_callees.json` entry w28b's pak_delete_file_retry, w3b's sound_bank_unload and w3c's display_enable
  are EQUIVALENT.
- `func_80104704` (= GC DrawName) takes p in **s5** (regmap_verify --derive, 60 calls; clobbers s0-s4,s6-s8): w30a's
  func_80105480 draft is EQUIVALENT with it (was a fault in object_manager_update).
- `func_800DD0C0` takes (row, col) in the caller's first two outgoing stack words (**sp+0, sp+4**: retail `sw
  s3,0(sp); sw s4,4(sp); jal`, the callee's 440-byte frame reads 440/444(sp)).
- **Clobber shims from the retail bodies** (`draft_check.retail_clobbers`): build/frontier.json has
  unsaved_callee_regs only for functions its scan covered, so a MATCHED IPA callee that writes callee-saved
  registers without saving them got no shim. func_800DCD58 (control_settings' cell frame, matched) writes s0,s1:
  that was the `lb a0,1(s0)`, s0 = 0 fault of the w29c/w30d control_settings drafts (wave-30 "known draft bug").
  draft_check now derives the unsaved set of every game function the draft's text names from the retail body
  (regmap_verify.Interproc, transitive) and merges it with the facts; callees with non-o32 register parameters
  still need a table entry. w30d's control_settings: EQUIVALENT on ghost_delete_fail, pak_corrupt and pak_repair (90
  calls each); pak_nopak (cell type 2, func_800DD0C0) still FAULTS (N64 read of 0x8 at frame 721, no N64 PC in the
  report) with the sp+0/sp+4 entry: open.

**dc.args for every `--lane` ERROR** (w28d): `attract_mode_handler/dc.args` control_cell_text `--params
row=s5,col=s6,text=s7`; `attract_video_handler` control_cell_text2 `sel=s5,row=s6,col=s7,text=s8`; `attract_demo_handler`
control_cell_text3 `sel1=s5,row=s6,text=sp+8,s1=sp+12,s2=sp+16,s3=sp+20,col=s7,sel2=s8` (the callee's 56-byte frame
reads 64..76(sp)); `func_800E56F8` `ipa_s4=s4` (matched since; not run by --all-lanes). All three cells EQUIVALENT
(control_cell_text 90 calls pak_corrupt, control_cell_text2 90 ghost_delete_fail, control_cell_text3 90 pak_repair).
FindGhosts (s8) and SetHighScoreState (s2), the other two `--lane` ERRORs of Phase 10, are matched: their drafts need
no check and --all-lanes lists them MATCHED. `--name NAME` in a dc.args names a single draft whose directory is not
its function (w30a `audio_options` = game_results_render, `controls` = func_80100564).

**`draft_check --all-lanes [GLOB,...]`** (default `w2[8-9]*,w3*`): every `<lane>/<NAME>/best.c` of the matching lane
directories, one line per draft, then one Markdown table (lane, function, verdict, detail) and the counts. A function
in blob_matched.lock.json is MATCHED and not run (`--include-matched` runs it). Rows are cached in
`~/rush2049/recomp/dc/lanes/cache.json` keyed by the draft and its includes, NAME, dc.args, forwarded options (not `--jobs`), the
text of every script the run uses (the census default choice and `pak from` chains), the ipa_callees.json entries the draft names and
`CACHE_EPOCH` (bump it with a harness fix that can change verdicts); `--refresh` ignores the cache; ERROR rows are
never cached. `--remote rocky` copies the drafts, their includes and dc.args into the service tree and runs there.
`--lane` keeps its old behaviour (runs everything, no cache). Concurrent `--all-lanes` runs over disjoint globs share the cache (rows merged under a lock); a last run over all globs then prints the one table from the cache.

**Census refresh** (2026-10-14, 39 scripts, 166 unmatched at wave31 880e729e): **163 reached (182,996 B), 3 never
(632 B)**: world_effect_update, fp_call_wrapper, object_bytes23_sum (no static reference: dead or computed-jalr
only). Newly reached: func_8010C974 (battle_mine), control_cell_text (pak_corrupt), control_cell_text3 (pak_repair),
func_800DD0C0 (pak_nopak). Overlay census (same tree): image A 99 of 193 functions reached (unmatched: 45 of 112,
92,504 of 101,892 B), image B 35 of 49 (unmatched: 19 of 28, 28,696 of 29,688 B); `census/OVERLAYS.md` lists them.



## Phase 12: spill-aware clobbers, exact fault PCs, the boot-read pak crash, overlay drafts (lane w32r, 2026-10-14)

Lane notes: `cloud/work/frontier/w32r/RESULTS.md`; plan section 21. Not a matching lane; no gameplay change (no
package).

**The control_settings pak_nopak fault was the harness.** w29c/w30d's control_settings drafts faulted on pak_nopak
(cell type 2) at frame 721, N64 read of 0x8. With the PC (below) the faulting instruction is `lb t6,0($s0)` right
after the draft's call of the retail func_800DD0C0 through its IPA shim, with s0 = 0xB. Retail func_800DD0C0 writes
s0, s1 and s3 without saving them (`move s0,v0` = the font call's result 11, `addiu s0,sp,0xAC`, `move s1,v0`,
`addu s3,t6,t7`); it also stores them to the stack (s1/s3 spilled around a call, s0 passed at 0x18(sp)), and
`regmap_verify.Interproc` counted those stores as prologue saves, so the clobber set (and the hand-written
`ipa_callees.json` entry: s2,s4-s8) lacked s0,s1,s3 and the shim did not protect the draft's s0. Rule now: a store
of a register is a save only if the body has not written that register at a lower address (IDO saves in the entry
block, before any write). Over the 1,216 game functions 24 unsaved sets grow (all checked writes: outgoing float
stack arguments `swc1 f22,0x10(sp)`, spills); the table entries func_800DD0C0 (s0-s8), func_80097164 (s0-s7) and
func_80104704 (+s5) are corrected, and draft_check widens every table entry the draft names with the retail set
(`NOTE ...: clobbers widened`) so a stale entry cannot hide one again. Verdicts: w29c and w30d control_settings
EQUIVALENT over 360 calls / 4 scripts (pak_nopak, pak_corrupt, pak_repair, ghost_delete_fail 90 each); w31d's group
draft (func_800DD0C0 in the TU) EQUIVALENT on the same four; w31d's func_800DD0C0 alone (`draft_check func_800DD0C0`,
inbound shim sp+0/sp+4) EQUIVALENT 90 calls pak_nopak. The draft was right. (replace.py's `WARNING ... writes
callee-saved s2,s4,...` line still quotes build/frontier.json; the shim uses the corrected set.)

**Fault reports always carry the N64 PC.** The PC is the `// 0xADDR` comment of the recompiled-C line addr2line
gives for the host pc. clang merged identical tails of several N64 sites (here the `b join; lb t6,0(s0)` delay slot
of every switch case of control_settings) into one block with line 0, so the report had no PC. The host build now
compiles the recompiled game with `-mllvm -enable-tail-merge=false -mllvm -simplifycfg-sink-common=false` (GCC:
`-fno-crossjumping`; CMake option `RUSH_EXACT_FAULT_LINES`, default ON; code size only). A line-0 frame still
names its function (`N64 PC unknown in NAME: ...`). Report of the old fault with it: `N64 PC 0x80800C14 `lb $t6,
0x0($s0)` = control_settings+0x368 (draft)`. Build trees made before Phase 12 rebuild once.

**`pak fail read 1 ERR` on a `pak from` pak crashes the retail game** (the stock-host fault of Phase 11). The boot
reads the player note (SaveServer -> func_800C7EC8 -> drone_set_catchup = the note read: buffer handle at +0x48 of
the file record, 0x80146074 for player 1). A failed read opens the pak error dialog through the function pointer
0x80144008 = save_settings, and save_settings pumps SaveServer every dialog frame: SaveServer re-enters
func_800C7EC8 -> drone_set_catchup for the same note, finds the read in progress, and the player code
(draw_speedometer -> pak_delete_file_retry) deletes the note and frees its buffer, clearing +0x48. When the dialog is
closed the suspended read retries: `lw t8,0x48(s6)` = 0, `lw t9,0(t8)` = a read of 0x00000000, a TLB exception on
the console (the game maps only 0xC0000000 with __osTlbInit). All game code (gdb backtrace, host breakpoints on
each function); the host only injected the error. Sweep (stock host, player_reload + `pak fail read N ERR`, 600
frames): read 1 with ERR 1, 2, 3, 4, 5, 7, 8, 9, 12 crash when the dialog closes (frame 120 or 210); ERR 6 shows no
dialog, 10/11 the repair dialog (no crash in 600 frames); read 2 fails nothing at boot (one read). `parse_script` now
refuses `pak fail read 1 ERR` (ERR not 6/10/11) in a script with `pak from`, naming the cause;
`RUSH_PAK_FAIL_ALLOW_CRASH=1` runs it anyway. Fresh-pak scripts (pak_nopak etc.) are unaffected.

**draft_check of overlay drafts.** NAME may be `ovl_a:NAME` or `ovl_b:NAME` (or `ovl_a_NAME`; a plain name is
found in either image's extents.json; func_8038A400 exists in both and must be qualified). The draft replaces the
function inside its image (replace.py's `population` ovl_a/ovl_b; the hook lives in the recompiled overlay section,
so it runs whenever the game has that image loaded); default scripts come from `census/overlays.json`; build trees
and run dirs are `ovl_a_NAME`; `--all-lanes` lists drafts of functions in ovl_matched.lock.json as MATCHED.
Smoke test (rocky):

```bash
python3 -m tools.recomp.draft_check ovl_a:func_80396F44 src/ovl_a/func_80396F44.c          # its own locked source
#   VERDICT: EQUIVALENT over 2047 calls / 6 scripts (controls_frontend 412, pak_corrupt 327, ... saves 327)
python3 -m tools.recomp.draft_check ovl_b:func_8038A8CC src/ovl_b/func_8038A8CC.c          # image B
#   VERDICT: EQUIVALENT over 16 calls / 6 scripts (attract 7, ...)
# an injected bug (return found + 1): DIVERGES, controls_frontend frame 210 call #1: return value differs
# from the Pi: python3 -m tools.recomp.draft_check --remote rocky ovl_a:NAME path/to/draft.c
```

A lane directory holding an overlay draft names it in its dc.args with `--name ovl_a:NAME`.

**Saves need their restore** (job 3 follow-up, w32n's finding). A store before the first write can still be an
outgoing stack argument: control_cell_text2 passes its s8 parameter on at 0x18(sp) before it writes s8, never reloads
the slot, and then writes s8 unsaved. Interproc now also requires the slot to be reloaded somewhere in the body (every
IDO save has its epilogue restore; a body without calls keeps the old answer). Eight more unsaved sets grow
(audio_doppler_calc s0, best_times_display s2, control_cell_text2 s8, entity_tick_main s5, func_800B0A88 s1,s2,
func_800E398C s7, render_replay_ui s4, save_validate s3; spot-checked: argument stores with no reload). CACHE_EPOCH
`w32r-2`. func_80097164: the conveyor scan says s0-s4 (its own writes); ipa_callees.json keeps s0-s7 because the
shim must cover the transitive set (s5-s7 are written, unsaved, by its callee SetupWaveModel,
was func_80096CA8): Interproc derives s0-s7.

**All lanes after the w32n renames** (2026-10-14, wave32 13219eb6 merged, build/frontier.json + ipa_members.json from
the Pi's post-merge `ipa scan` / `frontier scan`): `ALL LANES w2[8-9]*,w3*: 108 drafts: 47 EQUIVALENT, 5 ERROR, 56
MATCHED`, no DIVERGES/FAULT. The 5 ERRORs are unit directories without dc.args (w31a audio/f5ea8/hs, w32a hs, w32b
audio), each byte-identical to a named twin directory that ran (or a matched pair); the w32a hs unit by hand with
the new names (`--unit highscore_animText,DrawName,DrawGhost,DrawUnlocked`) is EQUIVALENT over 2,473 calls as before
the rename. Log: rocky `~/rush2049/recomp/dc/lanes/w32r-all.{log,json}`.

**Census refresh** (2026-10-14, wave32 13219eb6, 39 scripts, 150 unmatched): **147 reached (164,724 B), 3 never
(632 B)**: world_effect_update, fp_call_wrapper, object_bytes23_sum (no static reference). Overlays unchanged: image
A 99 of 193 (unmatched 45 of 112), image B 35 of 49 (unmatched 19 of 28).

**Build cores.** `RUSH_DC_BUILD_JOBS=N` sets ninja's -j per side (the retail and draft sides build at once; default
8 each).

**Test**: `tests/cloud/test_recomp_phase12.py` (spill rule on synthetic bodies and on retail func_800DD0C0's
committed words, table clobbers, widening, line-0 report, CMake flags, the refusal, overlay names/census/lock; on
rocky: control_settings pak_nopak EQUIVALENT, the overlay own-source/injected-bug pair). `draft_check.CACHE_EPOCH`
is `w32r-1`: every cached `--all-lanes` row reruns.



## Phase 13: unit dc.args, the conveyor's save clause (lane w33r, 2026-10-15)

Lane notes: `cloud/work/frontier/w33r/RESULTS.md`; plan section 22. Not a matching lane; no package.

**dc.args for the five unit directories** `--all-lanes` reported ERROR (Phase 12): w31a `audio`, w32b `audio`
(`--unit audio_animText,audio_animButtonText --scripts audio_options,options_menus,pause_menus`; both members are
locked, so the row is MATCHED), w31a `hs`, w32a `hs` (`--unit highscore_animText,DrawName,DrawGhost,DrawUnlocked
--scripts ghost_pak,unlock,player_finish,race_finish`: EQUIVALENT over 2,473 calls / 4 scripts each) and w31a
`f5ea8` (`--name animResults --params arg0=a0`: EQUIVALENT over 26,166 calls / 6 scripts, = its twin
w31a/func_80105EA8).

**`frontier.unsaved_callee_writes` has Interproc's restore clause.** The conveyor scanner (`build/frontier.json`
`unsaved_callee_regs`, the `group` recipe, the replace.py WARNING text) already took a stack store after the first
write for a spill (w32n); it now also requires the slot's reload (`lw`/`ld` of that register from that offset) when
the body makes a call, as `regmap_verify.Interproc` does since Phase 12. Over the 1,216 game functions exactly
Interproc's eight sets grow (audio_doppler_calc +s0, entity_tick_main +s5, render_replay_ui +s4 among the unmatched;
save_validate +s3, func_800B0A88 +s1,s2, best_times_display +s2, control_cell_text2 +s8, func_800E398C +s7 locked,
so not in the facts); no recipe changes; `frontier calibrate` still flags 0 of 692 standalone locks. Regenerate with
`python3 -m tools.conveyor.pipeline.frontier scan` (no Conveyor DB needed) and copy build/frontier.json to rocky's
service tree. Test: `tests/conveyor/test_frontier.py::test_a_save_in_a_calling_body_needs_its_reload`.

**All lanes, wave 33 final** (wave33 1fe5fe8c0, game 1077/1216, `--refresh`): `115 drafts: 40 EQUIVALENT, 2 ERROR, 73
MATCHED`, no DIVERGES/FAULT; the five old unit dirs produce rows (hs 2,473 calls each, f5ea8 26,166, audio MATCHED).
The two ERRORs were new w33a dirs not named after their function (`ar`, `f0e58`); their dc.args now carry `--name`
(EQUIVALENT 26,166 and 3,405 calls by hand). Census: 136 of 139 unmatched reached (152,616 B), the same 3 never
(632 B); overlays unchanged (A 99/193, B 35/49).
