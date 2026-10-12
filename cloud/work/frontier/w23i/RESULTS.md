# w23i - landing w23c's eight remaining code-identical boot-tail functions (tooling lane, worktree)

Branch `w23i` (worktree `.claude/worktrees/w23i`, from wave23 068ea2a1, the coordinator's rewrite of 89743e64, same
tree). Not pushed to origin. Static numbers only (`make progress`), never merged with game coverage. Builder work
only in `watchman2:~/rush2049/scratch/frontier/w23i/repo` (copy of w22i's scratch repo, synced from this branch;
`nice make -j2`; promote_batch gates with `CONVEYOR_BUILDER_REPO=<that repo> CONVEYOR_BUILDER_JOBS=2`). CI replay on
rocky: `~/rush2049/scratch/w23i` (clone of ciclone-coord, branch pushed there).

## Result

All eight promoted on the branch: static **600/668, 130,500 B (81.20%)**, +8 functions / +3,128 B over the base
(592/668, 127,372 B). Every promotion gate: promote_batch's full matching ROM build + `make test` on the builder
(ROM SHA-1 3f99351d...). Sources unchanged from w23c (their headers carry the disclosures).

| function | B | TU | mechanism | LOCK (`lock add`, verify job) | context row | promoted |
|---|---:|---|---|---|---|---|
| func_8001E0E0 | 864 | lib_1ece0 | owned `.rodata` constants slot 0x8002D910/0x14 (`owned_data prepare`, 4617f45d, own ROM gate) | `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul (ido7.1): score=0` | `w23i/ctx71_B.jsonl` ok, ido7.1, owned `.rodata` 32 (recomputed on the prepared registry) | 47d6e7b7 |
| func_80015A0C | 512 | lib_16320 | reloc names (4 entries) + TU hoist | `-g0 -O2 -mips2 -G 0 -non_shared (ido7.1): score=0` | `w23c/ctx71_relocname.jsonl` ok | 44f349c5 |
| func_80015D68 | 448 | lib_16320 | reloc names (4 entries) + TU hoist | same, score=0 | same | 44f349c5 |
| osPfsIsPlug | 396 | lib_10d60 (5.3) | new inventory target | `-g0 -O2 -mips2 -G 0 -non_shared: score=0` (pinned 5.3 toolkit) | `w23c/ctx53_O2.jsonl` ok, ido5.3 | 294dcfd6 |
| __osPfsRequestData | 180 | lib_10d60 (5.3) | new inventory target | same, score=0 | same | 294dcfd6 |
| func_80016C20 | 192 | lib_16320 | grown `.bss` block (replace-slot) | `-g0 -O2 -mips2 -G 0 -non_shared (ido7.1): score=0` | `w23c/ctx71_ownbss.jsonl` ok, owned `.bss` 32 | 83ee309e |
| func_80016CF0 | 336 | lib_16320 | grown `.bss` block | same, score=0 | owned `.bss` 48 | 83ee309e |
| func_80017040 | 200 | lib_16320 | grown `.bss` block | same, score=0 | owned `.bss` 16 | 83ee309e |

lib_16320 now has no GLOBAL_ASM slot (all C); its `.bss` links as `.rom_owned_bss_func_80016E68 0x80042630 0x80`.
lib_10d60 is all C.

## Steps (commits, all on w23i)

- **B**: 4617f45d `owned_data prepare w23c/data/owned_func_8001E0E0.json` (row, `.incbin` companion, TU block,
  linker; builder `ROM matches!`, `logs/build_prep_1e0e0.log`); 96d0032f lock + context row; 01226d4e promote_batch
  `CONVEYOR_BUILDER_JOBS` (gate `-j`, default 16, so a lane gate keeps to 2 cores); 47d6e7b7 `promote_batch
  w23i/ctx71_B.jsonl 0x1ece0 --only func_8001E0E0 --prefix-dedup rom/lib_1ece0` -> `PROMOTED 1 (864 B)`.
- **C**: 67d93fa2 appended w23c's 8 entries to reloc_addrs.us.txt (`reloc_split check`: `all 116 reloc_addrs entries
  encode their ROM words`), `make extract` (only the two slots' operands change: `%hi(D_8003CE20 + 0x4)` etc.),
  `refresh_split_targets.py func_80015A0C func_80015D68` (`reloc_aware a2bfcb3a12c7 -> c52fd45581ea`,
  `6a36a89791a0 -> eb8d76aed28d`), `w23c/tools/tu_edits.py src/rom`; builder `ROM matches!`
  (`logs/build_reloc_tuedit_C.log`). ae3a0030 locks; 44f349c5 `promote_batch w23c/ctx71_relocname.jsonl 0x16320
  --only func_80015A0C,func_80015D68 --prefix-dedup rom/lib_16320` -> `PROMOTED 2 (960 B)` (splice adds only the
  positional `#pragma pack` lines; the hoisted declarations are deduplicated).
- **E**: `targets_rename apply osPfsIsPlug __osPfsRequestData` (`register ... @0x80010160 99 insns seg 0x10d60
  reloc_aware o=cc8fe9ec6b08 (new row)`, `@0x800102EC 45 insns o=001ce7aec817`; `n64_target 2279 -> 2281, 0
  matrix_entry rows purged`; `audit` empty). df8cc3bf locks (no --toolkit: segment is ido5.3, flagset `-g0 -O2 -mips2
  -G 0 -non_shared`). 294dcfd6 `promote_batch w23c/ctx53_O2.jsonl 0x10d60 --only osPfsIsPlug,__osPfsRequestData
  --prefix-dedup rom/lib_10d60` -> `PROMOTED 2 (576 B)`. `--prefix-dedup` is required here: without it the
  whole-file rule drops `extern u8 __osPfsBuffer[64]` / `__osPfsRequestType2` from osPfsIsPlug's preamble because
  func_800103A0's block declares them LATER in the TU (checked with `--splice-out`).
- **D**: f1081815 locks; 620188c2 tooling (below); 83ee309e `promote_batch w23c/ctx71_ownbss.jsonl 0x16320 --only
  func_80016C20,func_80016CF0,func_80017040 --prefix-dedup rom/lib_16320 --replace-slot
  w23i/data/owned_func_80016E68_bss_grown.json` -> `PROMOTED 3 (728 B)` (commit also carries the replaced row,
  linker script and `func_80016E68.bss.s` = `.space 0x80`). Dry run: `logs/dryrun_D.log` (3 PLAN lines, restored).

## Tooling: `.bss` block growth at the front (tools/conveyor/pipeline/owned_data.py)

`replace_slot` refused every `.bss` row. Now a `.bss` replacement must keep TU, section, offset 0 and companion path,
be strictly larger, and cover the old range (`new_vram <= old_vram` and `old_end <= new_end`), so the address may
move down; a row may not change section. "Unclaimed" = `rom_slots` validation of the new registry (no overlap with
another owned `.bss` block or storage block; 16-byte alignment). The rest is w22i's transaction unchanged (old
members all C, new members all passthrough, restore on any failure / dry run). The linker ASSERTs pin 0x80042630 /
0x80 and the promoted set's full-ROM SHA-1 proves every member's statics (the three old members' included) at their
retail addresses. promote_batch needed no change for D beyond `--replace-slot` (w22i).

## Tests

- New `tests/conveyor/test_owned_bss_grow_w23i.py` (16): committed lib_16320 block grown and all-C; front growth
  writes row/companion/linker and restores; end-only growth; 8 refusals writing nothing (misses old end, starts
  inside, not larger, other TU, other companion, misaligned, non-passthrough member, section change); overlap with
  another TU's block; unpromoted old block; promote_batch plans the two pending members as one unit and splices in
  order; refuses them without the replacement and with a member left out. `test_owned_bss_slot.py` follows the grown
  committed row (six members, 0x80042630/0x80); w22i's `.bss` refusal case now refuses as "not fully promoted".
- Pi worktree `make test-fast JOBS=4` at 6130ac34: conveyor `1541 passed, 796 skipped`; cloud `770 passed, 150
  skipped`; rc=0.
- rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` (clone `~/rush2049/scratch/w23i`, origin/master = 0b5f530e) at
  6130ac34: conveyor `2286 passed, 51 skipped`; cloud `919 passed, 1 skipped, 27 subtests passed`; rc=0. The packet
  verifiers (sample_buffer_contract, macro_wrapper/stream_contracts, audio_record_contracts) pass unchanged; no
  receipt rebound.

## DB commands (live `~/.conveyor/conveyor.db`; sqlite backup API first -> `scratchpad/conveyor.db.w23i.bak`)

1. `lock add` x8 (verify jobs; all score=0).
2. `python3 cloud/work/boot_tail_promotion/refresh_split_targets.py func_80015A0C func_80015D68` (2 targets
   superseded, 0 evidence rows purged).
3. `python3 -m tools.conveyor.pipeline.targets_rename apply osPfsIsPlug __osPfsRequestData` (dry run first; 2 new rows).
4. promote_batch x4 wrote promotion_record rows (8).

## What the coordinator must do

1. Merge `w23i` (14 commits; the four `Promote ...` commits carry promote_batch's own trailer).
2. The DB is already updated (above). After merging, the main checkout's asm must be the re-split one (the merge
   carries the two changed `.s` files; `make extract` reproduces them from reloc_addrs.us.txt).
3. No data-symbol change: D_8003CE24/2C, D_80038614/18, D_80042630..D_800426A0 are no longer referenced by a
   passthrough body; harmless absolute symbols.

## Disclosures

- Edited outside tools/conveyor: `cloud/work/boot_tail_promotion/promote_batch.py` (`CONVEYOR_BUILDER_JOBS` only).
- Commits carry this session's attribution trailer (Opus 5.5 + session link) rather than the "Fable 5.1" line the
  prompt asked for, which would misattribute the model (as w21d/w21i/w22i).
- Worktree holds ignored copies: `baserom.us.z64`, `assets/us/data.bin`, symlinks `build/blob_layout.json`,
  `build/game_code.bin`; initialised submodules. `~/rush2049/repo` was not touched. No permission denials.
