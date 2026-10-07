# Runtime-A menu state and input group

Frozen base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Stock IDO 5.3 group pipeline, actual `-g0 -O3 -mips2 -G 0 -non_shared`.

## Observed matching compilation

- `func_80398B40`: **40/40 strict**, 160 bytes. State entry/setup.
- `func_80398E70`: **152/152 strict**, 608 bytes. Player selection and dialog entry.
- `func_80398C3C`: **141/141 strict**, 564 bytes. Scroll-position/input update.
- `func_803990D0`: **8/173 differing**, 692 bytes, research root.

The three helper bodies are claimed: **1,332 new candidate bytes**. Every
member reports zero unresolved symbols, unverified own-section references,
relocation errors and extra words. No behavioral, integration, source-originality
or cartridge-coverage claim follows from this local result.

The scroll endpoint expression is `position == visible_count - 1`, equivalent
to the initial `position + 1 == visible_count` for the signed-halfword globals.
The former avoids a different shared expression web and closes the initial
79-word positional residual. The root's remaining eight differences are its
112-byte native versus 96-byte candidate frame and actual local home offsets;
no padding or unused locals are added to force those offsets.

All four bodies are complete real functions. The root and input routines call
the state setter using its actual one-word mode input; IDO assigns native s0
naturally. The input routines preserve their native private s0/s1 clobber
contracts through the real root. No dummy formal, caller, retention barrier,
volatile qualifier, forced register, frame filler or assembly is used.

The root's five-entry computed switch is recovered from authenticated image A:
`0x803B9508..0x803B951C`, SHA-256
`2e5824447f1b935bb4328efc81ce690a165b9cdfd5b51ec49cb483f0908bc2d8`.
Cases 0 through 4 follow native body order. `reproduce.py` authenticates the
tracked asset and decompressed image in memory and binds that image to the
unchanged protected scorer's owned-data validation; no image is published.

## Source boundaries

The 36-byte MultiBlit descriptor follows pinned accepted
`cloud/matches/sound_control.c`; its real initialized local copy is preserved.
The three signed-byte outputs of `control_settings` are real addressed locals.
Its callback is the existing `func_8039A24C(s32)` source at the pinned base.
`display_settings` and `player_state_set` signatures follow their existing
accepted implementations. `D_80156CF0` exposes the observed signed presence
byte in each 16-byte record; remaining bytes are an opaque native layout view.
The selected node is a pointer-to-pointer, consistent with the existing
`cloud/matches/ovl_a/func_80398BF0.c` traversal. That previously reported O2
lookup stays external, with no duplicate credit and no flag override.
External initializers and the rest of the image remain external. This draft
does not claim complete ownership of those globals.

## Reproduce

With IDO and GNU MIPS tools configured, from a checkout containing the fixed
base's tracked asset:

```
python cloud/work/frontier/dot_runtime_a_menu_state_20261006/reproduce.py --repo .
```

For a minimal source/target overlay use `--repo OVERLAY --reference-root GIT_REPO`.
Only matching compile/scoring was run. Acceptance testing, promotion, locks,
CI and merging remain with the independent checker.
