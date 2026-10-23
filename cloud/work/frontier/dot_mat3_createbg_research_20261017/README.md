# createBG: corrected source and table ownership

Research only, protocol2, empty claims and zero accepted bytes. Native
`particle_lifetime_set` at800B8374 is front.c::createBG. It remains59/183
differing words,732 bytes/frame184. No source-built image or ROM acceptance.

This packet corrects reusable source facts absent from the old near-miss:

- The camera table is a signed-int[4][4] at801145E4. The old shifted801145D4
  declaration pointed16 bytes before it. The original `windows-1` index is
  restored. Rectangle storage is the preceding896-byte[4][4] array.
- Both original tables are file-static. Every emitted byte of both complete
  initializers,960 bytes total, equals the protected native data, including
  all zero tails. The actual native updateBG/FC9F8 writer shares that storage.
- Polygons use the full88-byte record with four20-byte vertices at+8 and
  signed texture coordinates at vertex+12/+14, rather than a fictitious
  20-byte header and one-element trailing array. NewPoly and UpdatePoly
  declarations agree on the actual matrix and color pointer types.
- Original six-local order and the genuine ShowPoly boundary replace the old
  four coordinate temporaries. A meaningful native distance snapshot remains
  disclosed; it is absent from the later platform's DWARF, whose distance is
  a constant. Native scale1025, texture-result store, fixed-point coordinates,
  two-blit count and the updateBG positive-Y-bound quirk remain unchanged.

Three established bodies stay strict in the same group: NewPoly/A78BC252B,
UpdatePoly/8C0741,232B and updateBG/FC9F81,016B. The named8-byte deleted
ShowPoly helper residue is explicitly nonnative and unclaimed. This is not
complete downstream callee closure: texture lookup, blit creation and their
callees remain external. Claims must stay empty.

## Evidence and limits

The complete saved front.c donor is configured Matching, with recorded
individual100% status; this packet did not freshly compile or accept MAT3.
Its source SHA is d88dcefd03fcff890ca045d3bc826db7cf8868f4d989b87a1d9df9139fb95f31.
Original createBG DIE413FC records polyIdx, polyVerts, texIdx, backColor,
backPoly, vertIdx. backColor is automatic/direct fourth child, with no nested
lexical or inline DIE; that cannot determine whether its original declaration
was textually inside the loop. The loop-initializer control was60/183.
Original array DIEs40F16/40EF4 and40F80/40F5E, and local ELF binding, establish
both dimensions and static storage. The only recorded writer is updateBG,
which writes offX/offY. The original updateBG records three locals; its
canonical native shift[2] and mixed zero spellings are inherited reconstruction
choices. No fresh original spelling claim is made for them or ShowPoly.

The inherited volatile model count is unauthenticated. Its ordinary control
is153/183,728 bytes/frame184 with four unverified own-data references; the
full table content still verifies independently. This does not authenticate
volatility. No unused locals, padding, fake caller, forced colors or new
compiled-out devices were introduced. Historical control source is archived
only, including its shifted base and abbreviated record limitations.

Five group builds include two exact repeats, the ordinary-count control, and
wrong rectangle/camera constants. Both wrong-content builds are rejected by
the independent full-table comparison. Seventeen IDO-emitted layout facts
and a wrong-capacity control check dimensions, offsets and record extents.
The historical control is also freshly compiled. Exact function extents,
all comparison fields, complete section hashes, compiler/scorer/target inputs
and every source/proof file are sealed. Guards remain active under python-O.
The observation-only traced and stock whole objects are identical.

This is a bounded source/data/compiler proof, not a runtime behavior fixture
suite. There is no native-execution or general host-C equivalence claim.
Finite arithmetic, allocation success, valid window counts1–4 and four-vertex
polygons are required to execute these reconstructed views safely. Existing
native shift/float-conversion edge cases are not repaired by this packet.

## Replay

`IDO_DIR` selects the existing IDO installation and MIPS tools must be on PATH.
Run `python3 verify.py --repo /path/to/complete/current/master`; repeat with
`python3 -O`. A source-only checkout can use N64_RESEARCH_REPO to locate the
protected artifact/scorer checkout. Focused tests are in
`tests/cloud/test_mat3_createbg_research_20261017.py`. Missing toolchains skip
compiler tests unless REQUIRE_TOOLCHAIN=1. Raw native data, disassembly,
objects and observation logs are private replay outputs, not packet contents.
