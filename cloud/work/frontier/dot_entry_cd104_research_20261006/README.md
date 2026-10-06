# CD104 save-entry constructor: repair digit-cursor reconstruction

Observed local research candidate, explicitly NONMATCH.
Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Target: `func_800CD104`, **1088 bytes / 272 words**.
Source/recipe baseline: `src/blob/groups/menu_save_options/`.

The local canonical comparison improves **253/272 to 249/272 differing words**.
The primary value is a concrete source-dataflow repair: the prior reconstructed C
initialized `dig` but read/decremented an uninitialized `src` during numeric suffix
formatting. The target initializes its cursor to the fifth digit immediately after
formatting (function +0x178). The candidate initializes `src = &digits[4]` there.
This defect is in the prior C reconstruction; it is not attributed to the game.

Other cleanup is narrowly related: remove the unused `dig` and `num` aliases,
express the stored handle tag using unsigned pointer bits before the left shift,
and use the accepted hash function's actual unsigned return/argument declaration.
No storage is added to compensate for the removal of unused aliases.

The candidate remains far from matching: its ELF body is **1068 bytes**, twenty
bytes short, and its frame is 184 bytes against the target's 224 (the old draft
was 192). The score improvement does not establish geometry or behavior parity.
No unresolved/unverified references, relocation errors or extra nonzero words
are reported. Natural filename/suffix helper experiments were worse and are not
included. Existing real menu/save context is retained; no additional body is
claimed, and no lock or production source is changed by this packet.

## Reproduce

With IDO 5.3 available through `IDO_DIR`, from the repository root:

```sh
python3 tools/cloud/score.py group cloud/work/frontier/dot_entry_cd104_research_20261006
```

Exact flags: `-g0 -O3 -mips2 -G 0 -non_shared -Wab,-r4300_mul`.
The canonical group pipeline includes `-Olimit 5000` and `as1 -r4300_mul`.
No broad tests, independent acceptance replay, image/ROM integration or CI
watching was performed. This is not accepted coverage; the independent checker
owns acceptance and merging.
