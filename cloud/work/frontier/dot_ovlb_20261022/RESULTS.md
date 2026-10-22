# Overlay B research handoff, 2026-10-22

Base: `dd4cf01d3846ef60ca88708dac83af446da5a4f2`.

**No new verified match and no coverage credit.** This note records isolated
research only. No installed/locked source, lock, target, checksum, scorer,
receipt, or verification file changed. No replacement source/patch, private
reference source, binary payload, or raw assembly is included.

## AnimateCase: zero differing text, not an accepted group

The historical genuine-helper reconstruction in
`cloud/work/frontier/w38b/v/sh/k1beforec0` emits `func_8038FCD8`, the deleted
AnimateCase helper, as an 8-byte stub. Its remaining ShotUpdate difference was
47/764 words: an extra 8 bytes of frame and shifted stack homes.

A source-lifetime experiment removes that difference. All eleven group members
(12,368 bytes total) and all six informational context members report zero text
differences with the unmodified canonical scorer. **Only 8 bytes are potentially
new**: the other ten group members are already locked. Selected exact output:

```text
func_8038FCD8:
  MATCH
```

For `func_8038FCE0`, the scorer reports `MATCH` **with 32 section-relative
relocations unverified**, including:

```text
    not verified: .rodata+0x0 at +0x6bc: no retail data available
```

Other members also have unverified owned-data relocations. The full
`score.py group` command exits **1**, not 0. Thus this is zero differing text
under the scorer's unverified-owned-data masks, not strict proof of a match.
Do not claim the stub independently from its real group or put it in
`cloud/matches/`.

There is also an authorization stop: reproducing the helper requires changing
locked ShotUpdate source inputs. The user's no-locked-function-change rule
prevents promotion. The experiment was kept isolated; no installed caller was
changed. A later owner decision and complete owned-data/group/image verification
would both be required before integration.

### Replay outline from committed historical source

Set up stock IDO 5.3 and GNU MIPS binutils as described in `CloudHandoff.md`.
Make a fresh ignored working copy of `w38b/v/sh/k1beforec0`; do not edit the
historical or installed inputs. Leave all group metadata and other units intact.
In that working copy's ShotUpdate body only:

1. Remove its two final hit-poly-cleanup pointer locals. That cleanup runs after
   the last uses of the existing `h` and `shot` carriers: reuse `h` for the current
   hit-poly node and `shot` for its successor, with accesses explicitly cast to
   `HitPoly`. Preserve all operations and ordering.
2. Move the existing `sizep` and scalar `z` declarations before `off[3]`, removing
   the now-duplicate inner `z` declaration. Keep the helper's own `idx` local.

This restores ShotUpdate's native 208-byte frame and 3,056-byte text extent.
It is a source-shape hypothesis, not recovered original spelling. The pointer
lifetimes/casts still need independent semantic review before any authorized use.
Run the full group, with no claims-only shortcut:

```sh
python3 tools/cloud/score.py group build/YOUR_FRESH_WORKING_COPY --targets asm/us/ovl_b
```

Expected in this data-less environment: all member text comparisons report
`MATCH`, several with the caveat above; overall exit 1. Never synthesize missing
owned data, weaken the gate, or treat the leading word `MATCH` as success.

## UpdateHUDExtraPostCamera: still nonmatching

`func_80391B00` is the only substantive unlocked B body (2,380 bytes); the other
five unlocked extents are 8-byte deleted-inline helpers. A bounded source-shape
pass improves the clean `w38b/91B00/best0` baseline from **407/595** to **112/595**
differing words. Its two unchanged sibling bodies remain text-zero, subject to
owned-data verification. This is not a match or an integration candidate.

This experiment can also be replayed using only already-committed source:
copy `w38b/91B00/best0` to a fresh ignored directory; bring across the four unused
window-offset assignments already present in
`w36j/func_80391B00/g16/group.c`, placing them after the `coin->y` interpolation
and immediately before `ZOID_WindowTo3D`; remove the unused `tex` and `texIdx`
declarations from animStuntCoins. Use the same full scorer command above.
The resulting extent is 2,380 bytes and frame 336, but aligned control flow and
stack homes still differ. Equal size is not matching evidence.

A fresh unforced traced-uopt run reproduced the clean baseline's complete stock
object byte-for-byte. It confirms the previously documented PRE/allocation cause:
the player-address expression splits, then a surviving piece with saving 3.5
colors `a0`; the native code retains a spill home. Natural control-flow and
window-offset simplifications did not resolve it. No forced diagnostic object
is claimed, and the older uninitialized `if (e) {}` holder is not a source fix.

Next useful work needs an evidence-backed explanation of the missing expression
web/stack-home interaction, not a broad syntax sweep. The two coin-helper stubs
can only accompany a genuinely matching caller group.
