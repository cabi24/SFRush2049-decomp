# Option-byte setter: bounded family-evidence reopen

Status: **NONMATCH; ABI still unproved; claims empty; no coverage change.**
Base: `55ddfc6b53d96d4c5dcaaa8891dedafb037b3994` (2026-10-05).

The new accepted `frontier_reverb_setup` getter family justifies revisiting the
nearby setter family. This packet restores three substantive, inlined
mode-specific setter bodies and narrows the remaining structural mismatch.
It does **not** add a fourth, unused parameter merely to reproduce an outgoing
stack store. None of the sources, stubs, helper evidence, or tests here is a
splice or match claim.

## Result

- `audio_bus_mix`, 0x800B4818: native extent 720 bytes; candidate ELF symbol
  extent 712 bytes. Canonical score: **141/180 words differ**, with four own
  section references unverified. Most positional differences are the consequence
  of two missing words and the resulting displacement changes. The body remains
  a nonmatch even though the final ELF section has alignment bytes after it.
- `voice_stop_2`, 0x800B4738: exact 224-byte ELF extent and **all fully relocated
  bytes equal**. Its complete 11-entry / 44-byte own switch table is verified
  against the authenticated repository data at 0x80123C38. This remains an
  individual-body lead in a nonmatching caller group, not an independent claim.
- `format_string_parse`: accepted source body is copied byte-for-byte from
  `src/blob/groups/codex_hash_a80/group.c`; its 180-byte extent, full relocated
  bytes and 24-byte switch table verify unchanged.
- Stubs `func_800B4720/28/30`: each remains eight bytes. Assigning the three
  substantive mode setters to these particular names is a source-order
  hypothesis; the identical stubs cannot prove that assignment. No new stub
  credit is requested.

The source is a profile-option setter, despite its historical audio label. It
updates one signed byte only if different, hashes the affected data, then calls
`slot_state_lookup` with the checksum-prefixed record. The actual record shapes
are offset/length 28/25, 72/20, 92/16, 108/20, and 128/12. The null resource path,
unsupported selectors, and unchanged values do not persist a record.

## What the new family evidence supports

The accepted getter and native setter have corresponding common, mode-A,
primary, secondary and fallback field maps. The three adjacent caller-less
setter stubs parallel the three accepted getter stubs. Splitting the three
mode bodies is therefore a meaningful source-structure experiment.

The first split retained a named `Resource *` alias, which unnecessarily kept
that web live in the compiled caller. Reading the actual resource field for the
null check and the profile pointer, without that redundant alias, recovers the
native caller's data-pointer and owner register flow. All three helper formals
are consumed by their actual operations. There is no keeper, asm, volatile
access, frame filler, fake literal external, or altered compilation flag.

Six bounded source forms were measured: the original A91 source, the direct
three-helper split, three equivalent placements of an explicitly consumed data
pointer in the inline helpers, and the split without the resource alias. The
consumed-data-pointer variants offer no improvement; the last form is retained.
No synthetic unused-page variant was built or proposed for acceptance.

## Remaining ABI blocker

The native caller's two direct calls to `voice_stop_2` are at +0xA4 and +0xCC.
Each delay slot stores the selected signed page value at outgoing stack offset
12, the ordinary fourth argument-home location. The improved three-input source
omits those stores and schedules the selector conversion into the delay slots.
The resulting caller is exactly eight bytes short.

The actual callee reads owner, unsigned-byte selector and signed-byte value.
After allocating its own 24-byte frame, its only stack loads restore its saved
return address at +20. It does not load the caller's page home at callee +36,
form an address to that home, or consume a page value through a register. Its
field writes, checksum range and persistence arguments also have no page
component. The sole native caller is `audio_bus_mix` (two sites).

The accepted getter models an unused third page parameter at outgoing offset 8.
That is sibling evidence for a possible deleted formal in the setter, but it
is not source provenance proving an unused fourth setter formal. The four
external caller functions of `audio_bus_mix` supply its owner, selector and
value; they do not establish the internal helper's missing source signature.
A complete byte match obtained only by inventing that parameter would not
close this packet's source-validity gate.

Stop condition: preserve the narrowed nonmatch until original declaration
provenance or independent native consumer evidence establishes the page
parameter's role, or a genuine source construction explains the same stores
without an invented formal. Further register permutations do not resolve that
question. The accepted getter group and all accepted sources remain unchanged.

## Verification

`verification.json` records target/source/toolchain hashes, native callsites,
stack-access metadata, canonical comparisons, exact ELF symbol lengths, and
full helper/context relocation plus own-table checks. It contains no native
instruction arrays or raw disassembly. The existing protected scorer, own-data
checker, and group relocator are used without modification. A corrupted temporary
helper jump-table entry is explicitly rejected by the own-data checker; the
short caller is refused despite trailing zero section alignment.

`verify_semantics.py` executes the authenticated native caller and helper in a
fail-closed integer MIPS replay and compares them against the host-compiled C.
It reads authenticated native switch tables. The accepted checksum and external
persistence call use explicit functional contracts. The checksum is normalized
from host endian to target endian; pointer-bearing host structs are populated
by named fields and the native replay uses the real O32 offsets.

The final run passes **204,562 native-versus-host cases**: every signed page byte,
every selector byte, values -128/0/127, selected input-narrowing boundaries,
null-resource cases, and 6,524 changed-value-then-identical-value sequences.
It compares the complete 160-byte synthetic profile and complete persisted
record snapshots, including no-op paths. This is semantic evidence, not proof
of a byte match or a complete N64/game execution.

Reproduce from the repository root using the pinned IDO environment:

```sh
python3 cloud/work/frontier/dot_audio_bus_reopen/verify.py
python3 cloud/work/frontier/dot_audio_bus_reopen/verify_semantics.py
python3 -m pytest -q tests/conveyor/test_dot_audio_bus_reopen.py
```

The broader suite result is recorded in `checks.json`. No splice, whole-program
shadow build, image construction, compression, or cartridge build was attempted.
Those gates would be inappropriate for this explicitly unclaimed nonmatch.

## Provenance and reference check

- Prior source and result: `cloud/work/ipa-groups/codex_audio_select_a91/`.
  Replayed at 167/180 words and 696 bytes; the native caller is 720 bytes.
- New family evidence: `src/blob/groups/frontier_reverb_setup/reverb.c` and
  `cloud/work/frontier/w2d/RESULTS.md`, on the pinned base.
- Native external caller reconstruction: `cloud/work/large_init_state/compiler/`
  identifies the three actual public input types independently.
- Arcade source checked at historicalsource/rushtherock commit
  `845329d7b36f5a384c5625ed9a0aef584ab46139`: `game/cksum.c`, `game/menus.c`,
  `game/option_map.mac`, `game/option_pp.mac`, plus configuration/checksum
  searches. The arcade checksum is a different 64-bit XOR accumulator; no
  direct donor or setter declaration was found in this bounded search.
  Reference: https://github.com/historicalsource/rushtherock/tree/845329d7b36f5a384c5625ed9a0aef584ab46139
- The workbench diagnosis was run before source refinement. The original A91
  body is a structure mismatch, with 174 actual instructions versus 180 native;
  the improved body has 178. Raw diagnostic listings remain ignored locally.

Publication is limited to source, tests, counts, hashes, and this research note.
No ROM bytes, raw assembly, native data dump, lock change, accepted source edit,
scorer change, symbol edit, target edit, flag change, or remote-builder change is
part of the packet.
