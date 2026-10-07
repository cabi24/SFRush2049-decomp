# D816C menu geometry and its real D91A0 caller

New source-context research, **NONMATCH**, with no production or accepted-byte
claim. Branch/tool/source base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
The current master definition index and the additional C bodies in published
PR #163 tree `277d344e5d9a0edf6eea7732fe7d770cb04aed25` contained no complete
C definition of either function. This is not a replacement for an older
near-match. The observed native bodies, not donor code, are the primary source.

## Recovered context

- `func_800D816C`, `0x800D816C`, 2,796 bytes / 699 words: updates a scrolling,
  rotating set of menu objects. Twelve 64-byte entries occupy each 0x440-byte
  player record. It maintains positions and angular transitions, constructs
  matrices, selects textures, computes alpha, and hides/shows objects. Slot 8
  is omitted; slot 10 also updates the extra object.
- `func_800D91A0`, `0x800D91A0`, 3,860 bytes / 965 words: the actual sole direct
  caller, including initialization, four advancing 76-byte input records,
  selection navigation, duplicate-option handling, and completion paths.
- Four complete existing matrix/predicate definitions accompany the pair.
  They come from the pinned base's `src/blob/func_8008B32C.c`,
  `func_800B5898.c`, `func_800B5940.c`, and `func_800D8154.c`.

Names and C types describe observed accesses, not recovered original names.
The first target's logical argument is a signed player byte; native entry
receives it in private register t2. The candidate group does not reproduce
that register allocation. The caller and all target logic are real bodies;
there is no stand-in caller, pressure array, asm, or invented qualifier.

## Actual canonical result

Flags: `-g0 -O3 -mips2 -G 0 -non_shared`; standard `compile_group` stages,
with the canonical as1 `-r4300_mul` workaround. `group.json` records all real
files and retained roots. Claims remain empty.

| Body | Differing words | Emitted words | Native / candidate frame | Extra nonzero | Unverified relocs |
| --- | ---: | ---: | ---: | ---: | ---: |
| D816C | 654 / 699 | 672 | 232 / 296 | 0 | 10 |
| D91A0 | 910 / 965 | 1060 | 240 / 384 | 92 | 6 |

Both have zero unresolved relocations and zero score errors. All four reused
helpers score 0 differing words with no unresolved/unverified relocations or
errors. Those helpers are previously accepted source and receive no new
matching credit. Their production recipes are unchanged; this research group
uses O3 throughout, including the scale helper previously stored with O2.

The first complete paired development version used external symbols for five
read-only float constants and retained a redundant signed-argument copy. It
scored 676/699 with 681 emitted words. Restoring the observed literals and
removing that copy gives the 654/699 result above. This is a development-stage
comparison, not an improvement over an accepted or earlier published body.
The absent prior complete source, large remaining residual, short target
extent, and unverified owned-literal relocations are explicit limitations.

## Source-recovery corrections and assumptions

The ordinary seed normalizer introduced a synthetic global load at D91A0
+0x3B8, replacing the real advancing-record access with fixed D_8014A119.
The recovery pass retained already-formed pointer-relative loads instead.
The resulting source advances the input-record pointer by 76 each iteration.
The protected instruction stream and scorer were never rewritten or patched.

Other seed corrections are explicit source semantics: recover the actual
signed player passed in t2; preserve the caller's 10-entry table, counters and
25/19 exclusions across known private leaf calls; use byte-correct pointer
arithmetic; represent the observed RGBA word/alpha-byte overlap with a union;
and express the two native unsigned-byte float conversions as C casts.
The five target literals are ordinary float constants (tau, 0.4, half-pi,
four-pi, pi); the caller's scale literals are 0.9 and 1.3. Their placement is
still unverified by the canonical comparison, as reported above.

The pair does not claim a full original translation unit. Remaining external
private helpers include `drone_throttle_calc`, `render_replay_ui`,
`render_results_screen`, and the selector-related bodies. Their declarations
come from existing complete research/source interfaces where available;
`func_800B4FB0`'s ordinary one-word interface follows its entry and all observed
calls, but its body is not reconstructed here. These limits may affect
private register/clobber composition and are not hidden by fake prototypes.
No behavioral-equivalence, full-test, integration, ROM, or hardware claim is
made. The independent checker owns acceptance and further integration.

## Reproduce

With the repository's canonical compiler environment:

```sh
python3 cloud/work/frontier/dot_menu_geometry_20261006/repro.py
```

This compiles the provided real group and reports scores, complete ELF
extents, frames and relocation counts. Generated objects stay under `build/`.
No protected data, raw assembly, binary output, or compiler changes are
part of this packet.
