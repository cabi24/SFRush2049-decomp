# ZOID_InitializeWindow: restore the real return and packed-color expression

`arb_rate_set` at `0x800A5908` is the N64 window initializer. This source matches **78/78 words, 312 bytes**, with its native **56-byte frame**. The genuine background-color setter (`func_800A5560`, 28 bytes) and projection setter (`exhaust_smoke_effect`, 452 bytes) remain strict matches with their full extents and own-data references checked.

Only the root is a new candidate claim. Both helpers were already locked. Accepted image/ROM credit is **zero**; canonical integration and whole-unit/image/ROM gates remain pending.

## Genuine source evidence and the two changes

The accessible matched MAT3 `ZOID_InitializeWindow` returns its selected window record. It also narrows the packed RGB expression to unsigned short **before** adding the final opaque-alpha bit. Applying these two source-level facts to the current native reconstruction gives the exact result:

1. Change the root's return type from void to the window-record pointer, and return the indexed record. In the real helper context, this improves the old 25/78 residual to 6/78.
2. Write the packed color as `(u16)(RGB bits) | 1`, as in the donor, rather than narrowing the complete RGB-and-alpha expression only at the helper parameter. This closes the remaining six words.

The intermediate cast is value-equivalent for the three unsigned-byte color inputs. The host harness exhausts all 16,777,216 RGB combinations. Its compiler effect is not an arithmetic or register oracle: the expression comes directly from the genuine donor source.

The matching candidate retains direct array indexing. The donor's meaningful local pointer spelling remains six words different, and is retained as a negative control. Neither spelling is asserted to be recovered original N64 source. No unused locals, dummy callers, identity arithmetic, altered prototypes for allocation, forced registers, Ucode edits or raw-assembly replacement are used.

The return value also agrees with the native routine's existing return-register flow. The already-locked default-window caller declares a returned record, but its compiled-out holder is supporting reconstruction evidence, not an independent runtime use. The donor and exact native body support the initializer's nine-argument contract: index, viewport pointer, scissor pointer, then six floats; returned value is the selected record pointer. It does not harmonize other callers' historical declarations or claim their integration.

## Native differences retained

The later MAT3 implementation is not copied wholesale:

- N64 front/back clip values remain **2.5f / 2000.0f**, rather than MAT3's 0.01f / 4000.0f.
- N64 viewport scale/translation are signed shorts with the native multiply-by-two behavior. MAT3 has float fields and a multiply-by-0.5 expression.
- Both versions update the global viewport, not the viewport pointer parameter.
- N64 fog range remains **1000 / 999**, with black RGB and alpha255; MAT3 uses zero/zero.
- The native projection helper retains its already-locked N64 behavior, including interlaced-height handling. Its existing literal spelling choices are unchanged, not new claims in this packet.

Some historical field names in the native draft are misleading: the fields named `near_plane` and `far_plane` at +0x2C/+0x30 are projection centers; `zoom`/`fog` at +0x38/+0x3C are the clip distances. The native layout is still 0x48 bytes. Donor naming is documented here without an unrelated source-wide rename.

MAT3 provenance: `GameSource/src/zoidwindow.c`, ZOID_InitializeWindow at 0x80062FB8 (220 bytes), ZOID_SetWindow (336 bytes), ZOID_SetBGColor (16 bytes). Original DWARF lists one window-record pointer local. Source SHA256: `ce9be99cc01cacae7aa684ee42b476348ea48c585c32ad6fb7d48adc59bf9d25`. The existing report records all 13 functions matching; this packet does not claim a fresh MAT3 build.

## Replay and controls

With stock IDO 5.3 and MIPS binutils configured:

```
python3 cloud/work/frontier/dot_mat3_window_init_20261017/replay.py --check
python3 -m pytest -q tests/cloud/test_mat3_window_init.py
```

`--materialize <empty-directory>` writes the isolated source group. Canonical helper files are copied unchanged; the historical inline duplicate of the background setter becomes its real prototype. The source verifier permits only that boundary cleanup and the documented return/packed-color changes. `group.json` claims only the root, and the replay strictly checks all three functions.

The proof binds packet, context and protected-target hashes, function offsets and ELF extents, native frame size, complete scores and executable/data/relocation hashes. Two fresh builds agree. All controls preserve both helpers:

- Historical void body: 25/78 differences.
- Return alone: 6/78; intermediate cast alone: 23/78.
- Donor-style local pointer with both changes: 6/78.
- Wrong front clip, back clip, alpha or returned record: rejected.

The host harness compares full historical/candidate state, pre-callback records, seven callback arguments and callback counts over **20,324 finite cases**, and independently checks the newly restored return pointer. A bounded projection-callback stand-in records and updates actual interface fields; it is not a trigonometric implementation. The native helper is independently matched in the group. Signed-short conversion boundaries stay within range. UBSan is fatal, and wrong clip, alpha and return mutations must produce the exact differential failure code and message. No claims are made for invalid indices, invalid objects, arbitrary out-of-range float conversions or arbitrary NaNs.

No ROM bytes, literal target-data dumps, raw assembly, compiled objects, private traces or credentials are included. Independent review is recorded separately.
