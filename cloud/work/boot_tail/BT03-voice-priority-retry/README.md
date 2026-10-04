# BT03 voice-priority topology retry

**Rejected bounded control. The retained whole-function result remains 88/108.**
One new source form repairs an attributable bucket-test topology, but worsens
strict positional comparison to92/108 and expands the native48-byte frame to56.
No new verified function, verified bytes or unique attempted function is claimed.
This is source-family research, not cartridge integration or ROM coverage.

Exclusive claim: `func_8001EF8C`,432 bytes at `8001EF8C–8001F13C`, central claim
`4d0f5b4a`. Source base is `46e85da31134966ac521115b9543a7bdd8ffaa7b`, on
`dot/boot-tail-bt03-voice-priority-retry`. Only this new packet changes. The
historical packet, retained source, aggregate ledgers and draft PR77 stay frozen.
Central owns integration and publication; merging stays with the independent checker.

## Evidence before the one control

The entire native function, its only callee8001EE9C, all four direct boot-tail
callers, the retained source, both prior controls and stock IDO lowering were
inspected before editing. Fresh replay reproduces the retained88/108 result.

The pinned [AxioDL/musyx voiceSetPriority source](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthvoice.c#L270)
uses an assignment-valued bucket-head test and preserves a separate live bucket
read in the nonempty backlink update. Git blob
`3ad906e217a82e77b649edc8935fd6c139d09415` was recomputed from the local retrieved
file. The pinned repository [LICENSE](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE)
is CC0-1.0, blob `0e259d42c996742e9e3cba14c677129b2c1b6311`, as independently
verified in the earlier voice-allocation packet. No donor file is vendored here.

Native8001F00C loads the bucket head. F014 tests that same value, F018 stores it
into the link's next field in the branch delay slot, and F01C rereads the bucket
on the nonempty path for backlink indexing. The retained source instead stores
next, rereads the bucket for the test and reuses that second read for indexing.
The three historical forms only changed byte-slot spelling or loop stopping;
none had the assignment-valued bucket test.

The control changes precisely those two source statements to an assignment-valued
test. Its nonempty backlink expression is untouched. `verify.py` checks exact
source replacement equality, so declarations, argument widths, index expression,
loop condition/body and all other stores remain identical. The control reproduces
the native local load/test/delay-store/reload sequence. That local result is real,
but it does not establish a whole-function improvement.

## Strict results and stop

| Form | O2 result | O2 ELF bytes | O2 frame | O1 result | O1 ELF bytes |
| --- | --- | ---: | ---: | --- | ---: |
| Retained source |88/108, no excess|432|48|108/108 +50 excess|636|
| Historical initial byte-mask form |101/108 +2 excess|444|48|108/108 +51 excess; bounded HI16 error|640|
| Historical explicit loop-stop form |102/108, no excess|420|48|108/108 +48 excess; bounded HI16 error|628|
| New assignment-valued test |92/108, no excess|432|56|106/108 +51 excess|640|

Historical controls were replayed unchanged for provenance, not proposed again.
All eight rows remain rejected. The new form has complete432-byte STT_FUNC extent,
zero masks, zero unresolved or unverified references and zero relocation errors.
Relocations are also checked across the entire text of every object, including
rejected O1 controls. The old O1 bounded-comparison HI16 errors are preserved in
the receipt: their matching LO16 lies beyond the native comparison window, while
full-text relocation succeeds. The scorer and all errors are left unchanged.
Only zero section padding is permitted beyond each actual ELF function extent;
short or excess bodies receive no equality credit.

The unchanged workbench diagnoses the original and new forms as structural
mismatches. The new form has41 normalized geometry edits and18 opcode-distance
edits, versus53 and22 for the retained source, but the authoritative positional
strict score worsens. Its save slots remain8 bytes; non-save frame space grows
40→48. This is measured frame growth, not a claim that a specific hidden compiler
temporary has been identified. Workbench input was a temporary literal-word target
object, so missing target relocation names are a diagnostic limitation. The
strict scorer resolves every real candidate reference.

Other native differences remain independent: the retained source loads the
identifier's low byte directly while native code loads the packed word then
masks; the compound loop lowers differently from native traversal; and local
homes/register choices differ. The donor has different local widths/order,
group-store ordering and a final hardware-priority call absent from this native
body. None is transplanted without separate native evidence. No second new form,
repeat loop-stop experiment, index-width experiment, declaration sweep, ABI
widening, flag sweep or synthetic frame adjustment was attempted. A future
revisit requires new attributable evidence beyond this exhausted control.

## ABI and behavior

The real interface is a voice pointer plus one byte priority, with no return value
used by its four direct boot-tail callers:8001F898,80022514,80022580 and80024988.
The wrappers explicitly byte-mask or byte-load the second argument; the callee
also narrows it. The native layout is priority+46 and packed identifier+96.
Fourteen IDO assertions verify the32-bit pointer/type widths,100-byte prefix,
4-byte channel link and4-byte halfword group link with all relevant offsets.

The slot and its link pointer are captured before the real unlink helper.
Same-priority active entries return without mutation. When unlink is needed,
subsequent bucket/group tables are read live; a changed identifier does not select
a new link. The native helper has one pointer argument, no callees and no hidden
callback/extra parameter. The fixture's stronger synthetic replacement-table
behavior checks what the external call boundary preserves; it is not a claim that
the original helper arbitrarily replaces all tables.

The valid domain is registered slots0..31, byte neighbors0..31 orFF, priorities
0..255, group neighbors0..255 orFFFF, and valid acyclic sorted group lists. Actual
static arrays are distinct:32 four-byte links at504C8,256 bucket bytes at50548,
and256 four-byte groups at50648. Under that domain, assigning a link next byte
cannot alias the bucket-head array, so using the assignment value for the test
preserves behavior. The source keeps the live nonempty reread; it does not assume
broader no-alias, no-call-mutation or concurrency guarantees. Existing groups are
not inserted twice, and group equality is not an extra malformed-list domain.

Both baseline and control pass:

- The frozen2,048-case actual-source unlink/group fixture.
- A new122,880-case C89 ASan/UBSan fixture covering every registered slot and byte
  priority, five post-call group lengths, active values0/1/2, unchanged early
  return, independent live post-call tables, changed identifiers, exact lists and
  every untouched voice byte. Priorities0 and255 and both sentinel widths are
  covered.
- Fourteen separately compiled semantic mutations are rejected, including stale
  bucket capture, recaptured identifier/link, missing assignment/backlinks/head,
  wrong active test or order, lost early return and missing state update.

Only LeakSanitizer is disabled under ptrace. No malformed list, concurrent write,
invalid pointer or cross-array alias guarantee is inferred from these tests.

## Replay

With the pinned shared IDO/toolchain environment, run from the checkout root:

```sh
python3 cloud/work/boot_tail/BT03-voice-priority-retry/verify.py
python3 -m unittest discover -s cloud/work/boot_tail/BT03-voice-priority-retry -p 'test_*.py' -v
```

Input pins cover the whole immutable historical packet, scorer, target manifest,
extents, inventory, getter, compiler files and the new control/fixtures. Fresh
verification checks439 target starts /99,120 bytes, manifest integrity and the
existing getter. `verification.json` contains source hashes and source-free
results; `diagnosis.json` records compact workbench summaries. Native words,
assembly listings, compiler intermediates and objects remain temporary and are
not published. No canonical match, protected path, symbol/header/layout/lock,
T050 implementation, restricted helper or other frozen source is changed.
