# Image A statistics panels: three strict matches

## Deliverable and credit

Base: current master `3bec9ce30b8fbc707e2e1d6c5717826c03646232`.
Installable group source: `groups/runtime_a_stats_menu_20261006/`.

- New `func_803AC54C`: `[0x803AC54C,0x803AC7F4)`, 680 bytes, 170 words.
- New `func_803AC7F4`: `[0x803AC7F4,0x803ACA8C)`, 664 bytes, 166 words.
- New `func_803ACD5C`: `[0x803ACD5C,0x803ACF5C)`, 512 bytes, 128 words.

**1,856 newly strict-matched candidate bytes.** Preserved prior candidates
AAF0C (1,776), AC330 (540), and ACF5C (376) total **2,692 bytes** and earn no
new credit. All six verified bodies are both `members` and `claims`.
No cross-group lock migration. Seven complete genuine bodies remain explicit
NONMATCH `context`, with only the actual AD57C callback in `keep`.
Zero new accepted-image or ROM-verified bytes are claimed. Keep draft;
installation, acceptance and merging belong to the independent checker.

## Trace and safe storage inference

The archived complete statistics group reproduces two differences per target,
solely its stack allocation/deallocation: frames 120/112/120 against native
112/104/112. Independent GNU-resolved workbench diagnosis classifies all three
as `frame-layout`, owning pass `stack-home-assignment`. This classification
is diagnostic, not an extent/relocation acceptance gate.

Stock UGEN gives T=20/P=4 and M=84/80/88. Register hints and explicitly discarded
object-creation results are inert. Named coordinate-call carriers retain or
increase the frames; replacing the label local with repeated loads changes
register/code geometry. These bounded probes were not retained.

The prior 48-byte arrays were inferred declarations, not recovered capacities.
Before changing them, the complete write sets and the authenticated current
image's format strings were audited:

- AC54C passes unsigned halfwords to decimal formatting, and delegates its time
  row to unchanged AC330. Even its unreachable word-sized decimal case needs
  at most 11 characters plus NUL: **12 bytes** conservatively.
- AC7F4 uses the same bounded halfword formatting and AC330: **10 bytes**.
- ACD5C uses only halfword decimal formatting: **6 bytes**.
- AC330's narrow fixed no-time text is 9 characters plus NUL. Otherwise its
  three unsigned modulo results are below 100, 60 and 1000, and the fixed
  precision format produces 9 characters plus NUL. No language-table string,
  arbitrary player name or `%s` input is written to these three buffers.

The verifier authenticates every format and pins the actual game wrapper and
copy-helper target words plus the static formatter core's instruction bytes.
`fcvt_wrapper` is the existing variadic wrapper over the historical `fcvt`
symbol's printf-like core; `func_800BE6A4` takes its narrow copy path for the
fixed string. The decimal/precision bound is source/native-flow analysis with
independent review, **not freshly executed formatter proof**. Original helper
names/ancestry are not inferred from their historical labels.

Forty-byte real text arrays therefore have at least 28 spare bytes on every
write path. This safe capacity inference reduces M by eight. Moving the
existing x/y declarations ahead of label/index keeps every native x/y/text
home unchanged while reducing all three complete frames by eight. No dummy
local, fake formal, false prototype, volatile, assembly, keeper or compiler
patch is used. No original array capacity is claimed.

Final M=76/72/80; T=20/P=4 unchanged. Native text bases remain 52/44/52.
Diagnostic-on/off whole objects are byte-identical for all four replayed
source forms. `trace.py` reports metadata only.

Causal controls (AC54C / AC7F4 / ACD5C differing words):

- Restore only the old capacity: 5 / 6 / 5
- Restore only the old declaration order: 3 / 4 / 3
- Restore both, the exact archived baseline: 2 / 2 / 2

All old exact members remain exact in every control. The verifier also checks
that reversing these declaration edits reproduces the entire archived source,
so no unrelated body, statement, prototype or buffer is silently changed.

## Canonical score

`python3 cloud/work/frontier/dot_runtime_a_stats_frame_20261012/verify.py --score`
authenticates current-tree image A in memory and calls unchanged `score.py group`
without `--claims` or `--allow-unverified`:

```text
Members:
func_803AAF0C:
  MATCH
func_803AC330:
  MATCH
func_803AC54C:
  MATCH
    own .rodata verified at 0x803B98E8..0x803B990C
func_803AC7F4:
  MATCH
    own .rodata verified at 0x803B990C..0x803B9928
func_803ACD5C:
  MATCH
func_803ACF5C:
  MATCH
```

Context diagnostics remain explicit: AA3D8 439/717, AB5FC 124/147,
AB848 394/416, ABEC8 243/282, ACA94 166/178 (+5 extra words),
AD0D4 145/296 (+3 extra, one own-data error), AD57C 348/523. Their own-data
uncertainties are retained in the receipt. They are never counted as matches.

## Independent relocation, extents and private ABI

The whole-object reader verifies all six STT_FUNC extents, next-function
boundaries, zero alignment bytes and permitted allocated sections. GNU as/ld
independently resolves **221 text relocations plus 16 jump-table relocations**.
Every full function byte equals the protected target. Both complete jump
tables, 9 and 7 entries, independently link to their native 64-byte interval
`0x803B98E8..0x803B9928`; all table targets stay inside the claimed owners.
Unrelated NONMATCH-context rodata is not represented as verified owned data.
Missing and corrupted table controls fail closed for each new owner. A GNU
replay of the complete archived baseline confirms exactly two frame-word
differences per new body and zero differences in the three preserved bodies.

The new functions have genuine no-argument private ABIs. Native direct calls
are AD57C +0x77C (AC54C), +0x76C (AC7F4), and +0x5B4 (ACD5C). Their exact
clobber/save and caller conventions are recovered through the complete genuine
O3 group, not an asserted standalone o32 interface. Existing AC330's third
formal remains grounded in actual outgoing zero argument stores.

## Reproduction and review

Use stock IDO 5.3 and GNU MIPS binutils. No static TU is rebuilt, and no compiler
version is forced across a static per-TU boundary.

```sh
python3 cloud/work/frontier/dot_runtime_a_stats_frame_20261012/verify.py --deep
python3 -O cloud/work/frontier/dot_runtime_a_stats_frame_20261012/verify.py --deep
python3 cloud/work/frontier/dot_runtime_a_stats_frame_20261012/trace.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_stats_frame.py -o addopts='' -q
```

One focused packet test passes in under one second. It uses shared conftest
missing-IDO behavior; the explicit packet test supplies CI coverage for this
frontier group. Receipts pin source/verifier/test inputs, native target rows,
referenced symbols and helper words, not mutable tools, locks or assembly text.
All temporary objects/listings are outside the publication.

Independent lane-4 review reran deep proof, canonical full-group score, trace
and test; checked all three full buffer write sets and the actual formatter/
copy helper paths, and found no blocker. It confirmed that the 12-byte maximum
is deliberately conservative and includes an unreachable u32 case.

## Limits

The original whole-TU boundary, typedefs, names and 40-byte array declarations
are not established. Finite, representable float-to-u32 time conversions remain
a precondition of the inherited C. Native out-of-range conversions are not a
portable-C guarantee. Valid record/language pointers and synchronous ordinary
rendering/helper contracts are required; arbitrary memory aliasing, helper
corruption, concurrency and unrestricted data are not covered.

No newly executed renderer/gameplay/native-formatter behavior, full-image splice,
compression, cartridge hash or hardware proof is asserted. Existing complete
matching code plus independent relocation/owned-data identity establishes the
new candidate matches. No ROM bytes, raw assembly dumps, objects, secrets,
protected paths or unrelated data are included.
