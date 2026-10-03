# BT02 small packet

Nine strict relocated IDO matches, **344 native bytes**, at `-g0 -O2 -mips2 -G 0 -non_shared`
plus mandatory `-Wab,-r4300_mul`. Local replay, pinned-toolchain comparison,
all three target-manifest members, all 439 inventory extents, existing-getter
replay and host behavior tests pass. Independent review and exact-head CI are
required before the packet lead applies `status_delta.json` centrally.
This is matching-source evidence, not cartridge coverage or promotion.

## Claim and dependency

- Packet: D10 / Packet 3 / BT02-small; review-ready.
- Branch: `dot/boot-tail-p3-bt02-small`.
- Base: `76780b3a1b3e26c54b86e1f153344e92dac15b20` (completed Packet 2).
- Explicit unmerged dependency: [Packet 1 PR #59](https://github.com/cabi24/SFRush2049-decomp/pull/59),
  head `21e104a22575cf4d639261d2f6d9535913074e6a`.
- Prerequisites #52 and #54 were owner-merged to master `b16dd93b` before scoring.
- Central claim recorded by the lead at `9602cf96b41e7cb718dcf932da7786ffdb3a31f9`.
- Exact targets: `8001061C`, `80010A0C`, `80010A14`, `80010D3C`, `80011894`,
  `800119E0`, `80011A10`, `80011A3C`, `80012200`, within `[0x80010450,0x80014550)`.
- Edited scope: these nine sources and `cloud/work/boot_tail/BT02-small/` only.
  The lead alone owns central status/claims/D10. The existing `80010A00` is unchanged.
- No queued packet; no lock, layout, target, scorer, symbol or production-gate edits.

## Reproduction

From repository root, after the normal `bash tools/cloud/setup.sh`:

```sh
python3 cloud/work/boot_tail/BT02-small/verify.py
python3 cloud/work/boot_tail/BT02-small/test_host.py
```

`IDO_DIR` can point to a preinstalled compiler. The verifier requires every compiler
file's SHA-256 to equal the pinned Packet 2 receipt, checks target/census integrity,
and compiles each actual source separately. `verification.json` binds all nine
source hashes to zero differing words, zero extra words, and empty unresolved,
unverified and error lists. It also recompiles the existing getter. Compiler
objects and native disassembly are temporary and are not archived.

## Reconstruction and ABI audit

All roles below are hypotheses except the counted-static symbol resolution;
matching proves the body, not the historical identifier or complete type system.
All nine initial natural C forms matched at O2. There was no O2 mismatch, flag
sweep or schedule search, so workbench diagnosis was unnecessary. A fixed O1
control was then recorded for each unchanged source. Two final clarifications
also re-matched: callback-pointer storage for `8001061C`, and the
same explicit recovered-field layout in both AudioState sources.

| Function | Bytes | Behavior and native/compiler evidence | ABI evidence |
|---|---:|---|---|
| `8001061C` | 12 | Clear global callback pointer `80038020`; frameless store leaf. | Caller `80025DC0` supplies no argument; `80013DEC` tests and indirectly calls the same global as a no-argument callback. |
| `80010A0C` | 8 | Empty callback with one unused 32-bit formal, naturally emitted by O2. | Native return delay slot homes `a0` to the incoming argument area. This directly supports one formal, not an invented keeper. No direct caller is in the census; signedness and historical role remain unknown. No additional formals are claimed. |
| `80010A14` | 40 | Return address `8004F810` if unsigned byte `8002C630` is nonzero, otherwise null. Frameless conditional leaf. | No parameter reads; two pointer-valued returns. No direct caller found; pointee layout remains opaque. |
| `80010D3C` | 56 | Call counted-static `osAiSetFrequency` at `8000BF00`, then store returned word both to `8003828C` and through the incoming pointer. | `80014434` forwards its first pointer; native saves that pointer over the call. Ordinary O32 frame and call; declared external callee, no body stub in submission. |
| `80011894` | 44 | Snapshot `8003802C` to `80038038`, floor the same word to a 16-byte multiple in `8003803C`, clear halfword `80038028`. | `80013DEC` calls without arguments. One shared load feeds both stores; frameless CSE fits O2. Address-like word semantics and stronger names remain hypotheses. |
| `800119E0` | 48 | Forward global buffer pointer `800382D0` and the halfword pointed to by `800382D4` to `80011910`. | Caller `80013DEC` sets the two globals; callee `80011910` narrows its second argument to unsigned 16 bits and forwards the first to cache writeback/task data. No parameters are read on entry. |
| `80011A10` | 44 | Initialize six audio-state fields: state/step/value/saved value zero, scale one, count from offset `0x20`. | `80011C84` computes an audio record pointer and passes it in `a0`. Native halfword, word, byte and FPU accesses establish field widths and offsets. Frameless O2 with shared float zero. |
| `80011A3C` | 40 | Enter state 3, reset step, copy current to saved float, zero scale, count from offset `0x28`. | `8001489C` writes offset `0x28` then passes the same record; `80014B3C` also supplies an indexed record. Same recovered layout as the initializer. |
| `80012200` | 52 | Walk next pointers from global `80038344`, decrementing nonzero unsigned-halfword countdowns at offset `0x10`. | `80013DEC` calls without arguments. Frameless loop and branch-likely reused countdown load support O2. No store to links/head; zero countdowns remain zero. |

Unknown arrays in structures represent actual unexamined record offsets, not
artificial stack padding. They do not allocate automatic padding locals. The two
AudioState declarations are identical. AudioNode's pointer is 32 bits under the
verified O32 target; host pointer size can differ, so the host test does not claim
native Node-layout proof. Native instruction/offset equality is the layout proof.

## Fixed O1 controls

All submissions remain O2; this is one prescribed comparison per source, not a
flag search. Four bodies also match O1 (`1061C`, `10A0C`, `119E0`, `11A3C`), so
those small bodies alone do not distinguish the historical optimization level.
The other five discriminate toward O2 with the same natural source:

| Function | O1 differing words | O1 excess nonzero words |
|---|---:|---:|
| `10A14` | 6/10 | 0 |
| `10D3C` | 13/14 | 1 |
| `11894` | 10/11 | 3 |
| `11A10` | 11/11 | 1 |
| `12200` | 13/13 | 4 |

The overlong O1 `11894` control additionally has an unpaired HI16 at the compared
extent boundary; its LO16 lies outside the native-length slice. This discarded
control is not a match or an unresolved local-data claim. The selected O2 body
resolves every relocation with no errors. `verification.json` records both
levels on the actual final sources. No body was changed for the O1 controls.

## Tests and limits

`host_tests.c` is a separate host-only driver, never a matching candidate. It links
the nine submitted C files as separate translation units and tests callback clear,
empty callback calls, enabled/disabled pointer getter, both call wrappers and their
argument/result forwarding, mask edge values, exact AudioState writes (including
unchanged neighboring bytes), empty and three-node lists, non-underflow, repeated
updates, and unchanged links. The call replacements are isolated test doubles.
Host checks support behavior under the declared types; they do not prove hardware
behavior, native ABI, linking into the cartridge, or ROM identity.

Native source: read-only `asm/us/boot_tail` targets and caller/callee sections,
verified against `SHA256SUMS`. No third-party source was copied. No local literal
pools, jump tables, inline assembly, fake extra formals, dummy calls or split/merged
extents are present. The float constants are formed directly and all relocations
resolve strictly. No ROM bytes, raw disassembly or objects are published.
