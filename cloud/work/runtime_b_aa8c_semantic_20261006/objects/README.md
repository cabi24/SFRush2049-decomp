# AA8C source-object boundary and owner-domain evidence

Base: `6b2e9e506fe3d2267a710e41c85af5364ccd00c7`.
Outcome: **the original offset/color C object contract remains blocked**. This
investigation closes several tempting but unsupported routes and narrows the
owner-domain gap to specific upstream preconditions. It does not propose a
matching translation unit, change source/context, or redefine native targets.

## New usable result: normal battle setup suppresses owners 4/5

The actual native initializer `init_state_continue` at `800FAF6C..800FB234`
contains the missing battle-specific behavior, rather than merely another
six-slot loop:

1. It loads signed active-player count from `8014A108` and candidate AI count
   from `80142724`. The AI count is first limited by `6 - active_count`.
2. `800FAFA8..800FAFC4` sets the added AI count to zero for gameplay modes 4,
   5, and **6**. `800FAFCC..800FAFF0` stores the resulting total to `801543CA`.
3. The complete six-entry loop writes configuration records at `80153E88`,
   stride 8. In mode 6, slots below active_count receive +6 = 176 and +7 = 6
   (`800FB0BC..800FB0DC`); all remaining slots receive +6 = 0 and +7 = 7
   (`800FB0E0..800FB210`). It preserves model/config bytes +1 through +4.
4. `world_physics_tick` projects `(configuration[slot].byte6 & 128) != 0`
   into the signed active halfword physics+`7C8` at `800EC46C..800EC480`.
   Its separate tail also explicitly clears physics+`7C8` for records between
   total_count and slot 5 at `800EC7F8..800EC868`.
5. The registration loop's active test is precisely that halfword at
   `800B1B8C`, before the call at `800B1BB4`; its downstream mode-6 check at
   `800B0D04..800B0D20` invokes C910 with the same slot index.

Thus, **after that setup, with positive active_count <= 4 and no intervening
producer invalidating the state, owners 4/5 cannot be registered through this
loop**. The full initializer and the exact flag-projection slice execute in
`verify.py`; there are no external service stubs on the executed initializer
paths. Modes 4/5 are regression controls, not AA8C battle claims.

The Runtime-A UI at `80390560..803905D4`, within `func_80390418`, provides a
real upstream bounded-count producer. It scans four 16-byte records beginning
at `80156CF0`, stopping at the first status byte not equal to 1. This is a
consecutive-prefix count, not a population count. If the selected positive
active_count exceeds that count, it writes max(1, prefix_count). The decrement
and increment paths at `80390678..803906F4` wrap against the same limit.
All 81 four-record status patterns over {0,1,2}, with incoming count 1..7, were
executed for the scan/clamp slice. Every result is 1..4.

### What remains unproved about owners

These are conditional native dataflow facts, **not an exhaustive reachability
proof**. The initialization order across every game transition, alternative
config/active-count writes, save or external inputs, and post-setup alias writes
have not been closed. Two direct calls to init_state_continue are visible at
`800FB5B8` and `800FBFB0`; this packet does not prove they dominate every C910
registration. Other active-count producers include the mode-2-only path at
`800F6AB8..800F7304` (mode check `800F6B1C`) and several one-player resets.
The existence of the bounded UI cannot silently erase those obligations.

Native counterexamples are retained: a mode-6 setup entered with active_count
5 enables owner 4; count 6 enables owners 4 and 5. Therefore the six-slot
initializer itself supplies no unconditional four-player bound.

The layout collision is concrete. `99120 + 4*10C == 99550`, so the fifth
slot record begins at group 0. `99550 + 4*148 == 99A70`, so the fifth group
record begins at the separately used 25-object ring. `CACHE` at `99118` having
space before `99120` does not create six slot/group objects. The four-record
interpretation has strong native-layout support but remains a source extent
inference; adjacency alone is not an original array declaration.

## Offset/color source ownership: what the repository actually contains

The image-B initialized span is `[8038A400,80394F70)`, 43,888 bytes. Its scanner
marks text ending at `80393B34`. The loader `InitMaxPath` at `800A1244` selects
the B-image ROM pointer at `8002B024`, destination `8038A400`, initialized-end
`80394F70`, and BSS-end `8039B440`. Its shared loader at `800A11E4` inflates to
the destination (`800A1218`) and calls bzero for `[80394F70,8039B440)`
(`800A1220..800A1228`). This proves linked-image storage and BSS initialization.
It does not recover a C array, enclosing struct/union, or effective type.

At the recorded commit:
- `splat.us.yaml` holds the whole relevant cartridge region as the opaque bin
  `assets/us/data.bin`, starting at ROM `283D0`. The runtime image is compressed
  inside it; there are no original per-object initialized-data sections there.
- `tools/conveyor/pipeline/ovl_targets.py` explicitly builds `ovl_b/symbols.json`
  from the game symbol map plus scanner-discovered **function** names. Extent
  metadata derives from instructions and references. It is not original debug
  information, an original object-file symbol table, or original C declarations.
- None of `803943A4`, `80394884`, `80394888`, or `803948B8` occurs as an address
  in that generated symbols map. The generated game linker script supplies some
  runtime boundary addresses, not these data objects or their sizes/types.
- Merely naming a byte-array view for the loader region would declare new
  ownership. A linked address inside the loader span is not proof of one C
  source object shared with all callers. No verified original union/struct/byte
  allocation has been found.

## Independent consumers make an invented ninth row especially unsound

The observed offset address is `803943A4 + mode*156 + model*12 + component*4`.
For models 0..12, modes 0..7 end exactly at `80394884`; mode 8 spans
`[80394884,80394920)`. Actual independent uses include:
- `80394884`: one full-word source color read at AA8C `8038AAD4`.
- `80394888..803948B8`: four 3-float vertex positions passed to the polygon
  creation service by `8038A750` / `8038A7D0`.
- `803948B8..803948CC`: five separately indexed color words. A408 passes entry
  zero at `8038A7C8`, then entries 1..4 in the loop `8038A850..8038A884`.
- The rest of `[803948CC,80394920)` is beyond those two established views; no
  original ownership for it is inferred here.

Mode-8 model 0 straddles color plus the first two vertex components. Model 4
straddles the last vertex component plus two color entries. Model 5 consists
of three further color entries. Models 6..12 continue beyond that color span.
The receipt gives all 13 address intervals without exporting any data bytes.

Two additional users distinguish real mode 8 from ordinary offset rows:
`func_8038F568` tests signed mode < 8 at `8038F6C8..8038F6D4`; for mode >= 8
it uses the separate per-mode vector table at `80394B08`, bypassing the
model-indexed offset table. FCE0 does the same at `803905C0..80390620`.
This is stronger evidence for an eight-mode semantic table than arithmetic
adjacency alone, but still does not prove its original C declaration.

The existing selector/full-AA8C packets already establish reachable mode-8
native loads and that their values are unused on unchanged mode-8 paths.
Compiler speculation remains a possible explanation, not an established one.
It must be demonstrated from the genuine full root plus real private helpers
under the required unchanged build route if used to justify guarded ordinary C.
One must not simply remove those native accesses, add a fictional union or
ninth float row, or replace the target with a demonstration wrapper.

## Source ancestry examined

The repository's `src/game/battle.c` says it is `NON_MATCHING`, based on similar
vehicle-combat mechanics, and N64-specific. Its sample arrays and four-player
constant are not declarations recovered from AA8C. The battle documentation
based on that scaffold therefore cannot establish native source ownership.

The upstream Rush The Rock files were read directly:
- https://github.com/historicalsource/rushtherock/blob/main/game/visuals.c
- https://github.com/historicalsource/rushtherock/blob/main/game/visuals.h

They establish ancestry for Visual callbacks and attached car effects, but the
header's owner and object-number fields are 32-bit, unlike the observed N64
halfword fields. visuals.c contains no weapon/battle/missile/muzzle entries or
corresponding eight-by-thirteen weapon-offset initializer. Its separate skid,
smoke and car-part objects are not evidence that N64 weapons and neighboring
surface-ring data share one source object. No original N64 donor initializer
was identified. This is a scoped negative result, not a claim that every
possible upstream archive was searched.

## Exact blocked contract and next admissible evidence

A natural-C claim still needs either:
1. Original declarations/object metadata, or independently established actual
   allocation and access semantics, covering the full address interval with
   justified extent, alignment and cross-view alias behavior; or
2. A genuinely valid source-level guard for mode 8 whose full-root compilation
   naturally reproduces the authenticated loads as legal compiler speculation.

The owner requirement can now be stated precisely: establish the bounded
active-count producer and mode-6 configuration setup as dominating every
battle registration, with no later producer reactivating slots 4/5. Do not
substitute a four-owner fixture restriction for that missing dominance proof.
Model-byte producers and scene-pointer alias validity remain separate
obligations already documented by the selector/effect packets.

## Reproduction and limits

    python3 verify.py --reference-root /path/to/repo --output /path/to/fresh.json
    SFRUSH_REFERENCE_ROOT=/path/to/repo python3 -m pytest test_packet.py -q

Only target words loaded through canonical score.targets() are compared against
base-commit images. Ten complete native target identities are recorded, but
only the documented initializer and slices are executed. The verifier requires
no compiler or linker and never invokes IDO. Receipts bind owned notes/verifier,
not mutable manifests, live source, locks, context digests or the test file.
No raw native bytes/disassembly are emitted by the verifier. The local
native_inspect.py aid is not part of the portable deliverable.

This is research with compiler-free native tests. There is no matching source,
strict MATCH, whole-program safety, gameplay, publication or CI-watch claim.
