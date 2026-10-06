# Voice-unblock donor topology: 124-byte MATCH

`func_8001F954`, `[0x8001F954,0x8001F9D0)`, is a complete **124-byte / 31-word
MATCH**. This is the sole new claim. Accepted-byte and cartridge-coverage gain:
**zero**. Base: `cd22879d40b3de443cfde047b86e75e159b6cec6`.

The [matching source](../../../matches/boot_tail/func_8001F954.c) releases the
selected voice and clears its blocked byte. It is an N64 runtime reconstruction;
no arcade equivalent is claimed. Production integration and merging remain with
the independent checker. No protected source, target, symbol, lock, central
ledger, compiler flag or scorer is changed.

## Why this source change is justified

The archived `BT03-high-init/nonmatch/func_8001F954.c` freshly reproduces two
mismatching words at offsets `+0x60/+0x64`: a named record-pointer local uses
stack home `sp+0x18` instead of native `sp+0x1C`. The old retained source and five
archived controls all contain that local. Those controls explored a predicate
local, inner scope, copied index and predicate return width.

The authentic [MusyX voiceUnblock donor](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthvoice.c#L733)
uses repeated indexed global expressions, with no named pointer local. That
source topology closes the two home offsets. No padding or artificial local was
added. Independent controls separate the cause:

- Archived baseline: 2/31 differences, 124-byte ELF body.
- Final unsigned direct-index body: 0/31, 124 bytes.
- Signed formal with the same body: 0/31, 124 bytes.
- Original nested guard with direct indexing: 0/31, 124 bytes.
- Final early-return body with a named pointer restored: 2/31, 124 bytes.
- Final O1 recipe: rejected; complete extent and differences remain in the receipt.

The indexed-expression topology is decisive. Neither signedness nor the early
return explains the match. Final `u32` agrees with the real accepted caller's
existing unsigned-int prototype and the donor, without claiming original typedef
spelling. The sentinel exits before conversion to signed helper arguments.

The donor repository is pinned to `78d2e16e4905fc675952162d331c24d5198b2687`,
file blob `3ad906e217a82e77b649edc8935fd6c139d09415`. Its CC0
[license](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/LICENSE)
blob is `0e259d42c996742e9e3cba14c677129b2c1b6311`. No third-party file was
vendored. Native layout, ABI and helper behavior remain authoritative over the
newer public library. No fake helper, added parameter, volatile qualifier,
assembly insertion, register forcing or flag sweep was used.

## Verification and actual accepted context

The stock master scorer resolves every complete text relocation without masks,
unresolved symbols, unverified data or excess body words. Independent GNU `ld`
and `objcopy` resolve the unmodified object and agree over the complete 128-byte
text section: exact 124-byte `STT_FUNC` plus four zero alignment bytes. GNU
`readelf` separately verifies entry and extent. There is no owned data or table.

The verifier replays the **current accepted contract sources**, not merely their
older standalone equivalents:

- Caller `8001C7F4`, 108 bytes: `boot_tail_promotion/sources/func_8001C7F4.c`
  with `include/boot_tail_sample_contract.h` and typed sample-buffer `.mode`.
- Helper `80014AF0`, 76 bytes:
  `boot_tail_promotion/audio_record_contracts/sources/func_80014AF0.c` with
  `include/boot_tail_audio_record.h`.

Both exact function bodies are bound to their actual production-TU bodies and
current normalized lock hashes before fresh compilation. Both remain completely
native-equal after independent GNU linking. Selected header hashes are recorded.
An earlier scratch proof used the older equivalent files; independent review
identified that preservation gap, and this packet supersedes that proof.

Helpers `8001467C` and `8001F6EC` retain their real unchanged interfaces/native
hashes and remain complete nonmatching source reconstructions. They are declared
only in this candidate, never replaced with synthesized implementations. The sole
direct boot-tail caller is `8001C7F4+0x4C`; indirect and other-image callers are
not an exhaustive part of this census.

Bounded tests execute all 31 target instructions:

- 2,112 fixtures each through native and GNU-linked code, 4,224 executions.
- 2,112 actual-source C89 ASan/UBSan/bounds fixtures with strict aliasing enabled.
- Slots 0..31, both predicate results, 32 seed states, and the FFFFFFFF no-call path.
- Side-effecting boundary hooks; exact call arguments/order, complete voice-array
  contents, bounded stack stores, saved registers, stack and return address.
- Three actual-C wrong-contract mutants and three legal-instruction mutants
  rejected: sentinel, identifier store and final clear.
- Eight focused pytest cases, including full replay from an unrelated working
  directory, stale-source binding, causal controls and fail-closed optimized
  Python checks. Python `-O`/`PYTHONOPTIMIZE` is explicitly rejected.

The C oracle uses typed `VoiceState[32]` storage. Unknown struct spans represent
native object storage, not stack padding. Native layout is separately checked
by exact 32-bit compilation; host pointer/ABI behavior is not assumed to be N64.

## Reproduce

With the repository's pinned IDO toolchain and MIPS GNU binutils available:

```sh
python3 cloud/work/boot_tail/BT03-voice-unblock-topology/verify.py --check
REQUIRE_TOOLCHAIN=1 python3 -m pytest -q tests/cloud/test_dot_voice_unblock.py
python3 tools/cloud/check_submissions.py --base cd22879d40b3de443cfde047b86e75e159b6cec6
```

`--repo PATH` optionally chooses the repository. Verification derives its default
root and materializes only selected immutable Git blobs in a temporary directory;
there are no hard-coded workstation paths or copied scoring-tool dependencies in
the packet. The pinned base must be available in local Git history. Compiler,
selected inputs, native extents, source and proof-script hashes are in
`evidence.json`. Temporary objects/native bytes are never publication inputs.

## Scope and remaining work

Demonstrated C domain is valid allocated slots 0..31 or FFFFFFFF, valid downstream
helper state and ordinary sequential execution. Arbitrary indices, asynchronous
mutation, malformed objects and hardware behavior are not established. The
fail-closed integer interpreter executes this body with helper-boundary hooks;
it is not a whole-middleware emulator. The two nonmatching helper bodies are not
executed by that proof.

This verifies standalone candidate compilation and exact current accepted
caller/helper source bodies with their headers. It does not claim full combined-TU
integration, source-built image, compression, full-ROM, gameplay or hosted-CI
success. Existing production record declarations must be respected by the
maintainer's normal static integration workflow; this packet changes none.

T050 remains in force. The untouched 1,456-byte `8001F13C` is a strong later
source lead for `synthvoice.c:voiceAllocate`'s <=1.5.3 branch, but it was not
compiled here. The BT03 cluster still had 88 sub-1KB NONMATCH rows at the base;
this one provisional candidate does not fulfill the large-function gate. Even
C12 had eight residuals, including `20200` (28 bytes, frozen at 3/7). Its newly
identified `sndReadFlag` donor does not justify repeating the already-failed
wide-return control. Research and matching credit remain separate.
