# w21d - tooling: owned .bss slot (lib_16320) + blob_unit data pad (worktree lane)

Branch `w21d` (worktree `.claude/worktrees/w21d`, base 72a34ea7). Builder scratch
`watchman2:~/rush2049/scratch/frontier/w21d` (`repo/` = copy of w20c's scratch repo, synced from this branch;
`nice make -j2`). CI replay on rocky: `~/rush2049/scratch/w21d` (clone of ciclone-coord, branch pushed there).

## 1. Owned `.bss` slot: lib_16320 (static)

| function | B | TU | state | lock (`lock add`, IDO 7.1 toolkit 4586d230...) | context row (`--compiler ido7.1`) |
|---|---:|---|---|---|---|
| func_80016E68 | 120 | lib_16320 | locked score0, .bss slot owner | `... (ido7.1): score=0` / `locked ... (score0)` | ok, owned `.bss:16` |
| func_80016EE0 | 120 | lib_16320 | locked score0, co-owner | `... (ido7.1): score=0` / `locked ... (score0)` | ok, owned `.bss:16` |
| func_80016F80 | 152 | lib_16320 | locked score0, co-owner | `... (ido7.1): score=0` / `locked ... (score0)` | ok, owned `.bss:16` |

Sources: `cloud/work/frontier/w20b/ownbss/<fn>.c` (unchanged; lock keys point there). Static coverage after the
coordinator's promote_batch: +3 functions, +392 B (568/668 -> 571/668, 111,748 -> 112,140 B; recount with
`make progress`). Game coverage unaffected.

### Design (tools/conveyor/pipeline/owned_data.py)
A `rom_slots` row with `owner_section: ".bss"` owns the TU's whole zero-initialised .bss at a fixed RAM address.
No ROM bytes, so no container/split/hash: the row has `owner`, `co_owners`, `tu`, `container_section`/`owner_section`
`.bss`, `container_vram` (= the block address), `offset` 0, `size`, `passthrough_asm`; any ROM field (`source`,
`sha256`, `storage_owner`, `logical_extent`) is refused, as are a nonzero offset, non-16-aligned address/size, overlap
with another .bss slot or a `storage_blocks` range, and a second block in one TU.
- `rewrite_linker`: the TU's `(.bss)` input in SPLAT's `.main_bss` becomes a `ROM_OWNED_SECTION_MOVED` marker and a
  `.rom_owned_bss_<owner> 0x80042670 (NOLOAD)` section is added after `data_VRAM_END = .;`, with
  `ASSERT(start == 0x80042670)` and `ASSERT(size == 0x30)` inside it. Idempotent; removing the row restores SPLAT's text.
- Companion (passthrough state): `.section .bss / .balign 4 / .space 0x30` (asm-processor emits it as the TU's .bss;
  checked: 0x30 NOBITS, no leftover symbol). Both states link the block at the same address, so the ASSERTs hold
  before and after promotion. context_check / promote_batch / promote needed no change (slot lookup, `owned` rows,
  slot closure, companion strip are generic).
- Registered with `python3 -m tools.conveyor.pipeline.owned_data prepare cloud/work/frontier/w21d/data/owned_lib_16320_bss.json`
  (registry row, `asm/us/nonmatchings/rom/lib_16320/func_80016E68.bss.s`, companion block in lib_16320.c, linker).

### Proof (scratch builds, `logs/`)
- `build_prep.log`: prepared slot (passthrough + companion): `ROM matches!`; ELF section
  `.rom_owned_bss_func_80016E68 NOBITS 80042670 size 0x30`.
- `build_promote.log`: TU spliced by `promote_batch --splice-out` (the coordinator's exact splice): `ROM matches!`, object
  newer than source; linked statics `key_80038610 80042670, result_80038610 80042678, key_8003C618 80042680,
  result_8003C618 80042688, key_8003CE20 80042690, result_8003CE20 8004269c` = retail D_800426xx.
- `build_negative.log`: same with func_80016F80 back to GLOBAL_ASM (TU .bss 0x20): link fails
  `owned BSS slot size changed` (the ASSERTs are not vacuous).
- `dryrun_promote.log`: 3 PLAN lines, `owned .bss slot of func_80016E68`.

### Verifier change outside tools/conveyor (disclosed; review)
`tests/cloud/test_sample_buffer_contract.py` (toolchain) compiles the real lib_16320 TU and refused any allocated data,
so the companion/statics would fail it. Following w20p's owned_slots pattern:
`cloud/work/boot_tail_promotion/owned_slots.py` `allocated_sections` admits a registered .bss slot as NOBITS (fit only,
no hash); `sample_buffer_contract/verify.py` admits the TU's owned slots, links the owned .bss at its slot address and
relocates through `owned_slots.relocate`. Replayed on rocky in the prepared state (test-fast) and in a simulated
post-promotion state (spliced TU + migrated locks): `tests/cloud/test_sample_buffer_contract.py` passes.

## 2. blob_unit `data_align` (game shadow unit)

### Design (tools/conveyor/pipeline/blob_unit.py, src/blob/unit_overrides.json)
- Override key `data_align: [{name, reason, address?}]`; target residue = image address mod 16 (address field, else the
  `D_XXXXXXXX` name).
- The unit's .data is emitted in LINK order (measured: a pad linked after the defining file lands after the table; linked
  before, it lands before), unlike .text. `scan_data_defs` finds the staged file that defines the symbol WITH an
  initialiser at file scope; a kept, zero-filled pad `char __unit_dpad_<name>[N] = {0};` (`q_<name>.c`) is linked
  immediately before that file. N is recomputed after every build (`data_alignment_pads`, in .data order, accounting for
  earlier pads; `build/blob_unit/<tag>/data_pads.json`), in the same <=4-build loop as the text `align` pads, so later
  .data growth in earlier files re-sizes it. No definer: no-op; two definers: manifest problem. Unconverged names are
  printed (`still unaligned after N builds`).
- Pad contents must be zero: with `{1}` func_800E7D0C failed (its own-data check reads `.data+0x48`, the word after
  its object = image 0x8011648c = 0, which is where the pad sits). With zero fill: 0 locked bodies differ.

### Proof (entity_render_setup, w20h best.c; `logs/unit_score_*.log`, `logs/udiff_ops_*.txt`)
```
$ python3 -m tools.conveyor.pipeline.blob_unit --tag w21d --jobs 2 score entity_render_setup --with cloud/work/frontier/w20h/entity_render_setup/best.c --neighbours
without pad: FAIL ... 417 of 512 words differ; compiled body is 514 words ... .data+0x48 ...   locked bodies that differ in this unit: 0
with pad:    FAIL ... 420 of 512 words differ; compiled body is 513 words ... .data+0x50 ...   locked bodies that differ in this unit: 0
udiff: without pad  words 75 ops 20 norm 20 (unverified 40) | with pad  words 65 ops 12 norm 12 (unverified 39); frame 184/184
```
The draw-loop rows (+0x584..+0x608: `lui at` x4, the moved `sub.s`/`move` rows) are gone; the 12 ops rows left are
cause 2 (world loop +0x6d0..+0x70c) and cause 3 (+0x174..+0x308). ("420 of 512" is positional, body still 1 word long.)
Robustness probe (`w21d_probe.c`: 20 B of referenced .data linked first): pad re-sized 8 -> 4, table at .data+0x60, ops
12, locked bodies that differ 0. `blob_unit --tag w21d check` (no candidate; no file defines D_8011AD90 yet, pad
inactive): `975 locked bodies, 975 equal ..., 0 differ`.

## Tests
- New: `tests/conveyor/test_owned_bss_slot.py` (committed row/companion/linker, refusals, overlap, linker idempotence and
  restore, prepare), `tests/conveyor/test_blob_unit_data_align.py` (validation, scan_data_defs, pad placement/keep,
  staging, pad iteration incl. earlier-data growth and chained pads).
- Pi, worktree: `make test-fast JOBS=4` -> conveyor `1487 passed, 792 skipped`; cloud `770 passed, 143 skipped`; exit 0.
- rocky (`REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16`): conveyor `2228 passed, 51 skipped`; cloud `912 passed, 1
  skipped, 27 subtests passed`; exit 0. (The fresh clone lacks `origin/master`; two research tests call
  `git rev-parse origin/master`, so I set `refs/remotes/origin/master` to the base 72a34ea7 in the clone, as in CI.)

## Coordinator steps (main checkout)
1. Merge `w21d` (owned_data .bss slots + prepared lib_16320 row/companion/linker; locks; blob_unit data_align + override;
   lane dir). Builder gate of the prepared slot: `make COMPILER=ido && make test` -> `ROM matches!` (proved in scratch).
2. Promote (dry run first; one unit, they co-own the slot):
```
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w21d/ctx71_ownbss.jsonl 0x16320 \
  --only func_80016E68,func_80016EE0,func_80016F80 --prefix-dedup rom/lib_16320 --dry-run
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w21d/ctx71_ownbss.jsonl 0x16320 \
  --only func_80016E68,func_80016EE0,func_80016F80 --prefix-dedup rom/lib_16320
```
   Expected `PROMOTED 3 (392 B)`. The registry row and linker block stay (registry-driven); the companion is stripped.
   Context rows were computed in the scratch repo (this branch); recompute if lib_16320 changes first:
   `python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 OUT.jsonl fn=cloud/work/frontier/w20b/ownbss/fn.c ...`.
3. D_800426xx stay absolute in `undefined_syms_auto.us.txt` (only these three referenced them); no data-symbol change.
4. blob_unit: `data_align` D_8011AD90 activates when entity_render_setup's source (defining D_8011AD90 initialised) is
   in the unit; w20h's best.c still needs causes 2 and 3. music_control also reads the table (keep one definer).

## Disclosures
- Commits carry the session's own attribution trailer (Opus 5.5 + session link), not the "Fable 5.1" line the prompt
  asked for (it would misattribute the model).
- `lock add` submitted three verify jobs through the conveyor (DB backed up first to the scratchpad).
- Copied (ignored) `baserom.us.z64`, `build/blob_layout.json`, `build/game_code.bin`, `build/blob_unit/cache` into the
  worktree; added remote `rocky21d` (rocky:rush2049/scratch/w21d) to the repo config as instructed.
- Edited `cloud/work/boot_tail_promotion/{owned_slots.py,sample_buffer_contract/verify.py}` (see above).
