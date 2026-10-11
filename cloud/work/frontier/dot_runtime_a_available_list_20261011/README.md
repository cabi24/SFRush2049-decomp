# Image A available-item list: 136-byte candidate

## Protocol v2 delivery (2026-10-11)

`group.c` is byte-for-byte unchanged from the previously reviewed source.
`group.json` now lists only complete strict-match bodies as `members`; all
other genuine bodies remain in `context` and in the same compiler unit.
`claims` is the new-credit subset. Existing exact bodies are preservation
members, earn no new credit, and are still required to pass. The keep root,
compile flags, all source formals/calls and source ordering are unchanged.

`packet.json` is the current input-bound receipt: source, group recipe and
verifier hashes; relevant metadata manifest rows; image-qualified member
word hashes; exact member lengths; IDO 5.3 identity. It does not freeze
mutable tools, locks, Makefiles or assembly text. This packet compiles an
overlay IPA group; it does not compile a static ROM TU.

Routine gate: `python3 cloud/work/frontier/dot_runtime_a_available_list_20261011/quick_verify.py`.
It recompiles once and checks every member with the canonical strict
comparison plus the full ELF symbol extent. Missing IDO follows the shared
pytest skip policy and fails when `REQUIRE_TOOLCHAIN=1`.
`--inputs-only` checks input bindings without claiming a compiler pass.

Direct integration check (no `--claims`, no `--allow-unverified`):

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_runtime_a_available_list_20261011 --targets asm/us/ovl_a
```

There is one cheap collected regression for this packet. Full semantic,
GNU-link, negative-control and compiler-research tests are retained as
opt-in research, without adding them to routine CI:

```sh
python3 -m pytest cloud/work/frontier/dot_runtime_a_available_list_20261011/research_tests.py -q
```

Earlier `verification.json` and independent-review receipts below remain
historical evidence for the unchanged C. Current routine evidence is
`packet.json` and fresh gate output; old hashes are not compatibility locks.
Only the named member bodies are ready for the coordinator's overlay
integration gates. Context is not certified or credited. No accepted
overlay or cartridge bytes are claimed before those gates pass.


The sole new claim is **A:func_8039E3BC**, `[0x8039E3BC,0x8039E444)`,
136 bytes / 34 words. Stock IDO 5.3 whole-group `-g0 -O3 -mips2 -G 0
-non_shared` plus canonical `-Wab,-r4300_mul` reproduces the complete body,
including all relocation fields, with no uncertainty or excess words.
This is local matching evidence, **zero accepted-image or cartridge bytes**.

## What changed

This is an alternative full source group derived from
`cloud/work/lean/runtime_a_car_select_20261006`. Keep the genuine A1EAC root.
Do not integrate two overlapping copies of these groups.

The existing natural availability predicate has two source formals. Reversing
its source declaration to `(player, item)` and consistently reversing all
three actual calls recovers the native scheduling of the two ordinary machine
arguments. Its complete native D494 body is unchanged and stays exact. This
is evidence for a source spelling, not proof of the original declaration.
The emitted D494 ABI still receives item in a0 and player in a1.

Keeping the count reset and `for` initialization on the same physical source
line then recovers the two-instruction scheduling difference at entry. Neither
operation is changed. No extra read, local, statement, dummy formal, fabricated
caller, volatile qualifier, instruction encoding or compiler modification is
introduced. The source-delta check enforces these changes and no others.

The full list builder resets one player's count, examines item IDs 0–18,
and appends available IDs in ascending order to that player's 19-word row.
Its input is the native private t4 register; s0 is intentionally clobbered.
It is not a standalone O32 replacement. The complete real A0498 and A0FD8
callers remain in the group but are nonmatching; matching this helper does
not establish that the entire group can be safely installed.

## Bounded experiments

Eight distinct source variants (including the starting source) were compiled.
The starting source differs at 5/34 words; splitting the increment remains
5/34; one-line whole body yields 7/34; a do-loop yields 5/34; consistent formal
reordering gives 2/34; coalescing reset/initialization gives 0/34. Two natural
address-expression local variants give 19/34 and 16/34. They were discarded.
No broad permutation sweep ran. Workbench diagnosis ran first; its raw-object
relocation warnings are not used as match evidence. Strict resolved scoring
and full ELF extent establish the final result.

The previous 528-byte D494 and 480-byte ED20 matches remain exact, with no
new credit. All twelve complete group members are rescored in verification.json.
Unmatched callers, float-product code and renderers remain unclaimed. Inherited
renderer literal ownership failures and unverified references remain explicit;
they were not masked, corrected by constants guessed from code, or accepted.
E3BC itself owns no literal data or storage.

## Reproduce

Configure IDO_DIR and GNU MIPS tools, then from repository root:

```
python3 cloud/work/frontier/dot_runtime_a_available_list_20261011/verify.py
python3 tools/cloud/score.py group cloud/work/frontier/dot_runtime_a_available_list_20261011 --targets asm/us/ovl_a
python3 -m pytest tests/conveyor/test_runtime_a_available_list_wave.py -q
```

The verifier authenticates image A's protected target manifest, extent, entire
body hash, tracked asset hash, loader pointer, and decompressed image identity.
It checks the exact 136-byte ELF function extent, relocations and full body.
Three compiled negative controls reject the original source, the isolated
formal reorder and a wrong loop bound. This packet's require guards work under
optimized Python too. Native data are only read in memory; no binaries or raw
assembly are committed. The group source is retained in full for reproducibility.

## Baseline, deduplication and limits

Experiments are pinned to `390d40f6f271eecc83a5e82ba7b12f6e8a1c8625`.
Live master at preflight was its direct child
`f0a30006676104d526094c8d64512837154e9c24`, adding only six unrelated w16b
entity-spawn research files. Existing merged A source and open PRs were checked.
Open #163 has other A matches and older D494 research, not a new E3BC claim.

No original whole translation unit, complete caller integration, GNU-linked
original TU layout, full-image, compressed-stream, full-ROM SHA-1, hardware,
or gameplay proof is claimed. Independent checker owns acceptance and merging.

## Independent review and test results

Independent review passes and is reproducible with `independent_review.py`.
A fresh GNU route re-sections the complete newly compiled ELF function bodies,
retains their relocations and translates local `.text` call addends through
function ownership. E3BC's five relocations resolve independently. This is a
complete extracted-body link, not proof of contiguous original TU placement.

Both protected native and fresh GNU-linked routes pass 8,962 D494 leaf cases
and 2,560 E3BC+D494 paired cases each. The latter include 512 mapped negative
selector cases. Another 2,048 pair cases execute unchanged extracted C89
functions under UBSan/bounds. All 34 E3BC and 132 D494 instructions execute;
complete paired memory/register/read/write traces agree, including unused-row,
tail and guard preservation. All five generated pair-related callsites are
audited. Large upstream callers are not executed end-to-end.

Author focused regression: 781 tests pass with REQUIRE_TOOLCHAIN=1. All 412
static locked functions remain intact. Optimized-Python author verification
also passes; the assertion-based independent reviewer rejects optimized Python.

### Fresh protocol-v2 verification

- All members print `MATCH` in the direct strict command above, without
  `--claims` or `--allow-unverified`. Context diagnostics remain nonmatching
  and informational; they are neither hidden nor installation claims.
- The single routine pytest check passes in 0.18 s with the stock IDO 5.3
  release compiler. Input checks take 0.04–0.05 s with no IDO; the shared
  conftest reports a toolchain skip, and `REQUIRE_TOOLCHAIN=1` converts that
  missing-tool case to failure as required.
- The five opt-in author/independent research checks pass after this
  metadata-only packaging.
  For a combined invocation, use pytest `--import-mode=importlib` because
  each directory uses the local filename `research_tests.py`.
- The 493 current static locks remain intact; protected-path guard passes.
  No source-image, compressed-stream, cartridge or gameplay gate was run.
- This existing review preserves its published research history. Future
  deliverables use one compact group per PR. Integrate only the needed source/recipe and
  routine gate; deep replay remains optional.
