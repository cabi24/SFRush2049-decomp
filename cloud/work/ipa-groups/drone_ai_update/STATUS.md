# drone_ai_update + entity_tick_main (first pass, cloud round 5, agent r5_e)

**No claims.** Hand-written first pass from the assembly and `cloud/work/bigfish/seeds/`;
it builds and is structurally close, but nothing matches strictly.

Scored with `python3 cloud/work/r5_e/gnear.py cloud/work/ipa-groups/drone_ai_update [--diff FN]`
(whole-program `-O3` build with `as1 -r4300_mul`, then aligned LCS against the retail words;
`zbuild.py` gives the same 0 strict words):

| function | target words | emitted | aligned exact | aligned opcode shape |
|---|---|---|---|---|
| `drone_ai_update` | 820 | 792 | 290 (35%) | 671 (81%) |
| `entity_tick_main` | 671 | 676 | 129 (19%) | 569 (84%) |

## Facts established

- The pair is a closed group: `drone_ai_update` has no `jal` callers (function pointer only, so it
  goes in `keep`) and `entity_tick_main` has exactly one caller. Every other callee is ABI.
- **`math_utility` is not IPA.** It is an ABI 3x3 `f32` copy (`math_utility(src, dst)`, already in
  `src/blob/math_utility.c`). The m2c seed's 6-argument call is a stale-register artifact. The only IPA
  register in `entity_tick_main` is `s5` = the car index.
- `entity_tick_main(s16 ipa_s5, s32 flag)`: the prologue's `sw s5,368(sp)` is home slot 0, so the IPA
  parameter is the **first** parameter and `flag` (in `a0`) the second. Writing it that way (and
  calling `entity_tick_main(carIdx, isHidden)`) moved `drone_ai_update` from 270 to 290 aligned words.
  With that order the IPA register came out as `s5` as in the target.
- Key types recovered (see `group.c`): `ModelSlot` (0x44, table `D_8012E700`: flags bit 31 = hidden,
  `Node *node` at +8, `u16 tex` at +0x14, child/sibling at +0x16/+0x18, two colours at +0x3C/+0x40);
  `CarModels` (0x40, `D_80139320`: four slot ids, then two more at +0x10/+0x14); `Car`/`CarState`
  offsets for `player_array` (0x3B8) and `D_8014A250` (0x808); `Node` = 3x3 matrix + translation.
  `entity_tick_main` puts `node + 2*col1 + K*col2` (K = `D_80123A00`) into the translation of three
  model nodes (after `math_utility` copies the matrix), then jitters per-car scales with the game's
  LCG (`*0x41C64E6D + 12345`, `>>16 & 0x7FFF`, `/32768.0f`) and applies them.
- The colour lerp in `drone_ai_update` needs `(f32)(u32) byte` conversions (the retail code has the
  `bgez; add 4294967296.0f` unsigned fix-up) and a `(u32)` float cast on the way back (the `cfc1`/`0x4F000000` sequence).
- `func_80092FE0` (callee, 47 words) is matched separately: `cloud/matches/func_80092FE0.c`.

## Known differences (in order of size)

1. **Frame of `entity_tick_main`: target 368 bytes, ours 56.** The retail function never touches
   `sp+24..sp+324`; its live locals are `vec[3]` at 324, a colour word at 340 (accessed through
   `s1 = sp+340`, so its address is materialised), and three node pointers at 344..352. At `-O3` an
   unreferenced local (scalar, array, struct member, or union) reserves nothing, and a referenced
   `volatile` array adds extra stores and overshoots (432). What occupies the other ~300 bytes is unknown.
2. `drone_ai_update` palette averaging and colour blocks: scheduling/register order differs
   (aligned exact 35%); the structure of all branches is right (shape 81%).
3. `entity_tick_main` constants: the target hoists `32768.0f` (`$f18`) and `0.25f` (`$f12`) and keeps
   the LCG multiplier in `a0`; ours reloads them per use and uses `v1`.
