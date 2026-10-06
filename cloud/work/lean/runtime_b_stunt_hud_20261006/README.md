# Image B stunt/score HUD: lean research

Target: `B:func_80392894`, `[0x80392894,0x80392FE4)`, 1,872 bytes.
Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.

## Observed result

Standalone IDO 5.3 O3: **28 / 468 words differ**, zero extra words,
unresolved symbols, unverified relocations or comparison errors with
authenticated image-B data. The first complete reconstruction differed in
427/468 words. Expressing the alpha test as a direct floating truth test
instead of comparison with `0.0f` keeps the zero constant out of an earlier
call's argument register and removes the resulting whole-body phase shift.

Residuals include local-frame/buffer offsets, register allocation and a few
scheduling/operand-order differences. Candidate frame is 160 bytes; native
frame is 184. No pressure locals, unused variables or padding were added.
This is research evidence, not an accepted match or ROM-coverage gain.

## Reproduce

```
python3 cloud/work/lean/runtime_b_stunt_hud_20261006/reproduce.py
```

Actual flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The helper authenticates the frozen asset and complete image B in memory,
then runs the unchanged canonical compiler/scorer with that own-data context.
It intentionally reports NONMATCH. The required float literal at B:0x80394F50
is binary32 `2.69f`; image SHA-256 is
`b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd`.
No raw image/data/object file is included.

## Source and assumptions

One-pointer callback for the stunt/score HUD. It suppresses drawing for the
native global mask, selects a font, draws active per-effect counts and score
summaries, resets expired state, and draws timed multiplier/special messages
and the total score. Native record views are 120 bytes per score state and
72 bytes per effect, ten effects per player. A four-entry countdown array is
updated using the original -1/-2 sentinels and ten-frame reset.

Source names describe reconstructed behavior. Original type/variable names,
complete record meanings, a whole-function arcade donor and original local
buffer declaration are not recovered. The 32-byte candidate text buffer must
hold each formatted result including NUL. `fcvt_wrapper` remains the game's
external formatting helper, including its native same-buffer input/output
calls; no libc overlap contract is substituted. Player/table bounds require
indices 0..3 and selector 1..4. Effect kinds used to index counts must be 0..9
(the sentinel is 11). Alpha conversion to unsigned byte requires a finite
representable value for portable C behavior; language pointers must be valid.

Unknown fields preserve real native record storage. Helpers are external with
existing contracts. No invented callers/helpers, artificial volatile or
assembly. Only compile/scoring was performed. Independent checker owns
acceptance, further validation and ROM integration.
