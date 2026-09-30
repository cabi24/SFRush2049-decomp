# func_800E92C8

Regenerated 2026-09-29 by `blob_group seed` (IPA-mode m2c with patches 0001-0005).

BUILDS (cloud pass 2026-09-29) after declaring `fabsf`/`sqrtf` as IDO intrinsics (the seed left them undeclared, so they compiled as implicit-`int` external calls). Not matching; the body is the m2c seed. See the `camera_scene_manager` STATUS for the same fix.

With `-r4300_mul`:

```
func_800E92C8  184/197   (size 188/197)
func_800E95DC  376/421   (size 402/421)
func_800EA108   96/117   (size 98/117; 2 .rodata relocations unverified)
func_800EA2DC   43/70    (size 72/70)
```

## Pass 2 assessment (2026-09-29, not worked on)

Closure is complete (`func_800E95DC`, `func_800EA108`, `func_800EA2DC` are roots
with outside callers; `func_800E92C8` is their IPA callee with `$s0/$s4/$f22/$f24`
parameters and already has a stand-in). The seed bodies are *structurally* off
by tens of instructions, not just registers: e.g. `func_800E92C8` is missing
the `abs.s` on its local vector and builds its stack arrays differently
(target locals at `sp+124..156`, ours `sp+108..140`), and `func_800EA2DC`
passes `ipa_f24` in `sp+12` (the build gives the IPA callee a different
parameter register set because its body differs). Start from the assembly, not
the seed; the techniques in `func_800D2FA8`, `func_800E56F8` and
`func_8008B640` STATUS files (locals layout, `volatile` frame padding, operand
order) apply.

## Pass 3 (hand rewrite from the assembly, typed structs)

All four members rewritten by hand (`Cr` car, `V3`, `Fx` per-player effect record, real arrays for the
stack vectors). With `-r4300_mul`:

```
func_800E92C8  114/197 differ  size 197/197  (was 184/197, size 188)
func_800E95DC  401/421 differ  size 425/421  (was 376/421, size 402)  switch(*st) form; an if-chain is 417 words
func_800EA108   58/117 differ  size 116/117  (was  96/117, size  98)
func_800EA2DC   MATCH          size  70/70   (was  43/70)
```

What worked: `len = 0` (int constant) splits the 0.0 web so 100.0f/1.0f land in `$f26/$f30`; `r < 0` and
`1 - x` with int literals give the target's separate `mtc1 zero`/`lui 3f80` loads; `volatile s32 padv[12]`
declared after the vectors gives the 160-byte frame (locals at sp+124..156); `volatile s32 padv[5]` first
in `func_800EA2DC` gives frame 144 and the exact ROM; in `func_800EA108` the ROM keeps `pl` in the slot
just below the vectors (declare it last) and has two unused named scalars above them (declare `i`,`j` first).
Loop bounds of 30.0f (0x41F0) not 32.0f. The `D_8014A250` records are 0x808 bytes.
Left: `func_800E95DC` saves `s8` (we use one more callee-saved register than the ROM, which spills `car`
and `pos` at their home slots instead); `func_800E92C8` x/y/z temporaries land in `$f2/$f12/$f14`
instead of `$f12/$f16/$f14`; `func_800EA108` loop control (pointer walk + `sltu`).
