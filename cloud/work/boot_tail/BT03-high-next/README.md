# BT03-high: remaining under-64-byte functions

Seven new strict local matches, **248 B / 62 words**. Two complete NONMATCH
reconstructions, **80 B**, remain research only. No cartridge coverage or
promotion is claimed. Independent paired source/ABI review passed; exact aggregate-head CI is pending.

- Branch: `dot/boot-tail-bt03-high-next`.
- Clean source base: fresh master `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Central activation: `6dd6c6a4`, second-wave claims and D10.
- All nine targets are in the assigned `[0x80017500,0x80020610)` partition.
  The entire C11 `[0x8001E0E0,0x8001E9B0)` interval remains excluded.
- First-wave commit `53bff645` remains frozen and separate. This packet has no source
  dependency on that wave or unmerged research PR #59; earlier clustering/status
  is only a read-only reference. Central generated files and D10 are untouched.
- Fresh-base preflight: three target SHA256SUMS members pass; all 439 starts/sizes
  equal inventory, totaling 99,120 B; preexisting `80010A00` strictly matches.
  The pinned compiler installation from the completed Packet 2 check is reused
  through IDO_DIR; no compiler or build-tool source is modified.

## Frozen population

| Function | Bytes | Result | Native operation |
|---|---:|---|---|
| `8001D1C0` | 52 | NONMATCH 3/13 at O2 | If enabled, test word-at-+8 flag 0x10000 and return byte boolean |
| `8001D928` | 28 | strict O2 MATCH | Clear three global bytes |
| `8001E0C0` | 20 | strict O2 MATCH | Clear two global words |
| `8001EAEC` | 36 | strict O2 MATCH | Increment unsigned counter, return prior value, skipping sentinel -1 |
| `8001F864` | 52 | strict O2 MATCH | Call two no-argument initializers, then clear two global bytes |
| `800201D0` | 48 | strict O2 MATCH | Validate an input identifier; return input on success, -1 on failure |
| `80020200` | 28 | NONMATCH 3/7 at O2 | Read signed halfword table indexed by input's low nibble |
| `80020518` | 16 | strict O2 MATCH | Store input byte in global selector |
| `80020528` | 48 | strict O2 MATCH | If enabled, forward an actual state pointer to resident wrapper |

Exact flags are `-g0 -O2 -mips2 -G 0 -non_shared`; the unchanged scorer appends
`-Wab,-r4300_mul`. All seven submission sources have relocated full-word equality,
no excess words, unresolved symbols, local-data uncertainty or errors. Native
boundaries, stack operands and delayed stores are included.

## ABI review and rejected score-only candidate

The global clearers have no inputs. `8001EAEC` returns the counter's previous word;
unsigned arithmetic preserves native wraparound and excludes only `0xFFFFFFFF`.
The callees of `8001F864` initialize global state and do not require incoming
arguments. `800201D0` forwards its one genuine word input to `8001EDF4`, whose
entry tests a0 against -1; it preserves that input across the call and returns
it only when validation succeeds. `80020518` stores a byte argument; its input
home spill and low-byte store are both reproduced, without extra formals.

Important classification warning: the excluded census row for `80014BD8` was
labeled an identified `__d_to_ll`-family stub. Its canonical body instead contains
a call to `80010E80`. That body saves incoming a0 and dereferences a halfword at
pointer offset zero. Therefore an in-scope caller must forward the real pointer;
the census name is not authoritative ABI evidence.

The first `80020528` experiment had score zero with a no-argument declaration but
was rejected during this transitive ABI check. `initial_controls.json` explicitly
labels it REJECTED-ABI and `accepted_submission: false`. The final source declares
`void func_80020528(void *state)` and calls declared-only
`func_80014BD8(state)`. This correct pointer-forwarding form also strictly matches.
No excluded function is reconstructed or claimed, and no census/target is edited.
The maintainer should revisit the masked-skeleton classification separately.

Original public names, middleware version and full types remain unknown. The two
archive sources retain directly evidenced word/halfword access widths and real
inputs. Their role names remain hypotheses; no source ancestry is asserted.

## Bounded controls and next hypotheses

Initial O2 then O1 controls for nonmatches are in `initial_controls.json`.
`refinement_controls.json` records directed natural-source refinements. No target
exceeded eight source forms. Successful initial matches were left unchanged
except comments; the validator `800201D0` closed on its second source form by
using an early return instead of a ternary result temporary.

Before refinement, `tools/workbench.py diagnose` classified the validator as a
one-instruction structural difference and both residual leaves as allocation
mismatches. Temporary canonical objects were constructed only for diagnosis and
are not published. All final acceptance uses the strict relocated scorer.

- `8001D1C0`: O2 3/13; O1 10/13 plus two nonzero extra words. The direct byte-boolean
  form has the right frame-free structure. Meaningful flag/result locals,
  explicit unsigned comparison and conditional result controls worsened allocation
  or structure. Next: authenticate original boolean expression/type lowering to
  explain native reuse of the return register across mask and truth conversion.
- `80020200`: O2 3/7; O1 5/7. Structure and accesses agree; the temporary register
  pair is shifted. Signed-byte/halfword argument and wider return controls did not
  help and are not submitted. Next: recover authentic narrow-formal/index lowering
  context before further temporary-register experiments.

There is no extent, local-rodata or protected-tool blocker. No artificial keeper,
padding local, fake input, callee body, assembly insertion or flag sweep was used.
Archived nonmatches are not counted as verified bodies.

## Reproduce

```sh
python3 cloud/work/boot_tail/BT03-high-next/verify.py
```

The script verifies the exact source flag headers, canonical claimed extents,
seven matching bodies and both nonmatches at O2/O1. `verification.json` binds
all eleven results to source SHA-256 hashes. `status_delta.csv` is the sole central
writer's integration input. Publication/CI remain owned by that integrator.

Only this packet directory and the seven submission sources are changed. No ROM,
raw assembly dump, object, secret, target, scorer, symbols, layout, locks, runtime
image, farm, forbidden helper investigation or production gate is changed or
published.

## Independent review

The paired BT05/BT07 reviewer independently compiled all eleven final controls
from a separate clone and verified the exact frozen source hashes. Its
`independent_review.json` confirms seven strict bodies and two honest nonmatches.
The reviewer specifically confirmed the real pointer ABI through `20528` /
`14BD8` / `10E80`; the rejected no-argument seed is not a submitted source.
