# w9e results (wave 9, unit recipe)

Assigned: `entity_tick_main` (0x800930A4, 2,684 B, IPA s5) and `engine_torque_calc` (0x800AB7D0, 896 B, IPA s1).
Both are **internal** functions whose real callers are **unmatched**: `drone_ai_update` (3,280 B) for the first,
and `engine_sound_update` (1,428 B) plus `transmission_ratio_get` (1,808 B) for the second. A strict match
needs those callers in the unit, so nothing here can be more than provisional this session. Neither body reached
an exact match, even with stand-in callers.

| Function | State | Flags | Scorer output (exact) |
|---|---|---|---|
| `engine_torque_calc` | near-miss (69 aligned rows; 27 with forced colours), stand-in caller | `-g0 -O3 -mips2 -G 0 -non_shared` (unit) | `FAIL engine_torque_calc: 204 of 224 words differ; compiled body is 223 words, target 224` |
| `entity_tick_main` | far (frame and body structure open), stand-in caller | same | `FAIL entity_tick_main: 600 of 671 words differ; compiled body is 669 words, target 671; +0x140: .rodata+0x40: retail words encode 0x8012e700, outside the image; +0x320: .rodata+0x48: retail words encode 0x00010374, outside the image (+19 more)` |

Commands (repo root, Pi):

```
python3 -m tools.conveyor.pipeline.blob_unit --tag w9e score engine_torque_calc --internal engine_torque_calc \
  --internal func_800AB7D0 --keep transmission_ratio_get --keep engine_sound_update \
  --with cloud/work/frontier/w9e/engine_torque_calc/best.c
python3 -m tools.conveyor.pipeline.blob_unit --tag w9e score entity_tick_main --internal entity_tick_main \
  --internal func_8009309C --keep drone_ai_update --with cloud/work/frontier/w9e/entity_tick_main/best.c
```

"Aligned rows" is the differing-row count from `tools/udiff.py` (an opcode-aligned LCS against retail). The
scorer's count is positional.

## engine_torque_calc: what was established

Start: codex B129 `group.c` (cloud/work/ipa-groups/codex_node_store_flow_b129). In the unit it gave 219/224
positional and 129 aligned rows. The best source is `engine_torque_calc/best.c`, about 60 variants.

1. **Wrong global in every earlier draft.** The scene-list pointer is `D_801392D0` (`lui 0x8014; -27952`), not
   `D_801493D0`. `D_801392D4` is its count. The codex group and its `engine_sound_update` use the wrong address.
   All the other addresses in the codex draft check out against retail.
2. **Deleted static func_800AB7D0 (the retail stub just before the function) is the state-assignment block**
   (`if (D_801174B4<<9 >= 0) {...} else if (...category 5 slot search...)`), as `func_800AB7D0(node, metadata,
   category)`, inlined. This is proved by the frame: only this split gives the retail frame of 96 bytes and puts
   `transform` at sp+64 (the uopt area trace shows 68 bytes of locals plus 4 of spill). The other splits tried
   (the 0x4000 tail, the animation block, the matrix block, the slot search, and the whole body) give a frame of
   88 or 112, or the wrong slot. Inlining costs about 8 bytes of frame per inlined parameter.
3. **Locals** (each one moves the frame): the outer function has `metadata, category, resource, value(u16),
   animation, transform` in that order. `resource` and `value` are real temporaries in the texture statement, and
   they also gave the two frame slots that unused pads had been filling. The helper has `index, j, matrix, scene,
   slot`.
4. Other source forms that moved words:
   - the else-branch test is `metadata->category == 5` (it re-reads the field, so the CSE goes to v0 and the
     parameter copy to a2, giving retail's `lb v0; bltz; move a2,v0`);
   - the scene index goes through a variable (`index = node->index`);
   - the animation stores are in the order state, data, node, time.
5. **Residual, after forcing colours (`tools/fu.sh a1 engine_torque_calc "p1:w2=c15,p1:w74=c14,p1:w38=c2,p1:w243=c3"`
   gives 27 aligned rows).** Natural source does not yet produce these colours:
   - node gets s0 and matrix gets s1 (retail is the reverse). Both webs have save=2.00, and the tie goes to the
     lower web number, which is the parameter. Retail must give node a lower priority (more blocks, or fewer
     weighted uses).
   - the slot-loop index and the constant -1 are swapped (retail index v1, -1 a0). The -1 web (save 10.0) is
     coloured before index (9.0). Splitting the scene index into its own variable should lift index above 10.
   - the address `&D_801392D0` is a coloured live range in retail (`lui a0; addiu a0; lw t6,0(a0)`, plus
     node->index in v0). A volatile read gives the lui/addiu/lw shape but in temporaries. These did not reproduce
     it: a local pointer variable, a struct `{list,count}`, and an array declaration.
   - the 3-iteration zeroing loop: retail tests `slti at,v1,3; bnez`, while we get a hoisted `li a0,3; bne`.
     `s16 j` gives the slti but loses the strength reduction. Tried and not it: `u32 j`, `do/while`, `j<=2`,
     `j+=1`, array fields, and chained stores.
   - prologue: `multu` and `lui s3` are scheduled differently.
6. **Next hypothesis:** the slot search is itself a separate statement block whose index is a different variable
   from the record index (which would split web 38). For the address live range, look for a second use of
   `D_801392D0` that retail optimised away (for example a bounds check against `D_801392D4` written as debug
   code).

## entity_tick_main: what was established

Start: the r5_e group (cloud/work/ipa-groups/drone_ai_update). In the unit it gave 644/671 positional and 838
aligned rows. The best source is `entity_tick_main/best.c`, about 10 variants.

- The retail frame is 368 bytes. Retail locals: `v[3]` at sp+324, a 4-byte struct colour at sp+340 (copied by
  struct assignment from `Color D_8011B464`, address held in s1), and `Node *nodes[3]` at sp+344 (an array, so it
  is reloaded after every store). sp+24..324 is never touched.
- The retail stub `func_8009309C` just before is most likely the inlined LCG helper
  (`seed = seed*0x41C64E6D+12345; return (seed>>16)&0x7FFF`). There are 12 call sites in the body, each inline
  adds 8 bytes of frame, and the body grows from 602 to 669 words (target 671). The frame is still 216 against
  368. Making the colour and transform block a 6-parameter inlined helper adds 48 bytes, but it scrambles the
  registers.
- Retail evaluates `mdl->id[2]`, `id[3]` and `id[0]` into a1, a2 and a3, with `-5.6f` in f14 and `id[1]` as a
  full-word CSE (t0) whose `<<16` sits in a0. That pattern looks like the actual-argument registers of an
  inlined call. It supports a second inlined helper (colour plus transform), but the parameter order is not
  settled.
- Own literals, read from the retail image (`tools/rd.py 80123A00 25`), in use order: -5.6, 0.1, 0.2, 0.15, 0.1,
  0.55, 0.1, 0.28, 0.15, 0.01, 0.025, 0.2, 0.025, 0.2, 0.01, 0.15, 0.58, 0.1, 0.55, 0.2, 10000, 1.35, 0.15,
  0.1. 0x80123A60 (0.025) belongs to drone_ai_update. 0.25, 0.5, 1.0 and 32768.0 are `lui` immediates.
- `drone_ai_update` (the real caller) has a frame of 400 against 200 for the r5_e draft and uses `ra` as a
  temporary (a mdl pointer at 0x80093bec), which is the inlined-callee signature. It needs its own pass before
  either function can be claimed.
- Next: test the "two inlined helpers" model (rand plus colour/transform) and look for what else takes about
  150 bytes below sp+324. A compiled-out debug buffer is the other candidate.

## Recovered types and globals

- `D_80117530[]` Metadata48 {name, field4, callback@8, animation@12, kind s16@16, flags u16@18, category s8@22,
  element s8@23, value f32@24}. `D_80117510[]` u8 per-category record size. `D_80150E68/E98[8]` per-category
  buffers (base and cursor). `D_80118DDC[]` static per-category bases. `D_80150F38` Matrix64 array.
  `D_80150F80` matrix count. `D_80150E7C` = `D_80150E68[5]` Slot136 array of 16. `D_801391F0` animation list.
  `D_80118DFC` Vec3 constant. `D_801392D0` Scene36* list, `D_801392D4` count.
- Node112: state ptr at +108, index s8 at +101, texture s16 at +88, resource s16 at +14, metadata s16 at +16,
  flags u8 at +4, matrix at +20, vec at +68.

## Tools (cloud/work/frontier/w9e/tools)

- `r.sh FN CAND [blob_unit args]`: unit score plus the aligned-diff summary (tag w9e).
- `udiff.py`: the w8a aligned diff, pointed at the w9e unit.
- `gtu.sh LABEL PROC`: snapshots the **unit** stage (not a single-file group) and runs the w5d-traced uopt
  (PRE listing and colouring order) for PROC. The report is in `runs/LABEL/report.txt`.
- `fu.sh LABEL NAME SPEC`: CDX_FORCE colouring oracle on that unit snapshot.
- `sp.sh` / `sp_remote.sh`: frame-area trace using a copy of w8a's uopt3 (in w9e scratch `bin/uopt3`).
- `rd.py ADDR N`: retail words and floats from `asm/us/blob_data/opaque.hex`.

## What generalises

- **Frame size is evidence about which block was inlined.** Each inlined parameter costs about 8 bytes of frame,
  and the inlined procedure's locals go below the caller's locals. Sweeping "which block is the deleted static"
  against the frame size and the address of one known local identified the helper in 6 compiles.
- **Coloured registers v0..a3 where retail shows plain loads** usually mean source variables (or inlined
  parameters), not CSEs. Introducing named temporaries (`resource`, `value`) cut 30 aligned rows in one step.
- **Check the global addresses in older drafts against retail.** The codex B129 draft used `D_801493D0` for
  `0x801392D0`.
- No permission denials. Nothing committed, spliced, or edited outside `cloud/work/frontier/w9e/`. No
  `unit_overrides.json` entries are needed yet (none apply until a strict match exists).
