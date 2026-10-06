# Voice-free authentic donor route: 28/64 NONMATCH

## Current receipt schema (2026-10-06)

Current receipts omit whole-file manifest, scorer/own-data tool, lock and redundant
production-context digests. The recorded BASE and `git show BASE:path` source reads
remain, as do native target authentication, own packet/verifier/C-harness bindings,
compiler executable identities, complete compiled extents, relocations, owned data
and behavioral evidence. Historical compatibility normalizers accept only their
explicitly listed legacy fields; unknown proof fields and changed invariants still
fail comparison. Descriptions of the earlier receipt schema below are historical.
This schema correction adds no matching or accepted bytes; fresh replay and the
aggregate test matrix are separate required checks.

**Research only.** `func_8001F6EC`, `[0x8001F6EC,0x8001F7EC)`, remains a
complete **256-byte / 64-word NONMATCH**, with **28 differing words**. The
archived baseline freshly reproduces 30 differences. New matching bytes,
accepted-byte gain and cartridge-coverage gain are all **zero**.

Base: `cd22879d40b3de443cfde047b86e75e159b6cec6`. Only this research packet and
its focused test file are added. Existing source, locks, generated targets,
compiler recipes, scoring tools and central ledgers are untouched. The body is
[retained under nonmatch/](nonmatch/func_8001F6EC.c), never submitted as a match.
Merging and any eventual integration remain with the independent checker.

## Authentic source and the bounded hypothesis

The authenticated [MusyX donor, synthvoice.c:voiceFree](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthvoice.c#L643)
identifies the function's purpose: remove a voice from priority tracking, clear
its command pointer and priority, append its slot to the free queue if necessary,
update one running-voice count, then invalidate its identifier. No arcade
counterpart or recovery of the original N64 spelling is claimed.

Donor repository commit: `78d2e16e4905fc675952162d331c24d5198b2687`.
`synthvoice.c` blob: `3ad906e217a82e77b649edc8935fd6c139d09415`.
[Donor synth.h](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/include/musyx/synth.h)
blob `a885f4f9a4f21c0dfdf8aa82a1e6615944063e56` establishes an unsigned word
identifier, byte priority, pointer command address, and four-byte free-link record
with two byte links and a halfword user flag. The repository's
[CC0 license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE)
blob is `0e259d42c996742e9e3cba14c677129b2c1b6311`. No donor file is vendored.

The newer donor includes `macMakeInactive` and optional PC/debug work absent from
this N64 target. Those operations were not imported. Native layout and behavior
remain authoritative: one real `8001EE9C` call, 416-byte packed voice state,
identifier +96, priority +46 and external-voice flag +76. The accepted N64 source
currently represents command +0 as a word, and that representation is retained.

The archived `BT03-high-larger` packet had tried an indexed baseline, a real
link-pointer local and a byte-index control. It retained the link-pointer source
at 30/64. Its controls are not repeated as a new search. The donor supplies two
previously untested differences: index assignment nested in the link expression
after the two clears, and source ordering of the free-queue stores.

Five complete O2 builds isolate the outcome:

- Archived baseline: 30/64, exactly 256-byte ELF body.
- Full donor topology: 28/64, exactly 256 bytes; retained here.
- Only the donor index/lifetime topology: 30/64, byte-identical to the baseline.
- Only donor queue-store order: 28/64, byte-identical to the retained body.
- Donor pointer declaration for command +0: 28/64, byte-identical to the retained
  body on the 32-bit target; rejected as unnecessary contract expansion.

**The index-lifetime hypothesis is disproved for this recipe. The two-word
improvement comes entirely from queue-store order.** The pointer declaration is
noncausal. No signedness/narrow-formal, O1, parameter, padding, volatile, helper
stub, fake caller or compiler-flag sweep was performed. The older O1 rejection
remains historical evidence, not a fresh result.

## Full native, ELF and address proof

`verify.py` materializes only selected immutable Git inputs from the pinned base.
The protected boot-tail manifest, canonical assembly words, symbol entry and
full 256-byte extent are cross-checked. The stock scorer resolves the complete
unmodified object without masks, unresolved symbols, unverified data, errors or
excess words. Independent GNU `ld`/`objcopy` agree with all resolved words over
the entire text section; GNU `readelf` checks exact native entry and ELF extent.
There is no owned data and no alignment tail.

The [sanitized diagnosis](diagnosis.json) uses complete native and relocated
candidate word objects, so both diagnostic inputs have the same relocation-free
geometry. It reports 28 positional differences and equal 24-byte frames. The
first divergence at +0x2C is a mask result kept in a separate temporary, followed
by a copy-back and downstream register/branch-selection differences. Workbench
ownership labels are heuristic. It identifies no evidence-supported next lever;
raw objects and listings are not published.

### Actual current accepted context

Three source bodies are matched to their **current production definitions and
normalized lock hashes**, then freshly rebuilt and independently GNU-linked:

- Helper `8001EE9C`, 240 bytes: current
  `boot_tail_promotion/sources/func_8001EE9C.c`, production `lib_1f5b0.c`.
- Caller `8001F9D0`, 72 bytes: current
  `boot_tail_promotion/voice_lists/sources/func_8001F9D0.c`, production
  `lib_1f5b0.c`. This is the accepted shared `VoiceState` adaptation, not its
  older standalone precursor.
- Caller `80021BC0`, 48 bytes: `cloud/matches/boot_tail/func_80021BC0.c`, production
  `lib_22300.c`.

All three remain full-body matches. No source body is changed or counted again.
The direct-JAL census also finds callers `8001F954+0x5C` and `80023E9C+0x200`.
Their canonical bodies/entries are bound as witnesses only. The separately
submitted `1F954` improvement is untouched. The 2,796-byte `23E9C` body is not
compiled or claimed; it is not a large-function matching attempt. Indirect and
other-image caller completeness is not established.

This preservation proof covers exact current accepted sources independently,
not a combined production-TU build or automatic compatibility of the three
existing pointer-view typedefs. Any later integration must preserve the actual
production declarations through the normal maintainer workflow.

## Behavioral proof

The [native replay](native_behavior.py) executes all 64 native and all 64
GNU-linked candidate instruction offsets. It checks both outcomes of every
conditional in both bodies and fails closed on unknown instructions, unmapped
accesses, malformed control flow and nontermination.

- **13,824 fixtures each through native and linked code: 27,648 executions.**
- Slots 0..31; all four packed-state alignments; user flags 0, 1 and FFFF; empty
  and nonempty queues; external flags 0, 1 and FF; counts 0, 1 and FF; ordinary
  and side-effecting helper-boundary cases.
- Full state and queue/global images, surrounding state canaries, exact helper
  argument/call ordering, permitted stack stores, return address, stack pointer
  and saved registers are checked. Self-tail fixtures additionally characterize
  in-range aliasing of the selected and tail queue entries.
- The mutation hook changes the identifier and external flag, so stale reads
  before the real helper boundary are detectable. The entire entry state and
  queue must still be unchanged when the helper is called.
- **13,824 actual-source C89 ASan/UBSan/bounds fixtures** with strict aliasing use
  typed packed aggregate objects. Their complete canonical output is compared
  byte-for-byte to the same native-memory oracle: 7,686,144 bytes. Host-endian
  halfword serialization is explicit; host pointer layout is not native proof.
- Four compiled source mutants fail: stale pre-helper identifier, wrong command
  clear, inverted user guard and reversed running-counter update.
- Three legal-instruction semantic mutations and an unknown-opcode mutation
  fail. Focused tests additionally reject truncation, stale candidate source,
  entry-address drift and assertion-disabled Python.

The helper in behavioral replay is a bounded O32 mutation hook. Its real native
implementation and the real caller bodies are **not executed** in those cases;
they are separately proven byte-equal above. Fixtures establish the documented
bounded table/packed-object domain, not arbitrary indices, asynchronous writes,
validity of every possible list graph, hardware behavior or gameplay.

## Reproduce and stop condition

All ten focused tests pass, including full frozen replay from an unrelated
working directory. No broader repository suite result is claimed.

With the pinned IDO compiler, MIPS GNU binutils and a host sanitizer-capable C
compiler available:

```sh
python3 cloud/work/boot_tail/BT03-voice-free-donor/verify.py --check
REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/cloud/test_dot_voice_free.py
python3 tools/cloud/check_submissions.py --base cd22879d40b3de443cfde047b86e75e159b6cec6
```

`--repo PATH` and `--output PATH` are optional. The verifier defaults to its own
repository root and supports unrelated working directories, including paths with
spaces. It uses fewer than 100 temporary files and never clones a repository.
Frozen replay binds all selected source/native/proof inputs and compiler-stage
hashes. The initial development replay corrected one witness path typo and a
Python tuple-versus-JSON-list receipt serialization defect before freeze; neither
changed the candidate or its score.

**Freeze this route.** Reopen only with genuinely new N64 source/declaration or
compiler-pass evidence explaining the packed identifier's mask-result
coalescing. The authentic donor ordering, nested index assignment and command
pointer declaration are now measured; they are not fresh hypotheses anymore.
T050 remains in force and `8001F13C` was never attempted. No full-TU, source-image,
compression, full-ROM, hosted-CI or gameplay result is claimed.

## Integration-portable replay (2026-10-06)

`portable_receipt()` compares both saved and fresh evidence after excluding only
explicit historical whole-tree/tool/source-context digests. Packet source and
verifier bindings, selected native bodies and addresses, ELF extents, relocations,
owned data, behavior, and compiler executable identities remain authoritative.
Accepted production context is read from the recorded base commit rather than
the live integrated tree. Tests are deliberately not hashed into receipts.
