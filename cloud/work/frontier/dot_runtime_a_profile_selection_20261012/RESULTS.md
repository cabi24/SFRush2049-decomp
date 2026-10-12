# Image-A profile-selection renderer: stock exact A7B58

Baseline: current master `84d8a4827e7fed06365846e013351b580593d921`.
Protocol-v2 candidate only; independent checker owns installation, accepted locks,
ROM splice and merge. No native bytes, raw assembly, compiler objects or resources
are included. Native hashes authenticate transient inputs, not raw assembly text.

## Result and ownership

- New stock exact candidate: `func_803A7B58`, all 799 words / 3,196 bytes,
  plus all 36 owned literal/table bytes at `0x803B9848`.
- Preserved existing profile-group bodies: `95E24` 104 B, `A87D4` 920 B,
  `A8B74` 1,300 B, totaling 2,324 B.
- `96EE8` and `96F44`, 92 B each, remain exact: these 184 B are verification-only
  inherited duplicates. Their current standalone `src/ovl_a/func_*.c` ownership
  and O2 accepted locks remain unchanged. They are listed in members/claims to
  verify every emitted exact body, not to authorize automatic reassignment.
  The checker must reconcile standalone ownership before installing this
  superseding profile group. Do not blindly unlock or replace those locks.
- Total inherited verification coverage: 2,508 B. New landed-image bytes and new
  ROM-verified bytes: zero. No accepted production source or locks change here.
- A9088 remains 15/917, A9EEC 281/315. Genuine 96FA0 remains NONMATCH: 717/718,
  emitted 3,036 B versus native 2,872 B, including 39 extra nonzero words.
  These are context only, never members or claims.

## Source reconstruction and causal evidence

Reconstructed the real 96FA0 profile-input controller instead of an artificial
caller. Its real helper call frequency naturally keeps both eligibility helpers
out of line. The helpers each have one meaningful skip argument, in genuine
private a1 after whole-group allocation. Removing an eager player-count local
restores both native 92-byte bodies and removes the former artificial a0 setup
from A7B58. 96FA0 itself still has an o32 entry instead of its native private s0
player entry because its genuine outer 9842C context is absent. That ABI blocker
is explicitly unclaimed; no invented formals or fake callers were added.

A7B58 recovers native indexed slots, guarded profile walks, split postincrement,
conditional orientation, direct stable global reads, narrow width boundary,
and the genuine profile pointer's reused lifetime. One compiled-out `if(label)
{}` after measurement raises the traced name-pointer allocation priority above
width. This source-shaping read is disclosed; it emits no extra instruction and
changes no consumed value. There is no unused scalar, synthetic padding, inline
assembly or forced compiler in the candidate.

Stock controls, with every inherited body still exact:

- Current-master renderer body in the recovered real group context: 645/799,
  emitted 3,164 B. Baseline missing-tail and owned-table differences remain
  explicit; this is not a prefix-only score.
- Recovered renderer with previous text[40] and corresponding real declaration
  order: exactly two frame-adjustment differences / 799, complete 3,196 B,
  frame 184 versus 176 at +0x10/+0xc70 only. Every active home already agrees.
- Remove only the compiled-out label read: 11/799, complete 3,196 B.
- Final: strict stock MATCH, complete 3,196 B and all owned data. Stock IDO 5.3,
  `-g0 -O3 -mips2 -G 0 -non_shared`; no static ROM TU is rebuilt.

Stock ugen tracing records the final M=140, T=20, P=4, frame=176; the text[40]
control has M=148, frame=184. The trace tool verifies that diagnostic tracing
leaves the whole compiler object identical to the ordinary untraced build.
Prior forced-color experiments were diagnostic only and are not deliverables.

## Bounded temporary text and complete native frame

The safe 32-byte reconstruction was selected only after establishing the writer
bound independently of the matching frame. Exact original capacity and original
non-accessed scalar declarations remain unknown; this is not oracle recovery.

The complete A7B58 address-use census has the declaration, one formatter
write, and the read-only width/render consumer expression. Its authenticated
format at `0x803B899C` is `%s %d`. The current native resource loader association,
all six language payloads and their label[29] entries are independently decoded
and authenticated in `capacity.py`; longest label is seven bytes. Valid renderer
players 0..3 need at most 7 + 1 + 1 + 1 = 10 bytes including NUL. Independently,
a signed-32 formatter argument needs at most 11 decimal bytes, so the format
needs at most 20 bytes. INT_MIN/INT_MAX tests are formatter-only tests: no invalid
renderer player is used to evade the real player+1 and array-index domain.
The current accepted static formatter source and macros are input-bound;
`PRINT` copies only each calculated segment and `FLUSH` terminates it.

`consumer_audit.md` records the independent native provenance audit across
width, renderer, byte-counter, copy and glyph helpers. Every complete game-helper
word target is authenticated and bound, and the static memcpy target is bound
as the current tracked eleven words only (no new static ROM authentication).
No other writer or persistent input-pointer escape exists under the backed,
disjoint renderer-global and synchronous helper contracts. This does not cover
corrupt globals, arbitrary aliases into stack storage or asynchronous mutation.

Full native stack census, including every sp-relative access and all address
formations, is in the receipt. Native frame is 176 B. Address-taken objects:
position [92,104), point [112,116), reconstructed text [120,152). Native live
scalar homes: x [164,168), y [168,172), draw_x [172,176). Inferred non-accessed
homes: node [88,92), seen [104,108), i [108,112), limit [116,120), label [152,156),
profile [156,160), scale [160,164). These non-overlapping inferred homes are
reconstruction, not proof of original source declarations. The output bound
ends well before any later scalar home, and every actual native access agrees.

## Verification

`verify.py --deep` replays strict canonical scoring, complete ELF function
extents and suffix boundaries, owned-data negative controls, an independent
GNU relocation link, actual selective-installer extraction, stock stack tracing,
and actual-C host equivalence. GNU resolves 277 text relocations and all six
jump-table relocations and checks every linked member word and owned byte.
The fail-closed full emitted-function census covers exactly six checked members
and three unclaimed contexts, including their complete sizes, overlap/gap checks
and the final zero text suffix. Contexts are not GNU-match claims.
Unknown symbols, extra allocated sections, unresolved relocations, wrong extents,
nonzero suffixes, absent or corrupted owned data fail closed. Input bindings
cover source/recipe/test/own proof code, complete target-word hashes, real caller
census, helper targets, native languages, formatter source and semantic audit.
Shared tool or lock bytes are not pinned as proof inputs.

The host compares complete actual-C helper-event traces against the current
master renderer across 5,760 valid cases (all six languages, player counts 1..4,
all valid players, 96 scenarios), plus 60 independent signed-32 formatter
extreme cases. Profiles have genuine backing beyond their name[1] tail marker.
This is synchronous helper-contract evidence, not native gameplay testing.

Reproduce from repository root with the repository's configured stock IDO 5.3
and GNU MIPS toolchain:

```sh
P=cloud/work/frontier/dot_runtime_a_profile_selection_20261012
python3 "$P/verify.py" --deep
python3 -O "$P/verify.py" --deep
python3 "$P/verify.py" --sanitize
python3 "$P/verify.py" --score
python3 -m pytest -q tests/cloud/test_runtime_a_profile_selection.py
python3 -m tools.cloud.guard_paths --base 84d8a4827e7fed06365846e013351b580593d921
```

The single cheap regression test runs the normal strict replay; deep GNU/trace/
host/sanitizer checks are explicit. Production ROM splice, build/checksum,
emulator and gameplay gates remain unrun and belong to the independent checker.

Local verification: normal and Python `-O` deep replay passed; ASan/UBSan
passed the full 5,760-case matrix and 60 formatter cases. Ordinary group score
reports all six members MATCH. Strict focused test and missing-IDO skip were
checked separately. Independent peer review and exact-head CI are reported
in the draft PR, not implied by the local receipt.
