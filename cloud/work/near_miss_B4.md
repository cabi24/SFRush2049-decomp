# Worker B4 — structural packet (2026-10-01)

No new strict matches. Preserved improved full TUs under `near_miss_B4/`, clearly
NONMATCHs. No src/asm/lock/state changes, commits, splices, or ROM gates performed.
Current targets were exported read-only from coordinator DB; each named target
has its own authoritative .text section in current trusted layout. Rocky B
private scratch codex_B, serial compiles (one compute process).

Source origins: physics_collision_test best permuter via cloud_worklist/expanded
shim; other four refreshed near-miss/base.c. Read actual target assembly and old
hand notes before selecting new probes. Flags O2 -g0 -mips2 -G0 -non_shared except
explicit flag controls below; scorer adds r4300_mul. Diagnose's masked counts
are not accepted as strict results.

| Target | Strict original | Best strict | New probes | Result |
|---|---:|---:|---:|---|
| physics_collision_test | 14/60 | 5/60 | 38 | NONMATCH, real seed errors fixed |
| camera_update_a | 14/32 | 12/32 | 25 incl 1 compile failure | NONMATCH, known pool swap |
| func_800D11BC | 7/35 | 7/35 | 15 | NONMATCH, known schedule residue |
| func_800DCCE0 | 19/28 | 12/28 | 2 | NONMATCH, IPA unsaved-s0 prerequisite |
| entity_state_check | 25/30 | 10/30 | 24 | NONMATCH, pointer/web residue |

## physics_collision_test

The permuter seed has two genuine structural/semantic mistakes:
1. Its zero loop increments the pointer BEFORE storing, while target stores old
   element, increments, then compares. Target raw store is -4(v0) after scheduling;
   normalized synthetic disasm misleadingly names a data relocation there. Fixed
   C to store before increment so zeroing includes the actual first element.
2. D_80140BDC is declared word but target loads unsigned BYTE. Corrected to u8.

Best initial shape is pointer field first, then100, then88; all other initial
field operations unchanged. Correct types/loop + initializer order gives5/60.
`near_miss_B4/physics_collision_test_best.c` captures this semantic correction.
Remaining words: +0x70/+0x78 exchange lui t0 with addiu s1; +0xA4/+0xA8/+0xB8
rotate first call-argument load versus fifth-argument constant/store. Equal code,
no remaining wrong values or opcode classes in these sites.

Probes: natural physical lines17, six initializer orders12-19; semantic fixes12;
six initializer orders on corrected original layout best5; statement placements
5-8, named signed-byte argument5 versus s32 named argument23+1; four correct typed
callee prototypes5; pool dead reads6-17; volatile count/call/init line probes5;
four first-argument signedness/view casts5; O3 control5. Stopped below40.
Workbench alignment/lanes helped split errors from later scheduling; audited raw
strict diff was essential to see the off-by-one store and byte/word mistake.

## camera_update_a

Known node/counter saved-register swap. Target counter in s0 and node in s1;
seed opposite, plus delayed argument-copy scheduling. Natural rewrite modifies
arg1 directly rather than pass-through var_s1:14 ->12, consistent with hand notes.
Ten dead-read position probes14 orworse, named counter14; void/K&R callback14;
three dead reads with direct carrier12; counter types u32/s16/u16 all12;
initialized local + dead-read probes all12. True typed linked-list field source12.
One initial typed source accidentally altered earlier unrelated prototypes and
failed compilation; corrected to alter only target definition and it compiles12.
No new improvement beyond previously known12. Did not claim allocator impossibility.

## func_800D11BC

Strict baseline7/35 differs substantially from masked DB-object diagnostic19/36:
the project scorer adds VR4300 multiply errata scheduling and uses true layout
extent. Source has correct values/frame/registers; old hand notes already cover
store/volatile permutations. New bounded probes: drop named zero and restore
physical statement lines (7); three dead reads7; debug g1/g2/g3 controls33;
three-element clearing loop27+3; chained assignments in both directions7;
three byte-literal cast forms7, nonconstant boolean spelling26+1. No match.

## func_800DCCE0

Target clobbers s0 without saving it across resource_type_select, matching prior
hand-note IPA evidence. Ordinary single seed instead homes pointer and unused
arg0 in a40-byte frame; target24. Code-free read of unused formal removes its
home and improves19 ->12. Inline pass-through value remains12. Saved source
`near_miss_B4/func_800DCCE0_group_lead.c` is a group seed lead only.
No point chasing ordinary ABI register colors until real caller/callee closure
reproduces s0 usage; no stand-ins or group claims introduced.

## entity_state_check

Explicit cached base pointer, typed first address and integer-cast second address
recover missing duplicated address shape and improve25 ->12. Code-free record
pointer read gives10, saved in `near_miss_B4/entity_state_check_best.c`.
Remaining ten sites involve stride in a2 versus target t6, later byte/load temp
phase and address result t8 versus target v0. This is not a strict close match.

Four base/cast shapes12-21; pointer/argument dead reads best10, others12; pointer
extern types12; inline stride/cast variants10-21; typed24-byte record13; stride
u32 inert12, u16/s16 worse27; index narrowing worse27+1. No strict zero, no claims.
Tool identifies downstream allocation and missing address, but does not identify
which source web population would keep duplicated addresses in the target lanes.

All detailed variant sources/probe scripts remain Rocky private scratch;
no generic autopilot/permuter rerun was used. No image/ROM gates apply to these
NONMATCH sources. Root acceptance of the prior B3 genuine task group is separate.
