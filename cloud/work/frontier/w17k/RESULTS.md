# w17k - IDO 7.1 tooling for the Rush boot-tail static TUs

Branch `worktree-agent-ac31347ced350190c`, fast-forwarded to master 26b107a0, then 4 commits (not pushed):

| commit | what |
|---|---|
| `26564021` A | `tools/conveyor/compilers.py` (pinned IDO 5.3/7.1: release archive + sha256, install dirs, Make variable, fingerprints of cc/cfe/uopt/ugen/as1 per known build; `install`/`identify`/`segments`), `build_toolkit --swap-ido-from`, `publish-toolkit --no-pin`, `lock add --toolkit SHA` (records `"compiler"`), `lock verify` uses an entry's own toolkit. Tests: `tests/conveyor/test_compiler_pins.py` |
| `a74fd9be` B | `layout.derive` sets each segment's `compiler` from `rom_compilers.json` (stray pin refuses), `write_opt_overrides` emits `<tu>.o: CC := $(IDO71_CC)`, lock/segment compiler conflicts refuse; Makefile `IDO71_CC`, a missing-compiler check in the ROM-TU recipe, ROM objects depend on `opt_overrides.mk`; `promote run` + `promote_batch` refuse a lock or context row from another compiler; `context_check.py --compiler`; `promote_batch --dry-run/--splice-out/--lockfile`. With no pins, `opt_overrides.mk` is unchanged. Tests: `tests/conveyor/test_rom_compiler_override.py`, `unit/test_layout.py` fixture isolates the pins file |
| `cef57231` B2 | **the switch alone**: `rom_compilers.json` pins the 18 boot-tail TUs (0x11050 ... 0x26ab0) to ido7.1, regenerated `opt_overrides.mk` (18 `CC :=` lines). Test: `tests/conveyor/test_boot_tail_ido71_pins.py` |
| C (last commit) | docs: BUILDING.md (IDO 7.1 section: vendoring, builder/CI/cloud, toolkit), COMPILER_SETTINGS.md (segment table + evidence), conveyor README (lock line), ignores `tools/cloud/ido71/`; this lane dir |

Static numbers only. Nothing locked, promoted, published or pinned on the coordinator; no change to `~/rush2049/repo`.

## Proofs (watchman2, `~/rush2049/scratch/frontier/w17k`, 2 cores)

Scratch = this tree (master 26b107a0 + commits) + the Pi's `build/blob/game_code.deflate` (10:05, = 26b107a0). Logs: `switch.log`.
1. Baseline, IDO 5.3 only: `make COMPILER=ido` + `make test` -> `ROM matches!`.
2. **(i) Switch**: IDO 7.1 installed with `python3 -m tools.conveyor.compilers install ido7.1 --archive dl/ido71.tgz`
   (archive sha256 0d411696…e58e15 = pinned); B2's `opt_overrides.mk` -> all 102 ROM TUs rebuilt (new mk dependency),
   18 `ASMPROC … (IDO 7.1)` lines, compile line `tools/ido-static-recomp/build71/out/cc` (5.3 TUs still `build/out/cc`)
   -> `build/us/rush2049.us.z64: OK` / `ROM matches!` / test exit 0.
   Negative control: also pinning libultra `0x10d60` -> 19 CC lines -> `ROM does NOT match` (restored afterwards).
3. **7.1 toolkit**: `build_toolkit --swap-ido-from <d4c39cbc blob> --ido <7.1>` ->
   `4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557`, `compiler: ido7.1`; same sha built on the Pi
   (Python 3.11) and watchman2 (3.14). Swapping d4c39cbc's own `ido/` back reproduces `d4c39cbc…` exactly.
4. **(ii) Lock-style scores** (`score71.sh` -> `score71.log`; w16g `check.py` LOCK step = `lock add`'s verify_entry,
   with the 4586d230 bundle unpacked, against the reloc-aware inventory targets from the Pi DB, `fetch_targets.py`):
   all 58 w17g sources `7.1 LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical`; under the 5.3 bundle only the
   7 stubs are exact, the 51 are not (e.g. func_80010840 5.3 true=255). 51 fns = 9,796 B; 58 = 9,912 B.
5. **(iii) promote_batch path** (`dryrun71.sh` -> `dryrun71.log`): `context_check.py --compiler ido7.1` -> 58/58 `ok`
   rows with `"compiler": "ido7.1"` (`ctx71_O2.jsonl`); simulated lockfiles (`simlock.py`, never matched.lock.json):
   legacy (5.3) entries -> all 58 `REFUSED … lock was proven with ido5.3 but segment … builds with ido7.1`;
   7.1-bound entries -> 58 candidates, `--splice-out` 12 TUs. `fullsplice.sh` built those 12 spliced TUs in the full
   ROM under the switch: `ROM matches!`, every object newer than its TU, 58 more PROMOTED blocks (252 vs 194), no
   remaining passthrough for the 58 (`fullsplice.log`). src/rom restored.
6. pytest: new tests + test_layout/test_mips3_32_tu/test_cloud_setup pass. The rest of `tests/conveyor` on the Pi
   worktree: 20 failures, all environmental (empty `tools/decomp-permuter` / `tools/mips_to_c` submodules in the
   worktree, no runnable IDO on the Pi); none touch the changed code.

## Commands for the coordinator (main checkout, after merging A, B, B2, C)

Order matters: install 7.1 on the builder BEFORE B2 reaches `~/rush2049/repo` (otherwise its build stops with
`error: compiler tools/ido-static-recomp/build71/out/cc ... is not installed`; it never falls back to 5.3).
```
# 1. builder compiler (once; the repo sync excludes tools/ido-static-recomp/, so it survives)
ssh watchman2 'cd ~/rush2049/repo && python3 -m tools.conveyor.compilers install ido7.1 && python3 -m tools.conveyor.compilers identify tools/ido-static-recomp/build71/out'
# 2. the switch gate (no promotion): sync per docs/BUILDING.md, then on the builder
ssh watchman2 'cd ~/rush2049/repo && make VERSION=us COMPILER=ido -j16 && make test'      # ROM matches!
# 3. 7.1 toolkit, uploaded NOT pinned (Pi; expect sha 4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557)
python3 -m tools.conveyor.compilers install ido7.1 --dest /tmp/ido71
python3 -m tools.conveyor.bundles.build_toolkit --swap-ido-from ~/.conveyor/blobs/d4c39cbc85750cd02d3494f318c65e770bbf7d4b10aae137bcf5b5f149137c5b --ido /tmp/ido71 --out /tmp/tk71.tar.gz
python3 -m tools.conveyor.cli publish-toolkit /tmp/tk71.tar.gz --no-pin
# 4. locks: all 58 w17g sources with the 7.1 toolkit (the 7 stubs too: in a 7.1 segment a 5.3 lock is refused)
TK71=4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557
for f in $(ls cloud/work/frontier/w17g/sources | sed 's/\.c$//'); do
  python3 -m tools.conveyor.pipeline.lock add cloud/work/frontier/w17g/sources/$f.c:$f \
    --flags "-g0 -O2 -mips2 -G 0 -non_shared" --toolkit $TK71; done
# 5. 7.1 context rows (builder; copy w17g/sources there, bring the jsonl back) - or use w17k/ctx71_O2.jsonl (same sources)
#    python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 OUT.jsonl fn=cloud/work/frontier/w17g/sources/fn.c ...
# 6. dry run, then the real batch (no `layout convert`: all 12 segments are converted)
SEGS="0x113e0 0x11640 0x15a10 0x17dc0 0x1a660 0x1cf90 0x1ece0 0x1f340 0x207b0 0x21210 0x22300 0x26ab0"
ONLY=$(ls cloud/work/frontier/w17g/sources | sed 's/\.c$//' | paste -sd,)
DEDUP=rom/lib_113e0,rom/lib_11640,rom/lib_15a10,rom/lib_17dc0,rom/lib_1a660,rom/lib_1cf90,rom/lib_1ece0,rom/lib_1f340,rom/lib_207b0,rom/lib_21210,rom/lib_22300,rom/lib_26ab0
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w17k/ctx71_O2.jsonl $SEGS --only $ONLY --prefix-dedup $DEDUP --dry-run
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w17k/ctx71_O2.jsonl $SEGS --only $ONLY --prefix-dedup $DEDUP
```
Do not use `w17g/ctx/ctx71_O2.jsonl`: its rows carry no `compiler` and are refused as 5.3 rows. `w17k/ctx71_O2.jsonl` was
computed with the committed `context_check.py --compiler ido7.1` from the same 58 sources (`dryrun71.log` head: 0 not-ok).
If the 7 stubs are promoted with 5.3 locks BEFORE the switch (w17g's "today" path), drop them from ONLY.
Static would move 431/668 -> 489/668 functions (77,844 -> 87,756 B) per w17g's inventory; recount with `make progress`.

## Design notes
- Compiler identity is derived from toolkit CONTENT (fingerprints), not from a user label: `lock add --toolkit` refuses a
  toolkit whose `ido/` is not a known build. The 5.3 table holds two builds (the toolkit's source build used by every
  pinned toolkit and the builder, and the v1.2 release used by CI); 7.1 holds the v1.2 release.
- Lock entries without `compiler` stay IDO 5.3 (all 435 existing). Legacy 5.3 locks inside 7.1 segments are allowed
  (proven identical by the switch gate); NEW promotions need the segment's compiler exactly.
- The flagset is unchanged (`-g0 -O2 -mips2 -G 0 -non_shared`); the compiler is a separate axis everywhere, so the
  map hash, flag registry and `_segment_flagset` are untouched.
- `0x26f60` (asm, 4 fns, 5,232 B) is not pinned: it is not converted, and derive refuses a pin on it.
- CI: no ROM build there, and no test needs 7.1, so `tools/cloud/setup.sh` and its cache key are unchanged; cloud boxes
  can `compilers install ido7.1 --dest tools/cloud/ido71` (now ignored).

## Files
Scripts: `fetch_targets.py` (Pi), `score71.sh`, `check.py` (= w16g), `simlock.py`, `dryrun71.sh`, `fullsplice.sh`.
Logs: `switch.log`, `score71.log`, `dryrun71.log`, `fullsplice.log`; `ctx71_O2.jsonl`; `list51.txt`, `list58.txt`.
Builder scratch: `dl/` (release archives, `tk71.tar.gz`, unpacked toolkits), `cloud/work/frontier/w17k/spl/` (spliced
TUs), `build*.log`.

## Disclosures
- The worktree holds an untracked copy of `cloud/work/frontier/w17g/{sources,ctx}` and `baserom.us.z64` (both needed by
  dry runs; not committed). The Pi scratchpad holds the 7.1 download and `tk71.tar.gz`.
- Read-only uses of the live conveyor DB/blob store on the Pi (target objects, the d4c39cbc toolkit blob, pinned sha).
- No permission denials affected results (a few compound shell commands were refused by the worktree guard and re-run
  as plain commands).
