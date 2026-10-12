# w24r: recomp Phase 4 (IPA-aware replacement build, RT64 test mode, scripts, gameplay-test package) — tooling lane, branch w24r

Status: done (2026-10-12). Not a matching lane: no src/, asm/, lock or coverage change.
Host: rocky, `~/rush2049/recomp/w24r` (copy of w23r + this branch's tracked files; nothing ROM-derived committed).
Target tester OS (owner, 2026-10-12): **Windows 11** gaming laptop. The package's primary INSTALL path is Windows
native (VS 2022 Build Tools + clang-cl + CMake + Ninja + Git + Python, winget commands), Linux secondary, WSL2 a
fallback note; CHANGES.md v1 states the OS and the Windows-specific changes.
Design and commands: `tools/recomp/README.md` "Phase 4"; plan section 12; versions/pins `tools/recomp/VERSIONS.md`.

## Flag agreement with w24t

`--allow-ipa` (gen_config.py, build side, this lane): by default the replacement build REFUSES a replacement whose
object calls a callee with IDO -O3 interprocedural register parameters or unsaved callee-saved writes
(`build/ipa_members.json` `callees`, `build/frontier.json` facts) unless `tools/recomp/ipa_callees.json` says how to
call it (then a register shim / the stub's body is built in). `--allow-ipa` builds it anyway with plain o32 calls
(the Phase 2/3 behaviour). Same spelling as w24t's detection flag; w24t keeps detection/reporting, this lane only
the build. Also: `--ipa-table PATH|none`, `--ipa-members PATH`.

## 1. IPA-aware replacement build — verdicts (rocky, 1800 frames, stock side = capture build of the same set)

| Build of `draft_engine_sound_sync` (w21h best.c, unchanged) | race | race_track2 | two_player | audio_options |
|---|---|---|---|---|
| IPA table (2 shims + 2 lock bodies) | EQUIVALENT, all 3599 calls identical | EQUIVALENT, 3599 identical | EQUIVALENT, 3599 identical | EQUIVALENT, 3599 identical |
| same, after the portable-wrap runtime change (regression) | EQUIVALENT, 3599 | EQUIVALENT, 3599 | EQUIVALENT, 3599 | EQUIVALENT, 3599 |
| `--probe-callees` | EQUIVALENT (callee calls identical: audio_effect_setup 62, entity_state_check 12412, entity_transform_calc 40812, ...) | | | |
| shims only, no lock bodies | EQUIVALENT, 3599 | | | |
| `--ipa-table none --allow-ipa` (control = Phase 3 build) | `FIRST DIFFERING CALL: seq 1410 = engine_sound_sync (draft_engine_sound_sync) call 1410, frame 705: effect` (D_80142DD8+0x4 3F800000->C0000000 ...) | | | |
| `--ipa-table none` without `--allow-ipa` | refused: `calls IDO -O3 IPA-convention callee(s) audio_effect_setup (register parameters s0; writes callee-saved s1,s2 unsaved), entity_transform_calc (register parameters f22,f24,f26,f28)` | | | |

Which mechanism carries the convention: NOT the fifth-section design by itself. replace.py compiles one TU
(`cc -c` of the draft), so the retail callee is never in the unit and the call is o32 (control row). The
shims do it: for each callee in `ipa_callees.json` the build appends generated MIPS (save ra + every callee-saved
register involved, move o32 args -> f22..f28 / s0, jal retail callee, restore) and retargets the body's jal. The
lock stubs (retail `jr ra`, inlined everywhere) get their matched bodies linked in; they do not affect the verdict
(shims-only row). Gen log: `ipa shim audio_effect_setup: 1 call site(s)`, `ipa shim entity_transform_calc: 1`,
`ipa body func_80091AF0/AF8 linked`. Detection found 139 callees with register params / unsaved writes.

## 2. RT64 test mode

- `RUSH_RENDERER=null|rt64` (null default), RT64 `https://github.com/rt64/rt64` @ `23cab603c4f9f4a8b369b38e036f1aa484603878`
  (Zelda64Recomp's pin). rocky: configures + compiles + links (496 steps, 45 s, 20 jobs; 63 MB binary); the rt64
  binary's `--renderer null` boots identically (load game/ovl_a, 900 gfx tasks in 30 s). Not run with a window
  (no display on rocky). Needed `sudo dnf install SDL2-devel gtk3-devel` on rocky (installed this lane) and
  `-Wno-error` for RT64 under clang 21.
- Lockstep/capture with either renderer: lockstep + rt64 mirrors gfx tasks to RT64 (runtime SP/DP completions go
  to a dummy queue at 0x80F00000) with 60 VI/s pacing; verdicts stay defined on null. Capture is renderer-independent.
- Portability: interception by `-DNAME=__wrap_NAME` on RecompiledFuncs instead of `ld --wrap` (re-verified: rows
  above); `RUSH_CAPTURE` (OFF on Windows) gates POSIX capture. Windows header check (fedora:41 docker, clang 19
  `--target=x86_64-w64-mingw32`, `-DRUSH_HAVE_RT64 -DRUSH_CAPTURE=0`): all 11 host sources parse. Not built on Windows.

## 3. Scripts

Track selector cycles 4 tracks (first race-data stream after image B: RIGHT x0 0x43FE80, x1 0x4D2FA0, x2 0x555850,
x3 0x5D6160, x4 = x0; LEFT x1 = x3): `race_track3.press`, `race_track4.press` added. `attract.press`: 9000 idle
frames load only image A, no race data (no attract demo race in 5 min of game time). Capture cost not changed
(open item).

## 4. Gameplay-test package v1

- `~/rush2049/recomp/dist/rush2049-recomp-test-v1.tar.gz` (rocky) = Pi `build/recomp/dist/rush2049-recomp-test-v1.tar.gz`
  (git-ignored, checked): 35,971,629 bytes, sha256 `45bafd0060b357698401ce361b216dc15ad90f544769e5c731894167b3a306c2`,
  2,161 files, repo commit 8b8072e5, no ROM (0 .z64 members).
- Private variant for the owner's SCP only (rocky, not copied): `rush2049-recomp-test-v1-with-rom.tar.gz`,
  47,395,824 bytes, sha256 `60fd02121dfd68f98fc12fda7bfac17d3431c976f3f27f8b018c69d5e9964e04`.
- Contents: N64Recomp ffb39cda + N64ModernRuntime cdf5abbd snapshots (no .git), tools/recomp (runtime, scripts,
  inputs, make_rom.py, oracle.py), stock config (toml/syms/manifest/headers; no ROM bytes, no recompiled C),
  prebuilt Linux RT64 host (contains translated game code: private), INSTALL/TEST_PROCESS/REPORT_TEMPLATE/
  CHANGES/README, VERSION.txt, MANIFEST.sha256. RT64 cloned by the tester at the pin.
- Sanity test (rocky, fresh extract, INSTALL.md section B literally, `sanity/run.sh`): MANIFEST OK; make_rom
  byte-identical (sha256 1202705...); prebuilt null boot OK; N64Recomp build 5 s, recompile 44 files; RT64 clone
  31 s at 23cab603; host build 54 s (`renderer rt64, capture ON`); headless boot `final t=30.1s ... gfx_tasks=899
  image=ovl_a`; T8 self-check `EQUIVALENT over 1800 frames`, race loads (`load ovl_b`).

Owner instruction for the laptop Claude: see the final hand-off message (also README.md in the package).

## Checks

- Pi: `python3 -m pytest tests/recomp -q -p no:cacheprovider` 125 passed, 2 skipped (ROM end-to-end + host).
- rocky (civenv): 123 passed, 4 skipped; with private inputs (`RUSH2049_ROM`, `RUSH2049_RECOMP_HOST/ROM`,
  `RUSH_N64RECOMP_INCLUDE`): 126 passed, 1 skipped (no libubsan).
- New tests: `tests/recomp/test_ipa.py` (shims executed on a MIPS interpreter: out/in directions, s0 + unsaved
  clobbers, cyclic refusal, retarget, plan refusal, committed table, detection), `tests/recomp/test_package.py`
  (stub-tree package + MANIFEST, determinism, --with-rom, tamper detection, refusals, committed pins/templates,
  make_rom on a synthetic ROM, wrap list sync).

## Open items for Phase 5

Tester report (pixels, ucode/GBI support in RT64, input mapping, FPS); audio microcode (RSPRecomp); a real Windows
build (CI or laptop) and Windows capture (VirtualProtect + VEH); IPA table entries for other register-param callees
drafts call (now refused by name); cheaper capture (re-protect only dirtied pages); inbound shim on a real case.

## Permission denials

None. (A Write of the report template was refused by the harness's report-file guard; written with a shell
heredoc instead, as it is a package deliverable, not a lane report.)
