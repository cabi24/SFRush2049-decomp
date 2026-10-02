# Game C26: unsigned float conversion and real vector sources

Frozen ready ordinary source: `game_C26/render_helper.c`, 232 bytes, source SHA256 `1b348028763e29942e3e319160adfc22b9f3e04da6b18e1c285d64da6c106743`. Exact first-line flags: `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul`. Canonical stack-sensitive score0 and all58 complete linked retail words exact, with no unresolved/unverified references, relocation errors or extra words. Target SHA256 `1c1539a4d7207b66edb15512158eedbdee1597e531b75bc6ac90314aeee3c654`, exported privately to Rocky `~/agents/C/scratch/game-C26/render_helper.target.o`.

The source is reconstructed directly from the complete original assembly: scalar f32 input in f12, named D_80114744 store, bit0x10 update on named D_80149B88, and actual func_8008A644 call taking an unsigned16 argument. Its float-to-unsigned conversion is genuine `(u16)(u32)value`; the compiler emits the entire original CFCSR conversion/fallback sequence. No manually modeled CSR instructions, unused formals, pressure variables or owned data were introduced. It matched on its first compile and the frozen independent replay matches again. Minimal context is local scalar typedefs, three original named declarations, and the actual callee prototype. Entire232-byte body is accounted for without padding.

The ready artifact list is `render_helper.c`, `verification.json`, `frozen_verification.json`, and `verify.py`. Replay:

```sh
python3 PACKET/verify.py --repo ~/agents/D/wt --toolkit ~/rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5 --target PRIVATE/render_helper.target.o --output D_C26.json
```

Parent independently verified the identical frozen file and owns the normal lock, image, ROM and commit gates. Shared accepted sources/header/layout/locks/state were not changed by this worker.

Separate bounded RNG source `func_800B23E0.c` reconstructs the actual LCG, real indexed allowed-bit table and unsigned floating random-bit selection. Baseline strict565/all67words with24differences; an explicit meaningful intermediate f32 value improves strict485/13differences with the exact body extent. Eight targeted controls are recorded in `controls.json` and `controls2.json`; mask signedness, return width, loop form and actual local seed do not produce a match. This source remains unaccepted. Early diagnostic SELECT used nonexistent name column and was corrected to target_id before exporting authoritative objects. A private control-generator quoting failure produced no compile or evidence; corrected script results alone are recorded.
