# Runtime-A seven-option availability helper

Frozen base `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`; stock IDO 5.3
actual `-g0 -O3 -mips2 -G 0 -non_shared` group compilation.

## Observed scores

- New `func_80399394`: **22/22 strict**, **88 candidate bytes**, no unresolved
  relocation, unverified data, error or extra word. Only claimed body.
- Complete real renderer `func_803993EC`: **249/307 words differ**, 8 unverified
  own-data sites and 1 owned-data error from shifted native/candidate geometry;
  no unresolved symbol or extra word. Research context only.
- Complete real root `func_80399D10`: **293/335 differ**, 2 unverified own-data
  sites, no unresolved symbol/error/extra word. Research context only.

The helper takes one real index, reads a signed byte in the native seven-byte
rows indexed by player count, and conditionally checks option 18 for index 5.
Its s0 private clobber is naturally reproduced with the actual callers.
No fake formal, call, retention barrier, padding, forced register or assembly.

A follow-on native reread corrected the renderer's external byte symbol from
`D_80130BDC` to the actual `D_80140BDC`; the helper remains strict and the
renderer improves from 250 to 249 differing words.

Both callers are complete. The renderer has seven animated option slots and
the eighth rotating marker; the root has the actual repeated cleanup paths
and seven-way transition. The 64-byte object and 76-byte player views preserve
observed fields/strides, without claiming original type names. Opaque fields
are not new runtime storage. The selected-Y local follows the native producer:
it is assigned when the loop reaches the selected option. Normal caller state
must select 0..6; invalid selection would retain the native uninitialized-local
boundary. No invented fallback value is supplied.

External `func_803998C0` initialization remains unreconstructed. No claim is
made for either caller or for full source ownership, behavior, integration,
original source, or cartridge coverage.

## Data and qualifier evidence

The authenticated image supplies renderer float literals at
`0x803B951C..0x803B952C` (pi/2, 4*pi, pi, 2*pi), and root switch mapping at
`0x803B9560..0x803B957C`. The switch's SHA-256 is
`8f9e124ba25fbf09ae783e809389efaf5b6957310048e905102c67ddab27efc2`.
Cases 0..6 map to the native bodies in ascending order. Source-owned references
in the two nonmatching callers do not yet verify at shifted instruction sites.
`reproduce.py` authenticates the asset/image in memory for unchanged scorer
validation; no binary/image/assembly is published.

The published source keeps the ordinary `f32 D_8002EB94` declaration. A paired
one-change experiment with the existing accepted volatile view improved the
renderer positional count to 232/307 but added 13 extra words; helper and root
scores stayed unchanged. This is not a strict improvement or published
qualifier change. Existing same-symbol qualifier provenance is pinned
`cloud/matches/entity_collision_detect.c:20-21,50`,
`cloud/matches/func_8008B3C8.c:111` and `include/game_globals.h:51`.
The prior writer audit found no asynchronous writer; none is asserted here.

## Reproduce

Configure stock IDO and GNU MIPS tools, then run from a checkout containing the
fixed base's tracked asset:

```
python cloud/work/frontier/dot_runtime_a_seven_option_group_20261007/reproduce.py --repo .
```

For a small overlay, use `--repo OVERLAY --reference-root GIT_REPO`.
Only matching compilation/scoring was performed. Independent checker owns
acceptance tests, integration, promotion and merging.
