# Masked random selector: 268-byte matching candidate

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

`func_800B23E0`, **[0x800B23E0, 0x800B24EC)**, is a complete **268-byte / 67-word MATCH** in a genuine three-function O3 unit. Only this selector is claimed. **Accepted-byte and ROM-coverage gain: zero.**

## What changed from the archived research

This is an old reconstructed routine, not a new discovery. The prior [masked RNG packet](../../frontier/dot_fresh_masked_rng/RESULTS.md), published as PR #84, froze 41 unsuccessful controls and a 13-word nonmatch. Its unchanged best source freshly reproduces that result.

The new evidence is the authentic [`Random` boundary in pinned Rush The Rock LIB/fmath.c](https://github.com/historicalsource/rushtherock/blob/845329d7b36f5a384c5625ed9a0aef584ab46139/LIB/fmath.c#L801-L808), including the real outer `rand() & 0x7FFF` mask. `range.c` is unchanged from the independently reviewed B2E4 range-helper packet; `rand.c` is byte-for-byte the accepted `src/blob/func_8008B2B4.c`. The N64 range helper uses the native single-precision divisor 32768, rather than the arcade double 32767. The full donor file is not redistributed; its revision, URL and SHA-256 are in `claim.json`.

The selector calls that genuine range helper with 32.0, converts to unsigned and narrows to a byte, then reads its allowed-bit table in the loop condition. IDO inlines both actual helpers and legally hoists the stable table read. That produces the native seed-read-before-table-read order and its integer register allocation.

Two one-change controls establish the cause:

- Pre-caching the table entry in a local before the loop gives **14/67** differences, with both context bodies still exact.
- Removing the authentic outer mask from the range helper gives **11/67** selector differences.

The old direct LCG source gives **13/67**. There are no extra formals, fake padding, dead reads, unsupported volatile qualifiers, artificial helper bodies, stand-in callers, arbitrary keepers, or compiler/scorer changes. The three kept functions are the three genuine native entries.

## Contract and context

The selector takes one unsigned-byte index. Every attempt advances the 32-bit LCG seed at `D_8011735C`, derives a 15-bit sample, computes the range value in binary32 and chooses bit 0–31. It returns the first choice enabled by `D_80123418[index]`.

The native function has no frame and no calls. It stores the original full a0 word in its argument-home slot, truncates the index to eight bits, returns a zero-extended byte, and preserves the incoming FCSR after each conversion. It loads the seed and selected mask once before the rejection loop. Stable nonvolatile storage and no concurrent mutation are assumed. A zero mask never terminates. The ordinary host interface tests the byte parameter; separate native fixtures test nonzero upper argument bits and the exact argument-home write.

There are no direct native `jal` callers in the current authenticated target census. Inlined/indirect uses and original translation-unit ownership are not established. This source unit is a supported reconstruction; it is not recovery of the original whole module.

B2B4 is accepted context. B2E4 is the previously reviewed range-helper candidate, not accepted production source on this base. Neither earns duplicate byte credit here. Copying the context makes the packet self-contained and does not depend on merging its separate publication first.

## Verification

Run from the repository root with IDO 5.3, GNU MIPS binutils and a host C compiler available:

    python3 cloud/work/ipa-groups/dot_masked_random_b23e0_20261006/verify.py
    python3 -m pytest -q tests/cloud/test_masked_random_b23e0.py
    python3 tools/cloud/score.py group cloud/work/ipa-groups/dot_masked_random_b23e0_20261006

The receipt binds source, compiler, tools, selected native bodies, every consumed relocation symbol, and the selected protected data window. Current target and data manifests are authenticated on every replay, including a fresh data read after a warm cache. Whole-manifest hashes are historical provenance and are the only fields omitted from portable receipt equality.

- Exact ELF STT_FUNC extents are **48, 72 and 268 bytes**. All **97 words** of the complete unit bodies match, with ten independently resolved relocations, six inside the selector. All 12 zero alignment bytes are outside the functions. No owned data or literals are omitted.
- GNU readelf/nm independently verify the symbols. GNU ld links the unmodified complete ELF separately at each body's native address, and GNU objcopy reproduces every complete body. The production group reader independently agrees. These separate links do not prove original contiguous TU placement.
- **38,032** native/GNU-linked/integer-oracle/unchanged-host-C89+UBSan cases, **76,064** terminating native executions. Every 15-bit first draw is covered; 4,775 cases reject at least one draw, with a maximum of 248 attempts in this corpus. Single-bit masks include bit 31. The proof includes 32 nonzero protected data words and synthetic fixtures spanning all 256 byte indices; fixture capacity is not a claim about retail table size.
- Every reachable instruction in the modeled domain executes: **49/67** offsets. Both rejection-loop outcomes execute. The other **18** instructions implement unsigned-conversion fallback paths unreachable for the guaranteed [0,32) range. They remain included in exact full-byte comparisons; no fallback behavioral coverage is claimed.
- Seed traces, exact load order, unchanged table and canaries, permitted argument-home/seed writes, stack/register preservation and FCSR restoration are checked. A separate **128-attempt zero-mask native prefix** keeps advancing the seed; zero-mask host execution is intentionally excluded.
- Four compiled wrong-contract controls are rejected: ignoring the mask, wrong table index, excluding the highest choice, and wrong range denominator. The latter produces choice 32, whose invalid shift UBSan rejects.
- Adverse controls reject an unknown instruction, incomplete body, redirected seed/table/stack access, omitted FCSR restore, modified complete ELF extent, and warm-cache data corruption.

The inherited accepted rand uses signed overflow. IDO's native operations wrap modulo 2^32; the unchanged host source is compiled with **-fwrapv** to define the same behavior. This is not a portable ISO C signed-overflow claim. Native proof uses default rounding with varied sticky flags and no exception enables; hardware traps, FCSR exception behavior outside this domain, invalid pointers, data races and actual inlined consumers are not established.

No production source, protected targets, locks, symbol maps, shared headers or build recipes change. No ROM bytes, raw assembly, objects, credentials or unrelated private data are included. Full-game shadow/source-image, compression, ROM SHA-1, hardware/gameplay and final acceptance remain with the independent checker. No CI monitoring is part of this packet.


## Integration-portable replay (2026-10-06)

Production context and lock facts are historical evidence at the receipt's stated
base commit, read through `git show BASE:path` when used. Later source splices or
lock additions do not change those historical facts. Whole manifests and tool or
accepted-context digests are provenance, excluded by explicit field/path lists
from portable proof equality. Packet source/verifier bindings, selected native
words, complete extents, relocations, owned data and bounded behavior remain
binding. Tests are not hashed into the receipt. Compiler-dependent tests skip
when pinned IDO or the MIPS GNU linker is absent; source/native-only checks run.

The claimed source's bare O3 header and `group.json` use
`-g0 -O3 -mips2 -G 0 -non_shared`. The copied unclaimed `rand.c` keeps its inherited
O2 comment for ancestry fidelity; the group O3 recipe is authoritative for this
packet's actual compilation. Real rand/range context is required in the same
unit. No deleted-static stand-in is added or claimed.
