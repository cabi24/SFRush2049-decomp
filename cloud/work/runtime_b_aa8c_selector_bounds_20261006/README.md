# AA8C selector domains and storage aliases

Research against base `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`; the remote master was checked at that commit on 2026-10-06. No source match, source-object ownership, complete-function behavior, or production-context change is claimed. No compiler is required. The companion packet owns only these notes, a bounded native selector interpreter, its tests and a metadata receipt.

## Conclusions

1. Mode 8 is a genuine initialized/reset state. It reaches real offset-table loads. It cannot be ruled out to justify an eight-row C array.
2. The arithmetic address for mode 8 starts at `0x80394884`, exactly the separately loaded source-color word. Subsequent indices overlap other data. Nine independent float rows are not proved either.
3. The registration call chain bounds its loop index to 0..5, with a per-slot active predicate; it does not by itself establish 0..3. Four-player attachment storage and four-player tests are not a substitute for the missing battle configuration proof.
4. Model selectors are unsigned bytes. Their transfer from configuration into physics is established, and one fallback configuration producer explicitly checks `<13`. A global proof that every AA8C model is 0..12 remains open.
5. The pickup producer writes attachment modes 0..7; reset paths write 8. These establish real normal-state producers, not an exhaustive alias-aware write proof.

## Concrete descriptor initialization and call chain

- `world_physics_tick` initializes physics and player records from `0x8014A250` and `0x80152818`, using strides 0x808 and 0x3B8. The zeroing loops are at `0x800EC37C..394` and `0x800EC39C..3BC`. It passes the current player-record pointer to `func_800B27E4` at `0x800EC464` (argument established at `0x800EC428`). The outer loop is bounded by signed halfword `D_801543CA`, not a local literal four.
- `func_800B27E4` at `0x800B27E4..2818` starts at player+0x110 and visits exactly 21 records, incrementing by 24. At `0x800B2804` it stores callback+0x14 = null; at `0x800B280C` it stores handle+6 = -1. Thus record 20 is player+0x2F0 with a negative handle.
- The outer setup loop at `0x800B1B74..1BD0` uses indices 0..5. The signed halfword predicate at `0x800B1B8C` is physics[index]+0x7C8. If nonzero, it calls `audio_frame_update` at `0x800B1BB4` with that signed-short index. Historical symbol boundaries/names in this area are imperfect; instruction addresses and the loop are the evidence.
- `audio_frame_update` calls `func_8038C910` at `0x800B0D1C` only when `D_8014A110 == 6`, passing the same signed-short index. C910 itself has no range check.
- C910 computes player+0x2F0. At `0x8038C948` it skips setup if descriptor.handle is nonnegative. Otherwise it sets descriptor+4 = 0 (`C960`), callback = AA8C (`C964`), owner+8 = index (`C970`), descriptor+0x10 from a clock/global (`C978`), creates the scene object (`C9B8`), stores the returned handle to descriptor+6 (`C9E4`) and the separate scene-control word at +0x18 (`C9DC`). Initial positioning uses offset row 1 at `CA00..CA0C`, regardless of current attachment mode.
- `cpak_read` starts its descriptor cursor at player+0x110 (`0x800B01CC`), executes 21 callbacks (`0x800B04A8`), and invokes each non-null callback at `0x800B0490` with `(descriptor, 1)`.
- `func_800B1F30` visits the same 21 descriptors, with its separate player+0xF4 guard. It invokes non-null callbacks at `0x800B1FB0` with `(descriptor, 0)`.

This proves registration, owner provenance and real update values. It does **not** prove that battle mode cannot activate setup slots 4/5. The four-record attachment arrays require an additional external domain invariant. Keep entry/helper tests at owner 0..3 and label that restriction honestly.

## Mode producers and invalid states

`func_8010D3C0` handles pickup/resource kind input at record+0x50. It switches after subtracting 350 and checking an 11-entry domain (`0x8010D478..D49C`). Its mode-selecting paths explicitly set 0, 1, 2, 3, 4, 5, 6 and 7 at `D4A4`, `D4B4`, `D4C4`, `D518`, `D528`, `D538`, `D548`, `D588`. The fallback sets 1 at `D598`. The common replacement store is player+0x384 at `D5C4`; the neighboring selection/ammunition byte is at +0x385. Three switch paths instead update other player properties and do not replace the mode.

`func_8038CA24` writes mode 8 at `0x8038CA7C`, selection -1 at `CA80`, and cached kind 9 at `CAA0`. AA8C's early-exit path writes mode 8 at `0x8038AB6C`. FCE0 writes mode 8 at `0x8038FF70` after its reset/empty-selection conditions. Thus mode 8 and cached sentinel 9 are independently real values; the cache sentinel is not evidence that current mode 9 is valid.

AA8C reads the current mode as a signed byte for offset arithmetic. On the changed-mode path it loads the offset components **before** validating the unsigned mode for its nine-way switch (`0x8038AE0C..AE28`). Modes outside 0..8 take the common resource-setting path at `0x8038B798`; they do not receive a mode-specific assignment to the resource-index local at stack+0x80. No arbitrary signed-byte public domain is supported. The verifier demonstrates this separate failure boundary with current mode 9 / cached mode 8.

The source scan found the direct +0x384 stores above. It does not exclude aliased writes, bulk copies, save/network loads or indirect producer paths. The normal producer set is evidence, not a whole-program validity theorem.

## Model provenance and the offset-table problem

The address formula is:

`0x803943A4 + signed_mode * 0x9C + unsigned_model * 12 + component * 4`.

The physics model byte is physics[descriptor.owner]+8, read repeatedly. Changed-mode component-load sites are `0x8038ACE8`, `0x8038AD48`, `0x8038AD9C`. Same-mode component-load sites are `0x8038B880`, `0x8038B8F8`, `0x8038B94C`. Descriptor.owner and the player mode are reloaded during address construction; do not silently turn all of them into immutable values across external helper calls in the full function.

`world_physics_tick` transfers configuration[index]+1, from base `0x80153E88` stride 8, to physics[index]+8 at `0x800EC558..564` and `0x800EC670..688`. Those stores do not clamp the value. One model-selection fallback at `0x800BB4AC..4D4` tests the source configuration byte with `<13` before copying it to another configuration slot. The initial random selection in that same routine uses the literal 13. Neither fact resolves every human-selection, loaded-configuration or other possible producer path.

For fixture models 0..12:

- Modes 0..7 address `[0x803943A4,0x80394884)`, consistent with eight rows of thirteen position vectors.
- Mode 8 addresses `[0x80394884,0x80394920)`.
- `0x80394884` is also loaded as a color word at AA8C `0x8038AAD4`.
- `0x80394888` is passed as a four-vertex position array by `0x8038A750` / call `0x8038A7D0`.
- `0x803948B8` is passed as color data at `0x8038A7C8`; later consumers index words from that base (`A854..A868`, `A890..A8B0`).

On a first post-reset update that passes the entry guard, current mode 8 differs from cache 9: the first three offset loads occur, cache becomes 8, and switch case 8 exits at `0x8038C8E4`. On the next same-mode update, the second set of three loads occurs; the cache-kind dispatch skips animation because the cache is neither 1 nor 0 (`B964`, `BF98..BFA4`). The loads are real native memory traffic even though those values are not used by mode-8 animation. Some of the overlapped words are not finite floats; the bounded verifier preserves their bits and does no floating arithmetic on them.

An eight-row declaration gives out-of-bounds C access for a real mode. An invented nine-row float object absorbs independently typed data and is not proven original ownership. A complete reconstruction must explicitly resolve its external address view and alias contract; do not erase native loads, assert an unsupported source object extent, or claim source-level validity solely from readable native memory. For mode 0..7, a model value of 13 already aliases the next row rather than proving a fourteenth model.

## Resource-table bounds and aliases

AA8C's scene model/resource table at `0x801427C0` is read with **unsigned halfword** width at `0x8038B7C0`, using a two-byte index stride. It is not the signed-word model-ID array discussed below. The mode-specific resource indices are exactly 216..223 for modes 0..7: 216 is assigned at `AE50/AE74`, 217 at `B404/B428`, and 218..223 at `B754..B794`. C910's initial unsigned-halfword load at `C99C` is entry 217 of this same table. The handle returned by the creation service is separately narrowed/stored at descriptor+6.

B-image initializer `func_803908D0` separately resolves ten names into signed-word IDs at `0x80399B18` and seven names into signed-word IDs at `0x80399B40`. These ranges meet exactly: `B18 + 10*4 == B40`. Consequently a numerical index 10..16 from the first address aliases slots 0..6 of the second, but contiguous addresses do not prove a single 17-element source object.

AA8C uses `0x80399B40` as its first effect-ID cursor and `0x80399B54` as the one-past-five comparison limit (`AE80/AE88`, `B438/B43C`): effect slots 0..4 are used. Its extra effect creation explicitly reads slot 6 at `0x8038B31C` (`0x80399B58`). Slot 5 is used elsewhere (`0x8038FC74`, a status-effect path), not a sixth muzzle slot. Preserve these distinct actual accesses. Initializer-owned names establish five muzzle-flash resources followed by shield and smoke; no publication of extracted native strings or data is required here.

## Reproduction and limits

Run:

    python3 verify.py --reference-root /path/to/SFRush2049-decomp
    SFRUSH_REFERENCE_ROOT=/path/to/SFRush2049-decomp python3 -m pytest test_packet.py -q

The verifier authenticates the live protected AA8C words through `score.targets()`, binds their complete 7,812-byte identity, and compares them to the authenticated image inflated in memory from **base-commit** assets. It neither pins live manifest/scorer/production hashes nor asserts live lock state. No ROM/image/disassembly is emitted. Compiler-independent tests run even if IDO or a MIPS linker is absent.

The 936 fixtures are owner 0..3 × mode 0..8 × model 0..12 × changed/same mode. They cover 170 selector-slice instructions, with exact ordered read addresses and stack-copy bits. They stop before private cleanup, object creation, model changes or animation. They do not validate a complete C function, ABI preservation, all helper effects, resource availability, general caller bounds, FCSR behavior or matching.

Unresolved integration prerequisites remain: prove battle slots 4/5 inactive (or otherwise define the actual player domain); close all relevant model-selector producers; establish the external offset-table/source-object alias view; and preserve helper-effect-sensitive reloads in the complete AA8C root. This packet does not change or own the shared table, production source, contexts, locks or protected tools.

Local validation: the direct replay passed in the packet workspace and from a foreign working directory with an intentionally absent IDO path; receipts were identical. Python syntax compilation passed. After loading the established dependency environment, focused pytest passed (1 test) both normally and from a foreign working directory with IDO absent. A workspace-local temporary directory was used. This is focused research validation, not the later integration aggregate gate.
