# MP_TargetSteerPos

**1 of 1 member MATCHES** (103 words), rescored 2026-09-30 with
`zbuild.py --as1=-r4300_mul` ([../../R4300_MUL.md](../../R4300_MUL.md)).
**Spliced** (`src/blob/groups/MP_TargetSteerPos`). Without the flag it is one
missing `nop` (a VR4300 multiply workaround) off, 44/103 words differing.

| function | role | target words | result |
|---|---|---|---|
| `MP_TargetSteerPos` | member | 103 | **MATCH** |
| `camera_transform` | context, `keep` | 557 | 549 differ; seed emits only 456 words, untouched |

## MP_TargetSteerPos

Starts a sound and sets two byte parameters:

```c
if ((s->handle = func_80020174(s->id, 0xFF, 0xFF)) == -1) return 0;
func_8001fea4(s->handle, (u8) (u32) (pan * 127.0f));
func_8001fff4(s->handle, (u8) (u32) ((vol + 1.0f) * 0.5f * 127.0f));
s->started = 1;
return 1;
```

Key fix: both conversions are **float to `u32`** (`cvt.w.s` with FCSR
rounding-mode and `0x4F000000` = 2^31 sequence), then truncated to a byte; the
seed used a signed conversion.

## camera_transform

Context only. It calls `MP_TargetSteerPos` and has two callers outside the
group (`resource_process_thunk`, `func_80098FB8`); it is in `keep` so the
member lands alone. Not started (557 target words).

## Relocations

Calls `func_80020174`, `func_8001fea4`, `func_8001fff4`; no data relocations
(constants are `lui` immediates).
