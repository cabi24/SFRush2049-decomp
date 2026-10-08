# w14n RESULTS (matching wave 14, lane w14n)

Nothing here is a MATCH. No `cloud/matches/` file, no group, no claims, no overrides, no splice.
Builder scratch: `~/rush2049/scratch/frontier/w14n` (base copy; src/blob, include, tools/cloud, asm/us/blob and
blob_matched.lock.json synced; trace toolkit installed). Tag `w14n`.

## Results table

| Function | Bytes | State | Flags | Best scorer output (verbatim, whole-program unit) |
|---|---|---|---|---|
| `entity_lod_select` | 716 | not matched; **improved 5 -> 3 words**; residual is one register colour (andi temp) | -g0 -O3 -mips2 -G 0 -non_shared | `FAIL entity_lod_select: 3 of 179 words differ` (`entity_lod_select/best.c`, = R2 below; `blob_unit --tag w14n score entity_lod_select --with .../best.c --neighbours`; `locked bodies that differ in this unit: 0`) |
| `wheel_torque_apply` | 140 | not matched; no improvement (still w14g best) | -g0 -O3 -mips2 -G 0 -non_shared | `FAIL wheel_torque_apply: 6 of 35 words differ` (`wheel_torque_apply/best.c` = w14g `b3/v0000.c`; `locked bodies that differ in this unit: 0`) |

Standalone bscore lines: entity R2 `strict 3 mnem-missing 0 aligned-missing 5 size +0`; wheel `strict 6 mnem-missing 3 aligned-missing 6`.

## entity_lod_select

### Shaping constructs of w14l's best, priced (R1 = `entity_lod_select/b1/v0004.c`)

Template `t1.c` (5 choice points), 32 variants (`b1/`), standalone strict:

| construct | removed | strict |
|---|---|---|
| `if (op2) {}` | yes (free) | 5 (kept as R1) |
| `pad0`, `pad1` (unused locals) | yes | 14 (load-bearing: frame/colour) |
| `u32 pad[20]` (unused, frame residual) | yes | 23 (load-bearing) |
| `if (cmd[0] & 0x00FF0000) {}` | yes | 179 (load-bearing: compiled-out read keeps the load) |
| `was = !!first` | -> `was = first` | 7 (keep `!!first`) |

So `if (op2) {}` is dropped; the pad locals and `pad[20]` stay as disclosed frame residuals (`pad[20]` is
the exact frame residual: frame 232/232 in the unit); the compiled-out `if (cmd[0] & 0x00FF0000) {}` read
and the `!!first` form are kept (they earn their place).

Regressed base R1 = `entity_lod_select/R1.c`. **wbgen on R1** (`g1/`, 604 edits + control): best 5 (control and 12 ties
shown; none below 5). Matches the coordinator's round. Skip further one-edit rounds.

### Priority and first-appearance rounds (vgen, standalone)

- `t2.c` (5 choice points on R1: `if (cmd) {}` at top, `if (cmd[0] & 0xFF000000) {}` before the op
  assignment, `if (op) {}`, `if (op & 0xC0) {}`, `if (!op);` after op2). `b2/` 32 variants. **Best strict 3 = `b2/v0006.c`**
  (`if (op) {}` and `if (op & 0xC0) {}` present, the rest absent). Floor otherwise 5. This is R2.
- `t3.c` (R2 base, two stacked `if (op) {}` reads, two stacked `if (op & 0xC0) {}` reads, `if (!op);`, `if (cmd) {}`), `b3/` 64
  variants: best 3 (six variants), others 5 or worse. No further gain from read count (lever 9).
- `t4.c` (R2, the andi expression written into `pad1` or `pad0` at the first compare), `b4/` 3 variants: all 3. No change.
- `t5.c` (R2, `pad0`/`pad1` and `idx` declaration order, `if ((op & 0xC0) == 0x40) {}` placement), `b5/` 16 variants:
  best 3 (`v0008`), others 25 or 145. No gain from declaration order.

### Unit confirmation of R2 (one run this session)

`FAIL entity_lod_select: 3 of 179 words differ` (from 5). The three differing words (unit `--neighbours` diff):
`+0x078 image 31c400c0 unit 31c200c0` (andi `a0,t6,0xc0` vs `v0,t6,0xc0`), and the two `beq` words at
`+0x07c` / `+0x088` (`10810078` vs `10410078`, `beq a0,at` vs `beq v0,at`). All three are the same web, the
`op & 0xC0` temp: retail a0, ours v0. No jump-table word differs in the unit; the rodata relocation
rows are reported as unverified, not as words.

### Diagnose and colour oracle

- `workbench diagnose` on R2 (standalone): `verdict=mixed(structural:1, register:3)`, strict 4 words, lanes pool slot 7,
  `lever=none-known`. Standalone and unit differ by one word because the standalone target count includes the
  .text padding (the same caveat as w14l).
- `ctrace.sh entity_lod_select r2` (snapshot `r2`): the andi temp is most likely the low-save bb0 web `p1:w3`
  (inferred from its `v0` colour and two reads, not confirmed by a source line map), and `p1:w8` (`a0`, bb0) is the other low-save bb0 web.
- Forced oracle (`force.sh r2 entity_lod_select SPEC`): `p1:w3=c3` (a0) is **declined**; forbidden mask
  `0x1f60200000000000` (a0-a3, t0, t2, t3, s4 already held by interfering webs). Forcing `p1:w8` to each of
  c1, c2, c7-c13 does not free a0 for w3 (still declined; w8=c9 and c10 are declined as well). So the forced
  colour is blocked by an interfering web outside w8 (the a0-holder is a save-20 web, `p1:w101`, bb36). The
  colour residual is an interference question, not a single-web priority tie. I did not find a source lever for it.
- Read-count (lever 9), first-appearance (declaration order), andi-in-named-local (`pad1`/`pad0`) and
  compiled-out reads (lever 7/8) were all tried; none moved w3 off v0.

### Integration notes (entity)

- `entity_lod_select/best.c` (R2) is not a match: 3 of 179 unit words differ (one colour web). Its own rodata
  (jump table) is unverified, as in w14l.
- Disclosed shaping in R2: `pad0`, `pad1` (unused s32 locals, frame/colour residual), `u32 pad[20]` (unused frame
  residual, needed for frame 232/232), `if (cmd[0] & 0x00FF0000) {}` and `if (op) {}` / `if (op & 0xC0) {}`
  (compiled-out reads). Any promotion must carry these in the file header.
- Files: `entity_lod_select/R1.c`, `entity_lod_select/R2.c`, `entity_lod_select/best.c` (= R2), templates `t1.c`..`t5.c`,
  batches `b1/`..`b5/` (`score.txt` in each), `g1/` (wbgen on R1).

## wheel_torque_apply

Starting point: w14g `b3/v0000.c` (copied to `wheel_torque_apply/best.c`). Diagnose (standalone): `verdict=mixed(constant:2,
structural:6, register:3)`, strict 9. Unit: `FAIL wheel_torque_apply: 6 of 35 words differ`.

- `b1/` (`t1.c`, 16 variants): the store line in four forms (before the call, blank line before the call,
  same line, blank line plus the call on its own line) and blank-line splits before the `differential_output`
  and `func_800AB638` calls. Best strict 6 (all 16 tie or worse). No line-layout lever moved the sh.
- `g1/` (`wbgen.py best.c`, 19 variants): control 6, all others compile-fail or worse. No gain.
- `b2/` (`t2.c`): store moved after `func_800A473C(...)`: strict 11 (worse). Disassembly (`cdis.sh`, `b2/v0001.c`)
  keeps the `sh` before the second call, not in the delay slot.
- Retail vs ours (unit `udiff.py --all`): retail `li t6,-1; lui at; addiu a0,sp,36; jal func_800A473C; sh t6,-25200(at)`
  (store in the delay slot). Ours `li; lui at; sh; jal; addiu a0` (the addiu fills the delay slot). Also the
  `lui at`/`lui a0` split for the D_80149B80 reload (retail `lui at` for the store, `lui a0` for the reload; ours shares one).
- as1 trace (`as1t.sh wh0 wheel_torque_apply`, `build/trace/w14n/as1r_wh0.wheel_torque_apply.log`): in block 1 the
  `sh` (node 2) is picked at time 0x1c with the `addiu a0` (node 1) and the `jal` (node 0) still ready, so the
  store takes the earlier slot and the addiu falls into the delay slot. The tie at 0x1c is an as1 priority choice,
  and no source spelling I tried changed it.
- Not matched. Next hypothesis (untested): the retail store/call order may come from a source form that makes the
  addiu a web that as1 schedules before the store (for example a call argument computed through a named local,
  which costs a frame slot; frame 64 is fixed, so check first). The `lui at` / `lui a0` split for the D_80149B80
  reload is a separate address-CSE residual.

## What generalises

- A compiled-out read that removes a load (`if (cmd[0] & 0x00FF0000) {}`) can change the whole block: removing it
  went from 5 to 179 words. Price every shaping construct before a colour round; here, `if (op2) {}` was free and
  three others earned their place.
- A stacked compiled-out read on the losing expression (`if (op & 0xC0) {}`) moved the best from 5 to 3 in the
  unit; additional stacked reads did not add more. Lever 9 has a floor.
- When the forced-colour oracle declines (`forbidden` mask), find the interfering web first; a single force on the
  losing web is blocked by another web, not by its own priority.
