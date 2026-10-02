# Complete free-list node allocation: strict O2 match

Base: `cabi24/SFRush2049-decomp` master
`2f1f30c508495db08d61170c4a9b2409461ae326` (2026-10-02).

## Result

`func_80090284`, `0x80090284..0x80090308`, is a complete **33-word / 132-byte**
strict match through the unchanged canonical cloud scorer. Its ELF function
symbol is exactly 132 bytes. The emitted text is 144 bytes; the three trailing
zero alignment words are excluded. No helper or caller context is compiled.

Source: `cloud/matches/func_80090284.c`, with
`-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul` under pinned IDO 5.3.
No masks, unresolved or unverified relocations, relocation errors, or nonzero
excess instructions are permitted or present.

## Reconstruction and semantic evidence

The historical `cloud/work/tiny_A40/func_80090284.c` reproduces a 31/33-word
nonmatch with a 128-byte emitted body. It first initializes a local pointer,
then increments the counter before unlinking the free-list head. Workbench
comparison identifies a structural/allocation difference. Testing the global
head for null before assigning the actual returned node changes that result
to a 15/33-word nonmatch with the correct extent. Moving the existing unlink
operation before the increment then matches all 33 words. Ordinary readable
statement lines and neutral field names retain the match. The historical
source remains untouched; no allocator-pressure or padding trick is used.

The target consumes no incoming arguments and returns a node pointer in `v0`.
A null head at `D_801392C8` returns null without modifying any memory. Otherwise
it advances that global to the selected node's next link, increments the
signed-half counter `D_8012E66C`, and raises `D_8012E678` if the reloaded signed
counter is greater. It resets the selected node's fields at offsets 0, 4, 6,
8, 12, 16 and 20: the halfword at 6 becomes -1, the float at 16 becomes positive
zero, and the other listed fields become zero. Padding bytes 10 and 11 remain
unchanged. There is no stack frame, helper call, extra input or new check.

The 24-byte O32 node layout is verified by compile-time assertions under the
actual IDO compiler. Other than the list link, field names are deliberately
neutral; original developer types and identifiers are not claimed. Offset 20
can receive a callback address in callers, so it is represented as a raw word
rather than mislabeled as a flag. A writable valid node and valid list links
are the source's ordinary pointer-domain preconditions.

The native halfword store/reload wraps 32767 to -32768. Narrowing at that
boundary is implementation-defined in C; the pinned IDO's exact output proves
the required target behavior, and the host compiler behavior was separately
checked. No overflow guard is added.

All 33 native instruction addresses, including return delay slots, were
reviewed. The protected inventory scan found ten direct `jal` sites in nine
functions, with no PC-relative transfers to the entry, across 1,216 functions
and 140,252 protected words. Every call-site window checks the returned pointer
for null and then uses the node or preserves it for later use. This supports
the pointer-return interface; it does not exclude indirect callers.

The accepted `entity_transform_apply` source and its 65 native instructions
provide complementary evidence: it pushes a node onto the same free list,
decrements the same signed-half counter, and accesses the same high-water
counter. Its locked source hash is intact. This contribution changes none of
that accepted code and claims none of its bytes.

There are no fabricated operations, extra arguments, stand-ins, volatile
accesses, uninitialized locals, forced registers, or artificial stack objects.
The primary arcade checkout is unavailable; no direct arcade-source equivalent
or original source spelling is asserted.

## Reproduce

```sh
python3 tools/cloud/score.py fn cloud/matches/func_80090284.c func_80090284 \
  --flags '-g0 -O2 -mips2 -G 0 -non_shared -Wab,-r4300_mul'
```

Expected output:

```text
func_80090284:
  MATCH
```

The approved IDO 5.3 static-recomp v1.2 installation was reused. Its distribution
archive is pinned by `tools/cloud/setup.sh` to SHA-256
`ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506`.
Actual executable hashes appear in `verification.json`.

The independent replay copies the final source into a clean directory and
invokes the pinned IDO driver directly. Separate full-word arithmetic resolves
all three HI16/LO16 symbol pairs, including signed-low-half carry, with zero
addends and no masking. Every resolved body word equals the protected target.
The canonical relocation comparison is also checked. A separate reviewer
repeated this source-copy/recompile/manual-relocation process in another
worktree and audited the complete native body and interface.
The final source only appends a documentation comment to that peer-reviewed
source. A fresh publication replay independently recompiled the final source
and reproduced the recorded object hash and every resolved body word. The
proof records the peer-reviewed and final-source hashes separately.

## Additional validation

- Host C89 syntax and warnings pass with `-std=c89 -pedantic -Wall -Wextra -Werror`
- IDO compile-time assertions verify the actual 24-byte node and all field offsets
- **524,290 host cases** pass under AddressSanitizer and UndefinedBehaviorSanitizer
- Exhaustive signed-half counter inputs (-32768 through 32767), four high-water
  boundary values, empty and two-node lists, single-node exhaustion, returned
  identity, reset fields, preserved padding, and an untouched successor are covered
- Leak detection is disabled because the sandbox's ptrace environment does not
  support it; the test performs no dynamic allocation
- CI-style repository suite: **1,299 passed, 41 skipped, 9 deselected**, exit 0
- Cloud/scorer suite: **621 passed**, exit 0
- All **160 static locked functions** remain intact
- Existing blob and group source-hash checks report **zero problems**
- All **23 protected manifest entries** verify

Host tests supplement native identity; they do not execute an N64 binary.
Changed-source scoring and the protected-path guard run against the final
submitted tree.

## Integration and limitations

This is a draft source contribution and adds **no accepted cartridge coverage**.
Only the new cloud source and its two evidence documents are added. Accepted
C, locks, coverage totals, build wiring, protected native targets and scorer
remain unchanged. The target is absent from accepted locks and PRs #10–16;
the parallel run-decoder contribution covers a separate target.

The original ROM, full extracted image, and derived `build/blob_layout.json`
are unavailable. Source-image splicing, compressed-stream identity, full-ROM
SHA-1, and `make test` have **not run**. Source-hash checks do not replace those
gates. Live LAN coordinator ownership must be rechecked before normal image,
ROM and lock promotion. No merge or promotion is performed here.
