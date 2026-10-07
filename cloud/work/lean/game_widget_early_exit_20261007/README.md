# Widget opacity callback: native early exits

Follow-on to PR #247, using its complete genuine C unchanged except for control
flow spelling. Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target `func_80108154` is 896 bytes / 224 words, `[0x80108154,0x801084D4)`.

Canonical O3 comparison improves **167/224 to 45/224 differing words**. Candidate
extent is now exactly 896 bytes (previously 888), with zero nonzero excess,
unresolved symbols, unverified references or errors. Still **NONMATCH**.

The native invalid-player and hidden-widget paths both return 1 before the draw
body. Express those two exits directly instead of nesting the remaining work in
else/visible blocks. The entire valid draw path, float-to-u32 alpha conversion,
font/mode calls, record access and update order are preserved. No new variable,
operation, qualifier, padding, invented argument or context body is introduced.
The natural early-exit form restores the native extent without frame padding.

The exact previous callback is at PR #247 commit
`b1dbdfb53b13e2d4eab65f2d7e908a375d0c65a7`, path
`cloud/work/lean/game_widget_opacity_20261006/callback.c`, SHA256
`d603faa128de88b254248a8c28fe19752f9d975ae8385595cf11069a8b5dd53d`.
Both measurements use the same actual slot_state_setup and countdown context.
Context remains 18/58 (232-byte own extent; two canonical next-symbol excess)
and 105/150 (604-byte own extent; four canonical next-symbol excess), respectively.
Neither context is claimed. Returning font wrapper source remains genuine.

```
python3 tools/cloud/score.py group cloud/work/lean/game_widget_early_exit_20261007
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared`, plus the unchanged canonical
assembler `-r4300_mul`. Substitute the pinned PR247 callback to reproduce the
167/224 baseline. The target frame remains 152 bytes versus candidate 136 bytes;
remaining stack homes, allocation and scheduling differences are explicitly
unresolved. Private workbench diagnosis confirms the 16-byte non-save-frame gap.

Inherit PR247's native valid-domain assumptions: valid nonnegative player index,
sufficient arrays, valid font/glyph objects and finite representable alpha input.
Original source spelling and translation-unit ownership are not proven. Claims
remain empty. Protected target/scorer, locks and production files are untouched.
Only source, actual context/flags and notes are included. No broad tests, behavior
harness, CI wait, acceptance or integration. Independent checker owns validation,
acceptance and merging. No ROM bytes, raw assembly dumps, binaries or credentials.
