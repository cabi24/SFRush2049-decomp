# Image-A player-availability reset: 728 new strict candidate bytes

## Deliverable and credit

The source-first group `groups/dot_runtime_a_availability_refresh_20261007/`
adds **A:func_80394B00**, all **182 words / 728 bytes**, complete native
interval `[0x80394B00,0x80394DD8)`. It preserves the same group identity and
all accepted members: 94564 (720 bytes) and 94834 (400 bytes), **1,120
inherited bytes**, excluded from new credit. Every exact body is a member
and claim. The real 97FFC caller remains the sole kept root and NONMATCH
context. No lock migration is proposed.

**Zero newly accepted-image or ROM-verified bytes. Keep draft; installation,
image composition and merging remain with the independent checker.**

Initial research base: `4f59607f7b15e252cfcbab67b9eccb6d9e6afba5`. Rebased and
replayed normally and with Python -O on current master
`84d8a4827e7fed06365846e013351b580593d921`; relevant source, native inputs and
installer/scorer tools are unchanged. Current locks and
open PRs were checked before the claim. Closed superseded #163 was also
checked. `baseline.c` is the complete unmodified current production source
at this base, with all four genuine bodies. The compiled baseline's 94B00
is 724 bytes, not the 728-byte native extent; it must not borrow its neighbor.
The complete GNU comparison includes the missing final word.

## Native-led reconstruction

94B00 updates one player's selection state. Mode zero reconciles the stored
owner with the live linked list, uses a default owner if it is absent or has
no selection, derives the selected controller/index, counts earlier matching
owners with saturation at ten, and clears a byte flag. Mode three clears a
13-byte row and two halfwords. Mode four sets one byte and clears three.
All paths store the mode, mark refresh pending, and invoke the existing
availability producer. No exact arcade ancestor or original identifiers are
claimed.

Workbench diagnosis on the fresh baseline identified mixed control-flow and
register differences with the same 24-byte frame. Native inspection supplied
three finite, causal source changes:

1. Both list searches check only the null pointer in the loop condition and
   test equality with a `break` inside the body. This recovers the native
   backward branch edges instead of duplicated short-circuit tests.
   Baseline **162/182 → 138 → 132** differing words.
2. Preserve the native signed word-offset/index arithmetic on the player's
   byte identifier: `((s32)(id * sizeof(s32))) / (s32)sizeof(s32)`.
   Native code visibly scales the unsigned byte by four and performs signed
   division by four, including the compiler's negative-bias branch. The
   operation is an identity for every possible u8 value: the intermediate is
   0..1020 and never overflows signed32. This source shaping is disclosed;
   its original spelling and semantic reason are unknown. No pointer
   provenance, helper identity, or hidden buffer allocation is inferred.
   This restores the missing instruction sequence: **132 → 20**.
3. Reverse the source ordering of the side-effect-free owner-id equality
   operands. This restores which pointer chain is evaluated first and the
   subsequent temporary register sequence: **20 → 0**.

No unused local, artificial access, extra formal/caller, volatile, inline
assembly, frame filler, live-buffer resize or modified acceptance compiler
is used. Actual text capacities and all records are unchanged.

The retained causal rollbacks are separately compiled: full baseline
162/182 (724-byte function); short-circuit loops 160/182 plus three nonzero
extra words (744-byte function; 164 complete GNU differences); direct index
132/182 (708-byte function); old equality order 20/182 (728-byte function).
The two accepted bodies remain exact in every control.

Stock UGEN diagnostic-on/off **whole objects are byte-identical** for the
final source and every control. All report T=16, P=8, M=0 and frame=24.
`trace.py` and compact `trace.json` reproduce these facts. No diagnostic
or forced compiler output is used for acceptance.

## ABI and selective installation

94B00's actual private entry reads player in s0 and mode in s1; it saves
only ra, and calls 94564 with the player in a1. There are **24 direct native
calls across five complete image-A caller bodies** (95E8C, 968C0, 96FA0,
97FFC and 9842C). All consumed complete targets and the current census are
input-bound. The same accepted genuine group, with 97FFC's real body and
calls, recovers that ABI. This is not a standalone O32 replacement.

The current installer route `packet_spec → group_bodies → mismatch` is
exercised with only temporary files and the authenticated current image.
It emits exactly the three members, each byte-equal to its native extent;
97FFC is excluded. No source, lock or image is installed by this proof.

97FFC remains **260/268 NONMATCH**, 1,120 compiled versus 1,072 native bytes,
12 extra nonzero words, four unverified section references and the existing
truncated-window HI16 complaint. It is neither a match nor an installation
candidate. The independent whole-object GNU reconstruction covers its actual
extent and resolves its complete relocation pairs. Its eight literal bytes
plus eight alignment bytes are reconstructed at an explicitly diagnostic
address, **not asserted to match retail owned data**. No matching member
references them or any owned data.

## Verification

Commands from the repository root, with stock IDO 5.3 and GNU MIPS binutils:

```
python3 cloud/work/frontier/dot_runtime_a_availability_reset_20261012/verify.py --score
python3 cloud/work/frontier/dot_runtime_a_availability_reset_20261012/verify.py --deep
python3 -O cloud/work/frontier/dot_runtime_a_availability_reset_20261012/verify.py --deep
python3 cloud/work/frontier/dot_runtime_a_availability_reset_20261012/trace.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_availability_reset.py -q -o addopts=''
```

Canonical unchanged scorer output, without `--claims` or `--allow-unverified`:

```
Members:
func_80394564:
  MATCH
func_80394834:
  MATCH
func_80394B00:
  MATCH
```

- Independent GNU reconstruction consumes **all 188 original REL records**:
  22 for 94564, 20 for 94834, 46 for the new 94B00, 100 for the unclaimed
  root. No relocation is dropped. Internal call addends resolve to actual
  emitted function entries. Full member words, exact native placement,
  STT_FUNC extents, next boundaries and zero suffixes are checked. External
  bindings, undefined symbols, remaining relocations and unexpected allocated
  sections fail closed. Code corruption and overlong extents are rejected.
- The actual unchanged reset C body passes **25,000 logical-host fixtures**
  under UBSan/bounds, plus all 256 byte-index identities. The fixtures cover
  four players, modes -1..7, prior modes -1..6, owner absent/present/unlisted/
  missing-selection, lists of 0..14 entries, levels -3..12, all u8 identifiers,
  selected state fields, all row bytes and unaffected players. The refresh
  helper is a synchronous call-boundary double that checks the committed
  mode/pending fields. All-u8 coverage proves local reset behavior and the
  index identity only: the real downstream availability helper requires a
  valid controller index (ordinary four-player domain 0..3). A byte that
  narrows to a negative s8 is not thereby a valid downstream array index.
  Host pointer layout is not MIPS record layout.
- The protocol-v2 receipt binds source, recipe, verifier, ELF reader, test,
  harness, trace, selected full native bodies/symbols/callers, actual memset
  instruction words and eight stock compiler executables. It does not pin
  shared tools, entire locks, Makefile or assembly text. No static ROM TU is
  rebuilt.
- Normal and Python -O deep replay pass. The one focused test passes in about
  0.4 seconds; absent IDO cleanly skips after input validation.

No ROM bytes, raw assembly dumps, objects, generated images, compiler binaries,
credentials or unrelated private data are included. Valid player indices,
acyclic live lists, valid body/selection pointers where dereferenced and stable
synchronous memory are assumed. The index's signed-byte narrowing follows the
observed target/host low-byte convention, not an ISO-C portability guarantee.
No unrestricted alias/concurrency claim, executed retail-helper oracle,
full-gameplay/hardware result, image recompression or cartridge hash is proved.
