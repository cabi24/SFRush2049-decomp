# w27s - static boot tail (worktree lane): func_80018448, inflate leftovers, libultra rename leads

Branch `w27s` (worktree `.claude/worktrees/w27s`, from wave27 8a54d6d2). Static numbers only (`make progress`).
Status: DONE (wave27 8a54d6d2 merged: no-op, wave27 had not moved). Not pushed.

**Static 625/667, 147,280 B (91.64%) -> 627/667, 149,708 B (93.15%): +2 functions, +2,428 B.** Game/overlays untouched.

| function | bytes | state | scorer / gate line |
|---|---|---|---|
| func_80018448 | 492 | **PROMOTED** ef6833a7 | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (129 rows)`; `TU: true=0 strict=0 reloc_blind=0 objdump-dr identical (129 rows)` `RESULT PASS`; lock `(ido7.1): score=0`; gate `OWNED-OFFSETS: OK 19 ROM TU data sections ...`, `PROMOTED 1 (492 B) @ ef6833a7` |
| inflate_dynamic | 1936 | **PROMOTED** a1bbc86c (+ reloc_addrs prep 2 entries) | `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (537 rows)`; `TU: ... identical (537 rows)` `RESULT PASS`; lock `(ido5.3s): score=0`; `PROMOTED 1 (1936 B) @ a1bbc86c`, OWNED-OFFSETS OK |
| inflate_entry / inflate_entry_alt | 360/132 | left as GLOBAL_ASM: hand-edited call (evidence below) | best C `LOCK: true=25 strict=25 reloc_blind=4 objdump-dr DIFFERS(3 rows) (122 rows)` (u64 argument) |
| 7 libultra rename leads | - | **RENAMED** d8d00877 (all 7 proven) | each ultralib object `RESULT: IDENTICAL (...)` (below); gate `GATE PASS`, built ROM SHA-1 3f99351d... = baserom, `OWNED-OFFSETS: OK 19 ROM TU data sections ...` |
| func_80012D18 | 3148 | not attempted (no lever: no macro/do-while blocks in the draft; w26b's 7-missing-instruction residual stands) | w26b `LOCK: true=12735 strict=13017 ...` unchanged |

## func_80018448 (492 B, IDO 7.1 -O2): MATCH, promoted

Source `sources/func_80018448.c` (= `func_80018448/best.c`), written in the locked sibling func_8001824C's style
(its `SequenceTrackNotes` view and `TRACK(i)` macro; `--prefix-dedup` drops the repeated lines in the TU).

The lever: both delta reads take **short operands**:
`change = ((short)(first & 127) << 8) | (short)second;` (first part) and
`change = ((short)(first & 255) << 8) | (short)second;` (15-bit delta). w25f's device
`(u8)(first & 255) & 127` is gone.

How (about 1 h):
1. Ucode first (w26b's `uedit.py`/`urun.sh`, new `tools/uscan.py` = batch uedit+urun with 2 jobs, ~0.05 s/variant).
   Single `cvtl` insertions after every record of lines 40-70 of uopt's `r.O` (744 variants): a signed 16-bit
   `cvtl` after line 45's `lod second` was the only one that moved the first differing row (52 -> 62, 24 -> 18
   rows). Its sll/sra temps (deleted by as1: the byte is already zero-extended) are retail's 2 hidden pops before
   line 54, and their free order (t8, t7, t9) is exactly the `t8 t7 t9` order retail shows at line 67 - the thing
   w26b's greedy FIFO called "different free order".
2. Chains of narrowings on line 61's `first` + a 16-bit cvtl on its `second` (774 variants): `and 255` then a 16-bit
   cvtl on first, plus the cvtl on second, gave 0 rows (3 + 2 hidden pops, retail's free order everywhere after).
3. C: `(short)` casts on int-typed operands are what make cfe emit those cvtls. `(short)second` alone and
   `(short)(first & 255)` give them; `(short)first`, `(short)(first << 8)`, `(short)((first << 8) | second)`,
   `(short)(u8)first` do not (18 rows); `(short)(first & 127)` in the first part needs no other device.

Disclosure (also in the header): `first & 255` on an unsigned char is a no-op mask, load-bearing - the same
mask the locked sibling func_8001824C carries in its 15-bit read. No unused locals, pad arrays, implicit int or
false prototypes.

Proof (builder scratch, toolkit 4586d230, `tools/check.py` w16g replay):
```
LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (129 rows)
TU: true=0 strict=0 reloc_blind=0 objdump-dr identical (129 rows)
RESULT PASS
```
`context_check.py --compiler ido7.1` -> `func_80018448 ok` (`ctx71_O2.jsonl`).
`lock add ... --toolkit 4586d230...` -> `(ido7.1): score=0` / `locked ... (score0)` (commit f1eb254a).
```
CONVEYOR_BUILDER_REPO=~/rush2049/scratch/frontier/w27s/repo CONVEYOR_BUILDER_JOBS=2 \
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w27s/ctx71_O2.jsonl 0x17dc0 \
  --only func_80018448 --prefix-dedup rom/lib_17dc0
builder gate: OWNED-OFFSETS: OK 19 ROM TU data sections (16 owned slots), 23 named objects checked against retail addresses (18 in owned slots), 1 without a retail address
PROMOTED 1 (492 B) @ ef6833a7
```
No reloc_addrs, symbol, rom_tu.h or owned-data changes.

## inflate_dynamic (1,936 B, ido5.3s): MATCH, promoted

Source `sources/inflate_dynamic.c` (= `inflate_dynamic/best.c`), from w26a's best.c (344/484 register-equal,
`LOCK: true=1945 strict=2313 reloc_blind=213 objdump-dr DIFFERS(521 rows) (537 rows)` against the current target).

Traced, then one lever (about 1 h):
1. Oracle: `tools/dforce.sh` = 5.3 cfe, the wave-5 TRACED uopt (`~/rush2049/scratch/frontier/wtk/bin/uopt`, CDX_LOG;
   output byte-identical to stock here), stock ugen, then the ido5.3s assembly step. The colouring summary showed the
   `nl + nd` expression web (type 4, the value retail keeps in s8) with `tot=33` against a callee-saved cost
   `best=38.5`, so it was split; `n` itself is a memory variable in both (retail reloads it from 0x84 for the loop
   test). Forcing that web to c22 (s8) took the residual from 217 rows to 126 and fixed the frame.
2. The cost is blocks/4 (a `goto`-exit variant added one block: 38.75). 154 blocks -> the 16 `do { } while (0)`
   wrappers of NEEDBITS/DUMPBITS (8 uses each) cost 2 blocks apiece (NEEDBITS-only or DUMPBITS-only variants: 34.5).
   With **gzip's original brace macros** `{ while (k < (n)) {...} }` / `{ b >>= (n); k -= (n); }` the procedure has
   122 blocks, cost 30.5, the expression web is coloured s8 (after w0 took s7, as retail), and the td reload
   disappears too: 1 row left = the end-pointer relocation.
3. `&gInflateBorder[19]` (the zero-fill loop end) is retail's split symbol `gInflateCplens`:
   `reloc_split derive inflate_dynamic --candidate ...` proposed `rom:0x6BDC/0x6BE0 ... symbol:gInflateBorder
   addend:0x4C`; `reloc_split check`: `all 131 reloc_addrs entries encode their ROM words`; `make extract` changed only
   inflate_dynamic.s's two operands (commit 26d3bb6a); `refresh_split_targets.py inflate_dynamic`:
   `reloc_aware f82aa18a6528 -> reloc_aware 735d59c0892d` (Conveyor DB backup `scratchpad/w27s/conveyor.db.w27s.bak1`).

The macros are named NEEDBITS_BR/DUMPBITS_BR only because the TU already defines NEEDBITS/DUMPBITS (do-while form)
for the other slots (a different redefinition is cfe warning 581). No shaping devices, unused locals or pad arrays.

Proof: `LOCK: true=0 strict=0 reloc_blind=0 objdump-dr identical (537 rows)`, `TU: true=0 strict=0 reloc_blind=0
objdump-dr identical (537 rows)`, `RESULT PASS`; `context_check.py --compiler ido5.3s` -> `inflate_dynamic ok`
(`ctx53s.jsonl`); `lock add ... --toolkit 8bdf0cae...` -> `(ido5.3s): score=0`;
```
CONVEYOR_BUILDER_REPO=~/rush2049/scratch/frontier/w27s/repo CONVEYOR_BUILDER_JOBS=2 \
python3 cloud/work/boot_tail_promotion/promote_batch.py cloud/work/frontier/w27s/ctx53s.jsonl 0x5610 \
  --only inflate_dynamic --prefix-dedup rom/lib_5610
builder gate: OWNED-OFFSETS: OK 19 ROM TU data sections (16 owned slots), 23 named objects checked against retail addresses (18 in owned slots), 1 without a retail address
PROMOTED 1 (1936 B) @ a1bbc86c
```

## inflate_entry / inflate_entry_alt: the call was hand-edited in the .s (evidence), left as GLOBAL_ASM

Retail: `li a1,12000; jal sound_play_menu; addiu a0,zero,0` (sound_play_menu = the game heap allocator
`(Heap *heap, u32 size)`, NULL heap = default).
1. **`addiu a0,zero,0` is reachable from C** only as half of a 64-bit value: IDO 5.3 ugen writes `li $4, 0` for the high
   word of a 64-bit argument (the same encoding as __osInitialize_common's `__ll_mul(x, 3LL)` and osContInit's
   64-bit constants); 32-bit zero is always `move`. With `sound_play_menu(12000LL)` (or a `u64`/`s64` prototype) the
   result is `li a0,0; jal; li a1,12000`: `LOCK: true=25 strict=25 reloc_blind=4 objdump-dr DIFFERS(3 rows) (122 rows)`
   (inflate_entry, `inflate_entry/best_u64.c`; w26a's best 215). But ugen emits a 64-bit constant high word first in every
   spelling tried (12000LL/ULL, (u64)/(s64) casts, enum, sizeof, `+ 0ULL`, `<< 0`, a u64 local); only a non-constant
   u32->u64 conversion evaluates the low word first, and then it is a `move`, not `li 12000`. So retail's order (low
   word, then `li a0,0` in the delay slot) is not reachable.
2. **The rest of inflate_entry_alt has a ONE-argument call's register allocation.** With `sound_play_menu(12000)` (one
   s32 parameter) every other instruction matches retail: `src` moved to a3 (a0 is the call's argument in the same
   block) and `size` kept in its incoming a1 (`alt_onearg_probe.c`: the only differences are retail's `li a1,12000` +
   `li a0,0` instead of `li a0,12000`). Any two-word argument puts a1 in that block too, and then uopt cannot keep size in
   a1: every two-argument or 64-bit form homes it (`sw a1,36(sp); lw t8,36(sp)`, the 2 extra instructions w26a saw).
   inflate_entry with the one-argument call (`onearg_probe.c`) also differs only by the inserted `li a0,0` (and the
   branch offsets after it).
Conclusion: the object was compiled from a one-argument call and the .s edited afterwards (`li $4,12000` ->
`li $5,12000` + `li $4,0`), consistent with the TU's other hand-edited code (inflate_io_wait/lzss_decode non-ABI).
No C produces it; they stay GLOBAL_ASM.

## The 7 libultra rename leads (w22g/w23g leads in docs/SYMBOL_MISATTRIBUTION.md): all proven and applied

`rename/prove.sh` (builder): ultralib (decompals e24c836, `reference/repos/ultralib` of the main checkout) VERSION_J
`-D_FINALROM`, project IDO 5.3 (toolkit 796ae99a), `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul -Xcpluscomm` (the
holding TUs lib_8440/lib_e280/lib_a050/lib_f160 have no opt override and are ido5.3 in rom_compilers.json). Then w24t's
`compare.py OBJ VRAM` on the Pi (`rename/prove.log`, object sha256s included):
```
sptask.o       @0x80007840  RESULT: IDENTICAL (176 words, 54 relocated fields compared by value, 0 differing words)
sprawdma.o     @0x8000D680  RESULT: IDENTICAL (36 words, 2 relocated fields compared by value, 0 differing words)
controller.o   @0x80009450  RESULT: IDENTICAL (184 words, 38 relocated fields compared by value, 0 differing words)
siacs.o        @0x8000E560  RESULT: IDENTICAL (48 words, 19 relocated fields compared by value, 0 differing words)
pfsgetstatus.o @0x8000E620  RESULT: IDENTICAL (140 words, 18 relocated fields compared by value, 0 differing words)
```
| vram | old | new (ultralib J) | file |
|---|---|---|---|
| 0x80007840 | osViModeTableGet | _VirtualToPhysicalTask (static in ultralib) | io/sptask.c |
| 0x8000D680 | __osSpDma | __osSpRawStartDma | io/sprawdma.c |
| 0x800095AC | __osContGetStatus | __osContGetInitData | io/controller.c |
| 0x80009658 | __osContRamReset | __osPackRequestData | io/controller.c |
| 0x8000E560 | osSiInit | __osSiCreateAccessQueue | io/siacs.c |
| 0x8000E6F0 | __osContBuildRequest | __osPfsRequestOneChannel | io/pfsgetstatus.c |
| 0x8000E7B4 | __osContParseResponse | __osPfsGetOneChannelData | io/pfsgetstatus.c |

Applied in commit d8d00877 (table `rename/renames.json`, a pure permutation, no cycles; base 6b929277):
- `static_rename symbols` (7 definitions), `source` on src/rom/lib_{8440,a050,a330,e280,f160}.c + rom_tu.h (6 files),
  PROMOTED headers retitled by hand ("ultralib name since w27s; was X"), `lock` (`13 of 630 entries migrated`: 7 re-keyed,
  6 callers rehashed with the pure-rename proof), `owned ... --rebind-dir cloud/work/frontier/w27s/rebind`
  (`2 file pins refreshed`: timer_services + sdk_initialize context hashes, superseding w26a/rebind), then
  `owned_storage generate` (storage_overrides.mk now names w27s/rebind).
- asm/us/nonmatchings/rom: 7 files git-mv'd, names permuted in 13 files (these slots are all C; `make extract` does not
  rewrite them). asm/us/{blob,boot_tail}/symbols.json keys + SHA256SUMS and src/blob/blob.ld PROVIDEs permuted in place
  (no game source or asm references these names). ovl_a/ovl_b symbols.json not touched (Astra's; unreferenced there).
- Receipts that pin asm/us/blob/symbols.json re-pinned with the pure-rename proof (live = pinned 0b72f7be with the table
  applied): dot_nearest_point_donor_20261005 and dot_target_orientation_20261005 verification.json -> 89ee4b7a.
- `targets._typed_data_field_aliases` accepts `_VirtualToPhysicalTask` (and the old name) for the OSTask field family;
  test_typed_target_aliases.py follows. `tools/recomp/identity.json`: 7 entries map the new inventory names back to the
  pre-w27s names for N64Recomp (recomp output unchanged; giving N64Recomp the ultralib names is the recomp lane's call).
  `static_rename.RENAME_TABLES` += w27s table. docs/SYMBOL_MISATTRIBUTION.md section.
- Conveyor DB (`~/.conveyor/conveyor.db`): copy run first (`renamed 7 rows`, audit clean), then live
  `targets_rename rename cloud/work/frontier/w27s/rename/renames.json` -> `renamed 7 rows in /home/cburnes/.conveyor/conveyor.db
  (n64_target, function_status, matrix_entry, work_unit, promotion_record), 0 matrix_entry rows purged`; `audit` clean.
  Backups (sqlite backup API) in the session scratchpad `w27s/conveyor.db.w27s.bak{1,2,3}` (bak1 before the inflate_dynamic
  target refresh, bak3 immediately before the live rename, 17:13 CDT). **Until w27s merges, the DB and wave27's layout
  disagree on these 7 names.**
- Gate: `gate_only.py src/rom/lib_{8440,a050,a330,e280,f160}.c` in the lane gate repo -> `GATE-CLEAN: owned-slot inputs
  changed ...`, `OWNED-OFFSETS: OK 19 ROM TU data sections (16 owned slots), 23 named objects checked against retail
  addresses (18 in owned slots), 1 without a retail address`, `GATE PASS`; built ROM sha1 3f99351d7bb61656614bdb2aa1a90cfe55d1922c
  = baserom. `lock check`: `all 630 locked functions intact`.

## Integration notes (coordinator)

- Fast-forward wave27 to w27s (wave27 8a54d6d2 is an ancestor; merge was a no-op). Commits: f1eb254a/82ce6005 (lock +
  ctx func_80018448), ef6833a7 (PROMOTED func_80018448), 26d3bb6a (reloc_addrs + make extract), 2119732f (lock
  inflate_dynamic), a1bbc86c (PROMOTED inflate_dynamic), d8d00877 (renames), plus RESULTS commits. promote_batch's commits
  carry its own Opus trailer.
- **Builder gate repo needs** `cloud/work/frontier/w27s/rebind/*.json` (rom_owned_data.json / storage_overrides.mk now point
  there instead of w26a/rebind): rsync it to `~/rush2049/repo` before the next gate there. I did not touch ~/rush2049/repo;
  all gates ran in `watchman2:~/rush2049/scratch/frontier/w27s/repo` (seeded from ~/rush2049/repo without .git/cloud/build,
  its submodule .git dirs removed, deflate streams copied; CONVEYOR_BUILDER_REPO/JOBS=2). Scratch deleted at the end.
- Conveyor DB already carries: inflate_dynamic target refresh (`f82aa18a6528 -> 735d59c0892d`, needed for the
  reloc_addrs entries) and the 7 renames.
- No unit_overrides, no game/overlay changes, no data-symbol changes beyond the reloc_addrs naming; rom_tu.h changed only
  by the renames (no new declarations: OWNED-OFFSETS headroom unchanged).
- Worktree setup notes: `tools/decomp-permuter` was `git submodule update --init`ed in the worktree (reloc_split needs it);
  `tools/mips_to_c` untouched; baserom.us.z64, build/blob_layout.json, build/game_code.bin copied (ignored files).

## Tests (Pi, worktree HEAD after the merge)

`make test-fast JOBS=4`: conveyor `6 failed, 1583 passed, 793 skipped`; cloud `770 passed, 187 skipped`. The 6 are the
m2c tests (test_autodecomp_population x3, test_ido_syntax x3) because the worktree's `tools/mips_to_c` submodule is not
checked out (not to be touched). Re-run on a `git archive HEAD` copy with the main checkout's tools/mips_to_c:
`python3 -m pytest tests/conveyor/test_ido_syntax.py tests/conveyor/test_autodecomp_population.py` -> `28 passed`.
Not run: rocky REQUIRE_TOOLCHAIN replay (packet receipts re-pinned above are toolchain-only replays; w25a's re-pin of the
same two receipts was found that way).

## What generalises

- **The ucode-first scan finds hidden pops in minutes**: `tools/uscan.py` (batch uedit+urun, 2 jobs, ~0.05 s/variant)
  over single and chained `cvtl` insertions located func_80018448's three hidden-pop sites; a pop pair's FREE order is
  visible later as a permuted register order (t8 t7 t9), so a "different free order" can still be hidden pops.
- **`(short)` on int-typed byte operands is a code-free 7.1 pop lever**: cfe emits 16-bit narrowings, ugen allocates
  sll/sra (or and) temps, as1 deletes them because the byte is known zero-extended. `(short)u8var` alone does not; it
  needs an int-typed operand (`(short)second` after promotion, `(short)(first & 255)`).
- **`do { } while (0)` macros cost uopt 2 blocks each, and blocks/4 is the callee-saved cost** (5.3 too). A big
  function full of statement macros can lose a callee-saved colour for that reason alone: try the brace form `{ ... }`
  (gzip's original) before chasing priorities.
- **A 64-bit argument is the only C that gives `addiu aN,zero,0`** (`li` of a 64-bit constant's high word); a lone
  `li a0,0` after the other argument plus a ONE-argument register allocation elsewhere is the signature of a hand-edited
  .s call.
- Ultralib J compile proofs remain cheap: five files, one script, all leads closed.
