# Frontier wave 2, agent w2g — results (2026-10-04)

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w2g`. Flags everywhere: `-g0 -O3 -mips2 -G 0 -non_shared`.
Nothing was committed, spliced or locked. Only what the scorer printed is quoted.

| # | Function | Bytes | State | Where |
|---|---|---:|---|---|
| 1 | `traction_control` | 992 | **MATCH** in a real `-O3` group | `groups/traction_control/` |
| 2 | `NextMaxPath` | 520 | **MATCH** in a real `-O3` group | `groups/NextMaxPath/` |
| 3 | `func_8008A46C` | 472 | **MATCH** in a real `-O3` group | `groups/func_8008A46C/` |
| 4 | `func_80106D94` | 1,604 | code identical, own-rodata unverified (4 float literals) | `func_80106D94/best.c` |
| 5 | `sound_bank_load` | 364 | **MATCH** (single; also at `-O2`) | `cloud/matches/sound_bank_load.c` |
| 6 | `func_800F0674` | 472 | code identical, own-`.bss` unverified (3 local statics) — not spliceable today | `func_800F0674/best.c` |
| 7 | `dma_wait_complete` | 420 | **MATCH** (single file with real neighbours for `.align 5`) | `cloud/matches/dma_wait_complete.c` |

Strict: 5 functions, 2,768 bytes. Code-identical: 2 functions, 2,076 bytes.
`python3 -m tools.conveyor.pipeline.blob_unit --tag w2g score NAME --with FILE` reports EQUAL for 1–6
(`func_800F0674`: "own zero-initialised data (addresses consistent, nothing to compare)"); 7 is two `nop`s short
in the shadow unit for the documented alignment reason (see 7).

Scorer command for singles (run in the builder copy):
`IDO_DIR=…/ido python3 tools/cloud/score.py fn cand/NAME.c NAME --flags "-g0 -O3 -mips2 -G 0 -non_shared"`
and for groups `… python3 tools/cloud/score.py group cand/<groupdir>`; wrappers `sc.sh`, `grp.sh` here.

---

## 1. traction_control (0x800ACC18) — MATCH, group

`./grp.sh groups/traction_control` →
```
Members:
vector_diff_process:
  MATCH
traction_control:
  MATCH
Context (informational; excluded from exit status):
steering_sensitivity:
  230/232 words differ (19 extra words (nonzero beyond target length))
func_800A61B0:
  MATCH
math_utility:
  MATCH
```
`blob_unit score traction_control vector_diff_process --with groups/traction_control/group.c` → both EQUAL.

- The group is `src/blob/groups/codex_steering_a12/group.c` with `traction_control` rewritten. It **supersedes**
  that locked group (revert it first): `vector_diff_process`'s parameter order becomes
  `(origin, basis, position, out)` — the two call sites in `traction_control` set up `t0, a2, a3, a1` in that
  order and only that order reproduces them (lesson 5 of wave 1 again). Its own 20 words do not change.
  `steering_sensitivity` (the real third caller, layer 2, blocked on `func_800ACFF8`/`func_800AD090`) stays
  unmatched context exactly as in the locked group; I only adapted its call and the typed global.
- Semantics (N64-only, no arcade ancestor found): map a world position onto path segment `idx`; see the header
  comment in `group.c`. The previous attempt (A12, 240/248) was m2c-shaped.
- What closed it, in order: natural typed source (first draft 48/248, same length and frame) → parameter order
  of the IPA helper → `pos + right * k` operand order in the two offset loops (6 words left) → the cross product
  in the ordinary `a[1]*b[2] - a[2]*b[1]` spelling (0). IDO does **not** keep my source operand order for
  `fwd[1]*right[2] - right[1]*fwd[2]`; a 64-way sweep of the six products found the standard spelling.
- Plain `for (i = 0; i < 3; i++)` loops produce retail's pointer loops with `sltu` bounds and the rotated
  second/third loops as they are. No quirks, no unused locals.
- Recovered layout — `PathRec` (0x84 bytes, array pointer `D_80152034`, count `*D_801526F0` as `u16`):
  `f32 pos[3]` 0x00, `f32 uvs[3][3]` 0x0C, `f32 right[3]` 0x30, `f32 fwdL[3]` 0x3C, `f32 fwdR[3]` 0x48,
  `f32 halfWidth` 0x54, `len0` 0x58, `skew0` 0x5C, `skew1` 0x60, `wL0/wL1` 0x64/0x68, `wR0/wR1` 0x6C/0x70,
  `hL` 0x74, `hR` 0x78, 8 bytes unknown. Field names are mine (by use), not arcade names.

## 2. NextMaxPath (0x800A0FDC) — MATCH, group

`./grp.sh groups/NextMaxPath` →
```
Members:
audio_reverb_update: MATCH   audio_effect_process: MATCH   synced_model_render: MATCH
MP_TargetSpeed: MATCH        assign_default_paths: MATCH   stat_race_end: MATCH
NextMaxPath:
  MATCH
Context: func_80095F8C MATCH, func_80095EF4 MATCH, audio_buffer_sync MATCH,
         object_counter_decrement MATCH, object_counter_increment MATCH
```
(condensed; every line printed `MATCH`). `blob_unit score NextMaxPath --with groups/NextMaxPath/alloc_at.c` → EQUAL.

- The name is wrong: no `maxpath.c` constants. It is the game heap's **allocate-at-address**
  (`alloc_at(addr, size)`, same block allocator as `src/blob/groups/audio_heap`), called only by the overlay
  loader `PrevMaxPath`.
- Group = `src/blob/groups/codex_heap_release_a25/group.c` **unchanged** + `alloc_at.c`. All real, no stand-ins.
  Integrate either as a superseding group or by adding `alloc_at.c`/`NextMaxPath` to that group.
- Classification that mattered: the frontier's `group`/`ring` label is right but the function is **kept**
  (plain `a0`/`a1`). The four-wide `t6`–`t9` ring and the `t0`/`t2` webs that survive `func_80095F8C` come from
  the *callee* being internal with a known register summary, not from this function being internal:
  on the same first draft, kept callee → temps `t3`–`t9` and late reloads (64 aligned rows off), internal
  callee → ring and reloads exact (39 rows off, the rest being the frame and the `prev` web).
  (Making `NextMaxPath` itself internal also gives the ring, but then IPA passes its arguments in `s0`/`s2`.)
- Shaping (in the header): the empty `if (block end < addr + size) { }` is kept by IDO as retail's
  compare-and-branch with identical arms; `prev` assigned twice; `result` must be the 11th local for the
  72-byte frame — five `unusedN` locals stand for declarations I could not recover (inlined-helper locals are
  the likely source; static no-argument lock wrappers do **not** add frame, tested).
- Prior attempt (A155) was 111/130 with a 56-byte frame.

### Side result: the overlay loader cluster (`PrevMaxPath`, `InitMaxPath`, `sync_maxpath_to_checkpoint`) — lead, not claimed
`PrevMaxPath/lead.c` (single file, `--keep InitMaxPath,sync_maxpath_to_checkpoint,display_enable,MP_TargetSpeed,assign_default_paths`):
`PrevMaxPath` 4/24, `InitMaxPath` 8/34, `sync_maxpath_to_checkpoint` 8/34 (aligned rows; each caller has one own
string literal). All remaining rows are one permutation of the callee's parameter registers
(retail `s1` bssEnd, `s2` bssStart, `s3` loaded flag, `s4` ROM pointer; mine `s4, s2, s1, s3`). Findings:
- `PrevMaxPath` is `load_overlay(dst, name, end, bssEnd, romStart, bssStart, &loaded, rom)`: 8 parameters.
  The dead stores `sw …,4(sp)` / `sw …,16(sp)` in every caller are `name` ("Extra"/"Start") and `romStart`.
- **A parameter whose address is taken is homed in memory, the caller stores it straight into the home slot,
  and umerge will not inline the callee.** `if (&name == 0) return;` (code-free) reproduces both the
  `sw t6,4(sp)` and the fact that retail calls this 5-statement function by `jal` from three sites. Without it
  umerge inlines it everywhere (M3's "tiny" rule is not what protects it). This is a general lever for the
  "retail did not inline X" cases (M4) and a better-founded one than a dead `if (0)` block.
- `bssStart`, `bssEnd`, `romStart` and (in `MP_TargetSpeed`) the image base are **symbol addresses**
  (`lui; addiu`), `dst`/`end` are integer literals (`lui; ori`): write `extern u8 D_80394F70[]`,
  `D_00BE4C70[]` (the scorer resolves `D_xxxxxxxx` by pattern, ROM addresses included).
- INFERRED: `display_enable` is `MP_TargetSpeed(); sync_maxpath_to_checkpoint(); …` / `assign_default_paths();`
  with all three inlined (retail's copy uses `sync…`'s string address `0x80123850` and repeats the two unload
  bodies). In `lead.c` only `sync…` gets inlined (79 words wanted, 53 compiled), so the two unload functions
  are not tiny enough as written; `display_enable` also needs its parent for the unsaved `s0`–`s4`.
- The four-register permutation is body-driven (24 parameter orders × 3 body forms inert, as A14 found);
  moving `bzero` before `inflate` or adding a use moves it, so it is colouring priority. Not resolved; ~80 variants.

## 3. func_8008A46C (0x8008A46C) — MATCH, group

`./grp.sh groups/func_8008A46C` →
```
Members:
func_80086A50:
  MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0xc, .rodata+0x0 at +0x14)
func_8008705C:
  MATCH
func_800878E0:
  MATCH
func_8008A148:
  MATCH
func_8008A46C:
  MATCH
```
`blob_unit score func_8008A46C --with groups/func_8008A46C/rect.c` → EQUAL. First compile.

- Clipped solid rectangle: scissor clamp, `gDPPipeSync`, `gDPSetPrimColor`, state calls,
  `gDPFillRectangle(x0, y0, x1 + 1, y1 + 1)`, `gDPPipeSync`. SDK GBI macros only, no quirks.
- Group = `gfx_modes` (`func_80086A50.c`, `modes.c`) + `src/blob/func_8008A148.c`, all unchanged, + `rect.c`.
  `func_80086A50` must be internal: that is why retail leaves `t2`–`t5` live across its call and saves them
  only around the kept `func_8008A148`/`func_800878E0`. Its jump-table line above is the pre-existing one from
  `gfx_modes`; it is not claimed here. Prior attempt (B119) was 74/118 with hand-built display-list words.

## 4. func_80106D94 (0x80106D94) — code identical, own-rodata unverified

`./sc.sh func_80106D94/best.c func_80106D94 --flags '"-g0 -O3 -mips2 -G 0 -non_shared"'` →
```
func_80106D94:
  MATCH (8 section-relative relocations unverified: .rodata+0x0 at +0x3c, .rodata+0x0 at +0x54, .rodata+0x4 at +0x238, .rodata+0x4 at +0x254, .rodata+0x8 at +0x298, .rodata+0x8 at +0x2a4, .rodata+0xc at +0x3b4, .rodata+0xc at +0x3d0)
```
Same at `-O2`. `blob_unit score` → EQUAL. Literals checked by hand against `build/game_code.bin`:
`0x801248B4 3F19999A` 0.6f, `…B8 4900E800` 528000.0f, `…BC 474E4000` 52800.0f, `…C0 474E4000` 52800.0f.

- HUD odometer digit `Blit` AnimFunc (N64-only). N64 `Blit` offsets recovered: `X` 0x0E, `Y` 0x10, `Width` 0x14,
  `Hide` 0x1A, `Top/Bot/Left/Right` 0x1C/0x1E/0x20/0x22, `AnimDTA` 0x28, `AnimID` 0x2C (the function labelled
  `Input_ApplyPadConfig` is the arcade `UpdateBlit`). Car `+0x108` f32 distance, `+0xEF` s8; `D_8014A250[i]+0x7C6`
  s16 car index; `D_80115AE8[][4]` `{s32 x, y}`.
- First draft was 78 aligned rows off at the right length. Closers: repeated `itenths % 10 == 9` instead of a
  local (branch operand order), `hide = blt->Hide; if (hide)`, declaration order for the 64-byte frame.
- The last word (case 3 one instruction long) took ~95 C variants and was only named by working on the
  **ugen listing**: reassembling 1,914 orderings of the case-3 block with `as0`/`as1` (`asmt.sh`,
  `asmbatch_remote.sh`) showed retail needs `slot*8` emitted *before* the `Width / 2` divide. That means there is
  no half-width local: every use is `blt->Width / 2` and uopt's PRE supplies the lone divide on the default
  path. Rewriting with a `HW` macro matched at once.
- Three of the ten locals are unused (`unused0..2`): the original declaration list is unknown.

## 5. sound_bank_load (0x800B0F68) — MATCH

`./sc.sh ../../../matches/sound_bank_load.c sound_bank_load --flags '"-g0 -O3 -mips2 -G 0 -non_shared"'` →
```
sound_bank_load:
  MATCH
```
Also `MATCH` at `-O2`; `blob_unit score` → EQUAL. Second compile.
Name lookup over per-bank sorted directories (`D_80138670[bank] = {base, count}`, 24-byte records, bsearch =
`entity_name_copy`, comparator `func_8009508C`, strcpy = `func_800A473C`); handle = `index | bank << 10`.
Quirk: `volatile u8 D_80140BDC` for the address-form reads (27 rows without it). The stub neighbour
`func_800B0F60` is not involved.

## 6. func_800F0674 (0x800F0674) — code identical, own-.bss unverified

`./sc.sh func_800F0674/best.c func_800F0674 --flags '"-g0 -O3 -mips2 -G 0 -non_shared"'` →
```
func_800F0674:
  MATCH (18 section-relative relocations unverified: .bss+0x0 at +0x10, .bss+0x0 at +0x14, .bss+0x8 at +0x114, .bss+0x8 at +0x11c, .bss+0x8 at +0x110, .bss+0x8 at +0x124, .bss+0x8 at +0x130, .bss+0x8 at +0x134, .bss+0x0 at +0x138, .bss+0x0 at +0x144, .bss+0x4 at +0x13c, .bss+0x4 at +0x140, .bss+0x4 at +0x18c, .bss+0x4 at +0x190, .bss+0x4 at +0x1b4, .bss+0x4 at +0x1b8, .bss+0x0 at +0x19c, .bss+0x0 at +0x1c0)
```
Same at `-O2`. `blob_unit score` → EQUAL ("own zero-initialised data (addresses consistent, nothing to compare)").

- **Arcade ancestor proven by behaviour:** `fixword()` in `reference/repos/rushtherock/game/hiscore.c:1623`
  (high-score name censor). Transcribed with three N64 changes read off the code: unsigned chars, the
  player's word skips everything outside `A-Z`/`0-9`, `strictcnt` is constant 0. Second compile.
- The arcade's `static char *op1,*sp; static S16 cnt;` is required: as `extern` globals (or globals defined in
  the file, single or group) the body is 110/118 off because `*sp++ = '!'` may alias them.
- Retail addresses of the statics: `op1 0x80156940`, `sp 0x80156948`, `cnt 0x80156950` (the object has them at
  `.bss` +0/+4/+8, one-to-one). **Blocked on `.bss` ownership** (`blob_splice`/`blob_group` refuse `.bss`); this
  is a concrete first customer for it. Its caller is presumably the arcade `checkword()` (word table).

## 7. dma_wait_complete (0x8008AA40) — MATCH

`./sc.sh ../../../matches/dma_wait_complete.c dma_wait_complete --flags '"-g0 -O3 -mips2 -G 0 -non_shared"'` →
```
dma_wait_complete:
  MATCH
```
From the same file: `audio_queue_process` MATCH, `func_8008A704` MATCH, `controller_rumble_thunk` MATCH
(`sync_release_video` prints `MISMATCH (2 extra words (nonzero beyond target length))` from this file:
context only, not claimed; I did not re-score it from `cloud/matches/audio_queue_process.c`).

- Scheduler-client thread entry (retrace → optional callback + wrapping accumulator; pre-NMI → thunk).
- `controller_rumble_thunk` (the 8-word wrapper in front of it) is **inlined** for `case 4`; a direct call to
  `func_800205E4` leaves the frame 8 bytes short. Classic "matched neighbour with no `jal` caller".
- The epilogue after the endless loop is `.align 5` (third instance after `audio_queue_process` and
  `task_complete_signal`). To get a strict single-file score the real preceding functions are in the file so the
  body sits 0x10 past a 32-byte boundary. **For integration it needs an `align` entry in
  `src/blob/unit_overrides.json`**: `blob_unit score dma_wait_complete --with …` prints
  `FAIL dma_wait_complete: 13 of 105 words differ; compiled body is 103 words, target 105` (two `nop`s).
- Quirks: `D_8011EAA8(D_8011EAA8)` to get the callback pointer coloured `a0` (INFERRED, flagged in the header);
  `void *volatile` scheduler field +0x274. ~12 variants.

---

## What generalises

1. **Who is internal matters more than whether the function is.** Two of the three `group` functions here are
   *kept* functions whose whole-program signature (`t6`–`t9` ring, values left in `t0`–`t5` across a call) comes
   from an **internal callee**. Recipe: take the locked group that already contains the callee with its real
   callers, add the new function as one more kept member/file. The frontier's `unit:` line listed only the
   function itself for both; "callees that are internal in a locked group" would be the useful hint.
2. **Address-taken parameter ⇒ memory-homed, stored by the caller, callee not inlined** (see the loader side
   result). Explains stray `sw reg,N(sp)` into the outgoing argument area before a `jal` to an IPA callee.
3. **`lui; addiu` vs `lui; ori` on a constant** distinguishes a linker symbol from an integer literal — also for
   ROM offsets (`D_00BE4C70`).
4. **A missing/extra instruction that survives every C variant: go to the ugen listing and permute it.**
   `asmt.sh` + `asmbatch_remote.sh` reassemble edited listings in ~0.3 s each; the set of orderings that
   reproduce retail names the source property (here "no local for `Width/2`"). `.noalias` lines must move with
   the instruction they follow or `as1` schedules differently.
5. **A value recomputed on a lone `default`/else path is PRE**, so the expression is spelled out at each use
   rather than held in a local.
6. **Function-local statics** are recognisable by globals kept in registers across byte stores with explicit
   store-backs; `extern` cannot reproduce them. They need `.bss` ownership to land.
7. IDO canonicalises operand order inside some FP products; sweep the spellings (64 here) instead of trusting
   the retail operand order literally.
8. `rm` with a glob after `cd` is blocked by the harness: write variant batches to fresh directories.

## Tools added here (all thin wrappers over w1b/w1f)
`sc.sh`, `full.sh`, `bq.sh`, `batch.sh`, `grp.sh`, `dump.sh`, `o3s.sh` as before; new:
`gfull.sh`/`gfull.py` (aligned diff of one function compiled as a multi-file group dir),
`syms.sh`/`syms.py` (function offsets in the object, for `.align 5` cases),
`uv.sh`/`o3v_remote.sh` (`umerge -v` inlining log), `asmt.sh`/`asm_remote.sh`/`asmbatch_remote.sh`
(reassemble edited ugen listings, single and batch).
