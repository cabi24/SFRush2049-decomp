# C13 controller reset and copy pair

Central claim `96b9dd19` reallocates this lane to two fresh C13/BT04 bodies:
`80020820` (484 B) and `80020DA8` (420 B). Branch
`dot/boot-tail-c13-controller-pair` reuses the existing isolated repository and
explicitly stacks on frozen peer-approved BT05 core head
`9af97ad3c5e1968ccf5d3b7b7ec060fbb9d2f3c6`. Only this owned packet changes.

Both are **COMPLETE-NONMATCH** reconstructions, totaling904 B. Zero matching
credit and no new matching submission are claimed.

## Native source and ABI

`20820` has two genuine byte inputs, channel and set. Set255 chooses an external
134-byte channel row at `D_80055000`; other sets select a row in
`D_80050D00[][16][134]`. Native address arithmetic independently establishes
134-byte rows and2,144-byte set strides. These are the same external declarations
already audited for the C13 `20610` setter; no table values or storage definition
is invented here. The native body clears all134 bytes before any helper call.
It then sends ten explicit four-byte-input calls to `20610`, in order, using
controllers7/10/128/129/64/65/91/131/132/133 and their actual defaults. The final
`20F4C` and `20FDC` calls each take three real bytes, ending with255 and0.
Both helper entries independently mask all three inputs and select their real
regular/external byte tables. The constructor `24988` supplies a registered
external slot and set255, confirming the reset interface at a real caller.

`20DA8` has a real byte controller plus two actual packed voice pointers. Both
identifier words are loaded from+96 and their low bytes select source and
destination134-byte external rows. The five calls in reviewed `1B744` pass both
pointers unchanged with controllers7/10/91/128/132. A controller below64 copies
the selected low-five-bit entry and its+32 partner. Controllers128/129 and132/133
select the even base and copy two adjacent bytes. Other valid controller entries
copy one byte. The selected row/entry pointers represent those actual accesses,
not new ABI parameters. Self-copy is valid and preserves the read/store order.
Neither source nor destination state is modified.

Valid registered channel/set rows and controller entries0..133 are preconditions.
The byte formals alone do not prove arbitrary malformed indices safe; no bounds
check or invented fallback is added. Host tests allocate eight regular sets of
sixteen channels and32 external rows to exercise this registered domain. The
native strides, addresses and observed callers are source evidence; synthetic
helper tests do not prove unrelated storage ownership or full application safety.

## Bounded diagnosis

Every source uses natural C89 with O2 first and O1 as an explicit control. The
unchanged workbench diagnosed initial and retained sources; only hashes and
classification counts appear in `diagnosis_summary.json`. Native listings and
compiler objects remain temporary.

- `20820` is10/121 on its first form, with exact484-byte ELF function extent.
  Only the two compiler-unrolled byte-clear loops differ: native delay slots
  rotate the first store after the pointer advance. Pointer-cursor clearing
  gives109/121 with460-byte extent; postindex, exact-count termination and signed
  counter forms all remain10/121. A naturally selected flat-row indexed view,
  considered because it solved a genuine earlier initializer layout issue,
  gives110/121 with460-byte extent. Original source retained; six forms, stop.
- `20DA8` begins100/105 plus9 nonzero excess words,460-byte ELF extent.
  Selecting actual buffer-entry pointers reduces the extent to432 B and excess
  words to2, but remains100/105. Byte row indices yield104/105 and408 B;
  separately decoding identifier masks reproduces the432-byte result. Retain
  the real selected-buffer form; four forms, stop. No ABI declaration sweep is
  used to force the remaining original-argument/local allocation pattern.

| Body | Native size | O2 residual / ELF size | O1 residual / ELF size |
|---|---:|---:|---:|
|20820|484|10/121;484 B|120/121;448 B|
|20DA8|420|100/105 +2 excess;432 B|104/105 +27 excess;528 B|

All four final rows and twenty initial/directed rows have exact source hashes,
strict scores and ELF function-symbol sizes. Body extent is recorded separately
from zero section-alignment words. Any future accepted source must have exact
native function-symbol size as well as full relocated equality. No row here is
accepted. All retained relocations resolve with no unverified fields or errors.

No fake formal, keeper, padding local, assembly, volatile trick, target boundary
change or flag sweep was used. Earlier frozen sources remain untouched.

## Tests and verification

Two actual-source strict-C89 ASan/UBSan groups pass. Reset tests cover320 regular
and external channel/pattern combinations, check all134 bytes are cleared before
the first call, enforce the twelve-call sequence, preserve a deliberate helper
mutation and compare all untouched rows. Copy tests cover274,432 valid command,
source/destination row and byte-pattern combinations, plus the same-state-pointer
case. All controller widths, whole-state immutability and same-row copies are
checked. Synthetic helpers express external contracts, not original algorithms.
LeakSanitizer alone is disabled under ptrace; no heap is allocated.

Run `verify.py`, `verify_controls.py`, and unittest discovery for `test_*.py`.
Use the existing pinned compiler through `IDO_DIR`. Fresh preflight validates
all439 immutable target extents/99,120 B, protected manifests and the preexisting
getter. All161 static locks and whitespace pass. Shared ledgers/D10, prior packet
sources, targets/scorer/compiler/layout/symbol/lock/spec inputs, runtime image,
farm, production gates, accepted800D1248 and restricted helper work are unchanged.
No ROM, native dump, object, credential or unrelated private data is included.
Paired actual-source/ABI review precedes central integration and exact-head CI;
merging remains with the user's independent checker.

Independent paired review PASS binds immutable source commit
`8321aac9987bb298bcec3acfd65886b149cd3001`, tree
`2528ce516fd311202013e58c451a5adf30a8c226`. All four final and twenty archived
rows, exact source hashes and ELF symbol sizes were independently reproduced.
Both sanitizer groups pass. Full native/source inspection plus the three actual
callees confirms both genuine ABIs,134-/2144-byte strides, twelve-call reset
order and sequential byte-copy semantics. `independent_review.json` records
904 B of complete nonmatching research, with zero matching credit.
