# Genuine menu renderer sibling closure: research only

**Zero new matching candidates, accepted bytes, or cartridge coverage.**
This second-wave packet reconstructs the complete real 7180 and 7620 siblings
and retains 7918 as the sole externally callable root. It extends the frozen
02e08c715 wave-one packet, which remains separately reproducible.

The complete canonical stock IDO 5.3 O3 bodies have these exact strict results:

- A:803A6F2C: 596 bytes, 2/149 differing words; native frame320, compiled312
- A:803A7180: 1,184 bytes, 6/296; native168, compiled192
- A:803A7620: 760 bytes, 4/190; native144, compiled128
- A:803A7918: 576 bytes, 7/144; native608, compiled600

All differences are stack-frame sizes or addressed-local home offsets. Every
other instruction, register, branch geometry, ordered call and resolved
relocation agrees. The root improved from 139/144 plus unverified owned-data
sites to 7/144 with full owned-switch-table proof. The four distinct functions
span 3,116 native bytes, all **NONMATCH**. This number is research scope and
must not be counted as match/coverage gain.

## Genuine caller and ABI evidence

7918's native switch directly calls 7620, 7180 and 6F2C. The full source now
contains all those real bodies. It naturally recovers the native O32 root
save/restore set s0-s7/fp/ra. The private renderer helpers clobber their actual
saved-register allocations without preserving them, so they must not be
installed as standalone O32 replacements. No dummy formal, invented caller,
keep barrier, isolated placeholder helper, patched compiler or instruction
substitution is used. No other functions are added to influence allocation.

7180 renders the scrolling saved-record list, selected-record header, block
counts and action instructions. 7620 renders four player-storage slots and
checks the actual two indirect lists before choosing a status message. Native
fields establish two different owner-link offsets (+8 versus +4), an owner
slot byte at +16, 16-byte presence records, and record text starts +18/+53 with
the block count word +68. These are accessed-offset views, not proofs of the
complete original object definitions, string capacities or termination.

Main-image 800B3F50/800B7170/800B71A0 return signed16, supported by native
return normalization and the existing source declarations. These genuine
contracts are reflected here; changing from the initial wider declarations
left every emitted body identical. The main-image functions remain external.
No cross-image inlining or new main-image match is claimed.

## Frame evidence and capacity limits

The actual native buffer address homes are separately established:

- 6F2C: sp+36, sp+52, sp+180; compiled addresses now all agree
- 7180: sp+52, sp+68; compiled homes are both 24 bytes higher
- 7620: sp+80, sp+120; compiled homes are both 12 bytes lower
- 7918: sp+80, sp+344; compiled second-case home is +8, first-case home agrees

C capacities are initial reconstruction hypotheses, frozen after the first
complete sibling closure: 6F2C uses 128/128/16; 7180 uses text100/suffix16;
7620 uses status20/title40; 7918 uses two256. Native address spacing or the
unused end of a frame does **not** prove those exact capacities. They are not
authenticated owned data and establish no unrestricted buffer-safety claim.
No capacities were adjusted, no unused locals added, and no nonexistent
accesses inserted to force a frame match. Ordinary source trials using an
existing coordinate local as a word or short and the confirmed helper return
types did not close the frame gaps; the word-coordinate variant regressed.

Canonical stock UGEN tree/listing diagnostics were run separately, and their
entire ELF object is byte-identical to the ordinary scorer build. Only parsed
metadata is retained, never the raw listing or instruction trees. UGEN reports:

- 6F2C: external0, T16/P4/M288
- 7180: external0, T28/P4/M148
- 7620: external0, T16/P4/M100
- 7918: external1, T28/P4/M514

The metadata binds the observed pre-codegen frame inputs. It does not identify
an original missing local, justify padding, or prove a universal eight-byte
compiler effect. Authentic eliminated-expression or source-declaration evidence
would be needed to reopen the unresolved frame question productively.

## Complete independent relocation and owned data

The verifier authenticates the protected A manifest, compressed asset, loader
pointer and image hash. All four STT_FUNC sizes equal their full native extents.
Fresh GNU as/ld independently relocates complete re-sectioned compiled bodies,
including the three real local calls and the five switch entries. This is an
extracted-body link, not original contiguous translation-unit placement proof.

The only owned data is the five-entry, 20-byte switch table at 803B9834. GNU
relocates each table entry through its complete function owner and proves the
exact native table bytes. Both root code references to that table are compared
as complete GNU-resolved words, with no final masking. The scorer independently
reports every owned reference verified. Twelve zero alignment bytes in the
compiled rodata section are explicitly excluded from owned-data credit.
All other storage, text and formatting strings remain external address bindings.

Every residual offset and both body hashes are recorded. Unknown ELF ownership,
relocations, reference sites, table size, extra code, image identity, metadata
or diagnostic object fidelity fail closed. The source passes C89 syntax checks.
No native/gameplay execution, text-length bounds proof, hardware, full-image
splice or ROM hash is claimed. No arcade checkout is available for ancestry
research. No binaries, ROM bytes, raw assembly or credentials are committed.

## Reproduction and timing

```
python3 cloud/work/frontier/dot_runtime_a_menu_renderer_closure_20261011/verify.py
python3 -O cloud/work/frontier/dot_runtime_a_menu_renderer_closure_20261011/verify.py
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/conveyor/test_runtime_a_menu_renderer_closure.py -q
```

UTC on 2026-10-11: sibling work began14:57; first complete four-function closure
at14:58; stock diagnostic whole-object fidelity and frame metadata at15:03;
independent four-body GNU and 20-byte switch proof at15:06. These are work-phase
observations, not compiler performance measurements. Publication and independent
review are coordinated separately; the checker retains all merging decisions.
