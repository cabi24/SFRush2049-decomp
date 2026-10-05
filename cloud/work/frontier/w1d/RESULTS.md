# Wave 1, agent w1d - results (2026-10-04)

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w1d`. Scorer commands below are run there as
`IDO_DIR=$HOME/rush2049/cache/toolkits/796ae9…/ido python3 tools/cloud/score.py …` (wrappers `sc.sh`, `grp.sh` here).
Nothing was committed, spliced or locked.

| # | Function | Bytes | State | Flags | Deliverable |
|---|---|---:|---|---|---|
| 1 | `func_800F1D04` | 888 | **strict MATCH** (single) | `-O3` (also `-O2`) | `cloud/matches/func_800F1D04.c` |
| 2 | `func_800E6460` | 956 | **provisional**: strict MATCH as member of the real group; only caller 16 words off | `-O3` group | `groups/control_input_E681C/` |
| 3 | `func_800E3430` | 756 | **code identical, own rodata unverified** (real group with its caller) | `-O3` group | `groups/sym_controls/` |
| 4 | `func_800C0294` | 568 | **strict MATCH** (single) | `-O3` only | `cloud/matches/func_800C0294.c` |
| 5 | `menu_vibration_test` | 408 | **strict MATCH** (single) | `-O3` (also `-O2`) | `cloud/matches/menu_vibration_test.c` |
| 6 | `func_800AC8D4` | 232 | **strict MATCH** (single) | `-O3` (also `-O2`) | `cloud/matches/func_800AC8D4.c` |
| 7 | `func_800BF780` | 184 | **strict MATCH** (single) | `-O3` (also `-O2`) | `cloud/matches/func_800BF780.c` |

Not assigned, but needed as real partners:

| Function | Bytes | State | Deliverable |
|---|---:|---|---|
| `func_800E3724` (caller of #3) | 600 | code identical, own rodata unverified; strict MATCH if its two literals are spelled as externs | `groups/sym_controls/`, `groups/sym_controls_extern_rpm/` |
| `func_800E681C` (caller of #2) | 716 | 16/179 words, one register permutation | `groups/control_input_E681C/`, `func_800E681C/NOTES.md` |

Strict singles: 5 functions, 2,280 bytes.

## Scorer evidence

```
score.py fn cand/func_800F1D04.c func_800F1D04 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800F1D04:
  MATCH
score.py fn cand/func_800C0294.c func_800C0294 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800C0294:
  MATCH
score.py fn cand/menu_vibration_test.c menu_vibration_test --flags "-g0 -O3 -mips2 -G 0 -non_shared"
menu_vibration_test:
  MATCH
score.py fn cand/func_800AC8D4.c func_800AC8D4 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800AC8D4:
  MATCH
score.py fn cand/func_800BF780.c func_800BF780 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800BF780:
  MATCH

score.py group cand/<groups/sym_controls>
Members:
func_800E3430:
  MATCH (8 section-relative relocations unverified: .rodata+0x0 at +0xb0, .rodata+0x0 at +0xb4, .rodata+0x4 at +0xdc, .rodata+0x4 at +0xe0, .rodata+0x8 at +0x104, .rodata+0x8 at +0x108, .rodata+0xc at +0x13c, .rodata+0xc at +0x17c)
func_800E3724:
  MATCH (4 section-relative relocations unverified: .rodata+0x10 at +0x90, .rodata+0x10 at +0x94, .rodata+0x14 at +0x9c, .rodata+0x14 at +0xa0)

score.py group cand/<groups/sym_controls_extern_rpm>
Members:
func_800E3430:
  MATCH (8 section-relative relocations unverified: … same eight …)
func_800E3724:
  MATCH

score.py group cand/<groups/control_input_E681C>
Members:
func_800E627C:
  MATCH
func_800E6460:
  MATCH
func_800E681C:
  16/179 words differ
```

`func_800C0294` at `-O2`: non-match (saves `s0`, 40-byte frame). `func_800E6460` alone at `-O3`: `94/239 words differ`.
`func_800E3430` alone at `-O3`: `187/189 words differ`.

## Per function

### 1. func_800F1D04 - name-cache slot allocator (strict)
20 names x 13 bytes at `0x80151968`, `u16` ages at `0x80151A78`, 12x3x5 `s32` slot references at `0x80151690`.
Callees are `strcmp` (`func_8008AD04`) and `strcpy` (`func_800A473C`). Prior source (A68) was 104/222.
- Residual was coloured pool plus frame: `best` in `a2` instead of `t2`, missing `move a1,t2`, frame 112 instead of 120.
- Fix: the final purge compares against an **unsigned copy** (`u32 victim = best;`). The unsigned compare makes uopt hoist
  the conversion into its own register, which also changes the colouring order of `j`/`k`/row pointer, and the extra
  s32-sized local supplies the 8 frame bytes. `== (u32) best` plus an unused s32 local gives the same bytes.
- Also matches in a group with both real callees kept (`func_800F1D04_group_check/`), so it is a kept function.

### 2. func_800E6460 - throttle/brake input (provisional)
Internal (four-wide `t6`-`t9` ring). Prior source was 32/239 in a stand-in group.
- Fix: `InputRecord` has `u32 map[13]` at +0x18 (action -> source code or button mask: 0 throttle, 1 brake, 2 steer,
  3 gear up, 4 gear down, 5 reverse, 6 and 9 the two buttons), and tests are written `D_8013FED0[in->pad] & in->map[k]`.
  cfe orders `scalar & array[index]` array-first regardless of source order; with two subscripted operands the source
  order survives and retail's order needs the mask written first.
- With the real caller kept, neither `func_800E6460` nor the locked `func_800E627C` needs a stand-in caller: the
  group `keep` is just `func_800E681C`. `func_800E627C` still scores MATCH with the `map[]` record.
- Closes for real when `func_800E681C` closes (16 words, see `func_800E681C/NOTES.md`).
- Recovered layout: `volatile F32 brake` +0x728, `volatile F32 throttle` +0x72C, `volatile s8 gear` +0x730,
  button bytes +0x731/+0x732, `s8` +0x640 (auto-gear flag), `s16 car` +0x7C6. `D_8013FED0[pad]` held buttons,
  `D_801403C0[pad]` pressed buttons, `D_80140620[pad][2]` stick, `D_80156CF0[pad]` 0x10-byte record with analog bytes at +0xC/+0xD.

### 3. func_800E3430 - arcade `controls()` (code identical, own rodata unverified)
Arcade ancestor proven from the body: `controls()` in `game/controls.c`; its caller `func_800E3724` is `sym()` in
`game/drivsym.c`. Prior source (A85) was 180/189 and 98/150.
- Internal: uses `$f20`-`$f26` unsaved; the caller saves them and keeps `m` in `a3` across the call. Real group, `keep` = caller.
- Both members are code-identical. The only unverified items are their own float literals; retail words at
  `0x801243F0..0x80124404` are 0.7, 0.33, 0.99, 0.05, 9.5493, 0.9, matching the order of the group's `.rodata`.
- Externs instead of literals change `func_800E3430`'s code (0.05 is loaded inside the loop; the others re-colour
  `$f0`/`$f2`), so natural literals are the right source. Closes with plan workstream B.
- Shaping: `1 / m->dt` with integer 1; brake assigned first in the two locked branches; abs as `(x >= 0) ? x : -x`;
  CENTERFORCE copied to +0x130 before the chained zero stores.
- N64 MODELDAT offsets recovered (all from access evidence): autotrans +0xA (s8), BODYFORCE +0xC4 [4][3],
  CENTERFORCE +0x124, last-force copy +0x130, cleared vector +0x13C, peak_body_force +0x148 [2][3],
  peak_center_force +0x160 [2][3], steerangle +0x3B0, torque[4] +0x3B4, steergain +0x3C4, clutch +0x3CC,
  throttle +0x3D0, brake +0x3D4, brakegain[4] +0x3DC, gear +0x3F4 (s16), commandgear +0x3F6 (s16),
  engangvel +0x408, tires[4] +0x430 (0x5C each, angvel +0x48), dt +0x634, idt +0x638, crashflag +0x640 (s8),
  bog_state +0x65C (s16), flags +0x710 (sign bit = countdown), fastin.modeltime +0x718, wheel +0x720 (float),
  mainin clutch/brake/throttle +0x724/+0x728/+0x72C (floats), gear +0x730 (s8), net_node +0x7C6 (s16),
  mode byte +0x7CC, rpm +0x7D0 (s16). Car record (0x3B8): place_locked +0xEF (s8), +0x359 (s8).
  Coast flags: `D_80152718`, `D_8013FECB` (s8).

### 4. func_800C0294 - translate packed vertices (strict, -O3 only)
For every 0x20-byte record at `*0x801525EC` (count `u16 0x8015267C`) with `u16 id` (+0) equal to the argument,
unpack vertex `D_8015201C[rec->vtx]` (`s16 pos[3]`, `u16` of three 5-bit fractions), add `off[3]`, round to 1/32, repack.
- The match counter lives in `v0` and is **not returned**: a trailing `if (n == 0) { return; }` keeps it alive.
  `return n;` can never put it in `v0` (IDO never colours a returned variable into `v0`; it adds `move v0,vN`).
- `ipos[4]`/`pos[4]` for the frame; `pos[k] = pos[k] + off[k]` (differs from `+=`); fraction OR-ed x, y, z.

### 5. menu_vibration_test - arcade `make_uvs_from_quat` (strict)
`game/resurrect.c:874`, verbatim except float constants in the `s = (Nq > 0) ? 1/Nq : 0` line. Matched on first compile.

### 6. func_800AC8D4 - car callback-slot init (strict)
Five 0x18-byte slots at +0x12C of car record `idx` (`0x80152818`, 0x3B8): `s16 state` +0, `s16 id` +2, `s16 owner` +4,
handler pointer +0x10. Ids from five words at +0x2C of the 0x40-byte record at `0x80139320`. Skipped when bit 3 of
`0x801174B4` is set. Statement order per slot is func, state, owner, id (scripted over all 24 orders).

### 7. func_800BF780 - 3x3 matrix product (strict)
`out[i][j] = sum_k in[i][k] * m[k][j]`, plain loop over rows. Second compile.

## What generalises

1. **A counter in `v0` with no `move v0,…` before `jr ra` is not a return value.** IDO 5.3 never colours a variable
   that is returned into `v0`. Such a counter is kept alive by a later test that leaves no code
   (`if (n == 0) return;` at the end, probably a compiled-out diagnostic). Checked with small probes at `-O3`.
2. **Operand order of `&`/`+` is decided by operand shape in cfe, not by source order**, unless both operands have
   the same shape. `field & array[i]` always loads the array first; `array[i] & other[k]` keeps source order. When
   retail has the "impossible" order, look for a second array (here an action map) rather than permuting the source.
3. **`x = x + y` and `x += y` differ** when `x` is a local array element: the first reloads `x` at its next use
   and emits `add y,x`; the second forwards the sum.
4. **An unsigned compare against a signed short local hoists the conversion as a separate web** (`move aN,tM`),
   which changes the colouring of everything in that loop nest. A stray `move` of a loop-invariant local before a
   nest is the signature.
5. **`frontier show` misses FP-register internals.** `func_800E3430` uses `$f20`-`$f26` unsaved and was labelled
   `single`. The unsaved-register detector should include `$f20`-`$f30`.
6. **Stand-in callers are often unnecessary once the real caller is in the group**, even when the real caller does
   not match yet: `func_800E627C` and `func_800E6460` stay out of line and match with `keep = [func_800E681C]`.
   The existing `src/blob/groups/func_800E681C` stand-ins can go when the caller closes.
7. **The `mainin` bytes/floats of the N64 MODELDAT (+0x724..+0x732) behave as `volatile`** in the input code
   (no CSE of repeated reads, assignment value reused without reload). `controls()` reads each once, so it cannot confirm it.
8. Two more arcade identities by body: `func_800E3724` = `sym()`, `func_800E3430` = `controls()`,
   `menu_vibration_test` = `make_uvs_from_quat()`. The N64 MODELDAT offsets above are anchored on them.
