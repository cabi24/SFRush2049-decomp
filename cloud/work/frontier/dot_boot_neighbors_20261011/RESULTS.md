# func_80022324: strict IDO 7.1 match, 172 static bytes

Deliverable: `cloud/matches/boot_tail/func_80022324.c`.
Baseline: `c340dfcacc1a3051930f984c9c05ad16548c01f2` (wave-21 claims).
The function is unlocked and not assigned to a coordinator lane at that baseline.
This is 172 newly matched static bytes; no cartridge/landed-ROM credit is claimed.

## Exact scorer output

With the stock pinned IDO 7.1 release installed:

```
IDO_DIR=tools/cloud/ido71 python3 tools/cloud/score.py fn \
  cloud/matches/boot_tail/func_80022324.c func_80022324 --targets asm/us/boot_tail
func_80022324:
  MATCH
```

Flags: `-g0 -O2 -mips2 -G 0 -non_shared`; the scorer adds its normal
`-Wab,-r4300_mul`. The IDO 5.3 sanity checks for `sound_handles_clear` and
`resource_slot_clear` also passed before matching work.

## Cause and source change

The prior `BT05-macro-five/nonmatch/func_80022324.c` reconstructs the complete
native handler. Under 7.1 it differs in six of 43 words, with equal 40-byte
frames, equal opcodes and a one-register temporary-ring offset starting at the
command high-halfword extraction. Workbench diagnosis and the stock compiler's
`cc -S` listing localized that residual before source changes.

Use `(u32)(u16)(command->word[0] >> 16) << 16`. The redundant `u16` conversion
adds a mask in ugen's listing, advancing the temporary ring. as1 removes the
redundant mask and forwards the shift into the later register. The explicit
`u32` preserves unsigned left-shift semantics for all high-halfword values.
Removing the narrowing returns exactly six differing words, with no extra words.
Eight small, trace-directed expressions were tested; no broad sweep was used.
The neighboring matched `func_80022160` independently uses the same narrowing
idiom. No unused locals, stack padding, inline assembly or false ABI is used.

For integration, `VoiceState` and `MacroCommand` use the existing TU spellings.
The parameter's halfword at +0x4E uses a private packed view. The global voice
array has native stride 0x1A0 and its packed ID is at +0x60. The helper remains
the locked `int func_80021700(u32)` contract. The live count is reloaded after
calls and the return is byte zero. No public-source ancestry is asserted.

## Verification and integration

```
python3 cloud/work/frontier/dot_boot_neighbors_20261011/verify.py
python3 -m pytest -q tests/cloud/test_boot_neighbor_22324.py
```

The input-pinned receipt `verification.json` records:

- Complete 172-byte ELF symbol and target extent, all 43 words equal, only four
  zero alignment bytes afterward, no unresolved/unverified relocations.
- All seven GNU symbolic relocations and native object code equal independently;
  no allocated data sections.
- `rom_tu.h` fitting passes after dropping only four primitive typedef duplicates.
- Scratch-only promotion with prefix dedup produces identical current
  `lib_22300` text (13,552 bytes), relocations and allocated data. The verifier
  selects its compiler with `compilers.tu_cc` and also accepts a later promotion.
- One routine test, well below 30 seconds with IDO. The receipt pins the source,
  verifier, selected target rows/extent/symbol addresses, and no tools or locks.

The coordinator can regenerate the context row with:

```
python3 cloud/work/boot_tail_promotion/context_check.py --compiler ido7.1 \
  CTX.jsonl func_80022324=cloud/matches/boot_tail/func_80022324.c
```

After locking, promote segment `0x22300` with `--only func_80022324`
and `--prefix-dedup rom/lib_22300`. ROM, lock and image gates remain the
independent coordinator's responsibility. This packet changes no protected
source, target, layout, lock or build-tool file.

## Review and access notes

Independent overlay-B lane review of `9c352705479c09d48efde503889f482798e34735`
found no blocking issues after a fresh required-toolchain test replay and source,
ABI, full-extent, relocation, TU, and input-binding review. This paragraph is the
only post-review change. The PR remains a draft regardless of a nonempty claim.
The initial `git fetch origin master` tool review was cancelled without a detailed
reason and was not retried. Current master had independently become available
in the shared repository and was used to create this separate worktree. No ROM
bytes, raw assembly dumps, compiled objects, secrets or private data are delivered.
