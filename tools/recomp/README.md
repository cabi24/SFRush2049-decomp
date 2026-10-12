# tools/recomp: static recompilation (Phase 0 config, Phase 1 host)

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
| `oracle.py` | Phase 2: compares per-frame oracle logs (stock vs replaced): EQUIVALENT / DIFFERENT at frame F / NOT EXERCISED |

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
