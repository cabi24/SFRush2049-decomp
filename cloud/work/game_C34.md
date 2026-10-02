# Game C34 — complete packed-resource header parser

Ready: **func_800AB638, 212 bytes / 53 words**. Exact flags: `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`. The first canonical source compile and a fresh independent replay both pass strict scoring with stack differences enabled (0) and full unmasked linked comparison (all53 retail words exact). No extra words, unresolved/unverified references or errors. Current actual game locks and full prior name/address reports were checked before reservation; A60 F7A98 and B39 E7DD0 were excluded. No shared accepted paths, headers, layout, locks, state or compiler/scorer settings changed.

The original complete no-argument leaf reads a genuine packed header through D_801526DC and preserves the original pointer in D_801526F0. Six unsigned16 counts reside at offsets0,2,4,6,8,10; four reserved bytes complete the16-byte header. The first section starts at header+16, and successive section boundaries are computed using original strides132,20,24,8,32 and1. Every pointer is written to its existing named global; the fifth count is also copied to the existing unsigned16 D_8015267C. The source's pointer assignments reproduce the original reloads and arithmetic without invented locals, extra inputs, caller pressure, data, or operations. Header/record semantics are structural only: no guessed resource category names were needed.

Source SHA256 `45361aa4d65cd25f75f43ade772e7dc6d25b01ac5e7351e72a65ab8706ff4702`; authoritative target SHA256 `fc88617c74685ac44afa00a63eea43e78da6f4d10eb53cbbdefc837e54cba237`. Read-only exported target object: Rocky ~/agents/C/scratch/game-C34/func_800AB638.target.o. No new owned data or external callee/context is required.

Minimal frozen packet: game_C34/func_800AB638.c, verification.json, frozen_verification.json, verify.py, context_provenance.json, targets.json. baseline.json records the original compile. Copy these and target object to independent scratch, then replay:

```sh
python3 verify.py --repo "$HOME/agents/D/wt" --toolkit "$HOME/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5" --target func_800AB638.target.o --output independent.json
```

Screening rejected misleading aliases gfx_lui, mirror_mode_toggle, vector3d_store_transform, matrix_rotation_build, input_analog_read, voice_playback and random_range: their declared entry heads consume registers established before the supposed function, or expose unsaved callee-saved/FP state. They were not compiled or claimed, and no stand-in prologue/formals were introduced. A3424 is genuine but traverses the same allocation-heavy772-byte record family as C33, so it was left unreserved in favor of this clear utility. All private jobs are complete.
