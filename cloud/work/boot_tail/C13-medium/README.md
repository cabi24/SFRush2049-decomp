# D10 Packet 4: C13 controller tables and initialization

Active claim acknowledged by the central owner at wave-2 commit `6dd6c6a4`.
Branch `dot/boot-tail-p4-c13-medium`, exact source base fresh master
`301d9e7552ad4fd7f54a38796db84671e1000d35`. The prior nine C13 wrapper
sources are frozen separately in reviewed wave 1 `de8652c3`; none is edited here.

Exact targets: 80020610 (156 B), 80020F4C (76), 80020F98 (68),
80020FDC (76), 80021028 (68), 8002106C (228): 6 functions / 672 B.
Only their matching files and this directory are owned here. Central ledger,
claims and D10 remain sole-writer work. No target, boundary, scorer, shared-header,
lock, ROM/image, runtime-image/farm or unrelated helper edits.

Fresh preflight before candidates: setup with existing pinned IDO, all three
target manifest checks, 439 matching start/size pairs and the existing getter
strict MATCH. The fresh master changes none of these protected inputs.

Stop after 20 directed natural variants per hypothesis without new movement.
Start at O2, then O1; run workbench diagnose before further near-match variants.

## Frozen result

**One local strict MATCH / 228 B**, `func_8002106C`, on its first natural source.
**Five complete NONMATCH bodies / 444 B** remain research only. Independent
source/ABI/strict-replay review passed at `92861d8b` (see `REVIEW.json`); aggregate
exact-head CI is pending. No acceptance, promotion,
coverage or new verified-byte claim is made here.

| Function | Native bytes | O2 strict result | O1 control |
|---|---:|---|---|
| `8002106C` | 228 | MATCH, all 57 words, zero excess/errors | 57/57 differ, 24 nonzero excess words |
| `80020610` | 156 | 39/39 differ, 2 nonzero excess words; one unpaired HI16 in target-length window | 39/39 differ, no excess |
| `80020F4C` | 76 | 18/19 differ, 1 nonzero excess word | 19/19 differ, no excess |
| `80020F98` | 68 | 16/17 differ, no excess | 17/17 differ, no excess |
| `80020FDC` | 76 | 18/19 differ, 1 nonzero excess word | 19/19 differ, no excess |
| `80021028` | 68 | 16/17 differ, no excess | 17/17 differ, no excess |

All comparisons use the unchanged strict scorer, exact native extents and
`-g0 -O2/-O1 -mips2 -G 0 -non_shared` plus its automatic
`-Wab,-r4300_mul`. The submitted initializer has 240 aligned object-text bytes:
228 native function bytes and three ordinary zero alignment words. Every native
word equals the fully relocated compiler output, with zero masking, unresolved
symbols, unverified references, relocation errors or nonzero excess. It has no
external references or helper calls. O2 reuses the common scale/count constants;
O1 repeatedly materializes them and schedules stores differently. This directly
supports O2 for this member, without claiming a universal cohort flagset.

## Native reconstruction and layout evidence

The initializer uses one genuine pointer to a packed voice object. Its nine
controller records start at byte 196 and advance by 18 bytes, independently
corroborated by the nine previously matched C13 wrappers. Each record holds
four four-byte input sources followed by a byte count at offset 16. The current
body writes only source zero and the count. Each source has a byte controller,
byte combine mode and 16-bit scale. Native byte stores write big-endian scale
256 as two bytes. `#pragma pack(1)` models the observed byte-access contract.

The initial controller IDs, in record order, are 7, 10, 131, 128, 132, 1, 64,
65 and 91. The record-4 stores occur last, matching native order. All counts
become one and all combine modes become zero. `unknown00[196]` and `unknown11`
model untouched object bytes/record spacing; they are not stack locals and do
not create instructions. The full original voice layout and the purpose of the
last byte of each 18-byte record remain unknown. No shared header is modified.

The five table functions are complete behavior reconstructions:

- `20610`: four genuine unsigned-byte arguments (controller, channel, set,
  value); set 255 selects the effects table. Otherwise the address stride is
  16 channels times 134 controllers per set. Values are masked to seven bits.
  Named global bases are `80050D00` and `80055000`.
- `20F4C` / `20F98`: set/get one unsigned byte from `80050C60[set][channel]`,
  with 16 channels per set, or `80050CE0[channel]` when set is 255.
- `20FDC` / `21028`: the same set/get shape at `800560C0` and `80056140`.

Leading dimensions remain unspecified in external declarations; no storage is
allocated or table data copied. Widths, masks, strides and sentinel selection
come from the whole native bodies. Their compiled output is not a match.

## Bounded diagnosis and rejected controls

Both prescribed flag levels were checked before further edits. O2 normalizes
narrow arguments into temporary registers and inserts copies; native homes
those genuine byte parameters but masks them in the original argument registers.
O1 uses the desired registers yet removes the native homes. The residual is
an entry normalization/coalescing mismatch, not permission to ignore stack stores.

Before any variant, `tools/workbench.py diagnose` was run on each of the five
native targets and fully resolved candidate object slices. It reported structure
mismatch, absent frames on both sides and no proven source lever. The summarized
metadata is in `diagnosis.json`; raw words, reports and objects stay uncommitted.
The local diagnostic comparison resolves complete objects, while strict scoring
retains its unchanged target-length window. This explains the truncated HI16
warning in the rejected O2 `20610` comparison; it is never waived.

Four directed controls on the representative `20F4C` setter followed diagnosis:
an explicit prior prototype, register-qualified genuine arguments, an old-style
C89 parameter definition, and an early-return branch form. All reproduce the
same residual at both optimization levels. Their source is under `variants/`;
none is a submitted match. No artificial formals, keeper, dummy call, inline
assembly, redundant mask or frame-padding local was added. The other four
members were not subjected to equivalent unproductive sweeps. The session froze
well below 20 variants because no measured movement or authentic new context
justified repeating the same controls. Another worker independently observed
this same plateau in its byte-parameter reconstructions.

Next step needs new evidence: authenticate the original N64 parameter/type and
compiler-context conventions, or trace the unmodified source's entry-value
coalescing mechanism. Do not repeat these declaration/branch controls without
such evidence, change function boundaries or edit the scorer/targets.

## Reference leads and limits

Pinned [AxioDL/musyx snd_midictrl.c](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/snd_midictrl.c)
contains controller tables, last-note accessors, and `inpInit` assignments with
these controller IDs. Its [CC0-1.0 license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE)
is pinned at the same revision. These are family leads. Public Dolphin/PC
records use different scales/layouts and additional dirty-state behavior; their
source is not an authenticated N64 version. No third-party translation unit,
header or table is vendored. Exact source names for both byte-table pairs remain
unproved.

The unlicensed [Snowboard Kids IDO notes](https://github.com/cdlewis/snowboardkids-decomp/blob/main/DECOMPILATION_LEARNINGS.md)
were consulted for narrow-argument homing and source-context caveats, not copied
into this packet or treated as proof for this game. The vendored CC0 workbench
was used only for diagnosis; no compiler instrumentation or flag sweep ran.

## Checks and handoff

- `python3 cloud/work/boot_tail/C13-medium/verify.py --check`
- `python3 -m unittest discover -s cloud/work/boot_tail/C13-medium -p 'test_*.py' -v`
- `python3 tools/cloud/score.py fn cloud/matches/boot_tail/func_8002106C.c
  func_8002106C --targets asm/us/boot_tail`

`scores.json` binds every final source and control to native-body, compiler and
tool hashes, flags, exact lengths and strict outcomes. Four focused tests compile
all six final bodies under host C89/pedantic/Werror. They verify 120 table-pair
updates/readbacks, 240 controller-table masked updates, and nine initializer
alignment/sentinel scenarios, including all untouched bytes and packed offsets.
Host scale bytes follow host endianness; native big-endian equality is separately
proved by strict IDO replay. Behavioral tests award no matching credit to the
five nonmatches.

`status_delta.json` is a proposed six-row update for the central sole writer.
This branch does not modify the shared ledger or prior frozen matching files.
Only independently reviewed packet commits may enter the next aggregate draft.
Exact published-head CI and merging remain the lead/checker's separate steps.

Existing cloud setup/guard/submission/integrity/scorer regression: **631 passed**,
zero skips, using the available test environment. The final review receipt changes
no C source or scoring input.
