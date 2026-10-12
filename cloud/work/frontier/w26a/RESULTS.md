# w26a - inflate TU recipe (ido5.3s), 7 inflate slots promoted, gViTimeAccumLo retired (worktree lane)

Branch `w26a` (worktree `.claude/worktrees/w26a`, from wave26 46780d3e; wave26 merged in twice, last at
f3b09043 -> b454301d). Not pushed. Static numbers only (`make progress`), never merged with game coverage.

**Static 618/667, 142,436 B (wave26 after w26b) -> 625/667, 147,280 B (91.64%): +7 functions, +4,844 B.**
(Master at lane start: 617/667, 139,624 B.)

## 1. The inflate TU recipe (compiler `ido5.3s`)

lib_5610 (0x80004A10..0x800069FC) is IDO 5.3 `cc -S -O2` output, assembled at as1 -O1 - with two details
w25a did not have:

1. **No `.loc` lines.** as1 treats every `.loc` as a scheduling barrier. With them, prologue saves and the
   first global stores keep source order (w25a's inflate_loop 21/23, inflate_block 99/105 residual); with
   all `\t.loc\t` lines deleted, as1 -O1 interleaves them exactly as retail (found by stripping directive
   classes one at a time from inflate_loop's .s: only `.loc` matters; `.livereg`/`.mask`/`.frame` do not).
2. **`-g` on the assembly step.** Without it as1 -O1 moves the `sltiu at` of a macro branch (`bgeu r,imm`)
   into the delay slot of the branch right before it (huft_build 0x53C, inflate_dynamic 0x3FC); retail leaves
   a nop. `-g`, `-g1`, `-g2` all give the retail schedule and change nothing else in the matched bodies.
   (Neither 5.3 nor 7.1 as1 without -g, nor -mips1/-mips3/-KPIC/-Wb,-nopeep, reproduce it.)

Driver: `tools/ido_sas/cc` (stdlib Python, stands in for `cc`):
```
cc -S <flags> FILE.c                 # real IDO 5.3, same flags; run in a temp dir (cc -S ignores -o)
drop "\t.loc\t", "\t.option\tO<n>", " #" comment lines
cc -c <flags, -O<n> -> -O1, -g<n> -> -g> -o OUT FILE.s
```
Wiring (all committed, tests in tests/conveyor/test_compiler_pins.py + test_boot_tail_ido71_pins.py):
- `tools/conveyor/compilers.py`: compiler `ido5.3s`, a *driver wrapper* (`wraps: ido5.3`, `wrapper_sha256` =
  sha of tools/ido_sas/cc, `wrapped_dirs` ido53 / ../ido-static-recomp/build/out / ../cloud/ido).
  identify_dir: wrapper sha + wrapped dir identifies as ido5.3; identify_toolkit: `ido/cc` = wrapper,
  `ido/ido53/` = the 5.3 build. `install ido5.3s` refuses (in-repo). Editing the driver changes its sha:
  re-pin and re-prove.
- Makefile `IDO53S_CC := $(TOOLS_DIR)/ido_sas/cc`; `rom_compilers.json` pins `0x5610: ido5.3s`;
  `src/rom/opt_overrides.mk` regenerated (`lib_5610.o: CC := $(IDO53S_CC)`).
- `tools/asm-processor/asm_processor.py`: IDO `as` assembling a .s lists every compiler label (`$32`...) as an
  unused LOCAL UNDEF symbol; asm-processor refused ("local symbol "$32" is undefined"). Such `$<digits>`
  symbols that no relocation uses are now dropped (anything else still fails).
- Toolkit for `lock add --toolkit`: `8bdf0caebc9ada9612b9dae9422948e37bd119e663e9dda8bb8fdbb7acd96682`
  (= pinned 5.3 toolkit d4c39cbc with `ido/` = driver + `ido/ido53/`; built with `build_toolkit --swap-ido-from`,
  `publish-toolkit --no-pin`, identifies as ido5.3s). `4eb51387...` was the pre-`-g` driver (superseded,
  no longer identifies).
- docs/COMPILER_SETTINGS.md "IDO 5.3 -S recipe (inflate TU)".
- The two slots promoted earlier at standard -O2 had to follow: huft_alloc returns `(new_var - arg0) +
  gDisplayListHead` directly (as1 -O1 keeps the `move v0,v1` of a returned named local that as1 -O2 folded);
  inflate_flush_window unchanged, later `gDisplayListSize = 0` instead of the `*(s32 *)&` cast (see 4).

Whole-TU proof (asm-processor + driver, w24t compare.py, relocations by value):
`RESULT: IDENTICAL (2044 words, 268 relocated fields compared by value, 0 differing words)` - before any new
promotion, again with every promotion below, and after the rom_tu.h change.

## Per-slot table (lib_5610, 14 slots)

| slot | B | state | evidence |
|---|---:|---|---|
| inflate_io_wait | 236 | asm (hand-written, non-ABI) | w25a |
| lzss_decode | 596 | asm (hand-written, non-ABI) | w25a |
| inflate_flush_window | 28 | locked earlier; rebound to ido5.3s | TU IDENTICAL, gate |
| huft_alloc | 44 | locked earlier; body re-shaped, rebound | TU IDENTICAL, gate |
| huft_build | 1740 | **PROMOTED** 3c9774d2 | lock score=0 (8bdf0cae); C23 draft unchanged |
| inflate_free_window (gzip inflate_codes) | 1272 | **PROMOTED** 3a25e54c | lock score=0; new reconstruction; 1 disclosed unused local |
| inflate_stored | 544 | **PROMOTED** 3c9774d2 | lock score=0 |
| inflate_fixed | 456 | **PROMOTED** 3c9774d2 | lock score=0 |
| inflate_dynamic | 1936 | open: 344/484 insns equal (register-aware) | best `inflate_dynamic/best.c` |
| inflate_block | 420 | **PROMOTED** 8dfc3680 | lock score=0 (4eb51387) |
| inflate_loop | 92 | **PROMOTED** 8dfc3680 | lock score=0 |
| inflate_read_bits | 320 | **PROMOTED** 8dfc3680 | lock score=0 (after target refresh, below) |
| inflate_entry | 360 | open: 2 words | best `inflate_entry/best.c` |
| inflate_entry_alt | 132 | open: 2 extra insns | best `inflate_entry/alt_best.c` |

Gate lines (promote_batch, builder gate repo `watchman2:~/rush2049/scratch/frontier/w26a/repo`):
```
PROMOTED 3 (832 B) @ 8dfc3680      builder gate: OWNED-OFFSETS: OK 19 ROM TU data sections (16 owned slots), 23 named objects ...
PROMOTED 3 (2740 B) @ 3c9774d2     builder gate: OWNED-OFFSETS: OK 19 ROM TU data sections ...
PROMOTED 1 (1272 B) @ 3a25e54c     builder gate: OWNED-OFFSETS: OK 19 ROM TU data sections ...
```
Lock lines: `... (ido5.3s): score=0` / `locked ... (score0)` for all 7 (verify jobs on the pool). Context rows
(`context_check.py --compiler ido5.3s`): `ctx53s.jsonl`, `ctx53s_b.jsonl`, `ctx53s_c.jsonl`, all `ok`.
Sources: `sources/*.c` (headers carry semantics, gzip ancestry, devices).

Source notes:
- One NEEDBITS text for the whole TU, as a conditional expression:
  `b |= (u32)(InPtr < InEnd ? (InPtr += 2, InPtr[-2] | (InPtr[-1] << 8)) : inflate_read_bits()) << k`.
  The word is `low | high << 8` (the other operand order swaps the two lbu registers); a named block-scope
  `word` compiles identically in inflate_block but gives inflate_stored's `n--` temporary v0 instead of v1.
- inflate_stored: `DUMPBITS(k & 7)` (retail computes the alignment into a temporary, not into n).
- inflate_read_bits: no `volatile`, no ptr/high locals (the 8-byte frame residual), toggle store before
  `next` in the else arm.
- inflate_free_window = gzip inflate_codes rebuilt from gzip's text; copies use memcpy when the distance is
  more than 8 back, else a byte loop. **Unused local `w`** (gzip's window position, declared where gzip has it):
  exact frame residual, retail frame 0x60, 0x58 without it, one slot (+4 alignment) moving ml/md 0x40/0x44 ->
  0x44/0x48. Disclosed in the header (wave-12 rule).

### Open slots (stop rule reached)
- **inflate_entry / inflate_entry_alt**: retail evaluates `sound_play_menu(0, 12000)` as `li a1,12000; jal;
  addiu a0,zero,0` - a constant 0 materialised with `li` after the second argument. IDO's ugen emits
  `move a0,$0` (before a1) for every spelling tried (0, (void*)0, 0u, 0L, s16/u8/f32/void* prototypes,
  unprototyped, a zero-initialised local). Best hypothesis: the .s was hand-edited here (a one-argument
  allocator call turned into a two-argument one: `li $4,N` -> `li $5,N` + `li $4,0`), consistent with the
  file's other hand-edited code. inflate_entry is otherwise exact (2 words; locals in OSMesg-then-window
  order); inflate_entry_alt additionally homes `size` instead of keeping it in a1.
- **inflate_dynamic**: structurally aligned (484/484 insns, 344 register-equal). Retail keeps n = nl + nd in
  s8 (saved at 0x48, home 0x84) and does not reload td after storing it; ours spills n (0x48/0x7C) and the
  stack homes sit 8 B lower. Unused locals at every declaration position change nothing. Next: trace why
  uopt gives n no callee-saved register (ctrace/force on the static compile), try gzip's exact `ll[]`
  replacement shapes.

## 2. gViTimeAccumLo retired (b3caca14)
reloc_addrs.us.txt names osGetTime (rom 0x7FDC/0x7FE0) and __osTimerServicesInit (0xCC90/0xCC9C) as
`%hi/%lo(__osCurrentTime + 0x4)` (`reloc_split check: all 129 reloc_addrs entries encode their ROM words`);
symbol_addrs line removed; `make extract` (undefined_syms_auto: the slot is splat's anonymous `D_80037C54`);
the two leftover nonmatchings .s rewritten to the same operands (both functions are C, ROM unchanged);
test_dll_init_alias.py updated (the fold still serves pre-w26a text). Not touched: the name in
asm/us/{blob,ovl_a,ovl_b,boot_tail}/symbols.json and src/blob/blob.ld PROVIDE (unreferenced; ovl_* are
Astra's). Gate: GATE-CLEAN full rebuild, `ROM matches!`, OWNED-OFFSETS OK.

## 3. Rename leads - not attempted (time)
Still open (docs/SYMBOL_MISATTRIBUTION.md): osViModeTableGet, __osSpDma, __osContRamReset, __osContGetStatus,
osSiInit, __osContBuildRequest, __osContParseResponse; each needs the w24t/w25a ultralib compile proof.

## 4. rom_tu.h
No declarations added (headroom stays 6). `gDisplayListSize` lost its `volatile` (an -O2 shaping device for
huft_alloc's reload; under ido5.3s the reload is natural, and the inflate bodies need the plain store).
Owned-storage context pins rebound with `static_rename owned w26a/rebind/empty_renames.json --base HEAD
--rebind-dir cloud/work/frontier/w26a/rebind` (twice: rom_tu.h, then symbol_addrs).

## Integration notes (coordinator)
- Fast-forward wave26 to w26a (wave26 f3b09043 already merged; storage_overrides.mk conflict resolved by
  regeneration). rom_owned_data.json context proofs now point at `cloud/work/frontier/w26a/rebind/*` (supersedes
  w26b's sdk_initialize pin): **rsync cloud/work/frontier/w26a/rebind to any builder gate repo.**
- **Builder `~/rush2049/repo` needs**: tools/ido_sas/cc (the Makefile runs it for lib_5610; it finds
  tools/ido-static-recomp/build/out/cc), tools/conveyor/compilers.py, tools/asm-processor/asm_processor.py
  (the label-symbol fix), rom_compilers.json, src/rom/opt_overrides.mk. I did not touch ~/rush2049/repo.
- Conveyor DB: `refresh_split_targets.py` on the 10 inflate targets (2 superseded: inflate_read_bits had the
  pre-rename `__osPiRawStartDma` callee name, inflate_entry's object changed too; backup `scratchpad/w26a/conveyor.db.w26a.bak1`);
  promote_batch wrote promotion_record rows. Two unpinned toolkits uploaded (4eb51387 obsolete, 8bdf0cae current).
- Pi worktree `make test-fast JOBS=4`: conveyor `6 failed, 1583 passed, 793 skipped`; cloud `770 passed,
  187 skipped`. The 6 are the m2c tests (test_autodecomp_population x1, seed/macro x2, m2c x3): the worktree's
  `tools/mips_to_c` submodule is not checked out (told not to touch it); the same 6 pass in the main checkout
  (`6 passed`). (`tools/decomp-permuter` was `git submodule update --init`ed in the worktree for the rest.)
- What generalises: a static TU whose code shows only fill-from-above delay slots and kept reloads is a
  `cc -S` + as1 -O1 build; check `.loc` stripping and `-g` before suspecting a different assembler. A driver
  wrapper is now a first-class pinned compiler (compilers.py `wraps`), usable for any future per-TU recipe.
