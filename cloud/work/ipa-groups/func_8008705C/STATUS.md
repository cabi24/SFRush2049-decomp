# func_8008705C -> 45/45 MATCH (real function, extent 45 words)

## Current status (rescored 2026-09-30, master d0891f3)

**45/45 MATCH** (strict, `extscore.py --norm`: exact after alignment 45/45, structure 1.000; extent 45 words).
Caveats: matches only with stand-in callers and a `func_80086A50` stand-in (`stub.c`; retail is 387 words),
so the file is not spliceable as is. Closure gap: the real `func_80086A50` (387 words).

Real head (0x8008705C, `lui v1,0x8013` then `addiu sp,-24`), ABI parameter `a0` = mask; clears
the bits in `D_8012E608` and emits RDP set-other-mode words (`0xE2001E01`, `0xE2001D00`) into
the display list `D_80149438`, calling `func_80086A50(D_8014A248)` for bits 0x10 and 0x20.
Callers: `func_8008A46C`, `Input_ProcessGameplayPad`, `audio_doppler_calc`. Extent 45 words
(single `jr ra`); INDEX's 869 insns belong to the caller `audio_doppler_calc`'s cluster, not to this function.

Verify: `python3 cloud/work/tools/extscore.py cloud/work/ipa-groups/func_8008705C`

Tricks that got the match:
- display-list pushes written as `Gfx *g = D_80149438++; g->w1 = 0; g->w0 = ...;`
- the mask must live in `$t0` across the `jal`. That needs the callee `func_80086A50` in the
  group with a body that clobbers `a0-a3` but NOT `t0`. `stub.c` is a stand-in (4 params, the
  prototype in `f.c` is unprototyped `func_80086A50()`; a dead `switch` in the stub stops
  umerge inlining it). Retail `func_80086A50` is 387 words (a 5-way switch); the stub is
  not a match of it.
- `func_8008705C` is NOT in `keep` (two stand-in callers), yet keeps ABI `a0`.

Blockers: `func_80086A50` (387 words) is a context stand-in only.

## Spliceability

Unspliceable as is: MATCH, but callers and `func_80086A50` are stand-ins. The stand-ins only reproduce the IPA register/frame context; they are not
the retail callers, so this C cannot go through `blob_splice` until the real callers are in the group
(or the maintainers' whole-module IPA is used). Scores above were reproduced on 2026-09-30 with `extscore.py --norm`.
