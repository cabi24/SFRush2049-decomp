# BT05 note, sweep and curve helpers

**One strict O2 match, 124 bytes / 31 words; four complete NONMATCHs, 748 bytes.**
Independent source/ABI review and exact aggregate-head CI precede publication
credit. No cartridge coverage or promotion is claimed.

- Clean master base `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Branch `dot/boot-tail-bt05-variables`, central activation `a6f8cbed`.
- Exact five fresh extents: `800225FC+124`, `8002279C+216`, `80022F24+232`,
  `8002300C+164`, `8002321C+136`, totaling 872 B.
- Earlier BT05 packets and other workers' helper implementations stay frozen.
  Only this packet and matching `func_800225FC.c` change. Central ledger files
  remain with their sole writer.

## Whole native behavior and genuine contracts

`225FC` stores the command's seven-bit note at packed state+0x50 and detune byte
at +0xC0. If `21764(state)` reports that this is the current voice, it forwards
channel +0x4A, set +0x4B and the note's low byte to the genuine three-byte-input
`20F4C`. It then mutates command word0 to4 and forwards state/command to `2193C`.
The native dispatcher at `23E9C+0x58C` supplies both pointers and consumes the
low-byte status; `23564+0x190` also supplies both. The complete body, all calls,
frame and return match. No fake caller or callee definition is supplied.

`2279C` adds the command's signed byte to last-note +0xC1, stores the unsigned
halfword, then uses its signed interpretation for the lower clamp and unsigned
value for the upper clamp127. It stores detune, updates the channel's last note
unless the channel is255, rewrites word0 to4 and forwards to `2193C`. The sum is
in [-128,382], so signed16 interpretation and clamping are bounded. Its native
command reload immediately before the final call remains unmatched.

`22F24` looks up an envelope through `16E68(u16)`. On success, it reads four
packed halfwords and swaps each byte pair into a genuine address-taken local
8-byte envelope passed to `148F8(word_identifier_low_byte, pointer)`. It then
sets packed state flag0x200. The real two-argument `148F8` reads all eight bytes;
its own floating arithmetic does not create a float parameter or local literal
in this caller. No third argument from a newer source release is invented.
The local object has four meaningful u16 fields, with no frame-padding member.

`2300C` genuinely has a third integer sweep index: native dispatcher calls at
`23E9C+0x600/+0x620` pass0/1. It zeros aligned word array +0x40[index], sets byte
arrays +0x48/+0xB0, extracts a signed16 delta, converts its absolute magnitude
through `14D30(u32)`, restores the sign, stores the low word shifted16 at aligned
+0x7C[index], zeros command word0, and returns `2193C(state,command)`. The helper
returns a word despite using floating arithmetic internally. Its magnitude
argument is bounded0..32768. The signed local's conversion from a returned u32
uses the N64 two's-complement integer conversion convention; negative result
formation uses unsigned subtraction and the final left shift is unsigned.
Thus this reconstruction does not introduce signed-shift/negation overflow.
The documented two-element arrays assume the observed dispatcher index0/1.

`2321C` takes a genuine u32 fixed-point volume and u16 curve identifier. Curve0
or a null lookup returns the input. Otherwise it interpolates adjacent table
bytes below integer index127 and uses the single byte at index127. Difference
multiplication/addition is unsigned low-word arithmetic. Both canonical callers,
`232A4+0xE4` and `233B0+0xE8`, cap volume at0x7F0000 before this call; therefore
all observed indices stay0..127 and ordinary 128-byte curve tables suffice.
Arbitrary out-of-domain inputs or malformed lookup data are not asserted safe.

The packed prefixes model only genuine observed object offsets; unknown arrays
are object storage gaps, not local padding. The sweep arrays are naturally
aligned as their native accesses require. Layouts concern the 32-bit N64 ABI,
not wider-host pointer assumptions. Every called symbol is available in the
canonical population, with no boundary-blocked body needed for these callers.

## Source-family evidence and bounded controls

The pinned CC0 [MusyX synthmacros source](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/src/musyx/runtime/synthmacros.c)
provides analogous SetKey, LastKey, SetADSR, PitchSweep and TranslateVolume
operations. [synthdata.h](https://github.com/AxioDL/musyx/blob/78d2e16e4905fc675952162d331c24d5198b2687/include/musyx/synthdata.h)
also declares the curve lookup's u16 identifier. `references.json` records exact
revision/blob/license provenance. This is source-family context, not an
authenticated N64 release. Native byte sweep counters, zero curve sentinel,
0x200 flag and two-argument envelope helper are retained instead of copying
newer version-specific layouts, DLS branches or calls.

All seeds were scored at O2 first, then O1. Before refinement the unchanged
workbench diagnosed fresh relocated candidates against canonical target
objects held only in temporary storage.

- `225FC`: the explicit `note & 0xFF` expression kept a full packed-halfword
  reconstruction. Passing the note directly to the existing u8 formal lets
  ordinary argument conversion select the correct byte read, closing the body.
- `2279C`: all code before the final command write/call agrees; the candidate
  omits one native reload. Word-sized status, a genuine named return local and
  a raw two-word command representation do not change the seven-word residual.
- `22F24`: narrow-identifier/word-mask lowering consumes temporary registers
  differently, shifting subsequent byte-swap registers. A real u16 identifier
  local worsens the result; raw command words do not improve it.
- `2300C`: using one meaningful signed delta local instead of a separate result
  closes the frame from40 to32 bytes and improves35 to22 differing words. The
  remaining narrow conversion/temporary allocation and branch-delay slot do
  not move with explicit widening, a separate assignment, unsigned low-word
  storage, raw command words or a C89 declaration-form control.
- `2321C`: signed difference, explicitly unsigned shifts and a C89 formal
  declaration control preserve the20-word allocation/lowering residual.

No function exceeded eight natural source forms. The unchanged residuals were
frozen; no fake formals, keepers, volatile barriers, padding locals, assembly or
flag sweep was used. `initial_controls.json` and `directed_controls.json` retain
hash-bound numerical outcomes; research controls are not extra matches.

| Function | Final O2 differing/target words | Final O1 differing/target words | O1 extras |
|---|---:|---:|---:|
| `225FC` |0/31|30/31|8|
| `2279C` |7/54|54/54|17|
| `22F24` |35/58|57/58|8|
| `2300C` |22/41|39/41|12|
| `2321C` |20/34|32/34|8|

All final O2 rows have zero extra nonzero words, unresolved symbols, unverified
relocations and errors. Only `225FC` is a match. The other four remain archived
under `nonmatch/`, with no matching submission or verified-byte credit.

## Replay and limits

Run `python3 cloud/work/boot_tail/BT05-variables/verify.py` from this checkout.
`verification.json` binds ten complete controls to final source hashes.
`preflight.json` records all protected manifest checks, all439 starts/sizes
(99,120 B), the preexisting getter and unchanged compiler/source hashes.
All161 static locks and whitespace checks pass. The changed-submission gate is
run against the committed head; the central integrator owns aggregate CI.

No target, symbol, lock, layout, scorer, compiler, runtime image, farm, spec or
production gate is changed. Accepted `800D1248` and restricted helper work remain
untouched. No ROM bytes, raw disassembly, object or credentials are published.

The paired reviewer independently reproduced all ten controls and final source
hashes at `fd745adddb14acb7ec9b049ce5329bc683df17f1`, tree
`88f2aa04440df51d72df399ce8f71ff3e2caf9bf`. All five full source/native bodies,
actual helper entries, both volume clamps, dispatcher index0/1 and the eight-byte
envelope contract passed review. `independent_review.json` binds the one strict
124-byte match and four honest nonmatches to that immutable source head.
