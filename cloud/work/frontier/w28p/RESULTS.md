# w28p: integration of Astra's static boot-tail PRs #372–#377 (2026-10-12)

Branch `w28p` (worktree `.claude/worktrees/w28p`) from `wave28` `f45e3940` (= origin/master; wave28 did not move
during the lane, nothing to merge). Not pushed. Status: DONE.

**Static 627/667, 149,708 B (93.15%) → 631/667, 150,968 B (93.93%): +4 functions, +1,260 B** (`make progress`).
Game 1010/1216 (341,056 B, 52.71%), overlay A 81/193 (51,060 B), overlay B 21/49 (9,020 B): unchanged, reported
separately.

## Outcome

| PR | Function | Verdict | Bytes | Scorer / gate lines |
|---|---|---|---:|---|
| #375 | func_80010DD8 (lib_11640) | LAND | 168 | `score.py fn` (builder IDO 7.1) `MATCH`; `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (49 rows)`; lock `(ido7.1): score=0`; `OWNED-OFFSETS: OK 19 ROM TU data sections (16 owned slots), 23 named objects ...`; `PROMOTED 1 (168 B)` (e957a7f1) |
| #373 | func_80012234 (lib_11640) | LAND | 372 | `MATCH`; `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (107 rows)`; `(ido7.1): score=0`; OWNED-OFFSETS OK; `PROMOTED 1 (372 B)` (58e9d8f5) |
| #372 | func_8001558C (lib_15a10) | LAND | 348 | `MATCH`; `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (96 rows)`; `(ido7.1): score=0`; OWNED-OFFSETS OK; `PROMOTED 1 (348 B)` (1b0f196a) |
| #374 | func_80010450 (lib_11050) | LAND | 372 | `MATCH`; `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (115 rows)` (after the target refresh below); `(ido7.1): score=0`; OWNED-OFFSETS OK; `PROMOTED 1 (372 B)` (80398976) |
| #376 | func_80012D18 | IMPORT-ONLY (research) | 0 | `verify.py --deep` exit 0 before and after; finding on CLAIMS |
| #377 | func_80011910 | IMPORT-ONLY (blocked by owned-.bss rules) | 0 | `score.py fn` `MATCH` (code); lock-style `LOCK: true=65 strict=65 reloc_blind=0 objdump-dr DIFFERS(22 rows)` = the section-relative `.bss` references; context_check `refused object has data/rodata (.bss:64) outside an owned slot of func_80011910` |

All four landings: flags `-g0 -O2 -mips2 -G 0 -non_shared`, stock IDO 7.1 (segments pinned 7.1 in rom_compilers.json),
context rows `cloud/work/frontier/w28p/ctx71_O2.jsonl` (`context_check.py --compiler ido7.1`: all `ok`; dropped only
duplicate `u8/u16/u32` typedefs (1558C) and `typedef struct OSMesgQueue_s OSMesgQueue;` (10450)). Each promotion:
```
CONVEYOR_BUILDER_REPO='~/rush2049/scratch/frontier/w28p/repo' CONVEYOR_BUILDER_JOBS=2 \
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w28p/ctx71_O2.jsonl <SEG> \
  --only <fn> --prefix-dedup rom/<tu>
```
(0x11640 func_80010DD8, 0x11640 func_80012234, 0x15a10 func_8001558C, 0x11050 func_80010450, in that order), each
"builder gate: OWNED-OFFSETS: OK ..." and "PROMOTED 1 (N B)"; promote_batch's commit subjects were amended to carry the
PR number. No rom_tu.h, symbol_addrs, reloc_addrs or owned-data change, so no rebind re-pin was needed.

Final gate in `watchman2:~/rush2049/repo` (rsynced from the worktree per docs/BUILDING.md with `--exclude=.claude/
--exclude=.git`, then `rsync -a --delete asm/`): `make COMPILER=ido -j16` MAKE=0 "ROM matches!", `make test` TEST=0
"ROM matches!", `sha1sum build/us/rush2049.us.z64` = `3f99351d7bb61656614bdb2aa1a90cfe55d1922c` (= baserom);
`owned_offsets check`: `OWNED-OFFSETS: OK 19 ROM TU data sections (16 owned slots), 23 named objects checked against
retail addresses (18 in owned slots), 1 without a retail address`; lib_11050/11640/15a10 objects newer than their TUs.

## Integration notes

- **#372 shaping device (disclosed in the source header):** `result = id;` before the loop is an otherwise dead
  initialisation of the existing return local (every returning path overwrites it). It renumbers the ID-promotion
  expression ahead of the table-address expression in the preheader; removing it gives 4/87 differing words; no locals,
  frame bytes, calls or reads added. Accepted as a disclosed device.
- **#374 declaration placement:** commit b2572062 hoists lib_11050's `TransferRequest` typedef (previously in
  func_80010628's declarations) above the first slot, so `--prefix-dedup` sees it before func_80010450 and keeps one
  declaration. Typedef only; the gate proves bytes.
- **#374 stale inventory target:** `lock add` first gave `score=5` (`LOCK: true=5 ... objdump-dr DIFFERS(1 rows)`): the
  Conveyor DB's reloc-aware target for func_80010450 named the 0x800075E0 callee `osJamMesg` (pre-rename); the asm and
  symbol_addrs say `osSendMesg`. `refresh_split_targets.py func_80010450`: `reloc_aware 7f11b4aa6d8a -> reloc_aware
  c1b01692603a` (1 target superseded, 0 evidence rows purged); DB backup (sqlite backup API) in the session scratchpad
  `w28p/conveyor.db.w28p.bak1`. Other boot-tail targets calling the swapped osSendMesg/osJamMesg pair may carry the
  same stale name.
- **Verifier trap (as w25p–w27p):** the --deep TU stage of #372/#373/#374 spliced into the live `src/rom` TU (fails
  once the slot is promoted: `assert text.count(pragma) == 1`), and #377's C-only TU diagnostic compiled the live
  lib_11640 (fails `verification result drift` once 10DD8/12234 are C). Each packet now carries `baseline_<tu>.c`
  (byte-identical to the TU at 55e17941 = f45e3940), binds it, and splices/compiles against it (promote_batch's
  `REPO` pointed at a scratch copy); receipts re-recorded `--deep --write` on rocky: only that input and the
  verify.py hash changed (commit b90c56a5). #375's verifier compiles the current TU and passed unchanged.
- **#376** (research): stock baseline 674/787 differing, body 6 words short (3,124 vs 3,148 B); post-merge
  position/excess carriers keep the upstream expression webs (4 homes, frame 128); loop-field carriers reach the native
  120-byte frame with 2 expression homes at 743/787, refuting the frame-arithmetic identification of the missing locals.
  On the func_80012D18 CLAIMS row.
- Other PRs: #322/#325/#339/#342/#357/#361/#363–#365 last commits are before 2026-10-12T22:00Z; #367 (22:24Z), #368
  (22:27Z), #370 (22:28Z), #371 (22:29Z) are after it but before w27p's import (00:37Z); their packet files equal the
  tree (only #368's verify.py/verification.json differ = w27p's own verifier fix). Nothing re-imported.

## #377 assessment: why func_80011910 is not landed

- The descriptor is genuinely this code's static data: only func_80011910 references 0x800382F8..0x80038337 (all ten
  field stores; grep over src/rom, asm/us/nonmatchings/rom, asm/us, src/blob). It is a file-scope OSTask of lib_11640,
  and defining it in C is what makes as1 combine the field-pair address materialisations (packet causal matrix).
- The owned-slot mechanism cannot own it under the standing rules:
  1. `owned_data._bss_slot`: a `.bss` slot is a TU's whole `.bss` block, `alignment` must be 16, `container_vram % 16
     == 0`, `size % 16 == 0`, offset 0, one block per TU; `_rewrite_bss` emits `SUBALIGN(16)`. IDO's `.bss` section
     alignment is 16. 0x800382F8 is 8 mod 16 and the object is 64 B, so a block holding only the task is refused (and
     would link at 0x80038300).
  2. The only 16-aligned, padding-free window containing it is 0x800382F0..0x8003833F (0x50 = D_800382F0 4 B,
     D_800382F4 4 B, task 64 B, D_80038338 4 B, D_8003833C 4 B). That needs C definitions of four live neighbouring
     globals (D_800382F0: func_80010DD8/80010E80/800110C4; D_800382F4: func_80010DD8; D_80038338/3C:
     func_80011F60/800121BC), i.e. it touches neighbouring live globals (rule), and it would register a window that is
     not lib_11640's whole `.bss` (D_800382E8/EC before and D_80038340.. after are the same TU's objects), contrary to
     the slot contract. A pad array or invented padding is excluded outright.
- Path for the owner/coordinator: either an owner decision allowing a partial-window `.bss` slot that defines those
  four neighbours in retail order (IDO 7.1 lays `.bss` out in definition order; OWNED-OFFSETS would then check all five
  objects), or owning lib_11640's whole `.bss` once its extent and order are established. Recorded on CLAIMS.

## Tests

- Packet verifiers on rocky (`~/rush2049/scratch/w28p`, own clone, ci.env toolchain), before install: all six exit 0
  (`--deep` for #372/#373/#374/#376/#377, plain for #375). After the four installs (and the snapshot fix): all six exit 0,
  plus `python3 -O` replays of all six exit 0.
- rocky `REQUIRE_TOOLCHAIN=1 pytest` on the six new packet tests: `6 passed` before and after installation (at
  b90c56a5).
- rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` (at 2842773d): conveyor `2339 passed, 52 skipped`; cloud `966
  passed, 1 skipped, 27 subtests passed`; rc=0. (The first run failed 2 tests only because the fresh clone had no
  `origin/master` ref; fixed by pushing f45e3940 as refs/remotes/origin/master.)
- Pi `make test-fast JOBS=4` (worktree, at 2842773d): conveyor `1589 passed, 802 skipped`; cloud `771 passed, 196
  skipped`; rc=0. The worktree has `tools/mips_to_c` initialised (`--reference` the main checkout), so the m2c tests ran
  against it.
- Builder gate: see above (MAKE=0, TEST=0, SHA-1 3f99351d…, OWNED-OFFSETS OK). `lock check`: all 634 locked
  functions intact.

## Setup notes

Builder scratch `~/rush2049/scratch/frontier/w28p` (base copy + `repo/` seeded from `~/rush2049/repo` without
.git/.claude/cloud/build, submodule .git dirs removed, deflate streams copied); deleted at the end. Rocky clone
`~/rush2049/scratch/w28p` (local clone of w27p's clone with `tools/cloud/ido{,71}` copied; w27p's dir untouched),
env `~/rush2049/scratch/w28p.env`. PR comment script: `$SCRATCH/w28p_pr_comments.sh` (MASTER_SHA placeholder).
