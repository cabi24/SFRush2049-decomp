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
