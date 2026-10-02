# A30 frozen genuine guarded counter closure

Claim: `audio_loop_control`, 46 words / 184 bytes. Eleven existing real A25 functions are context only (330 words). Every context independently scores MATCH in this exact 376-word group; definitions are unchanged, with hashes in source_audit.json.

Flags: `-g0 -O3 -mips2 -G 0 -non_shared`

Canonical verification on Rocky A: `python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_counter_guard_a30 --claims` exits 0. Claimed function: strict_diff 0, exact size 46, extra 0, no unverified/unresolved relocations or errors. Sanitized results are in scores.json; instruction-bearing output stays ignored at build/codex-A30/score_claims.txt on Rocky A.

Source SHA256: `74e98679bf036a724c2696711e38b008db5d127ba511892d8c54ce16644afd97`.

Semantic audit: two consumed arguments are the full-width allocation address and decrement selector. Actual queue receive precedes the range-owner lookup and tagged block lookup (tag zero). The real owner/reference word at block offset 16 causes an early queue jam and return when nonzero. Otherwise unsigned byte counter at offset 22 decrements only above zero or increments only below 255; final queue jam follows. These are retail guards and saturating boundaries, with no invented reads, writes, wrappers, formals or compiler quirks. Actual lookup definitions preserve address and queue register lanes in this same compilation; the strict same-context verification establishes that scope rather than assuming ABI preservation from historical acceptance.

Single faithful source baseline reached MATCH without allocation, formatting or pressure sweeps. Current shared lock checked: claim was unlocked at freeze. No accepted tree, layout, lock or state changes. Root independently verifies and applies image/ROM gates before acceptance.
