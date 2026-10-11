# w20c - static boot tail: the code-identical, not-lockable set (tooling/data lane, worktree)

Branch `w20c` (worktree `.claude/worktrees/w20c`, from master 81d91115). Not pushed. Static numbers only
(`make progress`), never merged with game coverage. Builder work only in `~/rush2049/scratch/frontier/w20c`
(`repo/` = this branch, copied from w19c's scratch repo; `nice make -j2`), plus one promote_batch gate
pointed there with `CONVEYOR_BUILDER_REPO` (see Disclosures).

## Result

All seven functions (96+404+904+960+904+840+780 = 4,888 B) now lock at score 0 against their reloc-aware
targets.
Five are promoted on the branch (3,024 B). The other two (func_8001B9F8 and func_8001BE14, 1,864 B) are
proven but need a TU edit to lib_1a660 that only the coordinator may make.

Static: master base 550/668, 103,172 B -> branch **555/668, 106,196 B (66.08%)** (`make progress` in the
worktree). After the lib_1a660 step below: 557/668, 108,060 B.

| function | B | cause | fix | LOCK (check.py = lock add verify_entry; prove.log) | whole TU | promoted |
|---|---:|---|---|---|---|---|
| func_80021548 | 96 | own jtbl_8002D930 in the global .rodata | owned .rodata slot 0x8002D930/0x20 (`owned_data prepare`) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(2 rows) (26 rows)` | lib_21210: ROM matches; TU differs only in the table's in-place words (below) | yes |
| func_80023BDC | 404 | own jtbl_8002D950 | owned .rodata slot 0x8002D950/0x14, 4-byte logical extent (func_80023E9C's table follows) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(2 rows) (105 rows)` | lib_22300: ROM matches; same | yes |
| func_8001BE14 | 904 | D_8004F800 = &D_8004F300[32] x3 + own jtbl_8002D8E8 | reloc_addrs (D_8004F300+0x500, 6 lines) + shared slot 0x8002D8D0/0x30 (co-owner) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(2 rows) (242 rows)` | lib_1a660 (with TU patch): ROM matches | **no**: TU patch first |
| func_8001B9F8 | 960 | same + own jtbl_8002D8D0 | reloc_addrs (6) + slot owner | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr DIFFERS(2 rows) (256 rows)` | same | **no**: TU patch first |
| func_800114C0 | 904 | float->u32 `lui at,0x8000` + unrelated `addiu ..,63` paired as D_8000003F | reloc_addrs `CUSTOM_CONSTANT_HI/LO` (2) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (258 rows)` | lib_11640: IDENTICAL (4276 lines) | yes |
| func_80011104 | 840 | same artifact twice (as g_tlb_exception_vector+1) | reloc_addrs CUSTOM_CONSTANT (4) + D_800382D8+4 (2, TU's two-cell table) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (253 rows)` | lib_11640: IDENTICAL | yes |
| func_80013DEC | 780 | first D_80038292 read: unpaired `lui 0x8004; lhu -0x7D6E` (no relocation) | reloc_addrs MIPS_HI16/LO16 D_80038292 (2) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (255 rows)` | lib_11640: IDENTICAL | yes |

The jump-table functions' 2 DIFFERS rows are the relocation symbol only (`jtbl_rows.log`): the target names the
split label, the C object names its own section, e.g. `18: R_MIPS_HI16 jtbl_8002D930_main` vs
`18: R_MIPS_HI16 .rodata` (same words, types, offsets). That is what owning the table means. The owned slot puts
`.rodata` at exactly that address, and the slot ASSERTs in the linker script check it. No C can produce the
label name. The same holds for w19c's locked fixuns/osAiSetNextBuffer. Whole-TU (`tusplice.log`, orig =
committed TU, spl = promote_batch splice, both IDO 7.1 + `owned_data object`): lib_11640 IDENTICAL; lib_21210,
lib_22300 and lib_1a660 differ only in REL in-place addends of the table words and of the second table's `lw`
(`8c2e0000` vs `8c2e0018`). The companion addresses the table by label or absolute word, while compiled C
addresses it section-relative. The full-ROM builds are the proof: `logs/build_promote_all7_tupatch.log` (all
seven spliced), `logs/build_promote_1a660.log` (the pair on top of the five), and promote_batch's own gate:
`ROM matches!`, SHA-1 3f99351d7bb61656614bdb2aa1a90cfe55d1922c.

## What landed on the branch (commits)

| commit | what | gate |
|---|---|---|
| f67a0ac6 | `reloc_split`: `derive` also proposes unpaired sites (target raw words, candidate relocation -> name it), constant sites (`CUSTOM_CONSTANT_HI/LO`, spimdisasm's own `(v >> 16)` / `(v & 0xFFFF)`), and lists own-section (`.rodata`) sites as comments; `check` validates CUSTOM_CONSTANT entries. 20 reloc_addrs lines; `make extract` regenerated 5 slots | ROM matches (logs/build_reloc.log); tests/conveyor/test_reloc_split.py (+5 tests) |
| f113f0f1 | three owned slots via `owned_data prepare`; `companion_source` spells a logical-extent companion `.word OWNER + off` (validated as a word-relocated table on every build, like fcvt.table.s; the .incbin form fails `_logical_table`) | ROM matches (logs/build_prep.log); tests/conveyor/test_owned_slots_w20c.py |
| ab240bd9 | func_80011104 against lib_11640's two-cell D_800382D8 table: 2 more reloc_addrs lines; `owned_data.section_fits` | ROM matches (logs/build_reloc2.log) |
| f1ab5a9c | 7 locks, `lock add --toolkit 4586d230...` | score=0 each |
| 75cd8ea3, + logs | lane dir | |
| 4955bb5a | promote_batch: `PROMOTED 5 (3024 B)` (lib_11640 x3, lib_21210, lib_22300) | promote_batch's builder gate (ROM matches) |

reloc_addrs check: `all 100 reloc_addrs entries encode their ROM words`.

## Source changes vs w19a/w19b (type views only, re-locked; headers say so)
- Callees by symbol_addrs name: func_80006A00 -> osCreateMesgQueue, func_80008590 -> bzero, func_80007CA0 ->
  osWritebackDCache (the D10 placeholders made the R_MIPS_26 names differ).
- Declarations aligned with the TUs so promote_batch's `--prefix-dedup` splice compiles. 11104 uses
  boot_tail_audio_record.h's AudioState (bit_index = unknown60, changed = release_pending), the TU's
  `void *D_80038298` through a cast, and `D_800382D8[0]/[1]` (the TU's two-cell table, as func_8001144C). 114C0
  uses the TU's `unsigned char D_800382B0[]` with a cast. 13DEC declares `volatile short D_80038362` as the TU
  does (same code). 23BDC uses the TU's VoiceVariables_80023AD4 view and `MacroCommand { u32 word[2]; }`.
  1B9F8/1BE14 declare D_8004F300 as the TU's ChannelState (verbatim lines) and read it through one shared
  MasterFader view.
- No unused locals, no pad arrays, no implicit int, no compiled-out blocks.

## What the coordinator must do
1. Merge branch `w20c` (commits above, all ROM-gated; 4955bb5a carries promote_batch's own trailer).
2. **DB changes already made (live `~/.conveyor/conveyor.db`)**. A backup was taken first (sqlite backup API):
   `scratchpad/conveyor.db.w20c.bak`. Then `cloud/work/boot_tail_promotion/refresh_split_targets.py` re-registered
   five targets from the regenerated asm. Log: `func_80013DEC c332c63c33a1 -> 79b60fbd03b5`, `func_800114C0
   e3d4a192eb40 -> c8566012efbe`, `func_80011104 55f39ed1f42f -> 8482fba9582a -> 0bacf9f6a467`, `func_8001B9F8
   d90066129f6a -> f6aebf05370a`, `func_8001BE14 682d6ff4f584 -> 28d6e55df117` (`superseded: 5 targets, 0
   evidence rows purged`). promote_batch wrote its promotion_record rows. No `targets_rename apply` was run.
   Master's asm for these five still has the old operands until the merge. Nothing locked on master references
   them.
3. Apply `patches/context_check_logical_extent.diff`. It is one line in
   cloud/work/boot_tail_promotion/context_check.py, which this lane may not edit, and calls the new
   `owned_data.section_fits`. Without it context_check refuses func_80023BDC (`.rodata:32 exceeds the owned slot
   (20)`): IDO pads the section to 16 and the build trims it. `ctx71_w20c.jsonl` was produced with the patch.
4. lib_1a660 pair: apply `tu_patch/lib_1a660.c.diff` as a layout-neutral prep commit. It moves the ChannelState
   typedef and `extern ChannelState D_8004F300[32];` above the func_8001B9F8 slot; alone it gives ROM matches
   (`logs/build_tupatch.log`). Then:
```
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w20c/ctx71_w20c.jsonl 0x1a660 \
  --only func_8001B9F8,func_8001BE14 --prefix-dedup rom/lib_1a660 --dry-run   # dryrun_promote_1a660.log: 2 PLAN
# then without --dry-run (one unit: they co-own the slot)
```
   Proven in scratch on top of 4955bb5a: `ROM matches!` (logs/build_promote_1a660.log).

## Tests
- `make test-fast JOBS=4` (Pi, worktree, submodules initialised with `--reference` to the main checkout):
  `1454 passed, 791 skipped` (tests/conveyor), `770 passed, 140 skipped` (tests/cloud), rc=0 (`logs/test_fast.log`).
- ci_equiv on rocky: see the final section.

## What generalises
- Splat artifacts are fixed by naming in reloc_addrs, never by C. Unpaired raw immediates get a symbol
  (MIPS_HI16/LO16). Constants that were wrongly paired get `CUSTOM_CONSTANT_HI/LO` with a numeric symbol.
  `reloc_split derive` now proposes all three kinds from a candidate object and refuses anything that is not
  byte-equal.
- An owned table that only partly fills its TU's retail .rodata takes a logical 4-byte extent. Its passthrough
  companion must be word-relocated (`.word OWNER + off`).
- Lock first, then splice into the real TU with the TU's own declarations. Standalone matches from D10/w19a used
  private typedefs that collide in the TU (5 of 7 here). context_check (rom_tu.h only) cannot see this; a
  `promote_batch --dry-run --splice-out` + builder build does.

## Disclosures
- promote_batch's gate runs `make COMPILER=ido -j16` (hard-coded). I pointed it at my scratch repo with
  `CONVEYOR_BUILDER_REPO=~/rush2049/scratch/frontier/w20c/repo`. That build was incremental: three TUs, so at
  most three concurrent compiles. Every other builder build was `nice make -j2`.
- One candidate object was compiled to builder `/tmp/w20c_11104.o` and deleted right after. Nothing else was
  written outside `~/rush2049/scratch/frontier/w20c`; `~/rush2049/repo` was not touched.
- The worktree holds ignored copies/links: `baserom.us.z64`, `build/game_code.bin` and `build/blob_layout.json`
  (symlinks to the main checkout), initialised submodules, and test-modified files inside tools/mips_to_c (the
  same local modifications the main checkout's copy has; not committed).
- Proof scripts: `prove.sh` -> `prove.log` (its `TU:` rows read COMPILE FAIL for 1BE14/23BDC/11104 when the
  check.py preamble is appended without prefix dedup, the known w19a artifact); `tools/tusplice.sh` ->
  `tusplice.log`; `tools/obj.sh`, `tools/check.py` (= w19a's).
- No permission denials.
