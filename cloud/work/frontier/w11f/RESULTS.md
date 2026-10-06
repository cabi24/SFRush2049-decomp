# Wave 11, lane w11f: results (menu/mode-select callers of the provisional bodies)

All unit scores come from `python3 -m tools.conveyor.pipeline.blob_unit --tag w11f score ... --neighbours`, run from
the repo root on the Pi on 2026-10-06 (tree `bb2c157f`, branch `wave10`, clean). Aligned rows come from
`cloud/work/frontier/tools/trace/udiff.py` (TAG=w11f). Builder scratch: `watchman2:~/rush2049/scratch/frontier/w11f`,
which has the trace toolkit installed with `--reuse wtk`. There were no permission denials. Nothing was committed or
spliced, and nothing was edited outside this lane directory.

**No new strict match this wave.** Every function in this cluster still waits on unmatched callers or callees. The
cluster now builds as **one real-caller component with no stand-ins** (`comp/run.sh`). The three provisional bodies
stay EQUAL when every one of their real callers is present as a draft:

| Function | Bytes | State | Flags | Scorer output (`sh cloud/work/frontier/w11f/comp/run.sh`) |
|---|---:|---|---|---|
| `best_times_display` | 224 | provisional; both real callers present, no stand-in | `-O3` | `EQUAL best_times_display: 56 words (internal, c_d5e64.c)` |
| `func_800DED78` | 488 | provisional; real caller present | `-O3` | `EQUAL func_800DED78: 122 words (internal, c_ded78.c)` |
| `mode_select_input` | 152 | provisional; both real callers present | `-O3` | `EQUAL mode_select_input: 38 words (internal, c_mode.c)` |
| `func_800D5E64` | 584 | **10 words off** (was 122); the residual is in the listing (alias hint and store placement) | `-O3` | `FAIL func_800D5E64: 10 of 146 words differ` |
| `func_800DFBA0` | 1192 | **21 words off** (was 181); with 6 colours forced the oracle gives 0 rows | `-O3` | `FAIL func_800DFBA0: 21 of 298 words differ` |
| `func_800E05F0` | 1328 | 60 ops rows off; needs `func_800E0050` to be real | `-O3` | `FAIL func_800E05F0: 295 of 332 words differ; compiled body is 338 words, target 332` |
| `mode_select_handler` | 2976 | 149 ops rows off (m2c-shaped campaign draft) | `-O3` | `FAIL mode_select_handler: 684 of 744 words differ; compiled body is 763 words, target 744` |

The same run also prints `locked bodies that differ in this unit: 0` and
`blob_unit score: 3/7 equal; object build/blob_unit/w11f/unit.o (3.8s)`.

## Component harness `comp/` (the main deliverable)

| File | Contents |
|---|---|
| `d5e64.c` | best_times_display (w10g) and the w11f func_800D5E64 best. w10g's `__standin_msh` is removed. |
| `mode.c` | mode_select_input + func_800DFBA0 (w11f best) + func_800E05F0. The E05F0 changes are `s32` slot indices and style/level initialised inside the `mode==2` block: 66 → 60 ops rows, words 210 → 193. |
| `msh.c` | The campaign draft `module_campaign_20261002/ai/model_audio/ROOT_DEF68` with the index-first `func_800DED78` call adaptation from `dot_audio_force_20261005/verify.py::caller(True,True)`. |
| `ded78.c` | The `dot_audio_force_20261005/candidate.c` source, unchanged. |

Each file is its own translation unit (`--with` per file), so their conflicting type views of the shared globals
(`D_80140808`, `D_8002EB90`) never meet.

- This replaces w10g's `__standin_msh` for best_times_display. It also puts DED78's real-caller proof
  (`dot_audio_force`, which used the same msh draft) into the whole-program unit, not just a two-file group.
- From now on, each real caller that matches moves the cluster closer to splicing.
- `func_800E0050` (1,440 B, unmatched, IPA with s0–s8 unsaved) is still a plain extern here. E05F0 cannot match
  until it is in the unit. Retail E05F0 saves `s3` to `224(sp)` and reloads it around the E0050 call, and writes
  `sw s3,0(sp)`. A plain extern call reproduces neither.
- **Integration:** nothing to splice. When the callers match, the cluster has to land as one locked group. Its
  members:
  - kept: `func_800D5E64`, `func_800E05F0`;
  - internal: `best_times_display`, `mode_select_handler`, `func_800DED78`, `mode_select_input`, `func_800DFBA0`,
    `func_800E0050`.

  It also needs the `__inline entity_hierarchy_update` override that w10g already noted.

## func_800D5E64: 10 words, `d5e64/best.c` (body in `d5e64/best_body.c`)

Per-player engine and skid sound init: two definition-driven handles, the extra and the trio of layer records,
`best_times_display(player)`, then `D_8010FFC4[player] = 1`. Changes from the w10g draft, in the order they paid off:

1. **`s32 player`, not `s16`.**
   - Retail never hoists `-1`: it uses `li at,-1` in the loop and a fresh `li tN,-1` for each store.
   - The s16 narrowing at the `best_times_display` call (`sll/sra`) comes from the s16 parameter.
2. **`state->pair[i]` indexed by `i`, with `if (object == -1) continue;`.** This gives two pointer IVs (s0 for the
   store, s2 for the call) and the retail s6/s7/s8 order for vehicle, 64 and 2.
3. **`ready` comes from an inlined helper whose return value is a temporary**:
   `static s8 *rdy(s32 p) { s8 *r; if(1) r = &D_8010FFC4[p]; return r; }`.
   - A plain `ready = &D_8010FFC4[player]` is copy-propagated: it gets rematerialised at the store and the frame
     shrinks to 112.
   - The plain helper gives `v0`. The `if(1)` local gives retail's `v1`.
   - Shaping quirk: say so if this ever lands.
4. **Frame 136 needs 12 more bytes of cfe locals.** An unused `f32 vec[3]` declared before `ready` puts ready's home
   at retail's `116(sp)`.
   - Tested and ruled out:
     - unused scalars and unused `pad[4]`, which are dropped;
     - inlined-helper locals and inlined calls;
     - volatile dummies (three `volatile s32` also give 136/116, but they emit stores).
   - Shaping quirk: an unused local.

**Residual (10 words, all as1 placement):**
- The `sw v1,116(sp)` for `ready`.
- The schedule of the `state->definition` block.

**Proven by editing the ugen listing and reassembling** (`asm.sh`; the edited listings are in `d5e64/listing/`,
built from the `alt_d_body.c` variant, which is 16 words off in the unit and has retail's ugen temps exactly):
- Deleting `.noalias $21,$sp` gives 2 rows.
- Also moving `sw $3,116($sp)` below the loop-IV inits (`move $16,$21`) gives **0 rows**.

In retail, then:
- (a) `state` (s5) is not marked non-aliasing with the stack. It is a *variable* web, not the address-expression web
  that IDO makes from `&D_80140420[player]`.
- (b) ready's home store sits at the loop preheader, as a live-range-split spill does, not right after its
  definition.

Every way of turning `state` into a variable web either disturbs other colours or breaks the loop shape:
- inlined accessors (plain, `if(1)` temp, `(u32)` launder);
- two-step `state = D; state += player`;
- direct global indexing of the definition store.

None of these reproduces (a) without other damage. About 150 variants: stopped, per the breadth rule.
**Next hypothesis:** find a source form that keeps both `ready` and `state` as uopt variables, i.e. not
copy-propagated. One candidate is a single inlined helper that returns or initialises both. Check `.noalias` in
`tools/ulistA.sh` output, not `ulist.sh`, which filters alias lines.

## func_800DFBA0: 21 words, `dfba0/best.c` (same file holds mode_select_input and the E05F0 draft)

Four-contact skid/scrape layer selector: per-contact weighted fractions into three sums, then the dominant layer's
handle is (re)started with `frame_sync` and fed through `mode_select_input`. Changes from w10g (181 → 21):

1. **Divisor as a ternary:** `fraction /= countN==1 ? 2.0f : countN==2 ? 4.0f : 8.0f;` in all three arms.
   - An if/else with `/2.0f` is folded into `*0.5f`.
   - Retail divides by a hoisted 2.0 (`div.s …,$f18`).
2. **The 1 in the curve and clamp is an int literal:** `fraction>1 ? 1 : …`, `1-(1-fraction)*(1-fraction)`.
   - The in-loop `1.0` web is then separate from the post-loop `mode_select_input(…, 1.0f)` web.
   - This fixed the whole FP allocation:
     - constants f14=1, f16=0, f18=2, f20=4, f22=8, f28=80;
     - sums f24/f26/f30.
3. **Initialisation:**
   - Separate statements in the order `slot`, counts, then `sum1, sum0, sum2`.
   - `count1++` moved after the `power[i]` max.

**Residual is pure colouring.**
`TAG=w11f force.sh ap func_800DFBA0 "p1:w6=c2,p1:w9=c3,p1:w12=c4,p1:w24=c5,p1:w171=c6,p1:w182=c7"` gives
`want 298 words, got 298; differing rows 0 (words); frame 24/24`.

The forced colours are retail's:
- count0, count1 and count2 in v1, a0, a1;
- `i` in a2;
- the constants 1 and 2 in a3 and t0.

ctrace shows why ours differs:
- the shared constant webs 1 and 2 have 4 loop uses each: `save=2.667 (tot 40 / nocs 15)`;
- they outrank the counts and `i`: `save=2.5625 (41/16)`.

Retail needs the constants ranked below the counts and `i` but above the contact pointer (1.94).

| Tried, without moving it | Result |
|---|---|
| Increment and compare forms (`++c==1`, `c+=1`, `c=c+1`, `1==c`) | no change |
| Loop forms (do/while, while, for-init) | no change |
| Switch or if-else divisor | breaks the structure |
| `u32` counts | the counts win, but the contact compares then get separate constant webs: t3/t4/t5 instead of retail's shared a3/t0 |
| Unsigned contact compares | they re-merge the webs |

**Next hypothesis:** find a source shape that gives the 1/2 constant webs one more live block (nocs 16) or one
fewer use, while contact and count compares still share them.

## func_800E05F0 and mode_select_handler

- **E05F0** (`comp/mode.c`):
  - `original_slot` and `slot` are `s32`: retail spills a word to `220(sp)` and passes `t1` unnarrowed.
  - style and level are initialised after the `func_800E0050` call (retail sets `f28=0.5` after it).
  - Result: 66 → 60 ops rows, frame 224/200.
  - The rest needs E0050's IPA context (above). No further work.
- **mode_select_handler**: the campaign draft compiles and links in the real component (149 ops rows, 763/744
  words, two switch tables). It is the gate for best_times_display and func_800DED78. Not attempted beyond
  building it.

## Tools (lane dir)

| Tool | What it does |
|---|---|
| `tools/uvar.py BASE.c FUNC variants.py [unit opts]` | Variant runner. Prints masked positional rows, frame, aligned rows and the unit line. |
| `tools/cvar.py FUNC variants.py BASEFILE` | The same over the whole real-caller component. |
| `tools/cmp.sh` | Side-by-side retail vs unit, with relocation-masked diff counts. |
| `tools/ulistA.sh` | Pre-as1 listing that keeps the `.alias`/`.noalias` lines (w10g's `ulist.sh` filters them). |

## What generalises

- **The listing is a fixed point for as1, so test as1 hypotheses with `asm.sh`.** Deleting one `.noalias` directive
  and moving one store settled D5E64's last 10 words in two reassemblies. The `.noalias rN,$sp` that ugen emits for a
  register that holds `&global[...]` lets as1 move stack stores past stores through that register.
- **An int literal `1` inside float arithmetic is its own constant web.** It also stops the web from stretching to a
  later `1.0f` call argument, which had pushed the web into a callee-saved register.
- **IDO folds `x / 2.0f` into `x * 0.5f`.** A retail `div.s` by a hoisted 2.0 means the divisor was not a literal at
  the division, e.g. a ternary `n==1 ? 2.0f : …`.
- **The trace toolkit's `force.sh` oracle is the fastest way to prove a colouring-only residual.** Apply it first,
  then search for priority levers (`ctrace.sh`: `tot/nocs`).
