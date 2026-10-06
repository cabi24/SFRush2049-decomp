# Runtime-A option scene: C93C at 220/234 words

Frozen base `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Stock IDO 5.3 group pipeline, actual flags
`-g0 -O3 -mips2 -G 0 -non_shared`.

Complete new research bodies and observed strict code scores:

| Function | Native bytes | Differing words | Extra nonzero words |
|---|---:|---:|---:|
| func_8038C93C | 936 | 14/234 | 0 |
| func_8038B834 | 252 | 45/63 | 0 |
| func_8038ACB0 | 416 | 97/104 | 3 |
| func_8038DFEC | 1,520 | 380/380 | 38 |

C93C is a distinct initializer sibling of #230's B560. It loops over 48 real
64-byte slots, applies special transforms/textures to slots 42–47 and sets the
initialized/dirty state. The 72-byte frame, local offsets and integer allocation
match. The residual is the same f20/f24 constant/Y-snapshot swap and associated
load scheduling observed in B560. Complete native geometry is 234 words.

The genuine DFEC caller builds the option list, obtains and applies the selected
mode-specific value, initializes objects and constructs a 19-position carousel.
B834 checks the observed 0–6, 6–14, 14–18 and 18–19 option ranges; ACB0 combines
a 21-option-per-mode table with native restrictions. No missing jump-table case
mapping is guessed. All paths are reconstructed, but behavioral equivalence is
not established.

DFEC is kept as a root in this minimal group. Its native outer caller is not
included, so its save/frame and private-callee behavior remain different. ACB0
has one natural index formal; no fictitious first argument was added to force
the native a1 parameter register. No synthetic caller, retention barrier,
forced register or unused frame filler is present. B834 remains nonmatching;
its pointer-range spelling is a native-layout research view.

Native floats `D_803B930C` through `D_803B9328`, color `D_803B8394` and other
address-resolved tables remain external with unresolved value/original ownership.
The source does not invent their literal values. Main-blob `sign_extend_call`
stays external; the actual returning implementation is a separate sidecar listed
only in `external_sources`, never compile inputs. Its return is consumed by the
native initializer.

All four have no unresolved symbols or unverified own-section relocations in
this observation. All `claims` remain empty: zero accepted-byte credit and no
cartridge-coverage claim. The earlier A634 body is a true external declaration,
not repeated or retried here; this draft does not duplicate #171/#230 credit.

## Reproduce

With stock IDO and GNU MIPS tools configured:

```
python tools/cloud/score.py --targets asm/us/ovl_a group \
  cloud/work/frontier/dot_runtime_a_option_scene_20261006
```

Matching compilation/scoring and residual diagnosis only. No proof packet,
acceptance suite, CI wait, production change, lock or promotion.
