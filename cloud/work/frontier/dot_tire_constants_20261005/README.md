# Tire polynomial initializer: complete donor-backed match

`track_preview_handler`, **0x800D08E4–0x800D09E8**, is a complete **260-byte /
65-word** strict matching candidate at ordinary IDO O3 and O2. Its historical
label is misleading: this initializes one tire's load and polynomial constants.
Accepted-byte and ROM-coverage gain: **zero**. No splice or cartridge result is
claimed.

Base: freshly fetched master `e24b47d8`. Current locks, live research, archived
attempts and open draft PRs 110–115 were checked. The exact interval was announced
before edits. It does not overlap D11BC, D1248, or those PRs. Unpublished work on
other machines is not observable here.

## Authentic source evidence

Pinned donor: [historicalsource/rushtherock at 845329d7](https://github.com/historicalsource/rushtherock/tree/845329d7b36f5a384c5625ed9a0aef584ab46139).
Exact file blob IDs and SHA-256 hashes appear in `provenance.json`.

- `game/initiali.c:362–374`, `copy_tire_info`, supplies the tire-load expression:
  the vehicle load times the opposite tire's offset divided by wheelbase, halved.
- `game/initiali.c:377–391`, `tire_constants`, supplies the full ordered family
  Afmax, k1/k2/k3, l2/l3, m1/m2/m3/m4 and patchy, expressed through actual fields.
- Native `track_info_display` calls this function twice in its four-tire loop,
  at **0x800D0E54** and **0x800D0E68**. The two sites supply the appropriate front
  or rear opposite tire-position coordinate. The accepted caller also proves
  the 2056-byte model, 92-byte tire stride and wheelbase field at model +1480.

The N64 adaptation combines the load calculation and coefficient setup in one
three-argument leaf. It receives the already combined model value at +1468;
this packet calls that field `load`, without resolving all historical
mass/weight naming in other accepted type views. The original arcade function
names are evidence of ancestry, not recovered N64 symbol names.

## What closed the mismatch

Fresh O3 controls reproduce A96 at **62/65**, 264 bytes, and B27's natural
scalarized body at **48/65**, 264 bytes. Neither archived body is an exact match.
The genuine field-only expressions remove their decompiler scalar ownership.
No candidate local variables are added at all. IDO itself emits the native
32-byte frame and the live temporary spill at sp+16.

Two narrow, semantically grounded N64 adaptations matter:

1. The native l2 operation is `k2 + k2`. The literal arcade `k2 * 2` control
   emits 268 bytes and differs at 35 target words. Addition recovers the native
   operation, scheduling and remaining FP allocation.
2. The reciprocal coefficients are rounded to binary32 from the donor's
   original double constants. In particular `(float)(1.0 / 3.4)` has the correct
   owned coefficient; `(1.0f / 3.4f)` is one ULP lower. The float-first control
   reproduces all code words but is correctly refused by strict own-data checks.
   This is an explicit data mismatch, not a verified match with masked literals.

The remaining constants preserve the donor algebra, including 4.16 from
`40 * .052 / (Cfmax / 2)`, and reciprocal forms for 3.4, 46.3 and .000055.
No original N64 spelling or exact translation-unit boundary is claimed.

There is no filler, unused local, dead condition, added argument, artificial
keeper, stand-in, volatile qualifier, modified compiler recipe or protected-file
edit. Opaque bytes describe observed record offsets and extents.

## Complete verification

- Exact ELF STT_FUNC extent: 260 bytes. All 65 words independently GNU-linked at
  the native address equal the manifest-checked target, including all eight
  HI16/LO16 relocations.
- All 16 owned literal bytes at **0x80124138–0x80124148** equal protected data.
  Twelve zero text-alignment bytes are outside the claim. There is no other
  owned data or BSS.
- Genuine three-body O3 context preserves the complete unchanged accepted
  `track_info_display` (1124 bytes) and its real resistance lookup
  `func_800D0A1C` (24 bytes). Their source and existing keep recipe are unchanged;
  the candidate is added as its own retained entry. This is compiler-context
  preservation with separate existing type views, not caller runtime execution
  or a unified full-game shared-type model.
- **4096** deterministic oracle / unchanged host-C89+UBSan / protected-native /
  GNU-linked cases agree, for **8192 native executions**. All 65 instructions
  execute, including the return delay slot. Both separate objects and all four
  genuine embedded-tire offsets are exercised. Entire model/tire bytes, saved
  integer and FP registers, stack bounds and unchanged data are checked.
- Six wrong-contract source controls are rejected: wrong load scale, model
  field, stiffness ratio, m1 coefficient, l3 multiplier and patch reset.
- **820 selected tests pass**, including all ten packet tests plus the complete
  scorer, own-data, group/unit, protected-path and submission test modules.
- Ten focused tests replay the proof and source controls, reject actual ELF
  instruction/literal/extent mutations, check caller discovery and registered
  submission discovery, and prove unknown native instructions fail closed.

Run from the repository root with pinned IDO and GNU MIPS binutils:

```sh
python3 cloud/work/frontier/dot_tire_constants_20261005/verify.py
python3 -m pytest -q tests/cloud/test_dot_tire_constants.py
```

`--write` explicitly regenerates a reviewed receipt. Ordinary verification
compares against it without updating files. Native words, objects, raw
instruction dumps and private scratch outputs are never committed.

## Limits and handoff

The behavioral domain uses finite nonzero denominators and finite results;
negative inputs are included without asserting gameplay reachability. NaNs,
infinities, hardware FCSR/exception effects, arbitrary pointer overlap, invalid
records and concurrent mutation are outside the proof. The four embedded
layouts are specifically tested with real typed Tire members in Model, rather
than assuming all arguments disjoint or type-punning an opaque byte buffer.

This candidate is registered as `cloud/matches/track_preview_handler.c` for the
ordinary changed-submission scorer. Final source admission, full-game shadow
compilation, source-built image, compression, ROM SHA-1 and merging remain with
the independent checker. No publication or CI monitoring was performed here.
