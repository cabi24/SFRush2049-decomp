# w23m: merge w23g (ll.c / libultra rename) onto wave23

Branch `w23m` = wave23 (6f091757 = origin/master) + w23g's 13 commits (68477154..86042087), with
the rename re-applied to everything wave23 added after w23g's base. Not pushed.
Status: done; every gate passes; coverage identical to wave23.

## 1. Cherry-pick (13 commits, in order)

Only two commits conflicted, both the same way:

- `af6a259d` (game side of w22g's 43) and `ffc96fc4` (6 suspects): `blob_matched.lock.json` (content)
  and `src/blob/func_80098ADC.c` (modify/delete).
  - `src/blob/func_80098ADC.c`: wave23 (w23f) deleted the single and moved func_80098ADC into the
    `engine_sound_sync` group; kept the deletion.
  - `blob_matched.lock.json`: key-level 3-way merge (base = commit^, ours = w23m, theirs = commit;
    theirs for keys it changed/added, ours otherwise; `json.dumps(indent=2, sort_keys=True)` + newline,
    the file's existing format). One key changed on both sides, `func_80098ADC` (w23g re-pinned the old
    single, w23f re-pointed it at the group): kept ours, then renamed in step 2. Result 985 entries.
- Everything else (matched.lock.json, rom_owned_data.json, rush2049.us.ld, asm/us/blob/SHA256SUMS,
  symbols.json, blob.ld, src/rom) merged cleanly: no src/rom TU was changed on both sides. The
  regenerated outputs (step 3) equal the merged text.

## 2. Re-rename of wave23's additions

Scan (`static_rename.permute_c`, union table for game text, ll+suspect tables for src/rom) over every
file wave23 changed since 68477154: no src/rom body, static lock key or owned slot added by
w23a/w23c/w23i names a renamed function (the static `lock check` and `make extract` are unchanged). The
game side had `osJamMesg` (= static osSendMesg) in 18 files; renamed with
`static_rename.renamed_game_source()` (no redeclarations to drop):

- src/blob/groups/codex_visual_tire_module_20261002/{buffer_swap,codex_transform_b109}.c (w23d)
- src/blob/groups/engine_sound_sync/{engine_sound_sync,audio_effect_setup_b132,codex_transform_b109}.c (w23f)
- their lane copies cloud/work/frontier/w23d/groups/..., w23f/groups/... (still byte-identical to
  src/blob), w23d/buffer_swap/best.c, w23e/controller_poll/{base,best}.c, w23f/engine_sound_sync/{n6,oracle}.c
- cloud/work/lean_audio_effect_setup_20261006/group.c (unpinned; the engine_sound_sync group's
  audio_effect_setup_b132.c is documented as its byte-identical copy, which holds again)
- cloud/work/frontier/w21h/{engine_sound_sync,func_800D7634}/best.c: recomp draft sources
  (`tools/recomp/replacements.json` draft_engine_sound_sync / draft_func_800D7634). With
  `game_static_legacy.us.txt` empty, replace.py binds game bodies by inventory names, so an unrenamed
  draft would have called the true osJamMesg. (w23g missed these two; no receipts pin them.)

Blob lock: `resplice.py` (w23g's rename_resplice.py with this lane's builder scratch) proved both groups
are the pinned wave23 text renamed, recompiled them and gated the image: `image matches (647072 bytes)
bf7da3fa...`, 6 entries re-pinned (`blob_lock_migration_wave23.json`). Before that, `rebuild_stale.py`
recompiled the 522 singles + 75 groups whose local objects predated the rename (provenance), in
`watchman2:~/rush2049/scratch/frontier/w23m/{splice,group}`; 0 stale after.

Recomp: `identity.json`'s 35 rows all name current inventory symbols; `game_legacy_names()` returns {}
on the emptied file; README note added. `python3 -m pytest tests/recomp`: 108 passed, 2 skipped.

## 3. Regeneration and gates

- `make extract`: no change. `blob_tu generate`: asm/us/blob/*.s, symbols.json, SHA256SUMS, blob.ld
  unchanged from the merged text.
- `lock check`: all 603 locked functions intact.
- `blob_unit --tag w23m check`: 985 locked bodies, 985 equal, 0 differ.
- `blob_group check`: 0 problems. `blob_splice check`: 985 entries, 0 problems.
- `ovl_layout build` + `ovl_rom produce`: both streams == ROM; `ovl_splice check`: 63 intact.
- `blob_rom produce`: byte-identical to the ROM's stream; ROM build in builder scratch
  (`~/rush2049/scratch/frontier/w23m/repo`, -j2): `ROM matches!`, SHA-1 3f99351d7bb61656614bdb2aa1a90cfe55d1922c.
- `gate_only.py $(ls src/rom/lib_*.c)` (CONVEYOR_BUILDER_REPO = that scratch, jobs 2): GATE PASS.
- `make progress`: static 600/668 130,500 B; game 985/1216 325,760 B; overlay A 53/193; B 10/49 (= wave23).

## 4. Tests

- Pi worktree `make test-fast JOBS=4`: conveyor 1542 passed, 794 skipped; cloud 770 passed, 150 skipped; exit 0.
- rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` (clone `~/rush2049/scratch/w23m`, origin/master =
  6f091757, tested at 70ddbd48): conveyor 2285 passed, 51 skipped; cloud 919 passed, 1 skipped,
  27 subtests passed; exit 0.
- `check_submissions.py --base origin/master --head HEAD` (rocky): `Checked 92 changed cloud submissions`, exit 0.
