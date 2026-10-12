# Image-A aggregate-statistics renderer: ABEC8

## Deliverable and credit

`groups/runtime_a_stats_menu_20261006/` is the actual source-first deliverable.
Stock IDO 5.3 strictly matches **func_803ABEC8, 1,128 new candidate bytes**,
all 282 words over `[0x803ABEC8,0x803AC330)`, and its **28-byte owned switch table**
at `[0x803B98CC,0x803B98E8)`.

The same genuine group identity and genuine AD57C kept root are retained. All
ten current accepted members remain unchanged, members and claims: AAF0C,
AB5FC, AB848, AC330, AC54C, AC7F4, ACA94, ACD5C, ACF5C and AD0D4. Their **8,696 bytes**
and 152 table bytes are preservation only, excluded from new credit. AA3D8 and AD57C remain complete NONMATCH context. Master advanced during peer review; accepted PR #355's AD0D4 is now preserved in the source, members and claims. No production source or accepted lock changes.

Initial base: `4f59607f7b15e252cfcbab67b9eccb6d9e6afba5`. Refreshed on master
`84d8a4827e7fed06365846e013351b580593d921` before publication, including the
newly accepted AD0D4 source and all its words/data in every replay.
**Zero newly accepted-image or ROM-verified bytes. Keep draft; installation,
image composition/recompression, ROM validation and merging belong to the
independent checker.**

## Native-led reconstruction

The current-tree track-statistics group source reproduces the full baseline:
243/282 differing words, 1,120-byte ELF extent versus 1,128 native bytes, frame
144 versus 152. The workbench diagnosed a two-instruction geometry deficit.
Stock UGEN established M112/T20/P4 for that baseline.

1. The eight-entry record loop computes its own per-record pointer and then
   reads offset 8, matching the native address increment that the old combined
   offset expression omitted. The native words show a distinct pointer value
   for this loop, separate from the pointers spanning the bit-count calls.
2. Distinct race, track and record pointer locals retain their actual separate
   lifetimes. The accumulated-record counter and displayed-row counter are
   also distinct. All these locals are used by the real computation.
3. The scaled unsigned quotient is a common expression in the two actual
   formatter arguments. This removes an unnecessary named scalar home without
   changing unsigned 32-bit wraparound, division order or either argument.
4. Existing useful declarations are ordered as divisor, x/y, distance, total,
   five halfword accumulators, the two short counters, then text and pointers.
   Separate distance/total initialization lines recover the native scheduling.

Final stock UGEN: **M120/T20/P4, frame 152**, text at sp+72, x/y at sp+146/144.
The real **text[48] is unchanged**. No new unused local, padding array,
artificial memory access, volatile, fake caller/formal, forced register,
assembly, hidden compiler option or modified acceptance compiler is used.
Original source spelling and local names remain unknown.

A private diagnostic-only colouring replay found the remaining first-two-loop
pointer web ahead of the scan counter (s0 versus s1, benefits 40 versus 33.25).
A single two-web forced oracle removed that 29-word colour-only residual.
That oracle was never an acceptance compiler; the final source resolves the
lifetime cause under stock IDO. Diagnostic-on/off stock objects are identical
for the final source and all retained controls. No tracer binary or raw dump
is published.

## Reproduce

```
python3 cloud/work/frontier/dot_runtime_a_aggregate_stats_20261012/verify.py --score
python3 cloud/work/frontier/dot_runtime_a_aggregate_stats_20261012/verify.py --deep
python3 -O cloud/work/frontier/dot_runtime_a_aggregate_stats_20261012/verify.py --deep
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_aggregate_stats.py -q -o addopts=''
```

The canonical scorer uses the unchanged scorer with authenticated image A
provided in memory. It shows eleven ordinary MATCH members, without `--claims`
or an uncertainty waiver. The two context bodies are explicitly NONMATCH.

The protocol-v2 input receipt binds source, recipe, own verifier, stock trace
script, actual-source harness and focused test; selected complete native
functions, referenced symbols, helper targets and native caller census;
current-tree baseline and all eight stock compiler executable identities.
It does not pin mutable shared tools, whole locks, Makefiles or assembly text.
No static ROM translation unit is rebuilt.

Independent original-REL GNU reconstruction checks every complete claimed
STT_FUNC extent and next compiled boundary, zero suffixes, native placement,
all unmasked words, **480 member text relocations** (51 in ABEC8), and all
**45 table relocations / 180 owned bytes**. It refuses unexpected allocated
sections, unknown symbols, unsupported relocations and residual or undefined
GNU bindings. Every table owner rejects missing and corrupt native owned data.
The selected native target is checked against its next native boundary.

The native caller census is exactly AD57C+0x78c. The complete genuine parent
owns s0-s7/s8/ra saves, while ABEC8 saves only ra in its private convention.
It has no source formals and no introduced caller. Native caller and complete
context targets are input-bound. No caller substitution is claimed. The actual selective installer route
`packet_spec -> group_bodies -> mismatch` emits exactly the eleven claimed
native-equal slices and excludes both NONMATCH contexts. It operates only
on temporary files; no image, source or lock is installed.

Causal controls independently replay all ten preservation members and tables:

- Current-tree baseline: 243/282, 1,120-byte extent, frame 144.
- Merge race/track scan pointer: 29/282, full native extent, frame 152.
- Reuse scan counter for display: 49/282, full extent, frame 152.
- Reintroduce named quotient: 31/282, full extent, frame 160; stack-only delta.

Deep GNU replay resolves each control's original relocations and reproduces
its complete differences without borrowing a neighbour. Controls deliberately
remain NONMATCH; their output is never installation input.

## Behaviour and capacity

`host.c` includes the actual complete candidate source and executes **2,000
cases**: profile absent/present, both distance units, zero/full/random masks,
unsigned 32-bit distance and total wraparound, unsigned 16-bit found-count
wraparound, and 17 valid line heights. It checks every mask passed to the
bit-count helper, exact output strings, label/coordinate order, helper counts,
rectangle bounds and unchanged input bytes/profile pointer/unit state.

The same matrix passes ASan/UBSan with only LeakSanitizer disabled. Rendering
and formatting services are deterministic logical-host doubles. The formatter
models the existing O32 argument-bit convention explicitly, including signed
`%d` interpretation of the unsigned total; it is not retail formatter execution
or proof of cross-endian native struct layout.

All eight native format strings are authenticated. Every write to the unchanged
48-byte buffer is through one of these known formats. The greatest possible
write under the existing formatter contract is **12 bytes including NUL**:
a signed 32-bit total can use 11 characters; the nonmetric unsigned distance
quotient has at most nine digits, decimal point and one remainder digit. Other
fields are smaller. Localized labels are passed directly to the renderer,
never copied into text. This is a bounded writer proof using the established
formatter contract, not unrestricted libc/helper validation.

Valid backed profile/record/label storage, stable inputs and genuine helper
contracts are assumed. The finite host matrix does not prove arbitrary aliases,
asynchronous mutation, arbitrary line-height narrowing, actual game services,
gameplay/hardware, original declarations, image integration or ROM identity.

## Review and publication boundary

Normal and Python -O deep replay pass. The strict focused test passes in about
one second; deliberately missing IDO cleanly skips through shared conftest.
Protected-path and whitespace checks run before publication. Only additive
UTF-8 source, tests and verification notes/receipts are intended for publication.
No ROM bytes, raw/native assembly dumps, compiled objects, credentials or
unrelated private data. Independent peer review and exact-head hosted CI are
reported separately after they are observed.
