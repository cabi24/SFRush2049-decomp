# BT03 constructor dependency pair: COMPLETE-NONMATCH

Both functions are complete C89 reconstructions with **zero matching credit**.
No matching source is submitted under `cloud/matches/boot_tail/`.

- `func_80019F48`: `[0x80019F48,0x8001A270)`, 808 native bytes. Retained O2
  source differs in 170/202 relocated words; actual ELF function is 816 bytes,
  with two nonzero words beyond the target. Its frame is 168 versus 176 bytes.
- `func_8001A270`: `[0x8001A270,0x8001A5D8)`, 872 native bytes. Retained O2
  source differs in 195/218 relocated words; actual ELF function is 900 bytes,
  with six nonzero words beyond the target. Both frames are 96 bytes.
- Total complete research: 2 functions / 1,680 native bytes.
- Clean reused matrix worktree base: `1ba71e6c8cca24dcaffd0b831ace067b3046fe19`.
- Exclusive central activation: `35f9b0cf`, packet `BT03-constructor-pair`.
- Branch: `dot/boot-tail-bt03-constructor-pair`. Prior matrix commits preserved.
- Only this packet changes. Central owns ledger updates and publication.

## Native contract and source provenance

The complete native bodies, every direct callee, and actual callers were read
before source editing. Both functions use ordinary O32 with ten genuine inputs:

- Layer constructor `19F48`: packed word, allocation halfword, key byte, volume
  byte, pan byte, channel byte, set byte, offset halfword, section halfword,
  group byte. The last six inputs occupy old sp+16 through sp+36. Return is a
  root/portamento identifier or `FFFFFFFF`; zero is a valid successful result.
- Top-level dispatcher `1A270`: packed word, key byte, volume byte, pan byte,
  channel byte, set byte, offset halfword, section halfword, group byte, signed
  priority-offset halfword. The last input is read with signed `lh`. Both real
  callers `17D38+0x2A0` and `1B1D0+0xAC` establish all ten slots.
- `1A270` calls `19F48` at offsets `0x2F4` and `0x340`. There is no invented
  extra formal, register convention, indirect jump, switch table, or literal pool.

The packed word's high halfword is the resource ID, its middle byte is priority,
and its low byte is the allocation maximum field (public-source naming lead).
The top two ID bits select direct macro, keymap, layer, or invalid category.

A close SOURCE-LEAD is the public `StartLayer`, `StartKeymap`, and
`synthStartSound` family in pinned
[AxioDL/musyx synth.c](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synth.c),
revision `78d2e16e4905fc675952162d331c24d5198b2687`, under pinned
[CC0-1.0](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE).
Both reference and license were fetched; Git blob identities are pinned in
`input_pins.json`. This is family provenance, not proof of the original N64 TU.
No entire donor source is vendored. Arcade reference checkout was unavailable.

Native differences are deliberately retained: the N64 packed interface, simpler
portamento check, no recursive nested-layer/keymap dispatch inside `19F48`, no
later-version voice-block traversal, and no accumulated priority across layers.
The layer's priority adjustment starts from the original packed priority on
every iteration. Later public-source logic is not imported to fill nonexistent
native behavior.

## Whole behavior and concrete storage

`19F48` looks up a layer list and its real address-taken halfword count. Entries
are packed 12-byte objects: ID+0, low/high key+2/+3, signed transpose+4, volume+5,
signed priority delta+6, pan+8, with three unexplained trailing bytes. It visits
entries in order, skipping `FFFF` IDs and key ranges excluding `key & 127`.

For an eligible entry, it clamps transposed note to 0..127 and calls portamento
**before** rejecting non-macro ID categories. A successful portamento value
returns immediately, even after earlier voice allocations. Otherwise pan is
128 when the entry's high flag is set, or entry-pan minus 64 plus input pan,
clamped to 0..127. Volume is the integer product divided by 127 and then narrowed
to a byte, including modulo-256 narrowing when the quotient exceeds 255.
Priority is adjusted and clamped to 0..255. ID category zero creates a macro.

The first successfully created macro is registered as a root using `1ECE0`.
If root registration fails, later entries retry first-member behavior without
rollback. Once a root exists, new successful macros are linked by their internal
IDs at previous-voice child+16 and new-voice parent+20. The genuine previous-ID
local is assigned by constructor success before any semantically live use;
there is no invented zero initializer. Native code speculatively loads its home
before the loop. The source preserves its conditional initialization protocol.

`1A270` first applies the signed input priority adjustment and clamps. Direct
macro IDs check portamento then call the eleven-input constructor with start=1.
Layer IDs delegate to `19F48`; invalid category returns `FFFFFFFF`. Keymap IDs
look up a packed 8-byte entry array: ID+0, signed transpose+2, pan+3, signed
priority delta+4, and two unexplained trailing bytes. The selected ID/transpose/
priority use index `key & 127`. A sentinel selected ID fails; transformed direct
macro IDs check portamento then construct with start=1, while every other ID
category delegates to the layer function.

Important preserved asymmetry: the pan **flag** is checked in entry[key&127],
but when that flag is clear, the pan **value** comes from entry[raw key], which
can be index 128..255. The pinned public source has the same asymmetry. It is not
silently repaired by masking the second access or rejecting the external bit.

## Callee and caller audit

- `16F80(u16,u16*)`: resource pointer or null; sets count only on lookup success.
  The returned packed table uses 12-byte entries. The caller does not evaluate
  the C count when lookup returns null, although native delay-slot code loads it.
- `16EE0(u16)`: keymap pointer or null. Packed metadata lookup returns the stored
  pointer; table length is an external resource validity requirement.
- `19ED0(u8,u8,u8)`: channel 255 rejects; otherwise queries the real controller
  and normalizes key to seven bits before its `19C8C` call. Returns an existing
  identifier or `FFFFFFFF`. Its actual inputs and full behavior were inspected.
- `24988(u32,u16,u8,u8,u8,u8,u8,u16,u16,u8,u8)`: eleven real inputs. Its native
  frame reads seven outgoing stack slots, including start and group separately.
  The reviewed core reconstruction and full native body agree. With start=0 it
  returns an internal voice identifier; start=1 returns the root helper result.
- `1ECE0(Voice*)`: one pointer, root key or `FFFFFFFF` on exhausted free nodes;
  it writes the real sequence-node pointer at voice+24 and reads identifier+96.
  The caller's packed Voice layout has the actual 416-byte stride and only names
  the child/parent fields it directly writes. Remaining arrays are real opaque
  object extents, not frame padding.
- `17D38`: supplies clamped note/volume, pan 64, live channel/set/track values,
  offset zero, and priority offset -1 or 0. `1B1D0`: combines preset fields,
  forces the key external bit, and supplies the same ten-slot interface.

All layouts and widths are asserted by IDO, not inferred from host pointer sizes.
The external core and lookup sources can remain NONMATCH without leaving their
native ABI unknown. This packet closes the constructor interface required by
`17D38`; it does not claim that separately reconstructed helpers are integrated.

## Domain and semantic verification

Inputs are valid initialized resource records. Returned pointers must cover all
entries that can actually be accessed, and each successfully returned internal
voice ID must index a live voice record after low-byte extraction. The 256-slot
voice array in synthetic tests is fixture storage, not a claim that the game
owns 256 voices. Lookup-returned counts must stay within the real layer allocation,
including a valid one-past pointer at loop termination. Helpers must preserve
those lifetimes and domains. No malformed resource or allocation safety guarantee
is made; the native code does not check those bounds.

For keymaps with external-bit keys, normal pan mode requires backing storage
through the raw-key entry. Flagged pan mode skips that second access. A 128-entry
resource is not declared safe for every byte key. Tests provide 256 entries to
exercise the native asymmetry legitimately. Signed stored fields use the native
and tested host two's-complement representation; arithmetic itself stays in
small signed ranges, and all packed left shifts use unsigned words.

`test_native.py` compares an independently authored high-level oracle with both
canonical native words and the entire freshly relocated compiled candidate,
using two different stack poison fills. It passes 2,218 cases / 8,872 executions.
It covers all 256 keys, four category branches, sentinel IDs, signed transpose
and priority extremes, flagged and clamped pan, modulo volume, portamento early
returns, constructor/root failure, root retry, link writes, zero success, empty
lookups, count reload after a helper, and randomized valid layer sequences.
All modeled helpers poison caller-saved registers; saved registers and sp are
checked. Unsupported instructions and unmapped accesses fail closed.

The only permitted speculative uninitialized loads are recorded explicitly:
native layer previous-ID at old sp-12, candidate previous-ID at old sp-16, and
native null-lookup count at old sp-2. They are never live C reads. Both poison
fills must yield exactly the oracle's result, call trace and memory. This is a
bounded semantic test, not full dataflow proof or hardware emulation.

`test_host.py` compiles the actual retained sources as strict C89 at O2 and with
ASan/UBSan, passing 582 layer cases and 1,636 dispatcher cases in each build
(4,436 host calls total). The host uses the same independent expected fixtures
and verifies all external argument slots, returns, and complete child/parent
arrays. `-fno-inline` keeps synthetic helpers at explicit call boundaries and
avoids a GCC false-positive lifetime warning for the deliberately retained count
pointer, which is consumed only during the active call and cleared on return.
Leak detection alone is disabled for the traced sandbox; no allocation is used.
The native instruction interpreter is adapted from the independently reviewed
sequence-dispatch packet and expanded for signed arithmetic/packed accesses.
Actual helper bodies, paired end-to-end game execution, ROM identity and gameplay
are not covered by these contract-stub tests.

## Bounded search and residual

There are thirteen retained source forms, compiled at O2 and O1: six layer forms
(initial plus five directed controls), seven dispatcher forms (initial plus six).
Workbench diagnosis ran before tuning and on the final source. All 26 compile
rows replay with hashes; no declaration permutation sweep or additional flag
search occurred.

Layer hypotheses exposed the genuine ID/maximum/key fields or alternate lookup
null topology; none improved the complete baseline. Dispatcher hypotheses made
signed priority explicit, tested local transformed packed storage, staged the
low field, and cached the selected macro ID. Some lower positional scores add
an eight-byte frame change and retain excess code. The final deliberately keeps
the straightforward signed-priority baseline and native 96-byte frame instead
of selecting a scalar score reduction alone. There is no padding local, invented
argument, keeper, inline assembly, forced register, or fake helper.

O1 controls reject both bodies: layer 200/202 differing words and 1,028-byte ELF;
dispatcher 214/218 and 1,108-byte ELF. Native branch-likely shape and loop CSE
support O2. The remaining frame/structure/allocation differences are real; the
workbench's `cfe-spelling` ownership diagnosis is heuristic. Next useful evidence
is original packed-parameter/source-expression topology or compiler IR analysis,
not more blind register/stack shaping. This packet stops at its observed plateau.

## Reproduction and limits

Set `IDO_DIR` to the existing pinned IDO 5.3 installation. Diagnosis also accepts
`MIPS_OBJDUMP`, with that package's normal library path. No toolchain was installed
or copied. All 24 compiler files, scorer/setup, getter, census and target manifest
are pinned. All 439 starts/extents reconcile to 99,120 bytes, and the known getter
strictly matches at its exact 12-byte ELF extent.

```
P=cloud/work/boot_tail/BT03-constructor-pair
python3 "$P/verify.py"
python3 "$P/replay_controls.py"
python3 "$P/test_native.py"
python3 "$P/test_host.py"
python3 "$P/diagnose.py"
```

No protected targets, scorer, symbols, layout, shared headers, locks, or central
ledgers change. No raw native dumps, object files, ROM bytes, credentials, or
unrelated data are published. Promotion, splice, full-ROM hash, `make test`, and
cartridge-coverage gates are out of scope and NOT RUN.

## Independent review

Reciprocal review PASS binds frozen source commit
`d21bb30439a7fa4c2b4c826ead17657c5bb85c43` and tree
`75884fd429e79881039c6dabcfff5974df4e2972`. The reviewer independently read
both complete native functions, actual callers, all external interfaces, packed
record domains, source provenance and license, then reproduced all five reports
exactly. No source correction was requested. Approval is for complete NONMATCH
research only; it does not confer exact-match or cartridge credit.

Four independent actual-source defects were rejected: incorrectly masking the
raw-key pan access, rejecting the category before portamento, returning early
on root failure instead of retrying, and swapping the real start/group argument
slots. `test_peer_mutations.py` reproduces these checks in temporary copies;
`peer_mutations.json` records the rerun. This review-only follow-up adds the
immutable receipt, portable test runner and this section; retained C and its
verified source hashes are unchanged. Aggregate-head CI remains a central gate.
