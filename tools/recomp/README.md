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
inventory misses). The 578 static functions include 4 stubs (empty bodies: `osInvalICache_full`,
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
- **Why identity.json**: N64Recomp replaces libultra by NAME. The static inventory names ~40
  libultra functions wrongly (ultralib VERSION_J): e.g. `osJamMesg`/`osSendMesg` are swapped,
  `osViSetMode` is `osViSwapBuffer`, `osSetEventMesg` is `osCreateViManager`, `osSetTimerIntr`
  is `osViSetEvent`, `osViModeNtscLan1/Lpn1` are `osSpTaskLoad/StartGo`. Without the renames
  the host replaces the wrong functions and the real ones poke hardware registers (boot
  segfaults in `osPiRawReadIo` in `main`). Names ending `_rx` are deliberate non-canonical
  names for functions that must stay emitted (or be shimmed). Inventory/coverage unchanged.
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
