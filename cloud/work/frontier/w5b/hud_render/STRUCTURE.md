# hud_render (0x800CADA4, 605 words) — structure (w5b)

The name is a historical label: this is the per-frame update of the **exhaust/smoke puff list**, not HUD
drawing. Arcade relative: `game/visuals.c` `AnimateSmoke` (same "tire 4 = random jitter" special case, frame
sequence, release on timeout), but the N64 code is a rewrite, not a paste.

```c
void hud_render(void);   /* walks D_8013F1F0 (Puff list), releases into pool D_8013F1E0 */
```

Frame 320; saves s0–s8, f20–f26. Loop-invariant registers: `f20 = 0.0f`, `f22 = 0.15f`, `f24 = 0.03f`,
`f26 = 32768.0f`, `s0 = 3`, `s6 = D_8012E700` (68-byte resource records), `s7 = 68`, `s8 = &D_8011735C`
(rand seed). Stack: `next` 256, `car` (s16) 314 (spilled around the model calls), direction vec 268,
type-5 position 280, matrix 200.

| Retail | What |
|---|---|
| 0x800CAE20 | `for (puff = head; puff; puff = next) { next = puff->next; car = puff->car; type = (flags >> 17) & 0xF;` |
| 0x800CAE54 | **attached** (`flags & 0x10`): player = `D_80152818[car]` (952 B); speed = player->speed(@248) >> 2; aspeed = (s16)fabsf(speed); `view(@861) < 2` ? `model_data_load(id,1,1<<slot)` : `model_transform_setup(id,0,1<<slot)` |
| 0x800CAEEC | `if (D_8014A250[car].exhaust[type] (u16 @1564) != 2 || aspeed < 10)` → clear `D_801392D8[car]` bit `0x100<<type` (switch, 4 cases) → kill |
| 0x800CAFD0 | orient: copy player direction(@20) → `func_8008E0B8` → `vector_normalize_length(dir, m)` → `math_utility(m, puff->matrix)`; if aspeed < 90 scale matrix by `aspeed/70*.75+.25` (reversing) or `aspeed/90*.85+.15` |
| 0x800CB0B8 | position = player->exhaust[type] (@116 + 12·type) + puff offset(@68) + (0, 1.25, 0); frame timer −= dt; on expiry `D_8012E700[id.index].frame = D_80142948[1][frame]`, frame = (frame+1) & wrap 8, timer = 1/30 |
| 0x800CB1A8 | **type 5** (car glow): car disabled → kill; pulsing resource scale (±0.03 / +0.04 up to `life`), frame 0..14; position = player pos(@8) + off.x·row0 + off.z·row2 + `D_8011744C[car.model]`·row1 (player matrix @44) |
| 0x800CB3A4 | **free puff**: life −= dt, ≤ 0 → kill (`entity_spawn_callback(id.index,0,0); func_800AFA84(&D_8013F1E0, puff); continue`); a discarded volatile read of `D_8002EB94`; life < 1 → resource alpha(@63) −= 9 while ≥ 9 |
| 0x800CB43C | type 4: inline LCG rand ×3 jitter (`r·.3/32768 − .15`, `r·.25/32768`, `r·.15/32768 − .075`), scale += .05 |
| 0x800CB548 | frame timer; animation only if `D_80156994 || D_8014978C >= 6`; `flags & 1` → forward else backward (wrap 8 / 7); timer = 1/30; scale += (type < 6 ? .03 : .05); position += velocity·dt |

Own `.rodata` (10 floats, 0x80123FC0..0x80123FE4): .03, .15, .85, .0333333, .04, .3, .075, .05, .0333333, .05.
`D_8002EB94` (frame time) is `volatile` (`lui/addiu/lwc1 0(reg)` and the `lw zero` read).

## Residuals of best.c (377/605 strict, aligned 270)

1. **Frame filler.** Retail has 2 words between the vectors and `next`, 5 words between `next` and the
   matrix, and 15 words below the matrix that no visible variable explains. best.c reproduces every offset with
   `pad1[2]`, `pad2[5]`, `pad3[11]` plus the slots of the inlined helper below. Inlined statics' parameters
   get frame slots at the bottom (measured: two `resource_set_frame` calls = 16 bytes), so most of the filler
   is probably more deleted helpers (resource scale/alpha setters, a matrix-scale helper, `rand`).
2. **`resource_set_frame(index, frame)`** (inlined static, 2 sites) is supported by the registers: retail keeps
   the index and value in v0/v1 (parameter webs) — it fixed both sites.
3. **car register.** Retail colours `car` v1 and spills it around the model calls; mine gets a3 because uopt
   forbids v0–a2 for its web (`forbidden0=0x7c038000`, trace st_h1). Retail's web was split, not coloured.
4. **type/speed** come out s4/s5 swapped (retail speed s4, type s5): type's priority 100/13 beats speed's 30/4.
   The matrix-scale helper fixes this swap (s3/a.c) but adds a spilled matrix pointer — the loop in retail uses
   `puff` as base (`lwc1 4(v0)`), so if it is a helper it takes `puff`, not the matrix.
5. rand's multiplier `0x41C64E6D` is promoted to v1 in mine (retail `lui/ori at` per call) — same family as
   `render_display_list`'s promoted `0xF2000000`.
6. Two `add.s` operand orders (offset add in the attached path, type-5 row sums) — source operand order
   does not move them (tested), so they follow the register residuals.
