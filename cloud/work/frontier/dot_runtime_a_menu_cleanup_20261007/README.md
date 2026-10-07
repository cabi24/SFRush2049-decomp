# Runtime-A player-menu cleanup

Frozen base `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Stock IDO 5.3 actual `-g0 -O3 -mips2 -G 0 -non_shared` group compilation.

New complete `func_803949CC`: **77/77 strict words**, **308 candidate bytes**,
zero unresolved or unverified references, relocation/data errors, or extra
nonzero words. There is no owned literal data in this cleanup function.
Only this new cleanup is claimed. It matched on its first complete source
reconstruction, with the real 9842C caller already present.

The cleanup scans every active player's 22 real 64-byte display slots, releases
allocated model IDs and restores their -1 sentinel, clears per-player state
and owned display parts, removes the shared UI blit, and resets the view and
menu-active state. The signed player index, real matrix/position slot layout,
16-bit model-call argument and global types follow native accesses.

## Real call context and limits

The complete 1,804-byte `func_8039842C` menu caller is reconstructed, including
all seven mode cases, linked-list owner validation/removal, device-loss handling,
startup, commit/cancel cleanup, and trailing refresh. Its jump table was checked
in the authenticated image: cases 0 through 6 select the corresponding seven
native bodies in source order. It is **NONMATCH**, with 448/451 differing words
and two unverified own-data references. Other private callees remain ordinary
external declarations; no fictitious parameters or register shims are used to
imitate their private calling conventions.

The group reuses the availability/setup/state-change/refresh family from #290.
Existing `func_80394834` remains strict 100/100 words and earns no new credit.
The availability producer still differs at 3/180 entry-argument-register words.
The setup and state-change callers remain NONMATCH: setup 176/268 words differ
with four unverified own-data sites; state-change 162/182 words differ.
No unresolved references, relocation/data errors, or extra nonzero words occur
in any member in this group. Original source names are unknown; structure views
retain observed accessed layouts, not proven original declarations.

Both the frozen-master and full PR #163 source-definition indexes were checked
before reconstruction; neither contained a nonempty definition of the new
cleanup or its new caller. No fake callers, unused frame fillers, forced
registers, assembly, artificial volatility, or target instruction encodings
are included.

## Reproduce

Configure stock IDO and GNU MIPS tools, then:

```
python cloud/work/frontier/dot_runtime_a_menu_cleanup_20261007/reproduce.py --repo .
```

For a source overlay, supply `--repo OVERLAY --reference-root GIT_REPO`.
The script authenticates the pinned tracked asset, loader pointer and image A
in memory, then performs ordinary group compilation and strict relocation/data
scoring. It writes no target binary or assembly dump.

This is local matching evidence, not accepted-image or ROM coverage. No broad
tests, CI wait, promotion, proof packet or acceptance run was added. The
independent checker owns acceptance, integration and merging. Choose one
alternative full group when integrating; do not combine this and #290 twice.
