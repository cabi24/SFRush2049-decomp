# Image B: E114 parent-context reconstruction, first milestone

**Research only. Three complete semantic child sources, one partial genuine
parent region, and a corrected minimum closure. No matching-byte claim.**

Base: `dea99f09ab19b1d3b324ed7097162f7b378e7096` (2026-10-06).
Native image B SHA-256:
`b55fc2d1b22eb1ebdf01286a69a181b496da7b45ff7aec888ff74b7db748e7cd`.
This packet is frozen for independent source/test review before publication.

## Main finding

E114 is a complete 5,196-byte private-IPA parent, not an ordinary exported
function. Reconstructing it alongside D200/D328/E088 alone cannot supply the
genuine original calling context. The actual outer caller FCE0 is 3,056 bytes;
E114 also directly depends on private DA78 (868) and D498 (768). The minimum
known context is therefore **10,448 native function bytes**, with original TU
boundaries still unproven. No dummy roots, pressure locals or extra arguments
were introduced.

The three requested children are now complete natural C semantic units in
`children.c`. Each input has direct native consumption and real E114 caller
evidence. Their source argument order and original declarations remain
provisional. They are compiled only as an ordinary-ABI semantic smoke baseline;
this is not an attempted substitute for the real private calling convention.

`parent_regions.md` maps the entire E114 extent and starts genuine parent C
at its +0x1228..+0x142C region. It explicitly lists missing source. There is no
compilable E114 placeholder or partial function masquerading as a full body.

## Gates and ownership

PROJECT_PLAN sections 5/6 apply. Boot-tail spec 015/T050 explicitly excludes
ovl_a/ovl_b, so it neither grants runtime-image permission nor imposes its
boot-tail cluster gate here. The coordinator acknowledged the disjoint
runtime-parent research claim. Current master targets/scorer were checked;
the shared working checkout was not reset or modified.

Open exact-address PR searches found no E114/D200/D328/E088 claim. The broader
open runtime PR inventory and prior lives-text private-caller scout were also
read. Ordinary-B packets #142/#144/#147/#149/#151/#153/#156 are excluded.
FCE0/DA78/D498/DDDC/CB20 are read-only dependencies at this checkpoint; only
the three named child definitions and parent partial text were authored.
No accepted source, header, lock, target, flags, scorer or production gate changed.

The source ancestry check found no local original arcade tree or whole-function
donor for these N64 battle-object routines. The repository identifies battle
mode as N64-specific; that is context, not proof of the original author's code.
These sources are reconstructed from authenticated native behavior. Existing
accepted math_utility/PitchUV/pool source supplies helper semantics without
being copied into the candidate TU.

## Corrected contracts

- D200 `[8038D200,8038D328)`, 296 bytes: actual record in s1 and mode in a0;
  write object position, optionally copy its 3x3 matrix, scale the last row by
  0.2/0.4 under kind/flag conditions, and optionally PitchUV by 0.18. The object
  pointer is captured before the matrix-copy call; flags/kind are read after it.
- D328 `[8038D328,8038D3A4)`, 124 bytes: actual inputs are resource index,
  parent index, creation flags, and transform omission in s1/s2/s3/s4. Allocate
  a 60-byte object, create the scene instance, store its returned index and
  return the object. Real parent calls include resource index 4 and flag 0x2000,
  both omitted from the earlier scout summary.
- E088 `[8038E088,8038E114)`, 140 bytes: kind 3 destroys/frees secondary then
  primary; kinds 0/1/2/4/6/7/8 release primary; kind 5 or unsigned kind >=9 is
  a no-op. Handles narrow to signed 16 bits. Primary is loaded after secondary
  callbacks. Slots are not cleared and there are no invented null guards.

Important accepted-source discrepancy: native wrapper 8008E398 forwards
8008E26C's sign-extended scene index in v0, and D328 consumes it. The current
accepted dot_sign_extend group declares the wrapper void. This packet uses an
address-spelled, return-bearing external declaration; it does not edit that
accepted group. The independent checker must reconcile the shared contract
before eventual integration.

The pool initializer at B:908D0 establishes object stride 60/count 24 and 17
contiguous resource words in two loops (10 then 7). E114 consumes resource
indices 1/2/3/4/5/7/8/9. The 104-byte record layout includes a genuine effect
pointer at +0x5C before primary +0x60 and secondary +0x64.

## Verification at this checkpoint

Run from a normal current checkout with pinned IDO and MIPS binutils:

    python3 cloud/work/runtime_b_e114_parent_20261006/verify.py

For a source-only overlay, add `--reference-root /path/to/repository`; target
files remain read-only and the pinned source asset is read in memory through
Git. The current tools must be available in the overlay. No raw native inputs,
objects, disassembly, ROM bytes or image extracts are publication artifacts.

- Authenticated full extents and per-body hashes; 59 E114 call sites, 25 targets.
- 12 native-layout facts verified from IDO data emission, including 104/60-byte
  structures and all consumed field offsets.
- Complete ordinary-O2 ELF bodies: D200 304 bytes, D328 156, E088 140. Complete
  object and every allocated section are inspected. The independent GNU link
  binds all nine external symbols and all local data, with no unresolved
  relocation or hidden allocated section. Research addresses intentionally
  differ from native placement; no equality claim follows from that link.
- **33,024 paired native/GNU-source fixtures (66,048 executions)** agree on
  entire mapped nonstack source state, defined return values and ordered
  helper arguments. Every native instruction (74/31/35 words) and every
  candidate instruction (76/39/35 words) executes. External helpers are
  bounded, side-effecting contract hooks with caller-save poisoning.
- **791,296 C89 host fixtures** pass ASan, UBSan and bounds instrumentation.
  They cover all raw kind/flag bytes, copy modes, signed handle narrowing,
  resource inputs, helper mutation and primary rereads. Leak detection is
  disabled because the sandbox's ptrace environment is incompatible with
  LeakSanitizer; the harness performs no dynamic allocation.
- Four compiled wrong-source mutations are rejected. Native null-allocation
  and unsupported-instruction controls fail closed.

The ordinary-ABI smoke comparisons are deliberately NONMATCH: D200 71/74
scorer differences plus one excess nonzero word, D328 31/31 plus seven, E088
28/35. D200's six and E088's two local-data relocation sites remain unverified
in that native-shape comparison. These counts are not strict matches or an
allocator-search baseline. The independently linked behavior proof resolves
the complete candidate's own constants/table at its research placement.
No O1/O3 sweep, source-order permutation or matching tune was attempted.

`contract_audit.json` is the separate read-only ABI audit, not a review of
the subsequently written candidate or tests. `verification.json` is the
source-bound producer receipt. Independent candidate review is still required.

## Domains and stopping condition

The proven domain has initialized resource slots 0..16, aligned/disjoint
104-byte records and 60-byte objects, successful allocation, live nonnull
object slots where used, and finite binary32 values. Synthetic successful
hooks cover all resource entries but do not prove real asset availability.
Original helper bodies, FCSR/signaling NaNs, arbitrary aliases, concurrent
mutation, cyclic lists, invalid lifetime/ownership and gameplay are outside
these child tests. The actual allocation routine can return null; the native
child then dereferences it, so no graceful-failure claim is made.

Full parent/FCE0 behavior, original TU visibility, exact private ABI source
ordering, source-image composition, recompression and full-ROM gates were not
run. The next useful work is complete real DA78/D498/E114/FCE0 source and
contract closure, followed by one justified whole-context baseline. Until
then, this packet remains PARTIAL-SOURCE parent / COMPLETE-NONMATCH children.
