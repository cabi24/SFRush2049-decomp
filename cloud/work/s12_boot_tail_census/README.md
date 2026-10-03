# S12/P01: uncounted boot-segment code census (2026-10-03)

**Status:** READY-RESEARCH output. Proposes, does not apply, a static
denominator and naming change. Inventory: `functions.json` (439 functions).
Built on `cloud/work/r16_runtime_images/` §3.

## Extent

The boot image is ROM `0x1000–0x2F4E0` (`0x80000400–0x8002E8E0`). Static
coverage counts ROM `0x1000–0x10000` only (splat's `data` bin starts at
`0x10000`). Tiling with `targets.scan_extent` from `0x8000F3A4` gives **439
functions to `0x800277D0`, 99,120 bytes**. RSP microcode (`N64 RSP mixer
V1.1`, `F3DEX.NoN fifo 2.08`) and data follow. The data holds the libc
`vsprintf` tables and the build stamp `07-17-00 02:26:18 MERRILL BLACKOUT`.

## Defect: `__osPfsRWInode` is cut at the boundary

`asm/us/nonmatchings/rom/lib_f700/__osPfsRWInode.s` ends at ROM `0xFFFC`
(92 bytes). The function's true extent is **724 bytes, `0x8000F3A4–0x8000F678`**:
ROM `0x10000` (`0x57280006`, `bnel`) is mid-function. ultralib's
`__osPfsRWInode` matches the full 724 bytes. The current static target can
never match as a whole function. The fix is a splat/layout boundary change
(maintainer-owned).

## Identification against ultralib

Method: compile ultralib `e24c836` (`reference/repos/ultralib`) with IDO 5.3,
for versions K and L with its Makefile flags (`-O2`; `-O1` for os, debug and
host) and for I, J and K at uniform `-O1` and `-O2`. Then compare each tile
word by word, masking only the relocated fields (`R_MIPS_26` low 26 bits,
`HI16`/`LO16` immediates). Tiles under 16 bytes are not counted as
identifications.

- **21 functions (4,048 bytes) identified**, all between `0x8000F3A4` and
  `0x800103A0`, plus `__osPiRelAccess` again at `0x8002517C`. Versions K and L
  agree.
- **412 functions (95,012 bytes) do not match any ultralib build.** They run
  `0x8000F8D0–0x800268D0`. Profile: 97 under 64 B, 211 under 256 B, 96 under
  1 KB, 8 of 1 KB or more. None writes an unsaved callee-saved register (no
  `-O3` IPA conventions, unlike the runtime images). There are 770 internal
  calls and 54 calls into the counted static code. The game image calls 21 of
  them. This is most likely the developer's own runtime library (libc
  `vsprintf` and similar, the audio driver for its own mixer microcode,
  memory and thread services). It should be matchable with the ordinary
  single-function workflow once it has static targets.

### `symbol_addrs.us.txt` name conflicts

These are exact masked matches over the full extent. Hardware-register
constants are compared exactly because they are not relocations.

| Address | Current name | ultralib match |
|---|---|---|
| `0x8000FAC0` | `__osPfsDataChecksum` | `__osContDataCrc` |
| `0x8000FBA0` | `__osEnqueueAndYield` | `osDestroyThread` |
| `0x8000FCB0` | `__osTLBLookup` | `__osProbeTLB` |
| `0x8000FD70` | `__osPiDeviceBusy` | `__osSiDeviceBusy` |
| `0x8000FDA0` | `osEPiRawWriteIo` | `__osResetGlobalIntMask` |
| `0x8000FE00` | `osEPiRawStartDma` | `__osEPiRawWriteIo` |
| `0x8000FF60` | `osEPiRawReadIo` | `__osEPiRawReadIo` |
| `0x800100C0` | `__osPiGetCmdQueue` | `__osSetGlobalIntMask` |

## Proposed change (not applied)

1. Extend the static code population to `0x800277D0` through splat and
   `layout`. That is about 99 KB more static code: the denominator grows from
   61,440 to roughly 160 KB of static bytes. Report it separately until the
   new targets exist.
2. Fix the `__osPfsRWInode` extent.
3. Correct the eight names above through the context tooling.
4. Then the 21 identified functions are corpus candidates (`pipeline.corpus`).
   The other 412 enter the normal static matching queue.

Reproduce: `scripts/ulbuild.sh K|L`, `scripts/ulbuild2.sh <I|J|K> <-O1|-O2>` and
`scripts/ulasm.sh` on the builder (IDO 5.3 toolkit `796ae99a…`), then
`scripts/ulmatch.py` locally with the fetched objects under `$CLAUDE_JOB_DIR/tmp/ul/`.

## Scoring targets (added)

`python3 -m tools.conveyor.pipeline.ovl_targets generate --image boot_tail`
writes `asm/us/boot_tail/`: 439 functions from `0x8000F3A4` to
`0x800277D0`, the same partition as this census, with zero unproven merges.
The boot segment is mapped at `0x80000400`. Symbols are the game symbols,
then `symbol_addrs.us.txt`, then `func_XXXXXXXX` for each function. A tile
with no direct start evidence becomes its own function (`standalone`) unless
it branches or jumps back into the previous function or is a switch
destination. All 32 such tiles here are self-contained leaves, mostly with a
global load scheduled before the stack adjustment. Score with
`tools/cloud/score.py fn SRC func_XXXXXXXX --targets asm/us/boot_tail`.
CI rescores `cloud/matches/boot_tail/*.c`. Pipeline proof:
`cloud/matches/boot_tail/func_80010A00.c` (returns `D_8002C630`) is a
true match.
