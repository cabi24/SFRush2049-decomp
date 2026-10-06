# Runtime image B: texture and 25-object ring initialization

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

**Strict MATCH: B:func_8038A8CC, [0x8038A8CC, 0x8038A95C), 144 bytes / 36 words.**
New matching-candidate evidence only. Accepted-byte and ROM-coverage gain: zero.
Merging, source admission, image composition, recompression and ROM verification
remain with the independent checker.

The first natural typed reconstruction matches ordinary IDO 5.3
`-g0 -O3 -mips2 -G 0 -non_shared`, with the existing mandatory
`-Wab,-r4300_mul`. There was no source-variant search, inline assembly,
artificial local, pressure keeper, fake formal, modified helper or changed flag.

## Identity and source evidence

Base master is `cd22879d40b3de443cfde047b86e75e159b6cec6`. This is **runtime
image B**, ROM stream `0xB6FEC4`, loaded at `0x8038A400`, image SHA-256
`b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd`.
In image A the same address is inside unrelated `func_8038A820` (1,168 bytes).
Current source, runtime-image submissions, research and 32 open PRs were checked
before assignment; no complete prior A8CC source/receipt or active claim was found.
R16 had generic raw small-function seeds, not a retained A8CC matched submission.
The separately submitted B:8038CA24 reset is untouched and receives no credit here.

[Source](../../matches/ovl_b/func_8038A8CC.c) reconstructs these ordered actions:

1. Clear byte +0 and halfword +2 of the state at `0x80399A70`.
2. Call the real texture lookup at `0x800B24EC` with the existing texture-name
   address `0x80394D14`, halfword output `0x80399AD8`, signed-byte low bound 0,
   signed-byte high bound `(s8)(D_80140BDC - 1)`, and error policy 1.
3. Clear all 25 pointer slots beginning at state +4, after the helper returns.

The actual adjacent consumer **B:8038A408**, 1,220 bytes, witnesses a signed-byte
wrapped flag at +0, unsigned-halfword next index at +2, 25 four-byte pointers at
+4, and wrap at 25. Pointer values are passed to object services, including
`0x8008C874`, whose body dereferences its argument. The unused byte +1 is
persistent layout, not stack padding. Native layout is 104 bytes; `D_80399AD8`
is the following separate halfword texture-index view. Four IDO-compiled layout
assertions establish size, both member offsets and pointer width.

The accepted `src/blob/func_800B24EC.c` at the pinned base is independently
source-hashed (`405f7b1f5c31e4e4c19b82d745a778e85c2046903cea6d439d5d0564e925712b`)
and agrees with its existing game lock. It supplies the five-formal boundary
`NameEntry *(char *, s16 *, s8, s8, s32)`, derived from arcade
`MBOX_FindTexture_Sub`. Its fifth argument is the authentic `MBOX_WARN` policy;
diagnostics are compiled out in the accepted N64 helper. Older four-formal
research is superseded by this accepted source. The caller ignores the return.

The count's volatile byte view is inherited from that accepted helper and its
separately documented repeated address-form observations. This target itself
observes the count only once; it does not independently establish a volatile
source declaration, asynchronous writers or concurrency. No new qualifier was
introduced as a residual-tuning experiment. No whole-function arcade donor is
known for this N64-specific initializer. Names and original type spelling remain
inferences, and no original translation-unit boundary is claimed.

## Verification

`verification.json` binds the exact source, verifier, host harness, focused tests,
protected target/extent/symbol manifests, scorer and accepted-helper source.
Tool executable hashes are recorded in `tool_provenance.json` as provenance,
not compared across hosts. Recreate them with `--tool-provenance PATH`.

- Complete ELF function and `.text` extent are both 144 bytes. There is no
  omitted trailing word, alignment tail, owned literal, jump table or data.
- All **15** relocations have pinned offsets, kinds and symbol names. All target
  entries and consumed symbol addresses are checked. Independent GNU ld links
  the unmodified full object at `0x8038A8CC`; its complete body equals both the
  native body and project relocation. GNU readelf confirms symbol address/size.
- **4,096** fixtures compare the protected native, project-relocated and
  GNU-linked instruction streams: **12,288 native executions**, all 36 instruction
  offsets. Every count byte is crossed with 16 callback effects.
- The bounded texture-service hook checks all five arguments and the complete
  memory snapshot at the call. It then changes the flag, index, unused byte,
  pointer slots, texture output and count, and clobbers every caller-save GPR.
  Post-call pointer clearing and preservation of all other helper effects agree
  with an independent byte-offset oracle. The full write footprint, single
  count observation, frame/canaries, return and saved GPRs are checked.
- The unchanged C89 source with UBSan passes **4,096** valid-object host fixtures
  using legitimate pointers. Complete object representations, including padding,
  are compared with an independent expected record. Hosted pointer size may be
  64 bits; this is semantic evidence, not the separate native-layout proof.
- Five actual compiled source mutants are rejected by both behavior and strict
  scoring: missing final pointer clear, incorrect table upper bound, incorrect
  error policy, unwanted unused-byte clear, and resetting the index after the
  callback. Unknown instructions and invalid/misaligned mapped accesses fail
  closed. Six focused tests include a fresh mandatory full-proof replay.

Native replay is a narrow fail-closed interpreter, not N64 hardware. The real
texture-search implementation and adjacent consumer are not executed here.
The adversarial callback is a boundary model, not a claim that the actual lookup
mutates the ring. Finite tests do not establish unrestricted pointers, arbitrary
aliases, texture availability, unbounded callbacks, concurrency, image residency,
whole-game behavior or gameplay. No image, compression, ROM, broad repository
suite or hosted-CI gate was run.

## Reproduction

From a full checkout with IDO 5.3, MIPS GNU binutils, GCC and Python available:

```sh
python3 tools/cloud/score.py fn cloud/matches/ovl_b/func_8038A8CC.c func_8038A8CC --targets asm/us/ovl_b
python3 cloud/work/runtime_b_texture_ring_20261006/verify.py --check
python3 cloud/work/runtime_b_texture_ring_20261006/test_packet.py
```

For a sparse source-only review, `RUSH_REPO` can point to an existing checkout
holding the pinned target/scorer inputs and base Git objects, while the packet
and candidate remain beside one another. `IDO_DIR` and `TMPDIR` support an
existing compiler and a workspace-backed temporary directory. The accepted
helper and lock are read from the pinned base's Git objects without materializing
the rest of the source tree. No repository/protected file is modified.

The initial layout-check compile found the stripped IDO installation lacks
`stddef.h`; the verifier uses a compile-time member-offset expression instead.
The first narrow interpreter omitted the native OR instruction; adding that
actual opcode completed the decoder. Neither correction changed candidate C.
A focused test also caught an incorrect snapshot filter that excluded KSEG0
globals above the test stack. The filter now excludes only the explicit stack
interval, with mandatory global-presence assertions; all cases were rerun.
Initial quick scoring used an older local reader; all recorded verification
uses the pinned master's current scorer and independent GNU linking.

Only C, tests and proof metadata belong in this packet. Native words, assembly
dumps, objects, linked binaries, ROM contents, credentials and unrelated private
data are not published. No merging or CI watching is started.

## Integration-portable replay (2026-10-06)

`portable_receipt()` compares both saved and fresh evidence after excluding only
explicit historical whole-tree/tool/source-context digests. Packet source and
verifier bindings, selected native bodies and addresses, ELF extents, relocations,
owned data, behavior, and compiler executable identities remain authoritative.
Accepted production context is read from the recorded base commit rather than
the live integrated tree. Tests are deliberately not hashed into receipts.

The unchanged candidate body was freshly strict-matched through the canonical
scorer with the bare source recipe `-g0 -O3 -mips2 -G 0 -non_shared`; the scorer
still injects its mandatory `-Wab,-r4300_mul` backend flag. Full packet proof was
replayed under O3. No same-unit callers, inline helpers, or deleted-static stubs
are required. Historical O2 results remain available in Git history.
