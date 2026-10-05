# Wave 3, agent w3d — results

Builder scratch `watchman2:~/rush2049/scratch/frontier/w3d` (copied from `base`), unit tag `w3d`.
Helpers in this directory are the w2e/w1b scripts retargeted to `w3d` (`sc.sh`, `full.sh`, `batch.sh`,
`grp.sh`, `diag.sh`, `o3s.sh`). Nothing committed, spliced, or edited outside `cloud/work/frontier/w3d/`
and `cloud/matches/`.

| # | Function | Bytes | State | Flags | Where |
|---|---|---:|---|---|---|
| 1 | `model_transform_setup` | 404 | 8/101 words off | -O3 (same at -O2) | `model_transform_setup/best.c` |
| 2 | `arb_rate_set` | 312 | 25/78 words off (was 31) | -O3 | `arb_rate_set/best.c` |
| 3 | `func_800E8D50` | 448 | 1/112 words off (unchanged) | -O3 | `func_800E8D50/best.c` |
| 4 | `steering_sensitivity` | 928 | 106/232 in its real group (unchanged) | -O3 group | `steering_sensitivity/best.c` |
| 5 | `init_state_continue` | 712 | provisional lane, 89/178 with stand-in (unchanged; w2e best stands) | -O3 group | `../w2e/init_state_continue/` |
| 6 | `camera_shake_start` | 744 | **strict MATCH**, own rodata verified by the scorer | -O3 (also -O2) | `cloud/matches/camera_shake_start.c` |
| 7 | `func_800A4CB8` | 408 | 98/102 strict (one-instruction prologue shift; 53 aligned rows) | -O3 | `func_800A4CB8/best.c` |
| 8 | `func_800F6928` | 400 | **strict MATCH** | -O3 (also -O2) | `cloud/matches/func_800F6928.c` |

Spliceable now: `camera_shake_start` and `func_800F6928` (1,144 bytes); both are `EQUAL` in the
whole-program unit.

---

## 6. camera_shake_start — MATCH

```
sc.sh cloud/matches/camera_shake_start.c camera_shake_start --flags "-g0 -O3 -mips2 -G 0 -non_shared"
camera_shake_start:
  MATCH
    own .rodata verified at 0x80123E58..0x80123E5C
(-O2: same MATCH line)
python3 -m tools.conveyor.pipeline.blob_unit --tag w3d score camera_shake_start --with <final source>
  EQUAL camera_shake_start: 186 words (kept, c_final.c)
```

What it does (historical label): the set-up half of `race_setup_1`, the per-frame animation updater over the same
two per-track tables. It binds the palette-animation rows (`D_8011A840[track]`) and the object-animation frames
(`D_8011A31C[track]`) to resources found by name (`func_800B24EC`, `sound_bank_load`, `func_800BDA24`), and resets
delays and indices. Own literal `0.0333333f` = 0x3D088880 (one 30 Hz tick; race_setup_1 uses the same spelling).

What closed it, starting from B85's `camera_shake_start_globals.c` (162/186):
- natural literal `0.0333333f` instead of `extern f32 D_80123E58` (the extern kept the address in `s1`);
- two unused `int` locals declared after `id`: the frame becomes 112 and `id` lands at sp+94 (quirk);
- **`if (!row) goto second;` before the row `while`** instead of `if (row) { while ... }`. This was the
  last 13 words. With the `if` block, uopt re-materialises `&D_80140BDC` (s5) and `&id` (s4) in the shared join
  block; with the goto, the null path and the loop exit each get their own copy, as in retail
  (`bnezl s0; lw; b after; addiu s4,sp,94`). Every structured form tried (`if/while`, `if/do`, `for` with `&&`,
  `if/else`, a dead assignment after the loop) gives the merged join.

Callee prototypes take five arguments (retail stores `1` at 16(sp)); the locked `func_800B24EC` and
`sound_bank_load` definitions declare four. The unit compile is EQUAL anyway.

## 8. func_800F6928 — MATCH

```
sc.sh cloud/matches/func_800F6928.c func_800F6928 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
func_800F6928:
  MATCH
(-O2: MATCH)
blob_unit --tag w3d score func_800F6928 --with .../c3.c
  EQUAL func_800F6928: 100 words (kept, c_c3.c)
```

What it does: a piecewise-linear lookup in the (x, y) float table `D_80114750` (sentinel x >= 32000), then the same
code as the locked `render_helper` (store D_80114744; when positive, set bit 0x10 of D_80149B88 and call
`func_8008A644((u16)(u32)value)`, else clear the bit). No own rodata.

The game_C28 attempt reached 87 rows off. What closed it:
- the plain `while (value > tab[i].x && tab[i].x < 32000.0f) i++;` loop already gives retail's unfolded `sll t6,zero,3`;
- the three differences are named locals `a, b, d`, **assigned in the order d, b, a** (any declaration order).
  This gives retail's `$f18/$f16/$f14` colouring. Written inline they are ugen temps in other registers;
- **`value = a * b / d; value += y0;`** as two statements. Every single-expression spelling (either operand order)
  emits `add.s y0, q`; retail has `add.s q, y0`.
Calling an inlined `render_helper` for the tail isn't needed. In single-file mode the helper is kept and called,
not inlined.

## 1. model_transform_setup — 8/101

```
sc.sh model_transform_setup/best.c model_transform_setup --flags "-g0 -O3 -mips2 -G 0 -non_shared"
  8/101 words differ        (-O2: identical result)
```
Semantics: the arcade `MBOX_ShowObject` shape (MB/mb_util.h: SHOW_ALL 0, SHOW_EACHCHILD 1, SHOW_RECURSIVE 2) with an
N64 per-viewport mask. It clears 0x80000000 and `mask << 8` in object record `D_8012E700[a]` (0x44 bytes: flags@0,
child@0x16, sibling@0x18; read index `(s16)a`, write index `a`). Mode 1 tail-calls into the child with mode 2, and mode 2 walks the
siblings, recursing into children. A duplicated `mode == 0` branch (sets the mask bits, clears 0x80000000) is dead
but kept by IDO. It is the Show twin of `model_data_load` (Hide, agentB 3/93), which has the same loop residual.
Residual (workbench `diagnose`: pool lane): (a) the loop-invariant constants `1` and `0x80000000` swap `v1`/`a2`
(4 words). (b) The loop child loads into `a3` with a `move a0,a3`, where retail loads it straight into `a0` (3 words).
Tried (~250 variants): five mask spellings, three loop forms (do/while/goto), param, local and field types,
child via `t`/`c`/direct/`Ent *p`/index copy, tail-recursive sibling, if-chain/returns/goto/switch.
Next: the two functions are siblings with the same loop residual. Any form that fixes one should be tried on both,
for example the child assigned to `a` itself through a pointer that stays a single web.

## 2. arb_rate_set — 25/78 (from 31)

```
sc.sh arb_rate_set/best.c arb_rate_set --flags "-g0 -O3 -mips2 -G 0 -non_shared"
  25/78 words differ        (-O2: 67/78 + 1 extra)
blob_unit --tag w3d score arb_rate_set --with arb_rate_set/c2.c   -> FAIL 25 of 78; "umerge inlined into it: func_800A5560"
```
New facts: the fill-colour store is the inlined, locked `func_800A5560(u16 color)` (0x800A5560, same file region),
and it accounts for 8 frame bytes. The macro form gives a 48-byte frame. Direct `D_8017A510[index].field` indexing
instead of an `entry` local fixes the GPACK operand order (retail b, r, g). The colour bytes are read back from
the record after the `sb` stores (as1 turns the `lbu` after `sb zero` into `$zero` operands). This explains the
unfolded `sll t,zero,8`.
Residual: (a) a v0/v1 swap between the record address and `&D_8011EA30` (6 words; ~60 statement orders and
pointer forms tried). (b) The GPACK value is coloured `a0`, the inlined `color` parameter web, where retail uses temps.
as1 then hoists andi/sll/or/sw above the four `sb` stores (~12 words). Alpha spellings, macro forms, locals and
helper inlines don't change this.
Next: find why retail has no variable for the inlined parameter. It might be a different inline depth, or a caller
that passes a u16 local.

## 3. func_800E8D50 — 1/112 (unchanged)

```
sc.sh func_800E8D50/best.c func_800E8D50 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
    +0x160  want 46101402 mul.s $f16,$f2,$f16           got 46028402 mul.s $f16,$f16,$f2
  1/112 words differ
    own .rodata verified at 0x801244C0..0x801244C4
```
`best.c` is the dot_vector_blend source, unchanged. Same in the unit. The product `inverse * (delta + position)` always comes out
sum-first, whichever source order is used, so uopt canonicalises it. `P * weight` keeps source order.
Tried (~110): every operand order, inline `(1.0f - weight)` (fixes nothing, loses the load schedule), sum through
a local or through `out[i]` (the frame grows by 8), inlined blend helpers (the frame grows), compound `*=`/`+=`.
Next: a form where the sum is a leaf at the multiply, without a new named local (the frame is exact now).

## 4. steering_sensitivity — 106/232 in the real group (unchanged)

```
grp.sh steering_sensitivity/grp   (group.json = dot_steering_medium, context unchanged)
  106/232 words differ
    own .rodata verified at 0x80123BFC..0x80123C00
  vector_diff_process / traction_control / func_800A61B0 / math_utility: MATCH
```
`best.c` is the dot_steering_medium source with the natural literal `0.01f` (0x3C23D70A, own rodata) instead of
`extern f32 D_80123BFC`. New facts:
- `z = 0` (int) where the source had `0.0f`: retail uses two separate `mtc1 zero` (compare and store), and this
  fixes them (`alt_int_zero.c`). The alignment improves from 92 to 90 rows, but the function grows by one word
  (a `bc1f` delay slot is left empty), so the strict count is worse.
- `absHeightChange` shares stack slot 48 with the spilled `r0->uvs` pointer, so in retail it is a uopt spill
  temp and not a named local. Removing the local doesn't help on its own.
- Height and y are swapped in the callee-saved FP pair ($f20/$f22). Declaration order doesn't move them.
A 160-variant choice-point batch (zero literal types at four sites, five statement orders for x/width/height/y,
declaration swap) stays at 90 aligned rows. Next: needs the pre-as1 listing and FP-web tracing. This is a
register-colouring problem across ~100 rows, not a source-shape one.

## 5. init_state_continue — provisional lane, unchanged

```
grp.sh <w2e best + group.json (keep = standin_caller)>
  89/178 words differ
```
Its callers `display_list_flush` and `countdown` are still unmatched, so the best possible result is provisional.
Tried: six forms of the random-retry loop (goto retry, `while (1)` + break, `for (prior...)` with goto, while
with prior preset, explicit `while (prior < slot)`, reversed test). The `while`/`goto` forms give retail's
bottom-test-and-branch-back shape, but they re-colour the whole pool (lead 20 to 3, 77 aligned rows). The
others stay at 44–46. The w2e notes still apply.

## 7. func_800A4CB8 — 98/102 strict, 53 aligned rows

```
sc.sh func_800A4CB8/best.c func_800A4CB8 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
  98/102 words differ        (-O2: 101/102)
```
Rewritten from scratch with the `list_init` helper from `hud_speed_display` (inlined; this gives the store order),
`volatile s8 D_8011028C` (every reader uses lui/addiu + 0(reg)), and the peeled and unrolled `for (i = 0; i < 5; i++)`
zero loop (this reproduces `li s0,1` + `&D_801460C8[s0]`). Everything matches structurally. Residual: retail keeps the
parameter `count` in a caller-saved register (`move a2,a0` … `sw a2,48(sp)` before the first `jal`). Ours leaves it
in its home slot and reloads it, so one extra temp shifts `t6..t9` by one through the rest of the function. Tried: count
copies (these give `s5`), `register`, statement orders, u32, helpers taking count (incl. as the stub
`func_800A4E50`; non-static ones are kept and called in single mode), volatile `D_801460F4`.
Next: score the stub-helper hypothesis in the unit (`blob_unit --internal func_800A4E50`). It is the one form that
single-file scoring can't test.

---

## Recovered types and globals

- `D_8012E700[]`: 0x44-byte object records, `u32 flags` @0 (0x80000000 hidden, bits 8.. per-viewport mask),
  `s16 child` @0x16, `s16 sibling` @0x18 (model_transform_setup / model_data_load).
- `D_80114750[]`: `{f32 x, y}` interpolation table with an x >= 32000 sentinel. `D_80114744` is the published value.
- `D_8011028C`: `volatile s8` (all readers materialise the address).
- Camera/animation tables: `Row20 {char *name; u8[5]; s8 kind@9; u8[2]; f32 time@12; void *data@16}`, matching
  race_setup_1's PaletteAnimation20 (its `action` is read signed here). `Sequence20` = ObjectAnimation20,
  `Item12 {char *name; Resource32 *resource; void *output}` = AnimationFrame12, `Resource32` flags@28
  (0x08000000), lists@20/24.
- List helper store order and `List` layout as in hud_speed_display. `D_80146160`/`D_80146138` are Lists.

## Generalises

1. **`goto` instead of an `if` block around a loop changes uopt's join placement.** Re-materialised
   addresses/constants go to each incoming edge instead of the shared join block. The pattern to recognise:
   retail has `bnezl X; <copy>; b after; <copy>` with the same `lui/addiu` or `addiu sN,sp,K` repeated on the
   null path and the loop exit. That closed camera_shake_start's last 13 words.
2. **A final `x = expr + c` that comes out with swapped `add.s` operands can be split into `x = expr; x += c;`**
   (func_800F6928). Named locals for sub-differences, assigned in the order that matches retail's FP colours
   (here the first-assigned gets the lowest register: d→$f14, b→$f16, a→$f18), fix FP temp numbering.
3. **A u16-parameter callee that is inlined adds 8 bytes to the caller's frame** (its parameter home). A frame
   that is 8 too large or too small can mean a missing or extra inlined call, as with arb_rate_set and func_800A5560.
4. A spill slot that retail shares between a pointer and a float (steering: 48(sp)) means that value is a uopt
   temp, not a named local.
