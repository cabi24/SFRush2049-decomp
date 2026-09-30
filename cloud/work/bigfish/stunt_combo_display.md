# stunt_combo_display (0x800D3B28)

Feasibility: **low-medium (about 10-15 %)**. Effort: 4-7 days. Bigger payoff (1,175 words) but
floating-point register allocation across 1.2k words is the hard part.

## Facts
- **1,175 words** (4,700 bytes; the "5068 bytes" in `info.txt` is a historical label). Not spliced.
- **The name is a label only.** The body is a scripted/path camera driver: reads `arg0->0x7C6`,
  the `PathGraph` at `D_801407F0` (16-byte nodes, `s16` points, as in the matched `func_800B9B64`
  group), `sinf`/`cosf`, `camera_follow_path`, `camera_trigger_check`.
- Frame 400, saves ra, s0-s3 and **five FP registers f20-f28**; 230 FP instructions, 106 branches
  (20 likely), 29 `jal`, 11 `lui` bases, 18 mult/div.
- **ABI, not IPA:** one caller `func_800E543C` with `a0` only (`move a0,a3`). The function spills
  its own caller-save values across calls (`sw t0,400(sp)` ... `lw t0,400(sp)` around
  `camera_follow_path`, `race_countdown_display`), which IPA would not do. Callee prologues read only
  `a0-a3`/`f12,f14`: `camera_follow_path` (371 w), `camera_trigger_check` (308 w), `func_800D348C`
  (421 w, only caller is this function), `race_countdown_display` (280 w), `difficulty_select`,
  `lap_count_select`, `func_800C4180`, `func_800C40E8`, `math_utility`, plus already-matched
  `func_800D14F4`, `func_800C4C9C`, `func_8009E820`, and libm/libultra `sinf`, `cosf`, `viDeadlinePassed`
  (0x800015BC). Each callee only needs a correct prototype. Compile-alone `-O2` (+ `-r4300_mul`).
- 29 distinct call sites but only ~16 distinct callees; a few repeated blocks (one `sinf`/`cosf`
  pair is used twice). Not library code; not particularly repetitive.

## First pass
- m2c seed (own asm via `tools/`): compiles after 6 mechanical fixes (prototype arity conflicts
  neutralised with K&R decls, `s16` path-point reads, two float-to-pointer casts zeroed).
  **1,140 words vs 1,175; opcode-aligned 78.2 %** (the best seed of the five), opcode+regs 19.9 %,
  exact-word LCS 57.
- The seed is still a gotos-only dump (unreduced graph) and register assignment is nowhere near
  (target uses `f20-f28`, `t0` spilled at 400(sp), `t3` at 116(sp)).

## Recommended approach
- Do this second, after `net_session_update`, or only if that one lands. Type the record
  (`arg0` fields 0x660 `f32`, 0x678/0x6E0 vectors, 0x7C6 `s16` index, 1640/1656/1728/1740/1744/1990
  offsets are already visible in the seed), reuse the `PathGraph` typedefs from
  `cloud/work/ipa-groups/func_800D2FA8/group.c`, write natural control flow phase by phase.
- FP allocation is the risk: the ROM's f20-f28 assignment follows uopt web order, which is steered
  by named locals vs expression temporaries (see R2 findings).
