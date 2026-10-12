# Image A: 712 new BB80 bytes, all eleven installed siblings preserved

## Deliverable

Same-name group: `groups/dot_runtime_a_text_helpers_20261006/`.
Stock IDO 5.3, `-g0 -O3 -mips2 -G 0 -non_shared`, with the canonical
scorer's usual `-r4300_mul` assembler setting. No patched compiler or object
is used for acceptance.

`func_8039BB80` matches its entire `[0x8039BB80, 0x8039BE48)` extent:
178 words / **712 new exact code bytes**. Every one of the eleven installed
members remains exact, totaling **5,800 preserved bytes**, not new credit.
All twelve exact bodies are listed in both `members` and `claims`. The group
keeps its installed name; no cross-group owner transfer is required. Production
source and locks are untouched. The independent checker retains installation,
merging, image reconstruction, compression and the cartridge SHA-1 gate.
New accepted-image / ROM bytes claimed here: **zero**.

Baseline: current master `262e9f0f3`. Live claims, locks and open PRs were checked
before reservation commit `3e41806f0`. A4D4 and BB80 were reserved before edits.
The source starts from the complete installed text_helpers group, not the older
research copy that lacked B560. All genuine root/caller context is retained.

## Trace and source changes

Fresh BB80 baseline: `9/178 words differ`, exact 712-byte extent.
Workbench classified an allocation/schedule residual with four register-copy
rows and a 32-byte frame difference: native 160, candidate 128. Its raw-object
relocation warning is not acceptance evidence; canonical scoring resolves all
sixty relocations in the new body.

1. Stock UGEN's two real private calls emitted destination before player. Native
   scheduling needs player before destination. Correcting B908/B988 to
   player-first formal order, and updating every genuine in-group call, removes
   exactly four differing copy-schedule words. Both callees' complete code stays
   identical, including their native private registers. This leaves `5/178`.
2. The five remaining words are the allocation/restoration frame and three
   addresses of the same 80-byte live text buffer. Eight disclosed unused s32
   local homes account for exactly 32 bytes: frame 128 + 8*4 = 160, buffer
   offset 48 + 8*4 = 80. No buffer size is changed. Original local purposes and
   names are unknown; this is an explicit frame-shaping reconstruction allowed
   by the wave rules, not a historical-source claim.

The causal controls rebuild with the unmodified compiler:

- Remove the eight scalar homes only: `5/178 words differ`.
- Restore destination-first formal order only: `4/178 words differ`.
- Restore both: `9/178 words differ`.

Every control has the same complete native extent, no unresolved relocations,
no owned-data uncertainty and no nonzero extra words. Stock UOPT output and
symbol-table bytes were also identical to an unforced diagnostic tracing build;
no forced-register oracle enters the submitted source or acceptance build.

## Strict proof

`python3 cloud/work/frontier/dot_runtime_a_bb80_20261012/verify.py --deep`

```text
func_8039B120: MATCH
func_8039B214: MATCH
func_8039BE48: MATCH
func_8039A448: MATCH
func_8039B00C: MATCH
func_8039C140: MATCH
func_8039B908: MATCH
func_8039B988: MATCH
func_8039BA28: MATCH
func_8039BAE0: MATCH
func_8039B560: MATCH
  own .rodata verified at 0x803B9594..0x803B95B4
func_8039BB80: MATCH
PASS: 712 new exact bytes; eleven previous members preserved
```

The verifier authenticates the current-tree source asset, loader pointer and
image-A hash, reconstructing bytes transiently in memory. It binds that data
to the unchanged canonical own-data checker. A bare group command correctly
refuses B560's sixteen unverified section-relative relocation sites because
there is no standalone image-A data artifact; no uncertainty waiver is used.
The authenticated command strictly accepts all twelve members. Absent data
and a single corrupted owned literal both fail closed.

The optional deep check independently parses ELF32 big-endian MIPS, checks all
complete STT_FUNC extents and next boundaries, then re-sections the unmodified
compiler words and asks GNU MIPS to link every member at its actual native
address. All **537 relocations** resolve and every linked word equals the
protected target. The entire **32-byte B560 owned pool** matches the authenticated
image. The earlier twelve object-rodata bytes belong only to NONMATCH A5948
context; they are neither installed nor credited. Four final zero alignment
bytes are verified separately.

Protocol-v2 `verification.json` binds submitted source/recipe/own verifier/test,
selected target rows, parsed full target-word hashes, only source-used symbol
rows, direct private-caller sites and the authenticating input hashes. It does
not pin mutable tools, locks, Makefiles or assembly text, and it requires no
non-master historical checkout. No static ROM TU is compiled; IDO 5.3 is
identified through the repository's compiler helper.

One routine test:
`REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_runtime_a_bb80.py -o addopts='' -q`

Result: **1 passed in 0.41 s**. Missing IDO: **1 skipped in 0.14 s**, through
shared conftest. Normal and optimized-Python deep replay both pass; the
verifier's acceptance guards remain active under `python -O`.

## ABI and remaining research

The native direct-call census binds all calls involving the changed private
argument order: BB80 calls B988 at +0x1D4 and B908 at +0x1F8; A5948 has one
B988 and two B908 calls. All those genuine callers are present and all source
call arguments are updated consistently. BB80's real caller is BE48 at
+0x278, also a preserved exact member. C140 and A5948 remain the genuine kept
roots. BB80 is not an ordinary independently callable O32 function: it saves
ra and uses the image's private s-register argument/clobber convention.

BB80 computes panel dimensions using localized text, configuration counts,
player names and rank/count helper strings for one or two players. Helper
return types, live storage and global contracts stay unchanged. Valid strings,
records and game state remain assumed; this packet does not establish arbitrary
input memory safety or execute the actual external drawing/text services.

The reserved larger A4D4 remains **623/716**, A5658 **32/184**, A5948
**1025/1080**. These complete NONMATCH contexts are not claimed. A bounded
A4D4 investigation authenticated six float literals and tested natural literal
ownership, local scale/matrix carriers and two expression spellings. The
canonical residuals were 625, 637 and 645 words, respectively; none improved
the baseline. Those probes were rejected, and A4D4's submitted body is
unchanged from installed master. Its next useful work is structural/PRE and
live-range tracing, not a literal-only fix. This result earns no extra bytes.

No ROM bytes, raw assembly dumps, compiled objects, credentials, unrelated
private data, protected source changes or acceptance-state changes are included.

## Independent review

The separate image-B lane independently replayed all twelve complete matches
under normal and optimized Python, checked the 537 GNU relocations and 32 owned
bytes, and compared the source against the installed group. Its requested
hardening now explicitly verifies the GNU owned section's address/32-byte
extent and its offset anchor symbol, in addition to the existing byte comparison.
The final hardened proof and routine test were rerun successfully. Missing IDO
with `REQUIRE_TOOLCHAIN=1` was also checked and correctly fails instead of skipping.
