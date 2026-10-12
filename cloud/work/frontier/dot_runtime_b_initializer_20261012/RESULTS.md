# Image-B initializer: stock match (+560 candidate bytes)

Status: strict standalone MATCH; input-bound full-object and GNU proof passed.
Target B:func_803908D0, [0x803908D0, 0x80390B00), 140 words / 560 bytes.
Baseline master 1a2e0f005, with the target reserved in CLAIMS before implementation.
Prior locked B:803936A8 is not counted. No image/splice/ROM gain is claimed.

The unchanged prior initializer candidate reproduced 6/140 differing words.
Workbench diagnosis identified a scheduling residual. Stock UGEN/as1 tracing
showed an alias edge from the name load to the fifth outgoing stack argument.
An unpublished assembler oracle removing that edge reached equality; its bytes
are not acceptance evidence. Replacing only the two texture pointer-cursor loops
with ordinary indexed array loops makes stock UGEN recover global-array
provenance and emits the required no-alias regions. The source is a strict
MATCH without unused cursor declarations. No live buffer, ABI or ownership
change is involved.

A pointer-difference expression worsened the result and a mixed index/cursor
loop left eight differences. These were bounded mechanism probes, not sweeps.
The actual delivery is cloud/matches/ovl_b/func_803908D0.c. The historical six-word
research packet remains unchanged. Trace files and native bytes stay temporary.

## Proof and independent reproduction

The source uses ordinary O32 calls, the established five-argument texture lookup,
real ten/seven/five/five-entry name arrays, and four 52-byte player-effect records.
Its existing prior research packet documents the helper interfaces and consumer
layout. This delivery changes only the two texture loops and removes their now
unused cursor declarations. It does not change those interfaces or record size.
The inherited volatile count declaration is explicitly noncausal: removing it
still gives MATCH. No original source ancestry is claimed.

The fresh receipt binds the submitted source and verifier, selected native
function/extent/image identity and compiler recipe. Stock IDO 5.3 identity is
checked against the repository's known compiler fingerprints. No static ROM TU
is rebuilt. The entire ELF function and .text are 560 bytes, with no padding,
owned data, omitted tail, unresolved symbol or masked comparison. All 61 text
relocations cover 23 external bindings. Independent GNU linking at exactly
0x803908D0 produces the same complete native bytes and verifies every binding.

The generated negative control restores only the old two pointer-cursor loops:
its six differences are exactly +0x15c/+0x160/+0x170 and
+0x1ac/+0x1b0/+0x1c0. No other word or extent changes. The optional stock trace
compares whole ordinary/diagnostic objects byte-for-byte and confirms two real
name-cursor/stack no-alias regions in the indexed source, zero in the control.
No assembler oracle or patched compiler is used for acceptance.

Commands (with IDO_DIR and GNU MIPS binutils configured):

```
python3 tools/cloud/score.py fn cloud/matches/ovl_b/func_803908D0.c func_803908D0 --targets asm/us/ovl_b
python3 cloud/work/frontier/dot_runtime_b_initializer_20261012/verify.py
python3 -O cloud/work/frontier/dot_runtime_b_initializer_20261012/verify.py
python3 cloud/work/frontier/dot_runtime_b_initializer_20261012/verify.py --trace
python3 -m pytest tests/cloud/test_dot_runtime_b_initializer_match.py -q
```

Strict scorer: `func_803908D0: MATCH`.
Receipt replay: `PASS: 140/140 words; 560-byte GNU exact-VMA proof; 61 relocations; six-word control`.
One routine test passes in under one second. Both documented scorer setup sanity
checks also passed: sound_handles_clear and all three resource_slot_clear members.

New strict candidate yield: **560 bytes**. Preserved/integrated yield: **0 bytes**.
Accepted image and ROM gain: **0 bytes**, pending the independent checker.
This is exact compiler/object evidence, not an independent helper-execution,
gameplay, image-splice, compression or ROM-hash claim. The historical initializer
behavior suite was not rerun; its bounded abstract helper evidence remains
historical. No lock, protected input, shared tool or production source is changed.
Independent review and exact-head hosted CI are pending; keep the PR draft.
