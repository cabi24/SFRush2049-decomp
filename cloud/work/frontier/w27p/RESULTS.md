# w27p: integration of Astra's PRs #367–#371 (2026-10-12)

Branch `w27p` from `wave27` `2911f896` (wave27 did not move during the lane). Not pushed. None of #322, #325, #339,
#342, #357, #359–#366 has a commit after 2026-10-12T19:30Z (latest: #364 19:11Z, #366 19:10Z); nothing re-imported.

## Outcome

| PR | What | Verdict | Bytes | Gate / verifier line |
|---|---|---|---:|---|
| #369 | A `func_80397AD8` model-slot initializer (strict 327/327, own .rodata 40 B + .data 4 B) in genuine context (NONMATCH 97FFC root 15/268, 949CC, 9842C); installed as the 4th member of `dot_runtime_a_availability_refresh_20261007` | LAND | 1,308 | `image A: group spliced ['func_80394564', 'func_80394834', 'func_80394B00', 'func_80397AD8']; image gate byte-identical` |
| #367 | B `func_8039244C` HUD teardown, 4/95 (full 380 B, frame 64) | IMPORT-ONLY | 0 | `PASS input-bound complete HUD teardown proof (NONMATCH 4/95; zero coverage)` |
| #368 | A `func_80396FA0` profile controller, 31/718 in genuine 9842C context (frame 56 vs 88 + argument home) | IMPORT-ONLY (verifier fixed) | 0 | `PASS 96FA0 research 31/718; 7152 prior exact bytes preserved; zero new coverage` |
| #370 | B `func_8038D498` radial impulse, complete semantic reconstruction (ordinary-ABI diagnostic 888 B / frame 184 vs native 768 B / 104, private s2 ABI) | IMPORT-ONLY | 0 | `verify.py --deep` exit 0 (receipt JSON, `native_private_abi: false`, 222 GNU word differences) |
| #371 | A `func_803AA3D8` stored-name boundary audit (native loader/input-handler witness) | IMPORT-ONLY, finding on CLAIMS | 0 | `20 native loader/confirmation fixtures; 19-byte malformed-name witness; maximum 65 bytes including NUL; zero new coverage` |

Overlay A 80/193 (49,752 B, 25.63%) → **81/193 (51,060 B, 26.30%)**; overlay B unchanged 21/49 (9,020 B, 20.55%).
Static 627/667 (149,708 B, 93.15%) and game 1010/1216 (341,056 B, 52.71%) unchanged (`make progress`, reported
separately).

Gates after the splice (`ovl_layout build` → `ovl_splice check` → `ovl_rom rom`): A "image gate (81 locked C bodies):
byte-identical", B "(21 locked C bodies): byte-identical", "ovl lock: all 102 runtime-image entries intact", both
streams `== ROM`, **"ROM build (game blob + both runtime-image streams composed): SHA-1 EXACT"**, "ROM matches!",
`MAKE=0 TEST=0`. A fresh `ovl_splice rebuild` of A and B (objects deleted first) is byte-identical (81/21 bodies), and
the chain was re-run on those objects with the same result.

## #369: why it landed although the packet says "empty-claims research"

The packet's own `--score` run reports `func_80397AD8: MATCH`, "own .data verified at 0x803B5EE8 (4 bytes)", "own
.rodata verified at 0x803B94BC..0x803B94E4", with 94564/94834/94B00/949CC MATCH and 97FFC 15/268 — a complete body
compiled in its real group context (the genuine, still-NONMATCH caller is in the TU). The author held it back because
the caller is unresolved and 949CC is owned by another group; neither blocks the overlay installer, which splices
only claimed members and keeps the rest as context (same pattern as #358's 80390F60 and #366's 96EE8/96F44).
Installed with `--name dot_runtime_a_availability_refresh_20261007 --claims
func_80394564,func_80394834,func_80394B00,func_80397AD8`: the three locked members stay byte-identical (image gate),
949CC stays locked in `dot_runtime_a_menu_cleanup_20261007` and is byte-identical context here, 97FFC and 9842C are
context. The installed `group.json` provenance was rewritten to describe the landing (the packet's text says
"research-only ... zero installable bytes", which would be false in a production file): `splice_group` was called
from Python with `packet_spec` wrapped to replace only `provenance`; origin in the lock is the real packet path.
No new shaping devices: one consumed `f32 scale = 1.0f` local, declaration order placing color/output homes (both
causal controls replayed by the verifier). The packet still documents 97FFC 15/268 and 9842C 440/451 as open.

## Notes

- #368 verifier trap (as w25p/w26p): its receipt bound the sha256 of three production sources
  (`src/ovl_a/groups/{runtime_a_profile_menu_20261006, dot_runtime_a_availability_refresh_20261007,
  dot_runtime_a_menu_cleanup_20261007}/group.c`); the first two had already moved in w26p (#359/#366), so it failed
  "input binding changed" on wave27 before any install. It now binds packet snapshots
  `baseline_<group>.c` (byte-identical to wave26 `46780d3e`, the packet's base); receipt re-recorded `--record
  --deep` on rocky: only those three path labels (same sha256) and the `verify.py` hash changed.
- #368's packet group claims 8 members incl. 94564/94B00 (availability group) and the 96EE8/96F44 singles; it is
  research only, the author asks for no installation. Not installed.
- #371 does NOT change any extent or inventory decision: it refutes the proposed 18-byte stored-name bound (a
  checksum-valid malformed save gives a 19-byte name, so language 4/5 confirmation text needs 65 B), i.e. shrinking
  AA3D8's `text[80]` to `text[64]` is not justified by the loader. #363 (2/717, frame 192 vs 176) stays as is; the
  16-byte frame gap needs another explanation. Recorded on Astra's CLAIMS row, with AD57C 348/523 (negative probes).
- #370's `count_contract.py` reads `blob_matched.lock.json` rows of five accepted game sources (identity of the
  `extern volatile s16 D_801543CA` declaration); a future change to those sources would trip it (current: pass).
- #367 discloses one statement-line shaping (three initializers on one line) — research only.
- Other verifiers that name `dot_runtime_a_availability_refresh_20261007` (#359 `availability_reset`, #336
  `availability_selector`) use packet-local groups; both `--deep` pass after the install ("PASS: 728 new bytes;
  1120 preserved; ...", "PASS: 720 new exact bytes; 400 preserved bytes; ...").

## Tests

- rocky `REQUIRE_TOOLCHAIN=1 pytest -n 5 --dist loadgroup` on the five new/changed packet tests: before install 4
  passed + #368 failed (input binding, fixed as above); after install 5 passed.
- Every packet verifier before and after install, rocky with toolchain: `--deep` (#367, #368, #369 also `--score`,
  #370), `--host`/`--sanitize` (#371), and `python3 -O` replays: all exit 0 with the lines in the table.
- rocky `REQUIRE_TOOLCHAIN=1 make test-fast JOBS=16` (at `99831355`): conveyor 2339 passed, 52 skipped; cloud 960
  passed, 1 skipped; exit 0.
- Pi `make test-fast JOBS=4` (at `99831355`): conveyor 1589 passed, 802 skipped; cloud 771 passed, 190 skipped;
  exit 0. The worktree has the `tools/mips_to_c` submodule (initialised with `--reference` the main checkout), so the
  m2c tests ran against it.

## Setup notes

As w26p: `cp baserom.us.z64`, copy `build/game_code.bin` + `build/blob/obj`, submodules `--reference` the main
checkout, `ovl_splice rebuild` A/B, `blob_layout derive`, `blob_rom produce` (stream byte-identical; its rewrites of
`asm/us/blob/{SHA256SUMS,symbols.json}` and `src/blob/blob.ld` reverted), baseline gate chain SHA-1 EXACT before any
change. `OVL_BUILDER_DIR=/tmp/ovlsplice-w27p`. Rocky clone `~/rush2049/scratch/w27p` (renamed from w26p,
`receive.denyCurrentBranch=updateInstead`, branch pushed from the Pi), env `~/rush2049/scratch/w27p.env`.
PR comment script: `$SCRATCH/w27p_pr_comments.sh` (MASTER_SHA placeholder).
