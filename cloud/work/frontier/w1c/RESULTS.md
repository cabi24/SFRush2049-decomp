# Wave 1, agent w1c: frozen near-misses re-tested under the frontier model

Builder scratch: `watchman2:~/rush2049/scratch/frontier/w1c`. Flags everywhere: `-g0 -O3 -mips2 -G 0 -non_shared`.
Scorer command form (run on the builder, in the scratch copy):

```
IDO_DIR=$HOME/rush2049/cache/toolkits/796ae99a…/ido python3 tools/cloud/score.py fn cand/NAME.c NAME --flags "-g0 -O3 -mips2 -G 0 -non_shared"
```

| Function | Bytes | State | Scorer output | Old residual was |
|---|---:|---|---|---|
| `func_800E23A4` | 1,688 | **strict MATCH** → `cloud/matches/func_800E23A4.c` | `MATCH` | a source artefact (not flags, not whole-program) |
| `func_800E5D64` | 1,296 | **strict MATCH** → `cloud/matches/func_800E5D64.c` | `MATCH` | no prior work; written from the disassembly |
| `race_setup_1` | 1,884 | code identical, own-rodata unverified → `race_setup_1/best.c` | `MATCH (4 section-relative relocations unverified: .rodata+0x0 at +0x94, .rodata+0x0 at +0x98, .rodata+0x4 at +0xc4, .rodata+0x4 at +0xcc)` | source artefacts; `-O3` is required (465/471 at `-O2`) |
| `func_800F7F3C` | 1,396 | 117/349 words off (was 270) → `func_800F7F3C/best.c` | `117/349 words differ` | still real: register colouring in the first mode arm |
| `func_80087110` | 1,780 | 4/445 words off (unchanged) → `func_80087110/best.c` | `4/445 words differ` | still real: one `as1` delay-slot selection |

None of the five needed a group, real callers or a flag change to move. What moved them was source form:
declaration scope, literal type, folded casts, `volatile`, `?:` versus a named local, and physical line layout.

## func_800E23A4 — strict MATCH

- Tire forces, arcade `drivsym.c forces1()`. `frontier show` lists `camera_collision_avoid`, `camera_follow_target`,
  `func_800E1F80` as unmatched callees. They are not needed: the function is kept and ordinary O32.
- The old residual (the saved rear `poortract` boolean at `sp+120` instead of `sp+124`) was **not** a flags or
  whole-program artefact. It was one named local too many. Measured frame rule on this function:
  named locals get 4-byte homes from the top in declaration order; below them cfe reserves one temp slot per
  *value type* of conditional/logical expression; below that come uopt's call-crossing pointer temps.
  Retail has six named slots, the int temp of the `(a||b)&&(c||d)` boolean (`sp+124`), then a float temp
  (`sp+120`, never written). So the three surface factors are float `?:` expressions, not a `factor` local:
  `vehicle->drag += (D_80114160 * (D_80142DB0 == 2 ? 0.1f : 1.0f - vehicle->slip)) * speed;`
- Literal types that matter: `(float)1.0 - throttle` and the integer `0` in the last `?:` arm
  (`? 0 :` matches, `? 0.0f :` gives 57 words off).
- Own rodata (5000.0f, 0.1f, 0.1f at 0x801243CC..D4): written as externs in the match file because that gives strict
  `MATCH` with unchanged code. Natural-literal source: `func_800E23A4/natural_literals.c`
  (`MATCH (6 section-relative relocations unverified …)`). Also matches standalone at `-O2`.

## func_800E5D64 — strict MATCH

- Per-car control stream playback/recording (`s32 f(s32 car, f32 *dt_out)`); no arcade ancestor found.
  First structurally right draft: 241 aligned rows off; then 186 → 166 → 66 → 23 → 17 → 0.
- Levers, in the order they closed the residual:
  1. `ABS(x)` as `((x) >= 0 ? (x) : -(x))`, compare as `pos == count` (branch sense and operand order).
  2. `steer` field `volatile`: retail reloads it three times around the rounding test. This is the only way
     found to stop the CSE; it may not be the original declaration (QUIRK, in the header comment).
  3. `(s8) (byte & 7) - 1`: a folded sign-extension is two temp-ring pops and no instruction (166 → 66).
  4. `/ 127` (int literal) for playback against `* 127.0f` for record (66 → 23). With the same spelling
     uopt keeps one coloured constant (`f12`) across both branches; retail has a ring temp on playback.
  5. Bit packing as `(gear + 1) | (flagA * 8) | (flagB * 16)` (23 → 17). cfe orders the `|` operands by node
     shape: 24 permutations of the `<<` spelling all compile to the same (wrong) order.
  6. `((u8) (gas * 15.0f + 0.5f) * 16) | (u8) (brake * 15.0f + 0.5f)` (17 → 0).
- `D_80114738` is `volatile s8` (address materialised, then `0(reg)`).
- Own rodata 0.02f and 1.0f/60 (0x80124490/94) are externs in the match file; natural-literal source in
  `func_800E5D64/natural_literals.c` (`MATCH (4 section-relative relocations unverified …)`). Also matches at `-O2`.

## race_setup_1 — code identical, own-rodata unverified

- Real semantics: palette / display-list animation updater (11 actions through a jump table, then an object
  flip-book loop). Both callees are locked now; the function matches alone.
- 381/471 → 0 differing words in four steps:
  1. `D_8002EB94` (f32) and `D_8002EB98` (int) are `volatile` (381 → 244 aligned rows).
  2. Cases 0/1 as `i = last; saved = palette[i]; for (i--; …)`. The old spelling left one more web, and uopt then
     hoisted the constant 321 into `a1`. That hoist was an allocation side effect, not a source constant.
  3. The two constant-fill loops with the **body on its own source line**. On one line `as1` puts the pointer
     increment in the `bne` delay slot; on two lines it puts the first store there (`sh t5,-8(v0)`), as retail does.
     Loop form (`for`/`while`/`do`), index type and pointer spelling were all inert.
- Scorer: `MATCH (4 section-relative relocations unverified …)`: the float `0.0333333f` and the 11-entry jump
  table. Compared by hand: the object's `.rodata` is `3d088880` followed by `.text` offsets
  `d8 130 188 1f8 328 2d4 280 37c 3ec 4a8 53c`; the image at 0x80123E28..57 holds `3d088880` and
  `0x800BD2C8 +` the same offsets. All 48 bytes agree. A single-member `score.py group` gives the same
  unverified result, so it is **not** a strict claim (`groups/race_setup_1/group.json` has empty `claims`).
- `-O3` only: the same source is 465/471 at `-O2`.

## func_800F7F3C — 117/349 (was 270/349)

- Old residual (270 words, all register operands) was **not** flags/whole-program. `-O3` and `-O2` give the same
  words. It was variable scope: uopt keeps one web per variable, and retail has different registers for the index
  pair in the first arm (`a3`,`t0`) and the other two (`t4`,`t5`), so they are different variables.
- What moved it (270 → 134 → 117): `a`,`b` at function level; `s16 ia, ib` declared per mode arm; the tie loop
  with its own locals (`f`, `c`) instead of reusing `ib`. 2,187 scope combinations scored: 36 reach 117, none lower.
  Declaration order is inert (153 permutations identical).
- What remains is the first arm only (the other two arms are register-exact except the constant 1):
  retail `end pointer = t5, &D_80152038 = s0, 120 = s1, 1 = t5`; ours `t4, t5, s0, s1`. Retail skips `t4` across
  the first sort loop, as if one more coloured web were live there. Giving the arm two key locals
  (`ka`,`kb`, `func_800F7F3C/arm1_key_locals_106.c`) fills `t4`,`t5` and puts `1` on `t5` (106 words) but the keys
  then sit in pool registers instead of ring temps.
- Tried without movement on that residual (about 60 hand variants after the scope sweep): tie-count statement
  forms, leader hoisted, swap temp, `j+1` index local, pointer cursor, key pointer, local `n`/`last`
  (these unroll: +104..+227 words), per-arm `j`.
- Best next hypothesis: the first arm declares one more local than the others that is live over the sort loop
  and coalesced away. Needs the instrumented-uopt web trace (workbench `instrument`), not more blind variants.

## func_80087110 — 4/445 (unchanged)

- Already `-O3`, no callees, no rodata, no floats: none of the new facts applies. The residual is real.
- Mechanism narrowed: the stretched x-flip arm starts with a block that holds only `beq`. `as1` fills that delay
  slot from the fall-through block and picks its first *scheduled* node. Ours picks `lui v1` (the `la` of the
  display-list pointer), retail picks `addu t9,a1,t1`. In the unstretched twin arm retail also picks `lui`.
  Moving the `la` after the edge statement in ugen order (a `do {} while (0)` around the macro) still gives `lui`,
  so the choice is made on a key above line number and list position.
- Scored without movement: 128 line layouts of the arm, 400 random line layouts of the whole function
  (they flip other arms, 6..14 words, never this one), 47 multi-line macro-argument layouts, 16 statement forms,
  11 structural forms of the neighbouring arms, 6 definitions of the globals in-unit.
- `cc -Wa,-R` (as1's scheduler trace) aborts on this toolkit (`wrapper_printf … not implemented` in the static
  recompilation). Best next step: an as1 build with `printf`, then read which key decides that one selection.

## Recovered layouts and global types

- Vehicle model `D_8014A250[]`, 2,056 bytes (same record in three of these functions): `+0x720 f32 steer`,
  `+0x728 f32 brake`, `+0x72C f32 gas`, `+0x730 s8 gear`, `+0x731 s8 flagB`, `+0x732 s8 flagA`, `+0x7C6 s16 index`,
  plus the `VehicleModel` fields in `cloud/matches/func_800E23A4.c`.
- Replay stream: `+5 s8 mode` (>0 play, <0 record), `+0x34 f32 dt`, `+0x4C u8 *data`, `+0x50 u32 pos`,
  `+0x54 u32 count`; reached as `*D_80152698[car]->owner(+0)->stream(+0x28)`.
- `D_80152818[]` 952-byte car records: `+238 s8`, `+239 s8 locked`, `+931 s8`. `D_80152038[]` 120-byte entries: `+20 s32 key`.
- `volatile`: `D_8002EB94` (f32 frame time), `D_8002EB98` (int), `D_80114738` (s8). `D_8002AFB8` is a plain f32.
- Palette animation record (20 bytes): `+0 owner, +4 s16 first, +6 s16 last, +8 s8 step, +9 u8 action, +10 s16 ticks,
  +12 f32 delay, +16 u16 *palette`. Object animation record (20 bytes): `+0 s16 count, +2 s16 object_index,
  +4 s16 direction, +6 s16 index, +8 f32 delay, +12 f32 interval, +16 frames` (12-byte frames).

## Generalises

1. **Line layout is an `as1` input.** A loop written `for (…) body;` on one line and on two lines schedules
   differently (delay-slot choice). 20 unmatched functions in the image have the retail shape
   `addiu p,p,N; bne …; s? x,-N(p)` on a counter-controlled loop; 4 matched ones do. Check layout first there.
   `tools`-free test: `cloud/work/frontier/w1c/klist.sh FILE.c` prints the ugen listing with `.loc` stripped.
2. **`?:` and `&&`/`||` values own frame slots.** One cfe temp slot per value type, below the named locals.
   A two-word stack-offset residual with the right frame size is a local that should be an expression, or the reverse.
3. **One variable is one web.** Same variable name reused in disjoint branches forces one register for all of
   them. If retail uses different registers per branch, declare per branch.
4. **Folded casts and literal spellings are ring/colour levers** with no instruction change: `(s8)` before a byte
   store, `127` against `127.0f`, `* 8` against `<< 3`. Search them by script; they are cheap.
5. **`volatile` shows as `lui; addiu; l? 0(reg)`** on a single read of a global. Three more globals found here.
6. An unexpected hoisted constant (here `li a1,321`) can be allocation fallout from an unrelated statement.
   Fix the other residuals first.
7. The workbench's IDO 5.3 laws (`third_party/n64-decomp-workbench/docs/compiler-laws/ido-5.3.md`, as1 L59/L79/L80,
   cfe L52, ugen L64/L65) predicted levers 1 and 4. Its as1 trace (`-Wa,-R`) does not run on our toolkit.

Helper scripts in this directory: `sc.sh` (strict score), `batch.sh`/`bscore.py` (score a directory, 2 threads),
`full.sh`/`full.py` (aligned diff), `dump.sh` (objdump), `klist.sh` (ugen listing), `grp.sh` (group score).
