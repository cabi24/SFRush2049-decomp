# BT05 macro-five: native command and variable operations

**Five complete NONMATCH reconstructions, 888 native bytes; zero matching bodies
and zero verified-body bytes.** This packet preserves the whole source and its
compiler residuals. Independent review and aggregate CI are separate gates.
There is no cartridge-coverage or promotion claim.

- Base: `301d9e7552ad4fd7f54a38796db84671e1000d35` (master supplied by coordinator).
- Branch: `dot/boot-tail-bt05-macro-five`.
- Exclusive central acknowledgement: `b24a49f6`, before any source edits.
- Exact targets: `80022324+172`, `800230B0+224`, `80023AD4+124`,
  `80023B50+140`, and `80023DB8+228` (hex addresses, decimal sizes).
- Owner `/root/match_boot_tail_macro_five`; reciprocal reviewer
  `/root/match_boot_tail_low_eight`.
- Only this packet directory changes. Central status and D10 stay separately
  owned; `status_delta.csv` proposes five honest nonmatch rows.

## Native reconstruction and ABI

Names such as `pan` and `keyGroup` are structural/behavioral hypotheses, not
recovered original API names. No arcade or public third-party body was copied.
Ground truth is the protected whole-function target set, with inventory extents
cross-checked at 439 starts and 99,120 bytes. This packet needs no external source
release or local rodata to reconstruct its five bodies.

All ordinary calls were audited at the actual native entry and caller. There are
no invented formals, fake callee bodies, keeper locals, padding locals, assembly
blocks, merged boundaries or private helper-context experiments in scored C.
The genuine aligned command is two words, with stride eight. Packed voice fields
are a partial state view, not a claim that every name/field type is original.
The object stride 416 and all used offsets are independently checked with IDO.
Unknown byte arrays represent actual unrecovered object intervals.

- `22324` receives voice and command pointers and returns a byte zero. It forms
  `(((command bits8..15) + voice unsigned halfword+0x4E) << 8) |
  ((command high halfword) << 16)`, then checks `composite | index` against packed
  IDs at `D_8004BEB8[index]+0x60`. Matching IDs call `21700` with that actual
  composite ID. The live count `D_8004FA18` is reloaded after calls, as in native
  code; it is not snapshotted across the loop. Native `21700` consumes one u32
  identifier, validates it, updates flags and returns int. Its independently
  matched source in the coordinator's C13-tail packet agrees with this contract.
- `230B0` receives the same two pointers and returns byte zero. It sets flag
  0x10000, extracts the command high halfword into a genuine time local, stores
  it at voice+0xA8 and calls `1E930` on that word. The existing matched callee
  multiplies the word by 256. It stores command bits8..15 shifted by16 at +0x38,
  adds the *signed* low byte of command word1 shifted by16 into target +0xAC,
  and sets delta +0x3C by **unsigned** division by the unscaled time when scaled
  time is nonzero, or by the full shifted delta otherwise. Unsigned arithmetic
  explicitly preserves negative-offset wraparound. A high-halfword time ranges
  from 0 to65535, so scaling cannot turn a nonzero time into zero. No new divide
  guard, clamp, arithmetic-right-shift assumption or narrower denominator is added.
- `23AD4` genuinely takes voice, u8 controller selector and u8 index, returning
  signed16. With nonzero selector it calls `215A8(voice,index)`, then converts its
  actual unsigned16 return to signed16. Otherwise it masks index to five bits
  and reads a signed16 local variable at voice+0x180 for indices0..15 or a global
  variable for indices16..31. The global bank is modeled as
  `D_8004BE98[16]` with index-16; its addresses are exactly the native
  `0x8004BE78 + 2*index`, without inventing unused array slots overlapping nearby
  global storage. Indexed local-array decay produces aligned halfword accesses
  even with the surrounding packed structure; this was checked in emitted IDO.
- `23B50` genuinely takes the same three inputs plus signed16 value. Controller
  selection forwards the actual signed16 value to `2165C`; otherwise it writes
  the same local/global variable banks. Its native callers ignore any result.
  Native `2165C` clamps a signed16 argument and forwards unsigned16 to its setter;
  its reconstructed source in C13-tail agrees with the caller ABI. The wrapper
  does not assume the content of `21548`'s still-unproved translation table.
- `23DB8` receives voice, command and a genuine byte comparison selector. It
  obtains two signed16 values through `23AD4`, selects equality for mode0 or
  signed less-than for mode1, optionally inverts using command word1 bits8..15,
  and on success points voice+4 into its program base at voice+0 using the high
  command halfword times eight. It returns byte zero.

### Comparison and pointer domain limitations

The two native call sites in dispatcher `23E9C` are at offsets0x980 and0x9A0;
its delay slots explicitly supply mode0 and mode1, respectively. For other modes,
`23DB8` reads an uninitialized stack byte in native code. The source deliberately
retains the uninitialized local on that unreachable-by-known-callers path, rather
than inventing a default result. Tests cover only the proven selector domain;
this is not a claim of defined behavior for arbitrary external callers. Likewise,
program offset arithmetic requires a valid macro-program object and in-object
command offset. Malformed bytecode safety is not established.

The sources are ordinary individual-function reconstructions. Declared-only
callees are compiled separately. Native parameter and return types are audited
from their instructions, not inferred from source-family names or a jal scan.
No transitive table/body equality is claimed for the nonmatching C13 helpers.

## Compiler results and bounded stop

Initial sources were scored O2, then O1. Before any directed refinements the
unchanged workbench diagnosed temporary canonical-target and fully relocated
candidate objects. `diagnosis.json` contains metadata only; raw native words,
objects and full disassembly reports stay outside the repository.

| Function | Final O2 differing/target words | O2 excess | Final O1 differing words | O1 excess |
|---|---:|---:|---:|---:|
| 22324 | 2/43 | 0 | 43/43 | 10 |
| 230B0 | 43/56 | 0 | 56/56 | 11 |
| 23AD4 | 25/31 | 0 | 31/31 | 7 |
| 23B50 | 33/35 | 0 | 35/35 | 1 |
| 23DB8 | 49/57 | 2 | 56/57 | 10 |

Every final O2 result has zero unresolved symbols, unverified relocations or
scoring errors, and every body remains NONMATCH. In the longer O1 `23AD4`
negative control, comparison limited to the native extent also reports an
unpaired high relocation for the global bank. This error is preserved in the
receipt. It is not suppressed, a rodata proof claim, or a reason to change the
scorer; the O1 object is already structurally longer and mismatching.

The two-word `22324` residual is an extra temporary use during ID composition,
not missing behavior. Splitting the real ID calculation, combining it into one
expression, using an explicit group carrier, multiplication spelling, and mask
spelling did not establish equality. Other handlers exhibit narrow-argument
lowering and register/schedule differences; `230B0` also has a 32-byte candidate
frame versus the native40-byte frame. Real register storage hints, equivalent
SDK word-sized integer types, explicit word carriers, old-style C89 definitions,
and natural signed/unsigned result carriers were bounded negative controls.
No fake arguments or extra locals were added merely to enlarge the stack.
The allocator/frame hypotheses are frozen rather than swept beyond the bound.

`variants/` preserves the initial sources and every directed source control.
`verification.json` freshly binds all34 complete sources (five final plus29
controls) to68 O2/O1 results with source hashes and full relocated-word checks.
The largest per-function source count is ten, below the twenty-variant bound.
The older per-hypothesis JSON files are numerical experiment summaries; the
reproducible all-source verification is authoritative. The best complete
semantically faithful forms are retained under `nonmatch/`, with no submissions
under `cloud/matches/`.

Next useful input is authentic original N64 macro translation-unit/compiler
context or a focused, independently justified narrowing/allocation trace.
Repeating casts, storage hints, index assignment spelling or known negative
word-size variants is not a fresh hypothesis. `23754` and the blocked `21548`
table work were not assigned or investigated.

## Verification and reproduction

With the approved pinned IDO toolchain in `IDO_DIR`:

```sh
python3 cloud/work/boot_tail/BT05-macro-five/verify.py
python3 cloud/work/boot_tail/BT05-macro-five/test_layout.py
python3 cloud/work/boot_tail/BT05-macro-five/test_packet.py -v
```

Before editing, compiler hashes were checked against the approved packet2
receipt, protected SHA manifests and all439 extents passed, and the original
12-byte getter strictly replayed. `preflight.json` retains the hash metadata.
`layout_verification.json` binds each of the five sources to IDO compile-time
checks of pointer size, command size, 416-byte voice size and ten field offsets.

Five host test groups pass: live count changes and ID selection; zero/nonzero
scaled time with positive/negative panning offsets and unsigned division; all256
local/global indices with signed16 boundaries; equality/less-than/inversion over
signed16 edge cases; and source-receipt binding. Test-only callee implementations
supply audited external contracts and are never part of scored C. Host pointer
width and byte order differ; these tests are behavioral checks, not N64 execution,
full-body equality, or a claim about malformed bytecode. A separate 32-bit C89
syntax check and the actual IDO assertions establish the modeled native layout.

No target, symbol, scorer, spec, lock, layout, shared type, runtime image, farm
state or production gate changed. The restricted `800D1248`/helper experiment
remained untouched. No ROM, raw assembly dump, object, credential or unrelated
private data is included.
