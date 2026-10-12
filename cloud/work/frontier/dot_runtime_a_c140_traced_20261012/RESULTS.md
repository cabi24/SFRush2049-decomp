# Image A 8039C140: strict match, installation blocked

Deliverable: `groups/dot_runtime_a_text_helpers_20261006/`, complete stock IDO
5.3 O3 group. C140 matches all **496 words / 1,984 B**. All nine inherited
verified members stay exact and are included in both `members` and `claims`.
Gross: **10 functions / 4,864 B**. New candidate gain: **1 function / 1,984 B**.
Accepted-image and cartridge gain: **0 B**; this is a research draft.

**Migration blocker:** five included locked bodies belong to four other
accepted groups. The existing installer rejects that cross-group ownership.
No locks are moved/reverted and no claimed member is omitted to evade the guard.
The independent checker must decide whether/how to consolidate these groups.

## Traced source result

Baseline seven differences split into four frame/home words and three schedule
words. Workbench diagnosis on fully resolved objects confirms the native frame
136 versus 128, equal instruction count and unchanged save locations. Two
meaningful call-operand carriers (`matrix`, `angle`) recover the missing eight
bytes without unused locals or buffer changes: stock UGEN M12 -> M20, T16/P4
unchanged. Every frame/home word becomes exact, leaving three LUIs.

Stock as1 `-R` then identifies the competing instructions: the else-if's
D_803B3414 address LUI has a later source line than the -90.0f constant and clock
address, so the scheduler puts it last. A diagnostic same-line oracle confirms
the cause. The actual matching source uses a readable multiline animation-arm
macro and puts its invocation and the following else-if on one physical line.
That source-line tie selects the native order. No oracle object is match evidence;
all acceptance evidence recompiles the unmodified source with stock tools.

All ten members score strict without `--claims` or `--allow-unverified`.
The four unclaimed genuine contexts remain nonmatches. Cross-image helpers stay
external, and external float anchors remain native-address reads. Source header
discloses line-sensitive shaping and inherited volatile-clock uncertainty.

## Earlier bounded 803A7180 follow-up

Still 2/296 (frame192/native168). Register hints are inert; loop lexical scoping
moves buffer homes (6/296) without shrinking the frame; short coordinate carriers
produce 283/296 and frame184. None is retained or claimed. The 100/16-byte live
buffers remain unchanged. Prior C140 work begins after the published reservation
`6c12bd89f07aab8a98761fbecc159dce0810fa9e` on draft #323.


## Reproduction and strict member output

```
python3 tools/cloud/score.py group cloud/work/frontier/dot_runtime_a_c140_traced_20261012/groups/dot_runtime_a_text_helpers_20261006 --targets asm/us/ovl_a
Members:
func_8039B120:
  MATCH
func_8039B214:
  MATCH
func_8039BE48:
  MATCH
func_8039A448:
  MATCH
func_8039B00C:
  MATCH
func_8039C140:
  MATCH
func_8039B908:
  MATCH
func_8039B988:
  MATCH
func_8039BA28:
  MATCH
func_8039BAE0:
  MATCH
```

Context is informational and excluded from member acceptance: A4D4 623/716,
BB80 9/178, A5658 32/184, A5948 1025/1080 with six unverified own-data references.
The full command exits zero without a claims filter or uncertainty waiver.
No context is misreported as new matched bytes.

```
python3 cloud/work/frontier/dot_runtime_a_c140_traced_20261012/verify.py
python3 -O cloud/work/frontier/dot_runtime_a_c140_traced_20261012/verify.py
PASS: 10 strict members / 4864 B gross / 1984 B new; cross-group installation blocked

REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_c140_traced.py -q -o addopts=''
1 passed in 0.38s

python3 cloud/work/frontier/dot_runtime_a_c140_traced_20261012/trace.py
```

The optional trace uses stock UGEN diagnostics and stock `as1 -R`; every entire
object equals an ordinary stock compile. Matching source has all three relevant
source lines equal (311), M20/T16/P4 and zero differences; expanded ordinary lines
are 295/295/310 and restore three differences. Removing just the operand carriers
has M12/T16/P4 and restores four frame/home differences. Removing both mechanisms
restores the original seven-word residual. All nine old bodies stay exact in
all three controls. This diagnostic is separate from the one cheap routine test.

## Full-body, data and ABI proof

An independent ELF reader verifies each of the ten complete STT_FUNC extents.
GNU as/ld independently resolves all **389 relocation records** (162 in C140),
including genuine private-ABI internal calls, and every linked byte equals its
authenticated native member. No claimed body has a local-data relocation or
unknown data owner; the nonmatching A5948 owned-data issue stays in context.

C140's 136-byte frame preserves its exact sixteen register-save slots, totaling
88 bytes: s0–s8/ra and the six double-width FP saves f20–f30. The replay checks
paired save/restore offsets and the original externally kept roots. Private
helpers are not advertised as standalone o32 replacements. Exact full-body
identity is not an end-to-end gameplay, callback, concurrency or FP-exception
test, and no such behavioral claim is made.

Receipts pin packet source/verifiers, flags, image-qualified target rows and word
hashes. They do not pin shared tools, locks, the Makefile, raw assembly text or
non-master history. This is overlay IDO 5.3 work; no static ROM TU is rebuilt.
One test runs in under one second with IDO and checks binding before shared
missing-tool handling. Both normal and optimized-Python replay pass.

## Exact installation barrier

The retained group's four old members are B908/B988/BA28/BAE0. Five other fully
verified bodies already have different accepted group owners:

- B120: `dot_runtime_a_config_caller_group_20261006`
- B214 and A448: `dot_runtime_a_config_parent_group_20261006`
- BE48: `dot_runtime_a_scene_init_group_20261006`
- B00C: `dot_runtime_a_cleanup_group_20261006`

The actual current `ovl_splice.splice_group` ownership check, run against a
historical lock-mapping fixture in temporary storage, refuses all five with
`already spliced outside group dot_runtime_a_text_helpers_20261006`. The fixture
lock is unchanged and no production destination is used. Only unrelated layout
generation is suppressed for that refusal test; no acceptance logic is replaced.
The fixture deliberately records historical ownership so a future coordinator
migration will not make unrelated tests fail by pinning the evolving lock file.

This research draft keeps every verified member and the existing group name.
It does not propose or carry out any lock migration. A checker-owned coherent
consolidation is required before installation, image/recompression gates and
cartridge SHA-1 acceptance. No ROM payload, raw assembly dump, compiled binary,
credential or unrelated private data is published.

Protected-path guard passes. Mixed packet/renderer/installer tests pass **12/12**
in 0.69 s; the commit guard reports all **583 static locks intact**.
