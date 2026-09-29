# func_800E56F8

Regenerated 2026-09-29 by `blob_group seed` (IPA-mode m2c with patches 0001-0005).

BUILDS (cloud pass 2026-09-29) after declaring `fabsf`/`sqrtf` as IDO intrinsics (the seed left them undeclared, so they compiled as implicit-`int` external calls). Not matching; the body is the m2c seed. See the `camera_scene_manager` STATUS for the same fix.

With `-r4300_mul`:

```
func_800E56F8  356/359   (size 376/359)
func_800E6AF8  316/334   (size 337/334)
```

Not built together with `func_800E4300`, which also links to this cluster
(see that group's STATUS).
