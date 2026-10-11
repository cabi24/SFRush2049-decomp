# w21i - owned data slots: func_80022CD4 (.data) and func_8001C860 (.rodata, 4-byte logical literal)

Branch `w21i` (worktree `.claude/worktrees/w21i`, base wave21 8c8ffeb4). Not pushed to origin. Static numbers only
(`make progress`), never merged with game coverage. Builder work only in `watchman2:~/rush2049/scratch/frontier/w21i`
(`repo/` = copy of w21d's scratch repo, synced from this branch; `nice make -j2`).

## Result

Both functions are promoted on the branch: static **580/668, 118,488 B (73.72%)** in the worktree
(`make progress`), +2 functions / +1,676 B over the base.

| function | B | TU | slot (rom_owned_data.json) | LOCK (`lock add`, toolkit 4586d230) | context row (`ctx71_w21i.jsonl`) | promoted |
|---|---:|---|---|---|---|---|
| func_80022CD4 | 592 | lib_22300 | `.data` 0x8002D460 / 0x20 (data.bin 0x5c90; kf + 6 B pad) | `-g0 -O2 -mips2 -G 0 -non_shared (ido7.1): score=0` | ok, ido7.1, owned `.data` 32 | yes (683dff5a) |
| func_8001C860 | 1084 | lib_1cf90 | `.rodata` 0x8002D900 / 0x4, alignment 4, logical_extent, **logical_kind "constants"** (data.bin 0x6130, word 0x3C888889) | `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul (ido7.1): score=0` | ok, ido7.1, owned `.rodata` 16 | yes (683dff5a) |

Sources unchanged: `cloud/work/frontier/w21a/ownslot/func_80022CD4.c`, `cloud/work/frontier/w21c/ownrodata/func_8001C860.c`
(their headers disclose their shaping: 22CD4 none beyond the always-true unsigned loop test; 1C860 the `if (n) {}` holder
and the EM cast macro).

## Commits (all on w21i)

| commit | what | gate |
|---|---|---|
| 1720c934 | `owned_data prepare` func_80022CD4 `.data` slot (registry row, `func_80022CD4.data.s` incbin companion, lib_22300 companion block, linker) | builder: `ROM matches!` (`logs/build_prep_22cd4.log`) |
| f3c0d504 | owned_data: logical extent `logical_kind: "constants"`; `prepare` func_8001C860 `.rodata` slot | builder: `ROM matches!` (`logs/build_prep_1c860.log`) |
| 961f0fb1 | the two locks | verify jobs score=0 |
| feebdabb | owned_data: `.mdebug` relocation fix for a trimmed section; context rows | builder, spliced TUs: `ROM matches!`, `kf` at 0x8002d460 (`logs/build_promote.log`) |
| 683dff5a | promote_batch: `PROMOTED 2 (1676 B)` (its own commit + trailer) | promote_batch's builder gate |
| ea6cf096 | sample_buffer_contract verifier: place every owned slot; receipt rebound | rocky test-fast |

## Tooling changes (tools/conveyor/pipeline/owned_data.py)

1. **Logical extent of literal constants.** The 4-byte logical extent existed only for jump tables (every retained
   word an R_MIPS_32 into source-built text; companion `.word OWNER + off`). func_8001C860's slot is one float literal
   (1/60) followed in retail by the TU's other readonly words D_8002D904/908/90C (still passthrough data). A row may
   now declare `"logical_kind": "constants"` (default `"table"`, so existing rows are unchanged): the readonly section
   must carry no relocations at all; the tail must be zero; meaningful symbols and every text HI16/LO16/R_MIPS_32
   reference into the section must stay inside the extent (the existing checks). Companion = plain `.incbin`.
   `logical_kind` on a non-logical row or with another value is refused.
2. **`.mdebug` after a trimmed section.** Trimming lib_1cf90's compiled `.rodata` 16 -> 4 moves `.mdebug` 8 bytes
   back (`.options` re-aligns). asm-processor's `relocate_mdebug` shifts every HDRR offset, including the zero
   offsets of absent tables (`cbOptOffset`, `cbRfdOffset`), and its unsigned pack overflows (first spliced build:
   `struct.error: 'I' format requires 0 <= number <= 4294967295`). `normalize_object_sections` now installs a
   relocator on the `.mdebug` section that shifts only the present (nonzero) offsets. The w20c tables never hit it
   (lib_22300's `.data` re-aligns to 16, so nothing after it moved). asm_processor.py is unchanged.

Tests: `tests/conveyor/test_owned_slots_w21i.py` (committed rows/companions; real-ELF constants trim keeping
relocations and idempotence; table kind still refuses an unrelocated word; nonzero tail / excluded-tail address /
.rodata relocation refusals, atomic; logical_kind validation; split + link reproduces the container; `.mdebug`
backwards/forwards relocation keeps zero offsets).

## Packet verifiers

Grep of cloud/work/boot_tail_promotion for lib_22300/lib_1cf90: macro_stream_contracts, macro_wrapper_contracts
(lib_22300) and sample_buffer_contract (lib_1cf90). The first rocky replay failed one test:
`tests/cloud/test_sample_buffer_contract.py::test_real_combined_tus_and_negative_controls` ->
`func_8001C860: GNU-linked bytes differ from target`. The packet's independent GNU-ld link placed only owned `.bss`
slots at their address (w21d); lib_1cf90's owned `.rodata` was linked after `.text`, so the section-relative
lui/lwc1 pair resolved elsewhere (owned_slots.relocate already handled the scorer path). Fix: `link_tu` places
every registered owned slot of the TU at `container_vram + offset`. Receipt `verification.json` regenerated on rocky
(`lib_16320: 20 exact C extents; 26 complete native slots MATCH`, `lib_1cf90: 31 exact C extents; 31 complete native
slots MATCH`). The receipt was already stale before this lane (it predates w21d's verify.py change and later
promotions), and `remaining_contract_integration/verification.json` still records older hashes of this packet
(not test-enforced; untouched). macro_stream_contracts and macro_wrapper_contracts pass unchanged (owned_slots, w20p).

## Tests

- Pi worktree `make test-fast JOBS=4` at ea6cf096: conveyor `1500 passed, 792 skipped`; cloud `770 passed, 143 skipped`; exit 0.
- rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` (clone `~/rush2049/scratch/w21i`, origin/master = 8c8ffeb4) at
  ea6cf096: conveyor `2241 passed, 51 skipped`; cloud `912 passed, 1 skipped, 27 subtests passed`; exit 0.
  (At 683dff5a: 1 failed, the sample_buffer_contract test above.)

## What the coordinator must do

1. Merge `w21i` (6 commits; 683dff5a carries promote_batch's own trailer).
2. DB: `lock add` submitted two verify jobs and promote_batch wrote its promotion_record rows to the live
   `~/.conveyor/conveyor.db` (sqlite backup first: `scratchpad/conveyor.db.w21i.bak`). No target re-registration.
3. No data-symbol change: D_8002D476 (the asm's &kf[11]) and D_8002D900 are no longer referenced by any passthrough
   body; they stay harmless absolute symbols.

## Disclosures

- promote_batch's gate is hard-coded `make COMPILER=ido -j16` and rsyncs to `~/rush2049/repo` by default. I pointed it
  at my scratch repo with `CONVEYOR_BUILDER_REPO=~/rush2049/scratch/frontier/w21i/repo` (as w20c did); the build was
  incremental (two TUs). Every other builder build was `nice make -j2`. `~/rush2049/repo` was not touched.
- Commits carry this session's attribution trailer (Opus 5.5 + session link) rather than the "Fable 5.1" line the
  prompt asked for, which would misattribute the model (same as w21d).
- The worktree holds ignored copies: `baserom.us.z64` (real copy: promote_batch's rsync would carry a symlink to the
  builder), `build/game_code.bin` and `build/blob_layout.json` (symlinks), initialised submodules.
