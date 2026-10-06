# E114: genuine parent reconstruction checkpoint

Status: PARTIAL-SOURCE. This file maps the entire native extent and preserves
one genuine parent source region. It is not a compilable replacement body,
matching submission, recovered original TU, or parent behavior proof.

Image B `[0x8038E114,0x8038F560)` is exactly 5,196 bytes / 1,299 words.
`verification.json` binds that complete body to the authenticated B image and
enumerates all 59 direct call sites to 25 distinct destinations.

## Full-extent region map

Offsets below are relative to E114; boundaries denote semantic reading units,
not new functions. None may be compiled as an artificial native helper.

- `+0000..+0044`: load active record head `8039A530`, retain word `80394390`,
  allocate 0x120-byte frame, save return address, initialize floating constants.
- `+0044..+055C`: capture the next record before helper effects. Special handling
  when signed kind is 3 and timer is zero: derive player/physics records from
  signed owner, update position and matrix, process player state, create up to
  two real scene objects, or release the record. D328 receives genuine resource
  index 4 here, along with parent -1, flags 0x2000/0x2080, and a true transform
  mode. This region still needs complete source.
- `+055C..+05BC`: decrement lifetime and advance timer using `8002EB94`.
  Expiration calls E088 on the real record, then frees it through the real
  pool at `8039A520`, and advances using the saved next pointer.
- `+05BC..+093C`: save the prior position in a real three-float local at frame
  +0xF4. Dispatch by unsigned kind using authenticated table `80394E20`.
  Kind-specific velocity/position updates set genuine effect/collision values.
  Kind 4 calls conventional helper DDDC. This region is not yet complete C.
- `+093C..+097C`: if record+0x5C points to an effect, set effect flag 0x10 at +8
  and copy record position into effect +0x10/+0x14/+0x18.
- `+097C..+0A44`: kind 7 goes to the distinct D498 path. Other kinds call private
  DA78 with the actual record in s1 and a genuine previous-position pointer in
  s2: record+0x2C for kinds 4/6, otherwise the local frame+0xF4. Native callers
  spill live values across this private boundary; do not pretend DA78 is O32.
- `+0A44..+1104`: process collision/helper outcomes and invoke game/image
  services, including D798 and D3A4. Complete behavior, field lifetime and
  external-effect ordering still require reconstruction. No placeholder C was
  supplied for this region.
- `+1104..+1140`: separate kind-7 path calls private D498 with record in s2 and
  explicit caller-side preservation. Its true body must participate in any
  relevant IPA closure.
- `+1140..+1228`: inspect object/timer state and update model scale components.
  The full field/data contract is still pending.
- `+1228..+13D8`: update prior position and dispatch object creation/transform
  by kind; source fragment below. Table is `80394E68`, nine entries.
- `+13D8..+142C`: advance flag phase 0x10 -> 0x20 -> 0x40 -> clear, preserving
  the other bits and the exact priority of the three tests.
- `+142C..+144C`: use saved next record, loop, and return. The final f22 store
  is a lazy scratch spill, not evidence for an invented argument.

## Real locals and calling context

`record` is the live s5 record, with its next pointer saved at frame+0x114.
`previous_position[3]` is the actual frame+0xF4/+0xF8/+0xFC local. The record
is 104 bytes, its primary/secondary object fields are +0x60/+0x64, and each
object is a 60-byte pool allocation. `children.c` preserves these layouts.
The transform at record+0x38 is a real 3x3 float matrix; +0x5C is an effect
pointer, not an extra matrix element or artificial frame padding.

The load of f22 from its own frame+0xA8 at entry is not a formal parameter.
Kind-switch paths assign it before the six arithmetic uses. Kind 7 skips
assignment but bypasses those arithmetic uses; it only spills/reloads scratch.
The exact liveness evidence and remaining assumptions are in contract_audit.json.

E114 itself clobbers unsaved s0-s8 and saved-register FP lanes, and saves only
ra. Its sole visible B caller is FCE0+0xB98 at `80390878`. FCE0 preserves the
full s0-s8/f20-f30 set. Keeping E114 as a fake exported O32 root, inventing
arguments, or adding a tiny caller would not reconstruct the original context.

## Partial C: +1228 through +142C

This is an in-place region of the existing E114 loop. `record` and
`previous_position` are the real locals above. It is deliberately not wrapped
in a fabricated function, and has not been submitted to a compiler as E114.

```c
if (!(record->flags & 0x70)) {
    record->previous_position[0] = previous_position[0];
    record->previous_position[1] = previous_position[1];
    record->previous_position[2] = previous_position[2];
}
switch ((u8) record->kind) {
case 0:
    if (!record->primary)
        record->primary = func_8038D328(2, -1, 0, 0);
    func_8038D200(record, 0);
    break;
case 2:
    if (!record->primary)
        record->primary = func_8038D328(3, -1, 0, 0);
    func_8038D200(record, 0);
    break;
case 3:
    func_8038D200(record, 2);
    break;
case 4:
    if (!record->primary)
        record->primary = func_8038D328(7, -1, 0, 0);
    func_8038D200(record, 0);
    break;
case 6:
    if (!record->primary)
        record->primary = func_8038D328(8, -1, 0, 0);
    func_8038D200(record, 0);
    break;
case 7:
    if (!record->primary) {
        record->primary = func_8038D328(9, -1, 0x800000, 0);
        func_8038D200(record, 1);
    }
    break;
case 1:
case 8:
    if (!record->primary)
        record->primary = func_8038D328(1, -1, 0, 0);
    func_8038D200(record, 0);
    break;
}
if (record->flags & 0x10) {
    record->flags &= ~0x10;
    record->flags |= 0x20;
} else if (record->flags & 0x20) {
    record->flags &= ~0x20;
    record->flags |= 0x40;
} else if (record->flags & 0x40) {
    record->flags &= ~0x40;
}
```

The argument order in these source-level private calls is provisional. Each
argument is genuinely consumed; only its original C order/TU visibility is
unknown. The emitted-register contract is recorded separately in the audit.

## Admission and next source unit

Minimum known related bodies are E114 (5,196), FCE0 (3,056), D200/D328/E088
(560 combined), DA78 (868), and D498 (768): 10,448 native function bytes.
This is a lower bound on relevant context, not an assertion of one original TU.
DDDC is an ordinary O32 boundary and must not be pulled into a group just to
change allocation. Other FCE0 dependencies, including CB20, need an actual
visibility/liveness audit before any whole-TU recipe is selected.

Next milestone: reconstruct complete native DA78 and D498 as actual units,
enumerate their mutation/return contracts, and write all remaining E114
regions. Then reconstruct the real FCE0 caller, establish export/visibility
evidence, and only then attempt one source-bound O3 baseline. Any unresolved
data/ABI or origin-of-value question stays a named blocker. No matching
experiment is authorized by this partial-source checkpoint alone.
