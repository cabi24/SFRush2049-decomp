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

## RDRAM accessors

Phase 2's ROM-free host adapter is `rdram.h` (C11/C++11 or newer) plus
`game_rdram.h` (named field views from `include/game_types.h`). These are standalone
headers; they neither require nor modify the ROM, matching sources, generated
recompiler output, or runtime build. They do **not** make unchanged native C pointer
dereferences safe. A replacement must explicitly adapt each guest-memory read/write
and keep guest pointers as `rush_addr` (`uint32_t`) tokens.

The layout follows the pinned [N64Recomp 81213c18 `recomp.h`](https://github.com/N64Recomp/N64Recomp/blob/81213c18/include/recomp.h):
`MEM_B` uses address XOR 3, `MEM_H` XOR 2, and `MEM_W` accesses the unswizzled
word; `LD`/`SD` put the high word first. Thus guest bytes `11 22 33 44 55 66 77 88`
occupy host storage `44 33 22 11 88 77 66 55`. A native struct cast is wrong for
byte/halfword members, and host pointers also change layouts: e.g. `PadConfig.unk14`
is guest offset 0x14, regardless of `sizeof(void*)` on the host.

- `rush_rdram memory = {rdram, actual_allocation_size}` describes word-swapped
  storage. Supply its real allocated length, a nonzero multiple of four, at most
  512 MiB. The implementation never assumes the runtime's allocation is 8 MiB.
- `rush_rdram_read_KIND(&memory, guest_address, &value)` and
  `rush_rdram_write_KIND(&memory, guest_address, value)` return `bool`. `KIND` is
  `u8/u16/u32/u64`, `s8/s16/s32/s64`, `f32/f64`, or `addr`. The memory descriptor is
  passed as `const rush_rdram*`; writes change the backing bytes, not the descriptor.
- Only KSEG0 (`0x80000000 + offset`) within that allocation is dereferenceable.
  Physical/KSEG1/TLB/MMIO addresses are rejected; no broad address mask aliases
  them into RDRAM. Null is a valid pointer **value**, never a dereferenceable address.
  `rush_addr_from_gpr` accepts zero-extended or canonical sign-extended o32 tokens;
  other high bits fail. `rush_addr_add` rejects unsigned address overflow.
- Accesses assemble logical big-endian bytes without native aliasing/alignment
  casts, including misaligned buffers and spans crossing words. This is a data
  view, not an emulator of MIPS alignment exceptions. Integer accesses are host
  endian independent. Floats require binary32/binary64 (compile-time checks) and
  use `memcpy` bit transfer, preserving tested signed zero, infinity and quiet-NaN
  payloads. Use integer accesses when transporting arbitrary signaling-NaN bits
  without floating-point evaluation.
- A failure leaves the output and backing memory unchanged. Each scalar operation
  is checked before touching memory, including the complete width; a sequence of
  successful field writes is not a transaction. Check returns and choose an
  explicit replacement fault policy. The API does not add locking or volatile/MMIO
  semantics; runtime/thread synchronization remains the caller's responsibility.

A `rush_rdram_view` pairs the descriptor with an N64 base address. Named accessors
are `rush_TYPE_read_FIELD(view, &value)` / `rush_TYPE_write_FIELD(view, value)`;
`Track_Data.start_time/end_time` also take a checked index (0..7). All named scalar,
data-pointer and function-pointer fields in `game_types.h` have views; unknown
padding is left untouched. The embedded `SegmentHeader.unk9CC0` returns a bounded
subview via `rush_SegmentHeader_view_unk9CC0`; it is not copied into a native
`OSMesgQueue`. Tokens loaded from callback fields must go through runtime dispatch,
never a host function-pointer cast. Views do not bind globals or infer array counts.

Example adapted body (a fixture-style example, not a matched-source conversion):

```c
#include "tools/recomp/game_rdram.h"

bool clear_input(const rush_rdram *memory, rush_addr input_address) {
    rush_rdram_view input = {memory, input_address};
    uint8_t active;
    if (!rush_InputRecord_read_active(input, &active)) return false;
    return !active || rush_InputRecord_write_unk04(input, 0);
}
```

For other fields/structs, use the width-specific memory primitives and checked
`rush_rdram_subview`, with evidenced guest offsets. Do not use native `offsetof`
or `sizeof` to derive layouts containing pointers. The typed field table is checked
against **every non-padding named field** of the current `game_types.h`; header
changes require reconciling this table. The tests independently compute the o32
layout from source declarations with `pycparser` and, where supported, also compile
original-header offset/width assertions with a 32-bit compiler (MIPS preferred).

```bash
python3 -m pytest tests/recomp/test_rdram.py -q -p no:cacheprovider
# Optional comparison to your existing pinned N64Recomp checkout (no downloads):
RUSH_N64RECOMP_INCLUDE=/path/to/N64Recomp/include \
  python3 -m pytest tests/recomp/test_rdram.py -q -p no:cacheprovider
```

Fixtures compile/run with both C11 and C++17, strict aliasing and warnings-as-errors,
and separately with UBSan. They check independent byte-order examples, every offset
for 1/2/4/8-byte accesses in a synthetic buffer, cross-word accesses, signed extremes,
float bits, pointer tokens/chains/callback fields, padding preservation, arrays,
null/unsupported mappings, overflow, short buffers, and failure-without-partial-write.
No ROM is read. This verifies the adapter contract, not replacement equivalence,
runtime boot, or Phase 2's ten-function/in-game acceptance criteria.

The optional actual-header test compares both directions against upstream `MEM_*`,
`LD` and `SD` using an allocated synthetic buffer. It skips if the header location
is not supplied; no network access or vendored runtime code is needed for normal
fixture tests. Upstream-header execution was checked against commit `81213c18` on
the development host; CI without that checkout covers the independent fixtures.

## o32 replacement wrappers

`gen_wrapper.py` generates one C++17 wrapper with the runtime entry ABI
`extern "C" void NAME(uint8_t* rdram, recomp_context* ctx)` from one explicit C
prototype. It is ROM-free: it neither reads the ROM nor edits emitted code,
`patches.json`, replacement selection, or the runtime build. Compile the output
against **the real runtime's** `recomp.h` and `tools/recomp/`; do not use the
synthetic context header under `tests/recomp/fixtures/` in a host build.

```bash
python3 tools/recomp/gen_wrapper.py \
  --prototype 's32 example(s32 count, f32 scale, u64 mask);' \
  --target rush_host_example --out build/recomp/example_wrapper.cpp

# Memory-using functions need an explicit host adapter. Pointer arguments and
# returns become rush_addr (32-bit guest tokens), never native host pointers.
python3 tools/recomp/gen_wrapper.py \
  --prototype 'InputRecord* advance(InputRecord* record, s32 count);' \
  --guest-pointers --target advance_adapter \
  --out build/recomp/advance_wrapper.cpp

python3 -m pytest tests/recomp -q -rs -p no:cacheprovider
```

Input may instead come from `--prototype-file` (exactly one declaration).
`--wrapper ovl_a_NAME` selects the runtime symbol independently of the prototype
and target, including population-prefixed overlay names. The default target is
`rush_host_NAME`; the target and wrapper must differ to avoid recursive symbol
collisions. Output is deterministic, and rejected input leaves an existing output
file unchanged. The generator emits the host target declaration so its complete
contract is reviewable alongside the wrapper.

### Host target and pointer boundary

The default host target has C linkage and canonical fixed-width scalar types:
`s32`/o32 `int`/o32 `long` become `int32_t`, `u64` becomes `uint64_t`, etc. Do not
link a native LP64 `long` definition against an emitted `int32_t` declaration;
provide an adapter or compile the body with matching fixed-width types. Scalar
wrapping alone does not redirect globals, cross-calls, pointer arithmetic, or
`M2C_FIELD` accesses in a matched body.

`--with-rdram` prepends `const rush_rdram* memory` to the host target.
`--guest-pointers` enables flat pointer declarators, changes every pointer
parameter/return to `rush_addr`, and implies `--with-rdram`. For the second example
the target contract is:

```cpp
extern "C" rush_addr advance_adapter(const rush_rdram* memory,
                                    rush_addr record, int32_t count);
```

Implement this adapter with the RDRAM scalar/typed-view accessors. Original
`include/game_types.h` pointers and `record->field`/`M2C_FIELD` expressions are
**not transparently host-compatible** and must be adapted; a native struct cast
cannot fix word swapping or 64-bit host pointer layout. A pointer token may be
null or a non-dereferenceable guest address; token conversion does not dereference
it or validate its pointee. Register pointer arguments must be canonical zero-
or sign-extended 32-bit values, otherwise the wrapper aborts instead of silently
truncating a host pointer. Function-pointer declarators/calls are not supported.

`--rdram-size` supplies the accessible allocation span (default `0x800000`,
8 MiB); this must not exceed the actual backing allocation. Fixture wrappers
use their much smaller allocation size. Stack arguments use checked KSEG0
accessors, require an 8-byte-aligned, zero/sign-extended o32 stack pointer, reject
address-addition overflow and out-of-range reads, and abort **before calling the
host target** if a read fails. The runtime wrapper ABI has no error-return channel.
The adapter must separately handle accessor failures for its own memory accesses.

### Supported ABI subset

- Big-endian, hard-float **o32**, using the context fields in the pinned
  [N64Recomp 81213c18 header](https://github.com/N64Recomp/N64Recomp/blob/81213c18/include/recomp.h).
  The runtime's existing
  [helpers](https://github.com/N64Recomp/N64ModernRuntime/blob/cdf5abbd5026fef5c364c676e4667c45e42b6863/librecomp/include/librecomp/helpers.hpp)
  and local `__fix{sf,df}di` shims establish the scalar/64-bit return convention.
- `void` returns; signed/unsigned 8/16/32/64-bit scalars (`sN`, `uN`,
  `intN_t`, `uintN_t` and ordinary explicit signed/unsigned C spellings);
  `f32`/`float`, `f64`/`double`. Plain `char` is rejected because signedness is
  unspecified; `char*` is allowed only in guest-pointer adapter mode.
- Arguments occupy 32-bit words. The first four word slots use `r4`–`r7`;
  later slots use entry `sp + slot * 4` (first stack word at `sp + 16`, after
  the caller's register home area). 64-bit values align to even word slots,
  possibly leaving a GPR/stack hole, and use high word then low word.
- Only a leading first/second floating argument uses `f12`/`f14`, while still
  consuming its normal argument slots. An integer/pointer ends that prefix;
  later floats use GPR/stack bits. Mixed float/double pairs preserve even-slot
  alignment. This follows the o32 implementation in
  [libffi](https://github.com/libffi/libffi/blob/v3.4.6/src/mips/o32.S).
- Integer returns up to 32 bits go to `r2`. Signed narrow types sign-extend;
  unsigned narrow types zero-extend; the resulting 32-bit word sign-extends to
  the runtime's 64-bit GPR storage (including `u32` and guest pointer returns).
  64-bit integer returns use high `r2`, low `r3`, each sign-extended from its
  word. Float/double results go to `f0.fl`/`f0.d`. A void wrapper leaves return
  registers alone. Other context fields are preserved.
- Unprototyped `()`, variadics, aggregate/enum values and aggregate returns,
  unknown typedefs, function pointers, array declarators, `long double`,
  `_Bool`, `_Complex`, attributes, comments and preprocessor input are rejected
  explicitly. This is a bounded declaration parser, not a C preprocessor.
  Other ABIs, soft-float and structure-return hidden arguments are not inferred.

`tests/recomp/test_wrappers.py` checks the planner and compiles/runs generated
wrappers with synthetic register/RDRAM fixtures under UBSan, strict aliasing and
warnings-as-errors. Set `RUSH_RECOMP_INCLUDE=/path/to/N64Recomp/include` to run
the same wrapper fixtures against the real runtime header instead of the synthetic
context; no runtime linking or ROM is required. It covers FPR/GPR/stack transitions, alignment holes, signed
and unsigned widths, 64-bit word order, pointer tokens, memory writes, signed
zero/NaN return bits, and fail-closed invalid inputs/stacks. The separate
`Recomp ROM-free fixtures` workflow runs this and the other `tests/recomp` suites
on the submitted head; the private-ROM integration test still skips without its
private input. These checks do **not** prove a matched-game replacement, runtime
boot, frame equivalence, or Phase 2 acceptance. Build selection, adapting matched
bodies, ten-function integration and the frame oracle remain owner-machine work.
