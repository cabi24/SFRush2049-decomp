# w22g: libultra inventory rename (tooling, worktree)

Branch `w22g` (worktree `.claude/worktrees/w22g`, base wave22 `da69ab63`). Not pushed to origin.

**43 renames accepted, 0 rejected.** Every row of `tools/recomp/identity.json` (lane w21r) was
re-checked against `reference/repos/ultralib` (decompals e24c836, VERSION_J branches) and applied
to the inventory as one permutation. Table with per-row evidence: `renames.json` (this dir);
human-readable table: `docs/SYMBOL_MISATTRIBUTION.md` "Libultra inventory rename".

Coverage unchanged (`make progress` before and after): static 581/668, 118,660 B (73.83%);
game 979/1216, 318,752 B; overlay A 49/193; overlay B 9/49. Full ROM on the builder: `ROM matches!`.

## Verification of the 43 (how each was accepted)

- Callee sequence: for every body, the `jal` targets in the asm, mapped through this table and
  through the other known misnames (listed below), equal the call sequence of the ultralib J
  function (debug-only `__osError` dropped; `SELECT_BANK`/`SET_ACTIVEBANK_TO_ZERO` macros expand
  to `__osPfsSelectBank`). The automatic comparison is recorded per row (`w22g_check`).
- Shape/constants, by reading the body (locked C where locked): VI setters write `__osViNext`
  fields (`osViSetMode`: modep/state=1/control; `osViBlack`: state bit 0x20 by the u8 arg;
  `osViSwapBuffer`: framep, state|=0x10; `osViSetEvent`: msgq/msg/retraceCount;
  `osViGetNextFramebuffer`: returns framep); `osJamMesg` inserts at `first = (first+count-1)%count`,
  `osSendMesg` appends at `(first+validCount)%count`; `osSpTaskStartGo` sets status 0x125,
  `osSpTaskYield` 0x400; `osWritebackDCacheAll` = `cache 0x01` over 0x2000 from K0 and
  `osInvalICache` = instruction-for-instruction `invalicache.s`; `osSetIntMask` reads
  `__OSGlobalIntMask`, `MI_INTR_MASK` and `__osRcpImTable`; the locked C of `osCreateViManager`,
  `osSetThreadPri`, `osSetEventMesg`, `osStopThread`, `osGetThreadId`, `osGetThreadPri`,
  `osPfsRepairId`, `__osPfsReleasePages`, `__osPfsGetNextPage`, `__osSiRawReadIo/WriteIo`,
  `__osPiGetAccess/RelAccess`, `__osPiRawReadIo`, `osPiStartDma`, `osContStartReadData` is the
  ultralib J text up to names. `osPfsRepairId` and `__osPfsReleasePages` match only the >= J
  variants (the < J ones are different functions), which pins the SDK as VERSION_J.
- Names chosen are the VERSION_J link names. Three differ from w21r's recompiler names:
  VERSION_J renamed the PI raw functions (`include/PRinternal/piint.h`: the osPi* spellings are
  pre-J), so the inventory has `__osPiRawReadIo`, `__osPiRawStartDma`, `__osEPiRawStartDma`;
  identity.json keeps `osPiRawReadIo_rx`, `osPiRawStartDma`, `osEPiRawStartDma` for N64Recomp.
- `rmonPrintf` (0x8000870C) is medium confidence: its body (empty FINALROM varargs) equals its
  neighbour's; segment lib_92f0 holds exactly syncprintf.o's two functions, osSyncPrintf then
  rmonPrintf. Accepted on that position argument.

Leads seen while mapping callees (not renamed, not fully verified): `dll_remove`=`__osDequeueThread`,
`__osCleanupThread`=`__osEnqueueAndYield`, `osPiInit`=`__osPiCreateAccessQueue`,
`__osInsertTimer`(0x8000E390)=`osPiGetCmdQueue`, `osViModeTableGet`=`_VirtualToPhysicalTask`,
`__osSpDma`=`__osSpRawStartDma`, `__osContRamReset`=`__osPackRequestData`, `__osContGetStatus`=
`__osContGetInitData`, `osSiInit`=`__osSiCreateAccessQueue`, `__osContBuildRequest`=
`__osPfsRequestOneChannel`, `__osContParseResponse`=`__osPfsGetOneChannelData`,
`osPfsReadWriteFile_pages`=`__osCheckId`, `osWritebackDCache_full`(0x800086F0)=`osSyncPrintf`.
The same two tools apply to them once each body is checked.

## What changed (and how)

Tools (commit 1):
- `tools/conveyor/pipeline/static_rename.py` (new): `plan | symbols | source | lock | owned` over a
  rename table. Cycle-safe (one permutation from the original text). `source` rewrites code
  identifiers and asm-path strings only; comments are left for review. `lock` moves key and
  target_id and re-pins a body only after proving: body at `--base` held the old pin, and the
  renamed old body hashes to the new one. `owned` rebinds rom_owned_data.json (live pins refreshed
  after proving the base held them; pinned research files re-issued as renamed copies in
  `rebind/`).
- `targets_rename rename MAP.json [--dry-run]` (new subcommand): renames Conveyor rows in place
  through temporary ids (n64_target, function_status, matrix_entry, work_unit, promotion_record),
  rebuilds each reloc-aware object under its new name (the object carries the symbol; a changed
  object supersedes matrix evidence, as `apply` does). `apply` cannot do this (cycles, locked and
  promoted rows are refused by design).
- Game side: `game_static_legacy.us.txt` (new, hand-maintained) + `blob_tu.static_symbols`: the
  game/overlay link keeps the pre-rename names at the same addresses, because src/blob (and its
  preludes, asm/us/blob, asm/us/ovl_*) call the old names and the permutation cannot be expressed
  as aliases. `src/blob/blob.ld` renders byte-identical and `blob_tu symbols` regenerates
  `asm/us/blob/symbols.json` byte-identical. `ovl_targets` gives the boot tail the static names.

Rename (commit 2), all through the tools:
- `symbol_addrs.us.txt`: 43 definitions (comments rewritten to `libultra NEW (renamed from OLD)`);
  dropped the contradicted `dup-addr ... osPfsFileState = 0x8000F3A4` line.
- asm: `asm/us/nonmatchings/rom/**` permuted and renamed (41 files); `make extract` then reproduces
  exactly the permuted GLOBAL_ASM files and regenerates `asm/us/8410.s` and `rush2049.us.ld`
  (the osDpWait text-boundary assert). `asm/us/boot_tail/symbols.json`: the 43 keys renamed in the
  committed file + SHA256SUMS (a full `ovl_targets generate` would also carry unrelated pre-existing
  drift, so it was not used; ovl_a/ovl_b untouched).
- C: `src/rom/*.c|h`, `src/rom_auto` (4 files git-mv'd to the new names), static headers
  (`include/PR/os_{cont,pfs,pi,thread}.h`, `include/m2c_types.h`, `include/static_debug_context.h`,
  `src/rom/rom_tu.h`, `scheduler_declarations.h`, `pi_manager_declarations.h`) and the 7
  `cloud/matches/boot_tail/*.c` submissions. Hand edits: stale "historical label" comments removed or
  rewritten; duplicate prototypes the permutation created in os_pfs.h removed; `PROMOTED — NAME`
  headers retitled (layout derives promotion state from them; without it static coverage fell to
  546); `include/PR/os_thread.h` no longer declares `osGetThreadPri(OSThread*)`, which would
  conflict with the locked `s32 osGetThreadPri(void *)` body (dll_get_priority before; the old
  declaration graph had no shared prototype either).
- `matched.lock.json`: 73 of 584 entries migrated (43 renamed keys/target_ids, 30 bodies re-pinned
  because they call a renamed function), each with the pure-rename proof. `lock check`: all 584 intact.
- `rom_owned_data.json`: members/prior_locks/text-boundary names; pins rebound (renamed copies of
  `cloud/work/integration_B18/lib_7630.c`, `static_C16/candidate.c`, `static_C18/full_module.c`,
  `static_C14/osDpWait.c` -> `rebind/osSpTaskYield.c`, the endpoint proof, both context-hash lists).
  `owned_existing_storage.PROFILES` accepts the renamed source of timer_services/sdk_initialize as a
  reviewed `renamed` revision (the historical activation still validates).
- `src/rom/storage_overrides.mk` regenerated by the build (it lists the rebound context file).

Tests and receipts (commit 2): activation-replay fixtures (`tests/conveyor/test_existing_storage.py`,
`test_initialize_storage.py`, `test_owned_text.py`) resolve pinned context files from
`cloud/work/frontier/superseded/w22g_libultra_rename/` (byte-identical pre-rename copies, README there)
when the live file no longer has the recorded hash; `test_autodecomp_population.py` uses
`osViGetNextFramebuffer`; `tests/cloud/test_macro_stream_contracts.py` compares frozen bodies after
applying the rename; `cloud/work/frontier/dot_frame_dispatch_20261005/verification.json` re-pins
`viTickStart.s` (two jal operand names changed, words identical; `REBIND_w22g.md` there).
Game-side tests keep the old names (they read the game symbol tables) and pass unchanged.

`tools/recomp/identity.json` (commit 3): reduced to the 11 recompiler-only names (the `_rx` ones and
the three pre-J PI names), keyed by the new inventory names; `gen_config.apply_identity` accepts it
against the real layout. `tools/conveyor/pipeline/corpus.py` CANONICAL_ALIASES drops
`osGetThreadPri -> dll_get_priority` (pairs by name now).

## Conveyor DB

- Backups before anything: `scratchpad/conveyor.db.w22g.bak` (`cp`, 20:20; NOTE the live DB had a
  7 MB WAL, so this copy may miss recent rows) and `scratchpad/conveyor.db.w22g.bak2` (sqlite backup
  API, consistent, 20:41).
- Dry run and full apply on a copy (`scratchpad/w22g/data`): 43 renames, 29 matrix_entry rows purged,
  0 leftover temporary ids, `targets_rename audit` clean.
- Live apply: see "DB apply" below.

## Builder

`~/rush2049/scratch/frontier/w22g/repo` (copied from w21i's scratch repo, then rsynced from this
worktree; `nice make COMPILER=ido -j2` with every `src/rom/*.c` touched, then `make test`).
`~/rush2049/repo` not touched.
