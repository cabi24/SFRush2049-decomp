# C7110 record validation: genuine caller-context research

Observed local research candidate, explicitly NONMATCH.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `draw_ui_element` at **0x800C7110, 240 bytes / 60 words**.
Its historical name masks a record header/payload checksum validator.

Canonical local comparison improves **57/60 to 52/60 differing words** when
both real callers are visible and kept while the target and checksum helper are
internal. The emitted target is **236 bytes**, four bytes short. No unresolved
or unverified references, relocation errors or extra nonzero words are reported.
This is a context lead, not a close match or behavior-equivalence claim.

The unchanged validator body comes from
`cloud/work/ipa-groups/codex_checksum_a17/group.c`; that packet documents 57/60.
A minimal target-plus-accepted-hash control also reproduces 57/60 locally.
The unchanged checksum body comes from
`src/blob/groups/codex_hash_a80/group.c`.
The real caller bodies are extracted from
`cloud/work/frontier/w11a/menu_back/best.c` (`func_800CBF2C`) and
`cloud/work/ipa-groups/codex_func_800C7200_2/group.c` (`func_800C813C`).
Only their necessary type views/declarations are retained. The former uses the
recovered 44-byte record and actual slot fields; the latter retains its original
byte-offset view. Neither caller is claimed matched. Existing dead locals in the
large caller are inherited, not introduced pressure or stack-storage requests.

With only one of these real callers, the validator scored 60/60; both together
produce the observed reduction. Other genuine callers/callees may be needed.
Compiler visibility and recovered types remain hypotheses; no synthetic callers,
extra formals, volatile accesses, padding or assembly are used. No production
source or accepted lock is changed.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_validation_c7110_research_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`, canonical `-Olimit 5000`
and `as1 -r4300_mul`. Independent checker owns acceptance and ROM integration.
