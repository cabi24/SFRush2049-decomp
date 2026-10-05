# Frontier wave 2, agent w2a — results (2026-10-04/05)

Builder scratch `watchman2:~/rush2049/scratch/frontier/w2a`; unit runs used `blob_unit --tag w2a`.
Nothing committed, spliced or edited outside `cloud/work/frontier/w2a/`. No file was added to `cloud/matches/`
(the one strict match is a group member; the code-identical single is in `hud_speed_display/best.c`).

| # | Function | Bytes | State | Where |
|---|---|---:|---|---|
| 3 | `func_800A4E58` | 444 | **strict MATCH** as a member of the real `slot_sound` group | `groups/slot_sound/` |
| 5 | `hud_speed_display` | 572 | **code identical, own-rodata unverified**; `blob_unit` EQUAL | `hud_speed_display/best.c` |
| 4 | `car_setup_confirm` | 884 | 2 of 221 words: needs a second symbol for `0x8014A250` | `groups/car_setup_confirm/` |
| 4b | `func_800D1AB0` | 560 | `MATCH` in the real group with both real callers, **still provisional** (neither caller matches) | same group |
| 2 | `func_800CB748` | 600 | 9 of 150 words in the whole-program unit | `func_800CB748/best.c` |
| 1 | `physics_response` | 920 | 13 of 230 words (5 instruction rows) in the real group; caller unmatched | `groups/physics_response/` |
| 6 | `brake_light_update` | 448 | 74 of 112 words (one allocation residual) | `brake_light_update/best.c` |

All flags: `-g0 -O3 -mips2 -G 0 -non_shared`.

Helper scripts here: `sc.sh`/`full.sh`/`grp.sh`/`batch.sh` (copies of w1f pointed at w2a), `gf.sh` + `gfull.py`
(mnemonic-aligned diff of a member of a multi-file group directory), `us.sh` + `udiff.sh` (blob_unit score and a
side-by-side against `build/blob_unit/w2a/unit.o`), `callers.py NAME` (retail `jal` callers), `rd.py ADDR [N]`
(retail data words with float view).

---

## 3. func_800A4E58 — strict MATCH (group)

`grp.sh groups/slot_sound` (= `score.py group <dir>`) prints `MATCH` for all eleven members, the last being
`func_800A4E58:  MATCH`. `blob_unit --tag w2a score func_800A4E58 --with groups/slot_sound/func_800A4E58.c` →
`EQUAL func_800A4E58: 111 words (kept, c_func_800A4E58.c)`.

The group directory is the locked `src/blob/groups/slot_sound` (group.c byte-for-byte unchanged) plus one file,
with `func_800A4E58` added to `members`, `keep` and `claims`. Integrate as an extension of that group
(`blob_group revert slot_sound`, copy, drop `claims`, splice).

Model-table loader. First compile. The whole difficulty of the earlier attempt (111/111 off,
`codex_model_tables_b120`) was the call `func_80096288(index, 0, 0)` with the index kept in `a3`: that is
`slot_value_get(D_80140AF0)` from the slot_sound group, inlined by umerge. Written as that call it matches at once.
The stub neighbours `func_800A4E50` / `func_800A5014` are not needed to explain this function.

Types: `D_8017A4E0` = { u16 count0; u32 *ptrs0 (+4); u16 count1 (+8); u16 count2 (+0xA); u16 *data1 (+0xC);
u32 *ptrs2 (+0x10); u16 count3, count4, count5 (+0x14..); u16 *data3 (+0x1C); u16 *data4 (+0x20); u32 *ptrs5 (+0x24) }.

## 5. hud_speed_display — code identical, own-rodata unverified

`sc.sh hud_speed_display/best.c hud_speed_display --flags "-g0 -O3 -mips2 -G 0 -non_shared"` →
`hud_speed_display:  MATCH (2 section-relative relocations unverified: .rodata+0x0 at +0x1e4, .rodata+0x0 at +0x1e8)`.
`blob_unit … score hud_speed_display --with hud_speed_display/best.c` → `EQUAL hud_speed_display: 143 words`.
The one literal is `1100.0f` = `0x44898000`, equal to the retail word at `0x80123F90` (read with `rd.py`).

Not a HUD function: initialises four object lists and two record pools (60- and 44-byte records).
What closed it (3 variants):
- the four list initialisations are an inlined two-level helper, `list->indirect = a; list->doubly = b;
  func_800A370C(list);`. The retail store order (+1, +0, +8, +0xC, +4) is that of `func_800A370C`, not of
  `func_800A44E8` (+0, +1, +4, +0xC, +8), although `func_800A44E8` has the same signature;
- record bytes +8/+9 are `s8`: with `u8` the stored `1` joins the helper's argument web (`s5` instead of `a2` + `s4`),
  84 words off.
The helper is `static list_init` in the file (no retail symbol known). `func_800A370C` is defined in the file only
for the single-file scorer; with a prototype instead the unit result is the same (`hud_speed_display/e_unit.c`).
Types: `List` as in `src/blob/func_80091FBC.c`; `D_8011025C`/`D_80110260` pool pointers, `D_80110268`/`D_8011026C` counts.

## 4. car_setup_confirm + func_800D1AB0 — real group, 2 words

`grp.sh groups/car_setup_confirm`:
```
car_setup_confirm:
    +0x110  want 3c188015 lui t8,0x8015                 got 3c180000 lui t8,0x0
    +0x114  want 2718a250 addiu t8,t8,-23984            got 27180000 addiu t8,t8,0
  2/221 words differ (unresolved symbols: D_8014A250_alias+0x0 at +0x110, D_8014A250_alias+0x0 at +0x114; 2 section-relative relocations unverified: .rodata+0x0 at +0x6c, .rodata+0x0 at +0x80)
func_800D1AB0:
  MATCH
```
Group: `func_800F8EC8.c` (context, the 255/256 near-miss from `cloud/work/near_miss_B125`, unchanged),
`car_setup_confirm.c`, `func_800D1AB0.c` (agentC's body without the stand-ins); `keep` = the two real callers.
`claims` is empty on purpose.

- **Arcade ancestor:** `CarReportsGameOver` (`game/checkpoint.c` 1316), N64 signature `(s32 slot, f32 score)`.
  The declaration list is the arcade one (`S16 i, j, index, temp, num_locked, place[], num_humans_locked`).
- **The stand-in question is settled:** `func_800D1AB0` needs no stand-ins. With both real callers in the group it
  stays out of line and prints `MATCH`, even though `func_800F8EC8` is far from matching. It remains provisional
  only because neither caller is a match yet.
- **Group context was required for car_setup_confirm itself:** alone its FP temporaries run through six registers
  (`f4..f10,f16,f18`); in the group they run through four (`f4..f10`), as in retail. That was 17 words.
- **Residual lane: one symbol identity.** `if (D_8014A110 != 2 || X == &model[index]) num_humans_locked++;` —
  retail materialises `X` = `0x8014A250` with `lui/addiu` inside the loop instead of using `s0`, which holds the
  same address as the array base. Every spelling on `D_8014A250` (`&D_8014A250[0]`, casts through `u32`/`s32`,
  first member, block-scope `extern`, a helper in another file inlined by umerge) is commoned with the base and
  costs ~35 words. A differently named symbol with the same value reproduces all 221 words. `sym + offset` forms
  (`&D_8014A248[2]`, a trailing struct member) are not folded: IDO emits a separate `addiu`.
  **Next step (integrator decision):** provide a second data symbol for `0x8014A250` (there are no data aliases in
  `symbols.json` today) and rename `D_8014A250_alias`; then both functions close (1,444 bytes). I did not find what
  the second object is; the operand order (`X == m`) says the constant is on the left in the source.
- Literals: `5999.999f` = `0x45BB7FFE` = retail word at `0x80124178`; `9.0f` is `lui`.
- Quirks: `place[7]` (frame 136), the final loop indexes `place[i]` directly (an `index` local there swaps
  `v0`/`v1`), `5999.999f + (f32)(s32)(9.0f * D_801543AC) - distance` in that operand order.
- Types: model `D_8014A250[]` +0x7C6 `s16 slot`, +0x7CC `s8 drone_type` (2 = human), +0x7E8 `s8`;
  game car `D_80152818[]` +0xEE `s8 place`, +0xEF `s8 place_locked`, +0xF0 `f32 score`, +0x100 `f32 distance`,
  +0x359 `s8`; `D_80152744` `s8 num_active_cars`; `D_8014A108` `s16 num_humans`; `D_80152718` `s8` end flag.

## 2. func_800CB748 — 9 of 150 words (whole-program unit)

`blob_unit --tag w2a score func_800CB748 --with func_800CB748/best.c` → `FAIL func_800CB748: 9 of 150 words differ`
(prior best 147/150). It cannot be scored alone: it calls the internal `audio_reverb_update` (address in `a1`).

What moved it (about 45 variants):
- the table is a flat `Record44 **` indexed `[((mode ? 6 : 0) + player) * 3]`, not an array of 12-byte rows
  (decides whether the multiply or the base load comes first);
- `row[i]` everywhere, no cursor variable: the scan's induction pointer and the later `&row[i]` are then one web (`s2`);
- the scan uses a named handle local `h`, `*h` is not a variable;
- the release of the old entry is a single-use helper `static release_handle(u32 address)` holding the three
  statements `osRecvMesg; audio_reverb_update(address, 0); osJamMesg` and called under
  `if (row[0]->handle != 0)`. With the statements written in place the handle takes `t0` and the row `t1`
  (retail `v0`/`t0`). `audio_effect_process` has the same body but is not inlined by umerge here.
- five named locals + the helper give the 72-byte frame (`i` first, `row` fifth).

Residual (all in one block, the null test before the release): retail computes `&row[i]` before the `a0`/`a1`
set-up and stores the handle to its home slot right after the branch (`sw v0,40(sp)` then `li a2,1`, spills);
mine computes `&row[i]` last (it lands in the `beqz` delay slot) and stores the handle after the spills
(`jal` delay slot). In retail the helper's formal is a separate memory-resident variable; in mine uopt merges it
with the tested value. Tried without effect: formal/field types (8 combinations), a copy local in the helper, a
second parameter, nested helpers, test inside the helper (gives the right order but the wrong registers),
address-taken handle local (right store, wrong registers).
**Best next hypothesis:** the real helper has a statement before `osRecvMesg` that makes its formal live in
memory (or takes the record, not the handle); look at the other callers of the same three-statement sequence
(`func_800CB9D0`, `func_800CC50C`) for the helper's true shape before more variants.
Types are in the file header (`Header88`, `Record44`, 3 slots per row at `*D_8012E6F8`).

## 1. physics_response — 13 of 230 words (real group)

`grp.sh groups/physics_response` → `13/230 words differ (2 section-relative relocations unverified: .rodata+0x0 at +0x4, .rodata+0x0 at +0x18)`;
`gf.sh groups/physics_response physics_response` → `want 230 words, got 230; reg/imm rows 3; structural rows 5`
(prior best 225/230). The group is the real caller `audio_effect_apply` and `func_800B9338` as context (from
`codex_physics_a13`, the caller is still ~130 rows off) plus `physics_response.c`. Literal `1.4666667f` =
`0x3FBBBBBC` = retail `0x80123DF4`.

N64-only path-section timing (no arcade ancestor found). What mattered, in order of yield (about 60 builds):
1. **`path` is a local variable equal to 0**, used as `D_8012E5E8[path].pts`, `D_8012E5E8[path].num`,
   `start[path]` and as the second argument of `func_800B9338`. The constant is folded after uopt has chosen its
   hoisting candidates, so the addresses are loaded with `lui`/`lw` at each use and the stride 80 is expanded to
   shifts. With a literal 0 uopt keeps `&D_8012E5E8` and `80` in `s7` and the frame grows. This removed ~100 rows.
2. `i` first holds the next section index (`i = …; next = i;`), then is the point counter: retail's
   `v0` → `move s4,v0`.
3. No section pointer local: `D_80151CE8[cur].start[path]` / `.time[p]`; the address is one web (`s2`).
4. `if (cur + 1 == num) i = loop; else i = cur + 1;` — the if/else form leaves retail's `b` over the empty else.
5. `d[0]*d[0] + d[1]*d[1] + d[2]*d[2]` in natural order, speed converted through `(u32)`, `t / 30.0f` written twice.
6. The callee decides its own callee-saved set (checked with a trivial caller): the unsaved `s0..s6` of retail are
   exactly what the right source uses; nothing is inherited from the caller.

Residual: the placement of `cur = next`. Retail has it before the `next == last` test (`move v1,s4`, then
`bnel s4,a0,top`). Written there (`physics_response/alt_early_assign.c`) the branch shape is exact but `cur` is
coloured `a1` instead of `v1` (24 register rows, the first-section value takes `v1`); written at the end of the
loop body (`best.c`) every register is right and the 5 rows are the move and the two branches. 20 top/tail
spellings, declaration order and dead initialisers did not move the colouring. This is a caller-saved web-order
question (workbench law L83: ascending web number, lowest free colour): **next step** is the instrumented-uopt web
listing for the two files, or matching `audio_effect_apply` first (its empty byte-swap loops are documented in
`codex_physics_a13/STATUS.md`); the function is provisional until that caller matches in any case.
Types: section record 80 bytes at `D_80151CE8` (+0 total, +2 loop, +4 last, +6 first, +8 num in record 0;
+0x2A `s16 time[2]`, +0x30 `s16 start[]`); path set `D_8012E5E8[]` = { u16 num; PathPt *pts } (8 bytes);
`PathPt` = { s16 pos[3]; u8 speed; u8 } (8 bytes).

## 6. brake_light_update — 74 of 112 words

`sc.sh brake_light_update/best.c brake_light_update --flags "-g0 -O3 -mips2 -G 0 -non_shared"` →
`74/112 words differ (4 section-relative relocations unverified …)`; same in `blob_unit` (kept). Literals
`32767.0f` ×2 = `0x46FFFE00` at `0x80123BD4/D8`.

World → screen projection (not brake lights). Structure, frame (72), stack slots and the instruction mix are
right; the residual is **one allocation fact**: retail never keeps the clamped depth `r[2]` or the screen x in a
register (store, then reload in the next block; only `f0`/`f2` are coloured), mine keeps them in `f2`/`f12` with
deferred stores. About 30 variants did not move it: struct vs array locals, one 3×3 local, pointer access,
inlined static helpers for the clamp / the projection / the s16 saturation, ternaries, loops (they do not unroll),
`volatile` (changes addressing, worse), whole-program unit.
**Best next hypothesis:** the callers. All five are unmatched (`game_results_*`, `func_8010A8D0`, `func_8010EA08`);
if one of them makes this function internal the FP colour set may be restricted — the frontier detector does not
look at FP registers. Otherwise run the instrumented uopt on `best.c` to see why the two webs are declined.
Types: view = { f32 uvs[3][3]; f32 pos[3] (+0x24) }; port record 72 bytes at `D_8017A510` with f32 at
+0x1C/+0x20 (scale), +0x24/+0x28, +0x2C/+0x30 (centre).

---

## What generalises

1. **A local that only ever holds a constant index is a matching lever.** `D_xxx[path].field` with `path = 0`
   compiles to direct `lui`/`l?` loads and shift-expanded strides, where a literal `0` makes uopt hoist the base
   address and the stride into callee-saved registers. Signature in retail: a function that reloads the same
   global with a fresh `lui` several times in a loop with calls, and `move a1,zero` argument set-ups.
2. **`sll;addu;sll` in one loop and `multu` by a hoisted constant in another** for the same stride is the same
   effect, not two different arrays.
3. **`bne …; <delay>; b L; <assign>` with both branches to the same label** is an if/else (or `?:`) whose else arm
   was hoisted; write the else explicitly.
4. **Nested tiny helpers show in store order.** Compare the retail store order of an inlined initialiser with each
   candidate callee's own body before choosing which one was inlined.
5. **A single-use `static` helper changes colouring even when its body is the same three statements** (its formal
   is a separate variable). Try it when an argument value sits in `v0` and is stored before a call.
6. **Stand-in callers were unnecessary again** (`func_800D1AB0`): put the real callers in the group even when they
   are near-misses.
7. **Group context changes a kept function too**: `car_setup_confirm`'s FP temp ring is six-wide alone and
   four-wide in its group. A four-wide `f4..f10` ring is the FP form of the `t6..t9` signature.
8. **Symbol identity can be the whole residual** (`car_setup_confirm`): a constant address materialised in a loop
   next to a register holding the same value means two symbols; no spelling on one symbol separates them.
9. Tool notes: `gfull.py` aligns on mnemonics and separates register rows from structural rows, which was the
   useful metric for the 230-word function; `blob_unit score` is the quickest way to test a function that calls
   an internal register-parameter callee.
