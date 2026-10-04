# BT03-high allocation, release and program transitions

Three strict O2 matches, **592 B / 148 words**, and three complete NONMATCHs,
**608 B**. Exclusive activation `8f6b2118`; branch
`dot/boot-tail-bt03-high-transitions` from exact master
`301d9e7552ad4fd7f54a38796db84671e1000d35` in a separate sparse /tmp store.

| Function | Bytes | O2 | O1 |
|---|---:|---|---|
|8001F898|188|MATCH|44/47 +4 excess|
|8001FA18|204|MATCH|51/51 +1 excess|
|8001FAE4|200|MATCH|50/50 +26 excess, unresolved HI16|
|80018184|200|6/50|50/50 +11 excess|
|80017720|204|51/51 +1 excess|50/51 +9 excess|
|8001B1D0|204|48/51 +1 excess|47/51 +7 excess|

Selected O2 matches have full relocated equality and zero nonzero excess,
unresolved symbols, unverified references or errors. The failed O1 relocation
control is explicit and receives no matching credit. Every residual is a complete
body with its real operations/callees; none is submitted as matching C.

## Whole native operations and ABI

All three accepted sources share the packed416-byte state: command word+0,
next identifier+16, flags+36, external byte+76, full identifier+96 and active
byte+189. Their exact offsets/stride are independently checked in host tests and
by IDO equality. Registered handles and allocator results must select allocated
voice records; arbitrary malformed indices/cycles are not claimed safe.

`1F898(u8)` calls actual four-input1F13C(u8,u8,u16,u8) with255/FFFF/1 defaults.
A valid slot gets two active/external bytes set before1EB10, then its full reserved
identifier, optional1467C/14AF0 handling, command zeroing and1EF8C(state,u8).
Return is the allocated slot or-1. No status/pointer formal is invented.

`1FA18(u32)` gates a real1EDF4 lookup, saves each next identifier before any
callback, rejects stale full IDs, optionally resets a nonzero command through
1F9D0 and releases the actual integer slot through14B3C. It follows saved next
links even when callbacks mutate current storage. Return is0 for any handled
record and-1 otherwise. `1FAE4(u8)` walks the live global count, optionally
preserves external voices, resets eligible active commands and clears flag bits0/1,
then calls14B3C. Preserved external voices skip that release; empty commands do
not. The live count is reread after calls, as native code requires.

`18184` processes a finite timed linked list at current-context+3960. It saves
next before real1B8C4/174D0 stop/removal calls, otherwise advances a split fixed-point
clock. Low32 additions are unsigned with explicit N64 two's-complement signed
interpretation for the native comparison and arithmetic shift. This avoids
introducing signed-overflow UB merely to improve register allocation. Pointer
members concern the N64 ABI; wider-host tests establish list semantics, not native
pointer offsets. The six-word residual is entirely the carry-shift register group.

`17720` selects one of two128-byte program maps, using the alternate for channel9,
skips FF mappings and packs one genuine eight-byte recipe's u16/high and two byte
fields into a selected word. The context has pointer/map pairs at+4/+8 and+136/+140,
with sixteen selected words at+3968. Tests use valid programs0..127, channels0..15
and mapped entries inside allocated tables. Those are explicit valid-resource
bounds, not a guarantee for arbitrary byte inputs. Caller17D38 masks programs to
seven bits; other resource-fed callers still require well-formed configuration.
The pointer-containing context's32-bit layout is native-only evidence.

`1B1D0` looks up a packed12-byte preset with real17040(u16), substitutes its two
byte defaults for FF inputs, packs a word from halfword+2 and bytes+5/+4, and
calls actual **ten-input**1A270. Callee entry and stack loads prove the signature:
u32, three u8, two more u8, two u16, u8 and s16. In its96-byte frame the last
signed halfword is read at+134 (incoming+38). Defaults255/255/0/255/parameter/0
are genuine native outgoing slots, not padding arguments. Original lookup/helper
bodies are declared only and remain unchanged.

## Diagnosis and bounded controls

Every source used O2 first, then O1. The unchanged workbench diagnosed all four
initial residuals before any refinement. Temporary native listings/objects are
not committed; strict relocated comparisons remain authoritative.

- `1F898`, `1FA18`: first-form matches.
- `1FAE4`: initial8/50 contained an unnecessary byte-truth-value copy. Ordinary
  `!flag`/`flag` boolean spelling of the same predicate closes it. Two forms.
- `18184`: separating the empty-head fast path and using the real sum for the
  comparison/carry improves19/50 to6/50. Fresh diagnosis confirms a register-only
  carry-shift residual; stop after two forms rather than weaken overflow semantics.
- `17720`: modeling the recipe as one halfword and two actual bytes instead of
  two halfwords retains the complete native operation but does not solve the
  known narrow-formal home/mask allocation. Two forms, then stop.
- `1B1D0`: one complete source; the known u16 home/mask and narrow argument
  coalescing pattern is not subjected to another declaration/register sweep.

Twelve final rows and six rejected-control rows are source-hash bound. No fake
formal, keeper, local padding, forced register, volatile trick, assembly or
alternate flag sweep is used. Object gaps are actual unexamined record storage.

## Verification

Six strict-C89 ASan/UBSan tests check allocator/callback order, saved chain links,
release filtering and live counts, signed/wrapped timer arithmetic, all2048 valid
program/channel selections, and the genuine ten-argument preset call. Helpers
are synthetic contracts, not copied native implementations. Whole-object checks
preserve unrelated fields; LeakSanitizer alone is disabled under ptrace.

```sh
python3 cloud/work/boot_tail/BT03-high-transitions/verify.py
python3 cloud/work/boot_tail/BT03-high-transitions/verify_controls.py
python3 -m unittest discover -s cloud/work/boot_tail/BT03-high-transitions -p 'test_*.py' -v
```

Fresh setup, target manifests, all439 starts/99,120B and the existing getter pass.
Only three matching submissions and this owned packet change. Baseline lock and
storage-proof dependencies are materialized unchanged for sparse-checkout guards.
Prior sources, central STATUS/D10, targets/scorer, symbols/layout/locks, runtime
image/farm and forbidden helper work remain untouched. No ROM, native dumps,
objects or private material is included. Independent source/ABI review and exact
aggregate-head CI precede checker-owned merging; no cartridge promotion is claimed.

Independent paired review PASS at source commit
`47d6fd6d7b2769137919aaf529cd3efeb1c95e98`, tree
`908e0ceb0338518d898e574a17157ecd9c700b89`. All twelve final rows, six rejected
controls and six sanitizer tests passed independently. Native saved-next/live-count
behavior, the real allocator and ten-argument widths/slots were approved, retaining
the documented resource domains. The rejected O1 HI16 failure reproduced exactly.
`independent_review.json` binds the three matches /592B to unchanged source hashes.
