# D328 private-procedure admission: narrow maintainer proposal

Status: read-only design review; no implementation, replay, publication, or acceptance.
Base inspected throughout: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`, using `git show`, not this checkout's HEAD.

## Decision

There is no supported canonical admission route for this object today. A real emitted private procedure is missing from the scorer's function catalogue, not missing from generated code. The smallest useful change is an explicitly opted-in ECOFF private-procedure locator and exact-extent comparison, restricted initially to bodies whose relocations all name known external symbols. D328 fits that restricted case. Do not build a generic hidden-function/owned-data integration framework to unblock these 124 bytes.

Keep the genuine eight-body source and the single FCE0 keep entry unchanged. Claim D328 only; preserve the other seven bodies as nonmatching compilation context. No invented export, extra keep root, alias symbol, ELF mutation, offset-only claim, or declaration change is needed for object-level verification.

This would establish a canonical source/object-body MATCH, not production coverage. Runtime-B group splicing and the E398 return contract remain separate maintainer decisions.

## Evidence already present, not rerun here

`baseline/independent-d328/review.json` independently reports:

- D328 is `stStaticProc`, offset 264, size 124, symbol index 3, matching stEnd index 4, auxiliary end-exclusive 5, PDR index 1; PDR address 264, frame 24, ra-only integer save mask.
- Complete candidate interval [0x108,0x184) agrees with complete native [0x8038D328,0x8038D3A4), all 31 words/full 124 bytes, with no masking, missing/excess bytes, or neighbor borrowing.
- All nine body relocations are known externals: D_80394F70, D_80399B18, func_8008E3C0, func_8008E398. No internal-text or owned-data relocation occurs in D328.
- Eight real E114 calls enter this body. FCE0 is the sole kept root; all eight complete bodies are emitted. The other seven bodies are NONMATCH.
- Whole-object independent relocation reconstruction checked 414 records and all unchanged text/rodata bytes. That authenticates the historical link, not native placement/owned-data equality for every other body.
- Existing closure replay reports 552 paired fixtures and ten negative controls. Its bounded external models are not proof of whole-game semantics.

The historical object/ELF was removed after review (`baseline/cleanup.json`). This review did not regenerate it. A fresh canonical replay is a future explicit step, not something these notes certify.

## Exact blockers on the inspected master

1. `tools/cloud/score.py:199-206` discovers only defined ELF STT_FUNC entries. Only FCE0 appears there. `compare`, lines 372-382, rejects D328 by name.
2. Passing the existing raw `start=0x108` argument is not a safe workaround. Lines 383-388 still end at the next STT_FUNC (FCE0 at 0x2150 here), so hidden neighbors appear as excess; there is no validated end argument. Conversely, arbitrary offsets never establish procedure identity or prevent a fabricated window.
3. Text-section relocation ownership also uses STT_FUNC, lines 244-263. Adding only a name lookup does not make hidden call-target ownership correct. This is not exercised by D328's nine external-only relocations, but is why a generic patch cannot stop at name discovery.
4. `owndata._locate`, lines 308-318, requires a named ELF function even when `start` is supplied; its cross-function text mapping also uses STT_FUNC. Supplying FCE0's name solely as a locator is supplemental research, not a private-function admission interface.
5. The current closure group.json has only files/keep/flags, no members or claims. `compile_group` can build it, but ordinary group CLI expects members at score.py:516. The present packet intentionally is not a submission claim.
6. Existing claim selection is sufficient once discovery works: score.py:518-536 judges only claim names that belong to members; other members and context are informational. Seven nonmatches do not inherently require D328 to remain unclaimed.
7. `check_submissions.py:43-80` recognizes IPA group directories but invokes them with `--claims` and no target directory. Only runtime single-file paths receive `--targets`. Direct `score.py group ... --targets asm/us/ovl_b` already selects B, but a portable changed-group CI admission needs that selection carried through, or it defaults to the wrong image.
8. Production is separately unsupported: `blob_group.member_slices` (586-601) needs a symbol, and `relocate` (628-642) again demands a real function symbol at the slice start. Runtime `ovl_rom.py` discovers only flat `<sources>/ovl_b/*.c` singles (49-50), compiles singles, then strips mdebug before linking. It has no runtime-group/private-procedure supply path.

## Minimal API and manifest contract

Proposed names are design suggestions, not existing APIs.

Retain `claims` as a list of target names. Add one narrow private selector list and one enumerated target-set field to group.json:

    {
      "files": ["closure.c"],
      "flags": "-g0 -O3 -mips2 -G 0 -non_shared",
      "keep": ["func_8038FCE0"],
      "members": ["func_8038D328"],
      "claims": ["func_8038D328"],
      "context": ["func_8038D200", "func_8038D498", "func_8038DA78",
                  "func_8038E088", "func_8038E114", "func_8038F938", "func_8038FCE0"],
      "target_set": "ovl_b",
      "private_procedures": ["func_8038D328"]
    }

No manifest-supplied offset, length, frame size, or target bytes selects the body. Those are derived from compiler output and the protected target and may be recorded as assertions in the packet's tests. Reject duplicate/unknown private selectors, selectors outside members, and incompatible explicit target selection. Use an allowlisted target-set enum, never an arbitrary path from JSON. Keep the existing CLI --targets route available.

Internally:

- A read-only `locate_private_procedure(object, name)` parses the untouched compile output and returns a validated descriptor (section index, start, exact end, metadata provenance). It uniquely resolves stStaticProc by FDR-local identity/name and cross-checks matching stEnd, procedure aux-end index and PDR address/isym. The PDR supplies corroborating address/frame metadata; the stEnd supplies size. Do not call PDR alone an independent length field.
- Validate all involved tables, counts, indices, strings, alignment, positive lengths and section bounds. Reject ambiguity/conflicting duplicate names. Cross-check any ELF symbol for the same procedure rather than silently prefer one view. Check surrounding procedure extents for overlap and non-code padding; the existing eight-procedure census provides a concrete fixture.
- `compare(..., selector="ecoff-static-external")` (or an equivalent separate wrapper) invokes that locator internally. Do not expose an unvalidated arbitrary start/end acceptance API. Require derived procedure length exactly equal to the complete protected target length. This private route treats even extra zero instruction words inside stEnd as excess; zero alignment outside the proven extent is excluded.
- For this first route, inspect every relocation record in that entire descriptor. Accept only supported R_MIPS_26/HI16/LO16 against undefined, known external symbols, with full resolution, correct signed addends/pairing and call-region/alignment checks. Reuse canonical target/symbol integrity validation and the existing relocation machinery where applicable. Preserve every non-relocated bit and compare every full target word. No byte masks, no unresolved fields and no allow-unverified escape.
- Fail closed on any own-data or own-text reference, including named object-defined symbols, rather than passing a false no-reference count or proxying a different function into owndata. That preserves owned-data guarantees without implementing unused functionality. D328 legitimately has zero such references. Existing ELF/owned-data scoring remains unchanged.
- If private owned-data or internal-call support is needed later, pass the same validated extent/owner catalogue through relocate and owndata, including bounded text ownership for table entries. That is explicitly deferred; merely broadening score.symbols is insufficient.
- Canonical CLI reports D328's strict MATCH plus metadata-derived extent provenance. Context stays informational; if unsupported hidden context lookup remains NOT VERIFIED, do not relabel it MATCH. The packet's independent whole-body report still records its seven NONMATCH results.

Changed-group CI must honor the allowlisted target_set and invoke this route. A manually run correct --targets command alone does not make ordinary PR CI check runtime B.

## Testable acceptance and rejection cases

Compiler-free synthetic ELF/ECOFF tests:

1. Valid private stStaticProc, consistent end/aux/PDR, no ELF function entry: unique exact descriptor and strict exact comparison succeeds; ordinary public STT_FUNC behavior remains unchanged.
2. Missing/malformed mdebug; wrong magic/endian/record size; invalid FDR, symbol, auxiliary or PDR indices; unterminated/out-of-range strings; duplicate private names; mismatched end backlink/name, aux end or PDR start/isym: hard failure, no guessed offset fallback.
3. Zero/misaligned/out-of-section/overlapping extent; short private body followed by target-looking neighbor; longer body including extra zero or nonzero words: reject. External alignment padding is not body/coverage.
4. Missing, changed or wrong target-set identity; protected target/symbol integrity failure; claim outside members or private selector not claimed/member: reject. In particular, B is never inferred from the shared overlay address range.
5. Unknown external, wrong address/addend/callee, unsupported or malformed relocation, unmatched or cross-boundary HI/LO, unaligned/out-of-region call, mutated opcode/nonrelocation word: reject, even with --allow-unverified.
6. Any added .rodata/.data/.bss/.text or other object-defined reference in the restricted private route: explicit unsupported failure. No silent clearing of unverified fields. Existing public own-data mismatch/unverified tests continue passing.
7. Seven bad context bodies do not gate the one valid D328 claim; failed D328 does gate. Adding/removing claims never changes keep, source or compile semantics. Changed-group CI routes B correctly and rejects conflicting target selectors.

Future authorized compiler fixture: preserve the exact bare O3 header, identical closure source/hash and sole FCE0 keep list; use unchanged canonical compile_group and mandatory as1 -r4300_mul. Assert metadata-derived 124-byte D328, all nine resolved external relocations, zero owned-data references and canonical accepted(False). Retain complete context inventory and source integrity checks. The no-toolchain case must skip cleanly before score.ido is called.

Before any future submission, run the user's required current-master regression matrix with and without IDO. No such tests, compilation or matching sweep ran in this review.

## Admission versus integration

A maintainer-approved scorer change plus fresh canonical replay can admit a source-derived D328 object match without admitting the entire closure. That addresses the concrete verification loss. It does not establish original TU/export/formal-order identity.

Actual production/runtime coverage still needs an integration path that consumes the same validated private descriptor and source/recipe, splices only D328, and gates the full runtime-B image, exact compressed stream and cartridge as applicable. Do not move the whole nonmatching object into production or pretend a flat single-file compile preserves this IPA result.

At the base commit, accepted `src/blob/groups/dot_sign_extend/group.c` declares sign_extend_call (E398) void, while the research D328 contract consumes native v0. Full D328 byte equality is not invalidated by that naming/type discrepancy, but a coherent source-level integration must resolve the return contract. The existing accepted wrapper/header is unchanged; do not silently "fix" its declaration in an admission packet.

Accepted coverage remains zero until those separate integration gates are genuinely met.

## Portability and scope

Bind a future packet to its own source/verifier/recipe and full native target words; record this base commit. Read production facts through git show of that base. Do not receipt-pin live score.py/owndata.py, manifests, locks, or production C; do not assert live lock absence. Compare receipts on bytes/extents/relocations/owned-data/behavior only.

This report used no compiler, linker, source permutation, scorer/production/object edit, full checkout, or publication. Snapshot source files in this small notes directory were read from the named commit solely for audit. No ROM bytes or raw assembly are part of this report.
