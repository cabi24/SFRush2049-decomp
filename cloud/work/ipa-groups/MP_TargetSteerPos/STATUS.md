# MP_TargetSteerPos

**BUILDS.** `MP_TargetSteerPos` is one `nop` from matching; `camera_transform`
is untouched (seed).

```
$ python3 tools/cloud/score.py group cloud/work/ipa-groups/MP_TargetSteerPos
MP_TargetSteerPos:  44/103 words differ
camera_transform:   549/557 words differ
```

(The maintainer's figure of 445 for `camera_transform` predates the
`c4d4fe7` reseed; the current seed scores 549/557.)

## MP_TargetSteerPos

Rewritten from the assembly. It starts a sound and sets two byte
parameters:

```c
if ((s->handle = func_80020174(s->id, 0xFF, 0xFF)) == -1) return 0;
func_8001fea4(s->handle, (u8) (u32) (pan * 127.0f));
func_8001fff4(s->handle, (u8) (u32) ((vol + 1.0f) * 0.5f * 127.0f));
s->started = 1;
return 1;
```

The key fix: both conversions are **float to `u32`** (IDO's `cvt.w.s` with the
FCSR rounding-mode and `0x4F000000` = 2^31 sequence), then truncated to a byte.
The seed used a signed conversion.

Words 0x000-0x0EC now match exactly. The target then has one `nop` between
`mul.s $f6,$f10,$f4` and the dependent `mul.s $f8,$f6,$f24`; our build does
not, so every later word is shifted by 4 and differs (all 44 remaining
diffs, including the early-return `b` offset at 0x024). Tried: a temp for
`(vol + 1) * 0.5`, explicit parentheses, `(1 + vol)`, `* 127 * 0.5`,
`vol * 0.5 + 0.5`, `/ 2`. None changes it. It may come from as1 scheduling
that depends on something outside this function; not resolved.

## camera_transform

Not started (557 words). It calls `MP_TargetSteerPos` and has two callers
outside the group (`resource_process_thunk`, `func_80098FB8`). If
`MP_TargetSteerPos` is resolved first, `camera_transform` can move to
`context` so it lands alone.

## Relocations (for review until F2 lands)

`MP_TargetSteerPos`: calls `func_80020174`, `func_8001fea4`,
`func_8001fff4`; no data relocations (constants are `lui` immediates).
