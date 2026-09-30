# drone_ai_update (0x80093B20), scout report

Feasibility: **LOW** now (IPA pair with a 671-word callee); potentially the biggest payoff (1,491 words for the pair). Effort: 3+ days.

## Facts
- 820 words, `asm/us/blob/blob_8008d0c0.s`. Not spliced. Called only through a function pointer (`&fn` in `save_load_data`; no `jal` callers), so its own entry ABI is `(void *ent, s16 flag)` (`a0`,`a1` home-stored). Frame 400, saves `ra`,`s0..s7`,`f20`,`f22`.
- **IPA-dependent.** It calls `entity_tick_main` (aliased `physics_velocities_O`, 0x800930A4, **671 words**, only caller is this function). `entity_tick_main` reads `s5` (a `s16` slot index) and `a0` as parameters, uses `s0..s7` without saving them, and keeps its own local at `s1 = sp+340`. Around the call `drone_ai_update` stores `t5`/`ra` to the stack and uses **`ra` as a general register** elsewhere (`addu ra,t6,t8`), classic -O3 IPA output. Closed group: `{drone_ai_update, entity_tick_main}`; all other callees (`model_data_load` x10, `func_80092B80` x4, `matrix_scale_apply` x3, `model_transform_setup` x2, `string_copy_format`, `func_80092FE0`) are ABI (`ipa_signals.py`), so they can be `context`. `matrix_scale_apply` reads `a3` (its caller leaves a stale a3; get its prototype right).
- 7 distinct callees, 21 globals, 3 loops, 8 multiplies (struct strides 0x44, 0x3B8, 0x808, 0x40), few FP ops except the colour lerp (`u32 -> float -> u8` conversions with the `cvt.w.s` + FCSR + 0x4F000000 sequence: use `(u32)` casts). By content it is a per-car light/palette/model updater, not AI: averages four palette entries, lerps by a distance factor, then `model_data_load`/`model_transform_setup` for a car's model slots. The docs name (`DoDrones`) is a guess.

## First pass
`seeds/drone_ai_update.c` (m2c plus small type repairs): 692 words vs 820, **aligned shape 57%, aligned exact 9%**. The `M2C_ERROR(cfc1)` regions (the colour lerp) are wrong until rewritten with `(u32)` conversions, which alone accounts for much of the size gap. `entity_tick_main` seed does not compile yet (m2c uses unnamed stack reads `unksp154..`; 270 lines).

## Approach
Group of two under `-O3`: first rewrite `entity_tick_main(s32 flag, s16 ipa_s5)` (671 words, 247 FP ops), then the parent. Use `zbuild.py --as1=-r4300_mul` only if `mul;nop;mul` appears (8 multiplies here: check).

## Risks
Two large IPA functions must both match before either splices; a single register-allocation divergence anywhere blocks the pair. Highest payoff, lowest probability.
