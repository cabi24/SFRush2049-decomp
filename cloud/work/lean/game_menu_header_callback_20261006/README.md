# LEAN RESEARCH: menu-header callback (8010B5D0)

Base: `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`. Native extent: 556 bytes / 139 words, frame 152. This packet reconstructs the complete header callback with actual coordinate values, text pointers and formatter branches.

## Observed O3 result

IDO 5.3 `-g0 -O3 -mips2 -G 0 -non_shared`; canonical automatic `-Wab,-r4300_mul`:

- Cast-only repaired generated seed: **138/139 differing words**, 480-byte function, frame 64, zero excess, two unverified owned-data sites.
- Complete typed body with genuine font context: **131/139**, **544 bytes/frame 128**, zero excess and no unresolved/unverified sites or scorer errors.

The candidate remains twelve bytes and twenty-four frame bytes short. It is research, not a match. The baseline is frozen `cloud/work/registered-heads/seeds/func_8010B5D0/group.c`, cut before its helper definition to remove stand-ins, with pointer casts for the two integer-address dereferences. That control still has undefined stack aliases and a scalar scratch declaration; it is explicitly not a semantic reference.

## Complete native reconstruction

The callback clears/reloads the real Blit hidden byte, selects font 13, snapshots header coordinates, and chooses among formatted player text, mode-7/8 direct text, mode-6 empty text, or formatted player-name text. It retains the actual name-pointer chain and +20 text offset, uses the native full-word color lookup, centers with the native unsigned half-width operation, and resets rendering with `-1.0f`. The generated seed's uninitialized low-halfword stack aliases are replaced by the actual x/y values.

Native layout views include Blit hidden byte +0x1A, the coordinate halfword pair at D_80117280/+2, text-root pointer fields +4/+12/+16, unsigned bank index +0x70, 0x4C-byte input records with a name pointer at +0x48, and actual label slot 233. These express observed offsets, not recovered original type names. The width interface is current `s32 object_manager_update(u8 *, s16)`; the formatter remains the real external variadic operation.

## Fixed meaningful scratch hypothesis

The native formatted-text scratch begins at stack +96, with a distinct text-pointer slot at +136 and coordinate words at +140/+144. The candidate uses one **fixed 40-byte buffer hypothesis** from that observed region. Its original C capacity and the runtime encoded-text bound are not proven. Capacity was not varied for scoring; the frame gap is not filled with padding or extra locals.

The explicit assumption is that each formatted encoded result, including its terminator, fits this buffer. No broader behavioral-safety claim is made. Valid player/text indices, pointer chains, service objects and intended coordinate arithmetic are also assumptions. No fallback validation outside native behavior is introduced.

## Genuine context and minimal replay

`slot_context.c` is the actual frozen credits-scroll font body normalized as in PR #215. `countdown_caller.c` is the unchanged actual callback from corrected PR #220. The returning gfx/font abstraction follows frozen `cloud/work/s20261004/E/src/helper.h`. Only real call sites are used; context scores/hashes are included, and claims remain empty. Full font IPA, buffer-bound evidence and frame layout remain unresolved.

With project IDO and MIPS tools configured:

```
python3 cloud/work/lean/game_menu_header_callback_20261006/replay.py --repo .
```

The replay reads immutable tools, baseline and protected target/manifests into temporary storage, compiles and reports metadata. No behavior/acceptance checks, invented pressure operations, live-source changes, locks, splice claims or ROM integration are added. Independent verification, capacity resolution, acceptance and merging belong to the checker.
