# Boot-tail Packet 1: role and reference findings

Read-only analysis of `cabi24/SFRush2049-decomp` at `b16dd93ba51ac3e04e8332b1f710337b5b2fbd8f`, including the committed protected boot-tail targets. No compiler run, scoring, matching submission, source vendoring, or out-of-range image-byte inspection was performed. Inventory edges/references are leads; the instruction observations below are separately checked against the tracked text. All `+offset` values are byte offsets from the named function start. Reference metadata and pinned links are in `references.json`.

## Findings that change the initial hypotheses

1. There is a concrete MusyX source-family lead. `func_80023E9C` has the distinctive `macHandleActive` interpreter scaffold in the public AxioDL MusyX implementation: maximum 32 steps, a two-word/eight-byte macro fetch, seven-bit opcode, and an upper bound of 114 (`0x72`). This is materially stronger than a generic audio guess.
2. `0x80026360–0x800277D0` contains packed-bitstream/parameter decode and fixed-point processing. The large `800268D0` is not supported as printf by its native operations. The known printf strings are now traced to the existing counted-static formatter at 80002CD0, outside the 412-function population.
3. Nine direct calls from the protected tail text target four addresses at or past the declared text end. The inventory omits these by construction. This requires maintainer classification of `800277D0`, `800278C4`, `80027B44`, and `8002C5E0`. Do not extend, merge or regenerate any extent in this packet.
4. Of 21 `called_from_game` inventory rows, 20 are `in_scope`; the other is excluded `func_800205E4`. The supplied claim that all 21 belong to the 412-function lane should not be repeated.
5. The four D890–D8D0 seed references are not all microcode pointers. Several are scalar floating-point loads, a byte-string copy, and a switch jump table. Audio identification has stronger independent evidence in AI calls, the audio task descriptor, and MusyX source patterns.

## Named source leads

These labels mean an actual relevant definition was found. They do not mean complete reconstruction, exact SDK-version identity, a compiler match, or cartridge acceptance. Do not promote every neighboring row to SOURCE-LEAD merely because one anchor is supported.

### `80023E9C`: `macHandleActive` (SOURCE-LEAD)

Reference: `AxioDL/musyx`, revision `78d2e16e4905fc675952162d331c24d5198b2687`, `src/musyx/runtime/synthmacros.c`, named definition `macHandleActive`; pinned `LICENSE` is CC0-1.0.

- Native `+0x290..+0x2A4`: increment a byte counter and compare the incremented count against 33.
- `+0x2A8..+0x2D8`: fetch two words from the current macro address and advance that address by eight bytes, using unaligned accesses for the packed voice fields.
- `+0x2E0`: mask opcode with `0x7f`; `+0x2E4`: unsigned bound 114; `+0x2F0..+0x300`: indexed dispatch through `D_8002D964`.
- Reference definition uses `++DebugMacroSteps > 32`, two `MSTEP.para` words, `++curAddr`, `para[0] & 0x7f`, and cases through `0x71`.

This is a sound-macro interpreter candidate. It is not a printf format-character switch. The source explicitly supports PC/Dolphin targets and version-dependent 2.x layouts; it is not a recovered N64 translation unit. Missing jump-table entries prevent exact opcode-to-target naming from this packet alone.

### `8001E790`: `sndRand` (SOURCE-LEAD)

Reference: same pinned MusyX revision, `src/musyx/runtime/snd_service.c`, `sndRand`.

- State load/store at `D_8002CC50`.
- Native `+0x0C/+0x10`: assemble `0xA8351D63`; `+0x14`: unsigned multiply; `+0x18`: keep low 32 bits; `+0x1C`: shift right by six; `+0x28`: truncate returned value to 16 bits.
- Public definition multiplies `last_rnd` by decimal `2822053219` (`0xA8351D63`) and returns a `u16` after shifting six.

Consequently the inventory's `0xA8351D63` hilo value is an arithmetic constant, not a hardware-register reference.

### `80020494`: `sndMasterVolume`; `8002043C`: `sndVolume` (SOURCE-LEAD)

Reference: same pinned MusyX revision, `src/musyx/runtime/snd_synthapi.c`.

- Both wrappers gate on `D_8002C630` and bracket calls using `80014594` / `800145DC`.
- `80020494 +0x2C..+0x48` conditionally calls `8001B9F8` with volume byte, time halfword, and group `0x15` when the music argument is nonzero. `+0x4C..+0x68` does the same for group `0x16` when the FX argument is nonzero.
- This distinctive pair of group constants and argument widths matches `sndMasterVolume`'s public structure.
- `8002043C +0x28..+0x3C` forwards byte volume, halfword time and byte group into the same callee, consistent with `sndVolume`.
- Explicit difference: native passes zero as the fifth argument; current public source passes `-1`. Native initialization gating also differs from reference assertion handling. No exact-body claim is justified.
- `8001B9F8` is therefore a `synthVolume` role HYPOTHESIS, reinforced by volume interpolation arithmetic, not an RSP-setup label solely from its `D8D0` reference.

### `80024BF0–80024FA8`: MusyX sound-math helpers (SOURCE-LEAD for the four named bodies)

Reference: same pinned MusyX revision, `src/musyx/runtime/snd_math.c`.

- `80024BF0` / `salApplyMatrix`: three output floats. Each is the dot product of a 3-float matrix row with a 3-vector plus translation, with translation inputs at offsets 36, 40 and 44. Native output stores are at `+0x34`, `+0x6C`, `+0xA8`.
- `80024C9C` / `salNormalizeVector`: three input squares and their sum at `+0x08..+0x2C`, call to static `sqrtf` at `+0x28`, then three divisions by the returned length and in-place stores at `+0x40..+0x54`. Length remains the return value.
- `80024D04` / `salCrossProduct`: three paired products, three subtractions, outputs at `+0x20`, `+0x44`, `+0x6C`, with the standard cross-product component index pattern.
- `80024D74` / `salInvertMatrix`: opening three cofactors and reciprocal determinant at `+0x08..+0x90`, followed by output matrix writes. This is a strong structural reference, but the full 564-byte operation order has not been compared line by line here; keep that limitation with the row.

These operations alone are generic, but their contiguous order and the independently supported surrounding MusyX anchors make the named reference useful. Do not describe these as graphical task submission merely because they contain matrix math.

## Other grounded cohorts and anchors

### `80010450–800107D4`: callback-driven transfer/completion services (HYPOTHESIS)

Shared `80037FA0`-family state; `105C4` creates a queue; `10628` calls `osRecvMesg`, `osInvalDCache`, `osYieldThread`, and the `14594/145DC` access pair; `10714` invalidates cache and uses the same access pair. These are transfer/completion services. No direct call to static `osPiStartDma` was found among the 14 distinct in-scope static targets. Do not assign PI/cartridge identity solely from queue/cache operations.

### `800107E0–80010A40`: sound-library lifecycle/public wrappers (HYPOTHESIS except existing verified getter)

Game-called `10840` and `108E0` clear active flag `D_8002C630`, clamp voices to 32, store two additional byte limits in `D_8004FA18..1A`, call different hardware-init paths (`143C0` / `14434`), then common `107E0` if initialization succeeds. Public `snd_init.c` offers useful vocabulary (`sndInit`, `DoInit`) but has different argument ordering, a 64-voice cap and extra studios/ARAM logic. Use lifecycle-role labels; exact init names remain hypotheses. `10A00` is the pre-existing verified getter and is not a new match.

### Audio driver/task path, especially `80010A40–80014550` (HYPOTHESIS)

- `10A40` calls `osRecvMesg`, `osWritebackDCache`, `bzero`, `osAiSetNextBuffer`.
- `10C68` creates queue/thread, registers an event, starts the thread and calls `osAiSetFrequency`; `10D3C` also calls `osAiSetFrequency`.
- `11910` writes a task descriptor at `D_800382F8`: type 2 at offset zero, flags zero, boot pointer `80027DD0`, boot size from `80027EA0 - 80027DD0`, code pointer `8002A050` with size 4096, data pointer `8002E210` with size 2048, input pointer/count in expected `OSTask` slots; it writes back cache and invokes the callback at `D_80038000`. Only stored addresses were inspected, not data at those addresses.
- Public `include/PR/mbi.h` defines `M_AUDTASK=2`, and `sptask.h` documents the corresponding descriptor layout. Those headers carry restrictive SGI notices; cite structure only, do not vendor.
- `11104 +0x110/+0x1E4` load floating values from `D8B8/D8BC`; `114C0 +0x4C/+0x134/+0x244` load floats from `D8C0/D8C4/D8C8`. They are not microcode-pointer loads.
- `14198 +0x84/+0xD4` forms bases `D890/D8A4`, then `+0x17C..+0x1C4` performs a byte-terminated copy into playback-info offset 262. The data contents remain uninspected.
- `1B9F8 +0x54..+0x74` dispatches six cases indexed from group value 250 through a table at `D8D0`. This is not a microcode pointer either.

### `80014550–8001467C`: nested queue-token exclusion (HYPOTHESIS)

`14550` creates a one-message queue and seeds it. `14594` blocks receiving only when nesting count `D_8002C5DC` is zero, then increments. `145DC` decrements and releases the queue token only on transition to zero. The public ultralib/libreultra `piacs.c` provides an actual one-message lock-family comparison, but it does not contain this same nesting counter; avoid simply renaming the pair to `__osPiGetAccess`/`__osPiRelAccess`. The latter's identified excluded body at `14650` remains outside this lane.

### `80020610–800218CC`: controller/input-value calculation (HYPOTHESIS)

The block's state tables and its series of tiny wrappers `21428..21528` into `21150` resemble MusyX `snd_midictrl.c` input-controller calculation. Native first wrappers pass offsets 196, 214 and 232 into the same voice object; public `inpGetVolume`/`inpGetPanning`/`inpGetSurPanning` wrappers exist but add a third dirty-mask argument absent from these native wrappers. This is a family lead, not enough for exact per-wrapper names.

### `80024FD4–80025EAC`: shared stream/buffer state (HYPOTHESIS)

Exactly ten in-scope members reference `D_8002D480`: `24FD4`, `252AC`, `25594`, `255F0`, `25670`, `2574C`, `259A8`, `25AB4`, `25C68`, `25DC0`. Two also reference `D_8002D484`: `25AB4`, `25DC0`.

- These are byte-flag accesses in the inspected text, not proven 32-bit data words.
- Common state at `D_80056230`, `D_80058680..A0`; queue-token helpers at `250AC/250F0/25120/25150`; cache invalidation in initialization paths.
- `24FD4` computes a per-channel record address with stride 4648 and advances a per-record cursor at offset 4636 after calling `262BC`.
- `25AB4` uses the extra flag and a callback through `D_8003801C`; `25DC0` shares the callback/lock infrastructure.
- This is a good contiguous stream/buffer cohort. Exact codec, cartridge source, task names and original TU are unresolved. The public Dolphin/PC MusyX stream module is conceptual context, not a direct N64 donor proof.

### `80025EB0–800277D0`: stream decode state and packed parameter processing (HYPOTHESIS)

- `25EB0` zeroes an initial 400-byte structure region; creates a one-message queue at object+4524; initializes bit cursor +4552, read/write positions +4556/+4560/+4564/+4568, and byte state +4573.
- `262BC` consumes a bounded amount from buffer positions and changes state 3→4 when an outstanding count expires. `26328` changes state 2→3; `26348` sets state 4. These explain the tiny helpers naturally, without a printf assignment.
- `268D0` reads the bit cursor at +4552, repeatedly masks ring indexes by 1023, accesses source words at +416, extracts groups of 1–7 bits, and assembles fields passed to `26558`. `26640` extracts packed fields from a fixed input record into the same processing path.
- `26360` converts signed short parameters using fixed-point constants and writes short state fields; `26558` loops four times over 26-byte parameter groups before final processing.
- Specific codec identification requires the external callees and data contracts. Do not infer MPEG, ADPCM, a particular speech codec or libc from adjacency alone.

## Printf path resolved outside the tail

The inventory contains no direct hilo reference to the supplied `D4F8–D53C` strings. A bounded instruction screen of the protected 439 sections found no rs=28 (gp) use. Immediate arithmetic/logical comparisons against +37 or -37 only produced `25F74 +0x194/+0x260`, both unsigned numeric bound checks; the surrounding code works with buffered input positions, not format bytes. This screen is a negative heuristic, not a proof that formatting is absent: tables, transformed character tests, pointer loads and callers outside the tail can evade it.

The public ultralib/libreultra `_Printf` implementations explicitly scan bytes for `%`, parse flags/width/precision, dispatch format characters, and invoke integer/float conversion helpers. FreeBSD `vfprintf.c` contains lower/upper digit tables and a null-string fallback but is a current BSD-3-Clause implementation, not evidence of this binary's ancestry. Its current revision does not contain the exact `bad base` diagnostic. Do not use its generic string overlap as identity proof.

A narrow read-only lookup resolved the path in already tracked counted-static targets. `asm/us/nonmatchings/rom/lib_34a0/sprintf.s` shows `80004990 +0x2C` calling `80002CD0` with destination, format and varargs pointer. The callee is historically named `fcvt`, but `asm/us/nonmatchings/rom/lib_34a0/fcvt.s` provides explicit formatter evidence:

- Byte scans compare with `%` at `80002D7C/80002D80` and `80002D98/80002D9C`.
- `80002E10` subtracts ASCII space (`0x20`) from the format byte, `80002E14` bounds the result by `0x59`, then `80002E24..80002E30` performs a character-indexed dispatch through `D_8002D558`.
- Direct address formations: `800033DC/800033E0` → `D_8002D4F8` (lowercase digit table), `80003420/80003424` → `D_8002D50C` (null-string fallback), `800035CC/800035D0` → `D_8002D514` (uppercase digit table), `8000383C/80003840` → `D_8002D53C` (bad-base diagnostic).
- These associations use the string identities supplied by the spec; no out-of-scope data bytes were inspected.

The requested string/format-switch path is therefore located in counted-static code, not in the 412 tail rows. There is no need to invent a tail printf cohort. This is an evidence-backed scope correction: `80002CD0` is a vsprintf-like core native-evidenced role observation, not a symbol correction or a new target assignment. Historical `fcvt` and `gFcvtTemp*` names should be proposed for maintainer review separately; no symbol file was changed. No static function was reconstructed, compiled, or scored.

## Exact external-input requests / stopping boundaries

1. **Tail-end classification:** maintainer should classify the targets of these existing protected-text JALs using owned original mapping evidence, without publishing bytes: `10450+0xA0`, `12234+0xD8`, `12730+0x104/+0x380/+0x460`, `12D18+0x78` → `8002C5E0`; `26360+0x1CC` → `80027B44`; `26558+0x60` → `800277D0`; `26558+0x74` → `800278C4`. Report whether each is CPU text, a trampoline, an address-resolution issue or another declared category, with provenance. No out-of-range content was read here.
2. **Printf naming follow-up (path resolved):** maintainer may review the historical `fcvt`/`gFcvtTemp*` names at the existing static formatter and data aliases using the exact direct-reference path above. No missing-input blocker remains for locating the supplied strings; no rename is applied in this packet.
3. **Owned arcade/source lookup:** search authorized `reference/repos/rushtherock/` (not present in this checkout) for `bug in vsprintf: bad base`, lower/upper hexadecimal digit strings, `N64 RSP mixer`, `MERRILL BLACKOUT`, and the multiplier `0xA8351D63`/`2822053219`. Return exact path, function/definition name, source revision and licensing/provenance. Absence is useful; no arcade identity is assumed. If relevant owned N64/MusyX headers exist, also identify macro opcode/version and stream callback interfaces for the 32-voice / 0x15–0x16 volume-group implementation.
4. **F3DEX/PI scope:** no confirmed F3DEX task-submission family or direct PI-start-DMA call was located in the 412 rows. Preserve these as unresolved expected families; the audio task and callback-driven transfers do not establish them.

All graph cohesion percentages must state that they cover the inventory's tail JAL edges only. Indirect callbacks and the nine newly enumerated beyond-tail JALs are omitted by that model.

## Pinned reference index and licensing

No third-party source was vendored. `references.json` is the machine-readable
index. Each reference below records the inspected revision, paths and license
limitations. A reference name is not a redistribution grant for any separate
proprietary source drop.

### BSD_PRINTF

FreeBSD/freebsd-src at `262fa4695d90207b9080f2cc5cb01eccf07fe243`.

BSD-3-Clause in file header

Modern format parser contrast; digit tables/null handling do not establish source ancestry

- [`lib/libc/stdio/vfprintf.c`](https://github.com/FreeBSD/freebsd-src/blob/262fa4695d90207b9080f2cc5cb01eccf07fe243/lib/libc/stdio/vfprintf.c)

### LIBDRAGON_AUDIO

DragonMinded/libdragon at `e356bf3f56f7afbf7e5246329562f145965cfdfc`.

Unlicense/public-domain dedication per LICENSE.md

Modern audio-buffer/AI hardware design contrast, not historical donor

- [`src/audio.c`](https://github.com/DragonMinded/libdragon/blob/e356bf3f56f7afbf7e5246329562f145965cfdfc/src/audio.c)
- [`LICENSE.md`](https://github.com/DragonMinded/libdragon/blob/e356bf3f56f7afbf7e5246329562f145965cfdfc/LICENSE.md)

### LIBRE_PRINTF

n64decomp/libreultra at `1aca5c13ca041cef86f8dc194b727361dad9c09b`.

No license/copying file in pinned recursive tree; these source files have no permissive grant. Structure-only; do not vendor.

structure comparisons, not matches

- [`src/libc/xprintf.c`](https://github.com/n64decomp/libreultra/blob/1aca5c13ca041cef86f8dc194b727361dad9c09b/src/libc/xprintf.c)

### LIBRE_QUEUE

n64decomp/libreultra at `1aca5c13ca041cef86f8dc194b727361dad9c09b`.

No license/copying file in pinned recursive tree; these source files have no permissive grant. Structure-only; do not vendor.

structure comparisons, not matches

- [`src/io/piacs.c`](https://github.com/n64decomp/libreultra/blob/1aca5c13ca041cef86f8dc194b727361dad9c09b/src/io/piacs.c)

### MUSYX_API

AxioDL/musyx at `78d2e16e4905fc675952162d331c24d5198b2687`.

CC0-1.0 in pinned LICENSE (not cached default-branch web MIT rendering)

Named SOURCE-LEAD definitions for specific targets; PC/Dolphin sources with version2.x defaults, not a proven N64 source drop

- [`src/musyx/runtime/snd_synthapi.c`](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd_synthapi.c)
- [Pinned license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE)

### MUSYX_INIT

AxioDL/musyx at `78d2e16e4905fc675952162d331c24d5198b2687`.

CC0-1.0 in pinned LICENSE (not cached default-branch web MIT rendering)

Named SOURCE-LEAD definitions for specific targets; PC/Dolphin sources with version2.x defaults, not a proven N64 source drop

- [`src/musyx/runtime/snd_init.c`](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd_init.c)
- [Pinned license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE)

### MUSYX_MACROS

AxioDL/musyx at `78d2e16e4905fc675952162d331c24d5198b2687`.

CC0-1.0 in pinned LICENSE (not cached default-branch web MIT rendering)

Named SOURCE-LEAD definitions for specific targets; PC/Dolphin sources with version2.x defaults, not a proven N64 source drop

- [`src/musyx/runtime/synthmacros.c`](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthmacros.c)
- [Pinned license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE)

### MUSYX_MATH

AxioDL/musyx at `78d2e16e4905fc675952162d331c24d5198b2687`.

CC0-1.0 in pinned LICENSE (not cached default-branch web MIT rendering)

Named SOURCE-LEAD definitions for specific targets; PC/Dolphin sources with version2.x defaults, not a proven N64 source drop

- [`src/musyx/runtime/snd_math.c`](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd_math.c)
- [Pinned license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE)

### MUSYX_MIDICTRL

AxioDL/musyx at `78d2e16e4905fc675952162d331c24d5198b2687`.

CC0-1.0 in pinned LICENSE (not cached default-branch web MIT rendering)

Named SOURCE-LEAD definitions for specific targets; PC/Dolphin sources with version2.x defaults, not a proven N64 source drop

- [`src/musyx/runtime/snd_midictrl.c`](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd_midictrl.c)
- [Pinned license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE)

### MUSYX_SERVICE

AxioDL/musyx at `78d2e16e4905fc675952162d331c24d5198b2687`.

CC0-1.0 in pinned LICENSE (not cached default-branch web MIT rendering)

Named SOURCE-LEAD definitions for specific targets; PC/Dolphin sources with version2.x defaults, not a proven N64 source drop

- [`src/musyx/runtime/snd_service.c`](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd_service.c)
- [Pinned license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE)

### MUSYX_STREAM

AxioDL/musyx at `78d2e16e4905fc675952162d331c24d5198b2687`.

CC0-1.0 in pinned LICENSE (not cached default-branch web MIT rendering)

Named SOURCE-LEAD definitions for specific targets; PC/Dolphin sources with version2.x defaults, not a proven N64 source drop

- [`src/musyx/runtime/stream.c`](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/stream.c)
- [Pinned license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE)

### ULTRA_PRINTF

decompals/ultralib at `e24c836796df4bf520ff8b11a5c9d2cea3a66cbd`.

No project-wide license found in pinned recursive tree. sptask.h/mbi.h carry SGI proprietary notices. Structure-only; do not vendor.

structure comparisons, not matches

- [`src/libc/xprintf.c`](https://github.com/decompals/ultralib/blob/e24c836796df4bf520ff8b11a5c9d2cea3a66cbd/src/libc/xprintf.c)

### ULTRA_QUEUE

decompals/ultralib at `e24c836796df4bf520ff8b11a5c9d2cea3a66cbd`.

No project-wide license found in pinned recursive tree. sptask.h/mbi.h carry SGI proprietary notices. Structure-only; do not vendor.

structure comparisons, not matches

- [`src/io/piacs.c`](https://github.com/decompals/ultralib/blob/e24c836796df4bf520ff8b11a5c9d2cea3a66cbd/src/io/piacs.c)

### ULTRA_TASK

decompals/ultralib at `e24c836796df4bf520ff8b11a5c9d2cea3a66cbd`.

No project-wide license found in pinned recursive tree. sptask.h/mbi.h carry SGI proprietary notices. Structure-only; do not vendor.

structure comparisons, not matches

- [`src/io/sptask.c`](https://github.com/decompals/ultralib/blob/e24c836796df4bf520ff8b11a5c9d2cea3a66cbd/src/io/sptask.c)
- [`include/PR/sptask.h`](https://github.com/decompals/ultralib/blob/e24c836796df4bf520ff8b11a5c9d2cea3a66cbd/include/PR/sptask.h)
- [`include/PR/mbi.h`](https://github.com/decompals/ultralib/blob/e24c836796df4bf520ff8b11a5c9d2cea3a66cbd/include/PR/mbi.h)
