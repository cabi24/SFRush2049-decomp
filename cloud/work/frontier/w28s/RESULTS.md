# w28s - static boot tail (worktree lane): func_80011910 landed in an owned .bss window, static asm ceiling

Branch `w28s` (worktree `.claude/worktrees/w28s`, from wave28 `e3b1f926`; wave28 did not move during the lane, so
there was nothing to merge). Not pushed. Status: DONE.

**Static 631/667, 150,968 B (93.93%) -> 632/667, 151,176 B (94.06%): +1 function, +208 B** (`make progress`).
Game 1010/1216 (341,056 B, 52.71%), overlay A 81/193 (51,060 B), overlay B 21/49 (9,020 B): unchanged, reported
separately.

| target | bytes | state | scorer / gate lines |
|---|---:|---|---|
| func_80011910 (#377) | 208 | **PROMOTED** 4e32a60f | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (81 rows)`; `TU: true=0 strict=0 reloc_blind=0 objdump-dr identical (81 rows)` `RESULT PASS`; lock `(ido7.1): score=0`; gate `OWNED-OFFSETS: OK 20 ROM TU data sections (17 owned slots), 28 named objects checked against retail addresses (23 in owned slots), 1 without a retail address`; `PROMOTED 1 (208 B) @ 4e32a60fa` |
| func_80012D18 | 3,148 | skipped: no concrete lever | #376 found no source improvement; the w28g GameCube types have no counterpart (GC audio is `ngcAudio.c`/AX, with no loop/sample-record structs to borrow) |
| static asm ceiling | - | **documented** `docs/static-asm-ceiling.md` | 21 ultralib objects `RESULT: IDENTICAL` (27 functions, 4,816 B); floor 33 fn / 6,252 B; ceiling 634/667, 154,372 B (96.05%) |

## func_80011910: how the window was made real (owner decision 2026-10-13)

Source `sources/func_80011910.c`: #377's body with the descriptor typed as the SDK's `OSTask` (include/types.h,
`task->t.*`), plus `#include "types.h"` and five definitions in retail order:
`void *D_800382F0; unsigned int D_800382F4; OSTask D_800382F8; struct AudioNode *D_80038338; unsigned char *D_8003833C;`.
Each object has the type of its retail users, which are all C in lib_11640 already: func_80010DD8, func_80010E80
and func_800110C4 use D_800382F0; func_80010DD8 uses D_800382F4; func_80011F60 and func_800121BC use D_80038338
and D_8003833C. `struct AudioNode` is the tag of the TU's own `AudioNode` typedef. There are no pad arrays, no
padding, no stand-ins and no shaping devices. The standalone IDO 7.1 object's `.bss` is exactly 0x50:
D_800382F0 +0, D_800382F4 +4, D_800382F8 +8 (64 B), D_80038338 +0x48, D_8003833C +0x4C.

**No owned_data rule extension was needed.** A w21d `.bss` slot is the compiled object's whole `.bss`. lib_11640's
object had none, and with these definitions it is exactly the 16-byte-aligned window. The TU's other zero-initialised
words (D_80038290..D_800382EC, D_80038340..) remain extern, retail-addressed symbols, exactly as before (the
lib_16320 slot started as a partial retail window the same way, w20b). Steps, each gated:

1. `owned_data prepare data/owned_lib_11640_bss.json` (0x800382F0, 0x50, owner func_80011910, companion
   `func_80011910.bss.s` = `.space 0x50`) - commit 82017f6f. Builder: ROM matches!, make test ROM matches!,
   `OWNED-OFFSETS: OK 20 ROM TU data sections (17 owned slots)`.
2. Field relocation naming. The first `lock add` gave `score=65`: the target relocates against splat's per-field
   labels D_800382FC..D_8003832C, while IDO emits `D_800382F8+off`. `reloc_split derive` produced 13 entries
   (`rom:0x12534`..`0x1259C`, `symbol:D_800382F8 addend:0x4`..`0x34`). `reloc_split check`: `all 144
   reloc_addrs entries encode their ROM words`. `make extract` changed only func_80011910.s operands.
   `refresh_split_targets.py func_80011910`: `reloc_aware 6b0ee1613a9e -> reloc_aware 2e2fad13b43d` (Conveyor DB
   backup: session scratchpad `w28s/conveyor.db.w28s.bak1`). Commit 04f6209f.
3. `lock add ... --toolkit 4586d230...` -> `(ido7.1): score=0`. `context_check.py --compiler ido7.1` -> ok, owned
   `.bss` 80 B in slot func_80011910 (`ctx71_O2.jsonl`). Commit cf986d54.
4. **The .bss order trap.** The first promote_batch gate failed:
   `OWNED-OFFSETS: MISMATCH src/rom/lib_11640.c(.bss) D_800382F4: built 0x800382f0 (+0x0), retail 0x800382f4`
   (and D_800382F0 at +4), and the batch was refused. Measured (standalone IDO 7.1): when a definition follows its
   uses, the TU's `.bss` is laid out in **first-use order in code**, not in declaration order
   (func_80010DD8 stores D_800382F4 before D_800382F0). Reordering the extern declarations does not help (tried,
   then reverted). The fix is placement: the definitions go before every user, as in the original file. The
   companion moved to the `layout.generate_tu` position (directly after `#include "rom_tu.h"`; commit 4928c096,
   gated ROM matches! / OWNED-OFFSETS OK), and `promote_batch --bss-at-companion` put the definitions there.
5. `CONVEYOR_BUILDER_REPO='~/rush2049/scratch/frontier/w28s/repo' CONVEYOR_BUILDER_JOBS=2 python3
   cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w28s/ctx71_O2.jsonl 0x11640 --only
   func_80011910 --prefix-dedup rom/lib_11640 --bss-at-companion` -> `OWNED-OFFSETS: OK 20 ROM TU data sections
   (17 owned slots), 28 named objects checked against retail addresses (23 in owned slots)`, `PROMOTED 1 (208 B) @
   4e32a60fa`. The subject was amended to carry #377.

## Verifier fixes (the packets assumed lib_11640 had no data / #377 was unregistered)

- **#377 packet** (`dot_static_11910_20261013`): `--deep`'s tu_check required context_check to refuse the
  `.bss`. Now that the slot exists, the check replays the refusal against `baseline_rom_owned_data.json`
  (= rom_owned_data.json at e3b1f926, bound in inputs) through a slot-lookup shim. The receipt was re-recorded
  `--deep --write` on the builder scratch (stock 7.1). Only that input, the verify.py hash and `tu.registry`
  changed. verify.py, `--deep` and `python3 -O --deep` all exit 0. Commit 1ce60241.
- **audio_record_contracts** (actual-TU proof of lib_11640): refused any allocated data. It now accepts data only
  via `owned_slots.allocated_sections` (registered slot, NOBITS, fits), requires every object defined in the slot to
  sit at its retail address, and makes the GNU link place the slot section at the slot address. Two new toolchain
  tests in `tests/cloud/test_audio_record_contracts.py` check that a swapped D_800382F0/F4 order and a fifth word
  past the slot both fail closed. Commit 0f28ff9c. `remaining_contract_integration/verification.json` pins
  this verify.py's hash, but that pin was already stale before this lane (4b71e794 vs 87c17611 at e3b1f926), and
  no test reads it. It was left as is.
- BT03-voice-unblock-topology, dot_static_12d18, dot_static_10dd8_lifetimes, dot_static_12234: verify.py exit 0
  unchanged (rocky).

## Static assembly ceiling (`docs/static-asm-ceiling.md`, linked from docs/README.md)

`ceiling/prove.sh` (builder) builds ultralib e24c836 VERSION_J with ultralib's own IDO recipe (`makefiles/ido.mk`,
libultra_rom) and the project IDO 5.3. `ceiling/compare.sh` (Pi) runs w24t's `compare.py` on each object at its
address (`vram.txt`). All 21 objects print `RESULT: IDENTICAL`: setintmask, writebackdcacheall, writebackdcache,
invalicache, invaldcache, bzero, interrupt, getcount, **exceptasm (584 words, 76 relocations)**, bcopy, setsr,
getsr, setfpccsr, unmaptlball, maptlbrdb, getcause, sqrtf, bcmp, setcompare, probetlb, and **io/ai.c (C, -O2)**.
`ceiling/prove.log` holds the object SHA-256s and the output.

- The floor is 33 functions / 6,252 B: 27 libultra assembly functions (4,816 B), plus 6 hand-written (1,436 B:
  lzss_decode, inflate_io_wait, inflate_entry/alt from the w25a/w26a/w27s evidence, entrypoint, __setfpcsr).
- C-reachable: func_80012D18 (3,148 B) and **__osAiDeviceBusy (48 B, a new lead)**. That function is C in ultralib
  and byte-identical at `-O2`, but its segment 0x10760 is whole-asm, so landing it needs a segment conversion.
- The ceiling is 634/667 functions, 154,372/160,720 B (96.05%).
- Name leads, not applied: project __osException/__osExceptionPreamble are swapped relative to ultralib.
  __osSendInterrupt = send_mesg, __osExceptionPanic = __osCleanupThread, __osTlbFlush = osUnmapTLBAll,
  __osTlbInit = osMapTLBRdb, osCauseGet = __osGetCause.

## Tests

- Builder gate (`watchman2:~/rush2049/repo`, rsynced from the worktree per docs/BUILDING.md with `--exclude=.claude/
  --exclude=.git`, then `rsync -a --delete asm/`): `make COMPILER=ido -j16` MAKE=0 "ROM matches!", `make test`
  TEST=0 "ROM matches!", `sha1sum build/us/rush2049.us.z64` = `3f99351d7bb61656614bdb2aa1a90cfe55d1922c`,
  `OWNED-OFFSETS: OK 20 ROM TU data sections (17 owned slots), 28 named objects checked against retail addresses
  (23 in owned slots), 1 without a retail address`, lib_11640.o newer than its TU. `lock check`: all 635 intact.
- rocky (`~/rush2049/scratch/w28s`, clone of w28p's clone + bundle of w28s at 0f28ff9c, `tools/cloud/ido{,71}`
  copied, origin/master ref = f45e3940): `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16`: conveyor `2339 passed, 52
  skipped`; cloud `968 passed, 1 skipped, 8 warnings, 27 subtests passed`; rc=0.
- Pi `make test-fast JOBS=4` (worktree at 525067d6; `tools/mips_to_c` and `tools/decomp-permuter` were initialised
  in the worktree with `--reference` to the main checkout, so the m2c tests ran against it): conveyor `1589 passed,
  802 skipped`; cloud `771 passed, 198 skipped`; rc=0.

## What the gate repo / coordinator needs

- Merge `w28s` into wave28 (fast-forward; no CLAIMS/handoff edits made here: the #377 CLAIMS row should move from
  blocked to landed, and the wave-28 handoff should record the IDO 7.1 `.bss` first-use rule).
- Gate repo: plain rsync of the tree (no rebind dir needed: no symbol_addrs/rom_tu.h edit). The worktree needs
  `baserom.us.z64` for `owned_data prepare`/`linker` (owned_text reads it).
- Conveyor DB: func_80011910's target was refreshed on the Pi DB (reloc_aware 2e2fad13b43d). Other boot-tail targets
  are untouched.

## What generalises

- **IDO 7.1 `.bss` order is first reference when the definition follows its uses.** Definition order applies only
  when definitions precede every use. Put owned `.bss` definitions at the companion's generate_tu position with
  `--bss-at-companion` (w25a's flag, until now used only for 5.3). OWNED-OFFSETS catches the error; the ROM SHA-1
  does not.
- A struct object at 8 mod 16 can be owned through the 16-aligned window of real neighbouring words. as1's paired
  `lui` field stores need the object's definition visible (#377), and the split field labels become
  `reloc_addrs` entries `D_<base> addend:off`.

Builder scratch `~/rush2049/scratch/frontier/w28s` deleted at the end. Rocky clone `~/rush2049/scratch/w28s` and
log `~/rush2049/scratch/w28s-testfast.log` kept.
