# Independent AE940 review

**Pass for the complete standalone O2 source submission.** This is not image,
ROM, hardware, promotion or accepted-coverage evidence. New accepted bytes: zero.

Reviewed against a fresh, minimal snapshot of master
`7e62ed7b3c3e6f1e788bf013419b89e132b4c65d`, independent of the author's scorer
copy and compiled object. Final source SHA-256:
`4f1a98fd247bba5dc0c0f923c83e8ac12dae4872c27f7709d6ea5526b4c8eb15`.

## Matching and source

- Protected image-A extent is `[0x803AE940, 0x803AECC4)`, exactly 900 bytes /
  225 words. Its scanner evidence is `data_ref` plus `prologue`; neighboring
  function boundaries agree. Image A has its own population, separate from
  static and compressed-game coverage.
- Fresh IDO 5.3 with `-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul` gives
  strict zero differences, zero excess, and no unresolved, unverified, masked
  or erroneous relocation sites. No other optimization level is claimed.
- The complete object contains one 900-byte `STT_FUNC`, 12 zero alignment
  bytes outside that extent, 35 standard relocations, and no owned data.
- GNU links the unchanged complete object at the explicit native address
  with `SUBALIGN(4)` and `--hash-style=sysv`. Complete relocated text agrees
  with the protected target plus zero padding. Every external address and
  absence of remaining relocations is checked, independently of the scorer's
  relocation result. GNU readelf inspects object and linked metadata locally.
- The candidate is ordinary C89 with meaningful descriptor-nibble snapshots,
  typed native field views, helper calls and conditional updates. There is no
  inline assembly, filler, forced volatile pressure, dummy local, patched
  instruction, borrowed neighbor or fabricated callee body. The accessed
  resource-header prefix agrees with the accepted `ModelTables` loader;
  the paired-u16 view names a witnessed second halfword, not an original-type
  recovery claim. The author's plain-u16-array control changes code generation
  and is not part of the matching submission.

## Runtime boundary and behavior

The inspected native `UpdateActiveObjects` callback call at `+0x48` and
`sound_control` callback call at `+0x160` pass the Blit in `a0`, use its
callback field, and interpret zero `v0` as removal. AE940 returns one. The
matching source does not invent private arguments or depend on unstated
caller-save preservation. Whole dispatchers are bound and inspected, not
executed by the bounded harness.

The independent MIPS interpreter runs 5,840 cases through protected native
instructions and freshly GNU-linked IDO instructions: 11,680 executions.
Both agree with unchanged sanitized C and the shared state oracle. These
include all 5,824 author cases plus 16 additional selection-index boundary
and finite unsigned-conversion cases above `INT_MAX`. Callback mutations
exercise descriptor snapshots and later geometry/global reloads. Every hook
poisons all integer caller-save and `f0`–`f19` registers; preserved registers,
stack restoration and write confinement are checked.

The authentic protected 60-byte Hidden helper executes, covering all 15 of
its instructions. Target coverage is 222/225 instructions. `+0x1e4` and
`+0x1e8` belong to invalid float-conversion fallback outside the C-defined
test domain; `+0x1ec` is structurally bypassed by the surrounding branches.
The interpreter deliberately does not claim general FCSR, trap, NaN or
infinity behavior. Texture, renderer, fade and font internals remain explicit
side-effecting external contracts.

Five independently compiled source controls are rejected both by strict
matching and native semantic counterexamples: row spacing, texture flag,
signed half division, pulse selector and final return value. The author also
supplies eight sanitized-host negative controls.

An initial host fixture subtracted pointers from separate inner character
arrays. The author replaced that with bounded pointer-identity lookup before
this proof; the corrected harness is hash-bound by the receipt.

## Reproduce and portability

From repository root, with IDO, GNU MIPS binutils and GCC available:

```
python3 cloud/work/frontier/dot_runtime_a_list_control_20261006/verify.py --check
python3 cloud/work/frontier/dot_runtime_a_list_control_20261006/independent/review.py --check
```

The author receipt was independently replayed with GNU ld 2.42 (Ubuntu) and
2.44 (Debian). The independent receipt was generated using actual GNU 2.42
and checked with GNU 2.44.
Tool-version strings and raw ELF container hashes are local provenance only;
live protected manifests, compiler hashes and scorer hashes are retained as
provenance but excluded from semantic receipt equality. Python optimization
is explicitly rejected before verification, and required command-line tools
are checked. Portable receipt equality binds source, semantic target bytes, extents,
relocation outcomes, behavior and proof inputs. The committed host pytest
checks skip when GCC is unavailable.

Limits remain explicit: synthetic disjoint backed tables, item indices 0–3,
group/texture indices within 0–15, selection indices within 0–31, finite
nonnegative float products below 2^32, and target-specific signed-halfword
narrowing. These do not prove actual retail asset contents, complete index
reachability, original translation-unit ancestry or complete gameplay.
