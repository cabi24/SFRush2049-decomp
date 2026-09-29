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
