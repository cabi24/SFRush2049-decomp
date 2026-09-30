# Registering complete heads in the opaque game-image runs

Baseline: 482/1,165 game functions from C; 54,596/647,072 bytes (8.4374%).
The game lock has 482 entries; the separate static lock has 26 entries. Both were snapshotted before work.

## Method

`closure heads` derives opaque runs from gate-passed coordinator extents. It independently sweeps negative, aligned stack-frame prologues, pulls verified short hoisted prefixes back with `targets.stranded_head`, and collects raw `jal` and aligned pointer references. It does not import the cloud detector or its head list. Pointer-only leads must follow a return and its delay slot (optional zero padding); switch-case targets following internal jumps are not head boundaries. A prologue after a word that cannot decode as an instruction can start a new code island.

`targets.scan_head_extent` follows both direct branch arms, calls, internal direct jumps, return paths, and delay slots, including branch-likely annulment. It requires consistent stack depth and a balanced stack at every reachable return. Indirect jump destinations are not guessed. The next independently discovered head or the opaque run end (a known function boundary) bounds the proof. Every word in an accepted extent must decode as an instruction; direct branches must remain inside and preceding branches may not enter the proposed function. Final padding is excluded.

Dry run: `python3 -m tools.conveyor.pipeline.closure heads`. It writes no target changes or report by default. `--report PATH` writes metadata; `--apply` inserts all accepted heads atomically through the existing closure registration path, with `func_XXXXXXXX` names, extracted population, discovered provenance, raw target objects, and unmatched status. A second apply registers zero. New objects are subsequently upgraded with `targets.relocate_extracted(names=...)`; existing targets and their scoring evidence are not regenerated.

Independent discovery produces exactly the same 52 addresses as `cloud/work/unregistered-heads-full.md`. All 45 accepted sizes agree with the audit. The remaining seven cloud extents are unproved under this conservative gate; they are refused rather than copied. Accepted heads total 38,408 bytes.

## Decisions

Sizes are proved bytes, not the audit's proposed bounds. “Unproved” means no size is registered. Evidence addresses are image virtual addresses.

| Address | Size (bytes) | Decision | Evidence |
|---|---:|---|---|
| `0x8010221C` | unproved | refused | jal: 0x80102F90; prologue: 0x8010221C; indirect jump at 0x80102400 |
| `0x80102448` | 1336 | accepted | jal: 0x80102FC4; prologue: 0x80102450; 1 reachable return(s), delay slots included; bound 0x80102980 |
| `0x80102980` | 1456 | accepted | jal: 0x80102FD4; prologue: 0x80102988; 1 reachable return(s), delay slots included; bound 0x80102F30 |
| `0x80102F30` | unproved | refused | pointer: 0x8011471C; prologue: 0x80102F30; indirect jump at 0x80102FBC |
| `0x80103D28` | 2516 | accepted | jal: 0x801054EC; prologue: 0x80103D28; 1 reachable return(s), delay slots included; bound 0x80104704 |
| `0x80104704` | unproved | refused | jal: 0x80105730; prologue: 0x80104704; escaping branch at 0x80104A48; target 0x80104A58 |
| `0x80104B14` | unproved | refused | jal: 0x80105760; prologue: 0x80104B14; indirect jump at 0x80105160 |
| `0x80105480` | unproved | refused | pointer: 0x801148C4; prologue: 0x80105480; indirect jump at 0x801054E4 |
| `0x80105B74` | 564 | accepted | pointer: 0x801158F8; prologue: 0x80105B74; 1 reachable return(s), delay slots included; bound 0x80105DA8 |
| `0x80105DA8` | 256 | accepted | pointer: 0x80115940, 0x80115964 (+2 more); prologue: 0x80105DB0; 1 reachable return(s), delay slots included; bound 0x80105EA8 |
| `0x80105EA8` | 2508 | accepted | pointer: 0x8011591C; prologue: 0x80105EA8; 1 reachable return(s), delay slots included; bound 0x80106874 |
| `0x80106874` | 712 | accepted | pointer: 0x801159D0; prologue: 0x80106874; 1 reachable return(s), delay slots included; bound 0x80106B3C |
| `0x80106B3C` | 600 | accepted | pointer: 0x801159F4; prologue: 0x80106B3C; 1 reachable return(s), delay slots included; bound 0x80106D94 |
| `0x80106D94` | 1604 | accepted | pointer: 0x80114E6C, 0x80114E90 (+14 more); prologue: 0x80106D94; 1 reachable return(s), delay slots included; bound 0x801073D8 |
| `0x80107EDC` | 632 | accepted | pointer: 0x80115184; prologue: 0x80107EDC; 1 reachable return(s), delay slots included; bound 0x80108154 |
| `0x80108154` | 896 | accepted | pointer: 0x801151A8, 0x801151CC (+2 more); prologue: 0x80108154; 1 reachable return(s), delay slots included; bound 0x801084D4 |
| `0x801084D4` | 1500 | accepted | pointer: 0x80115238; prologue: 0x801084D4; 1 reachable return(s), delay slots included; bound 0x80108AB0 |
| `0x80108AB0` | 760 | accepted | pointer: 0x8011525C; prologue: 0x80108AB0; 1 reachable return(s), delay slots included; bound 0x80108DA8 |
| `0x80108DA8` | 408 | accepted | pointer: 0x80115280, 0x801153C4 (+2 more); prologue: 0x80108DA8; 1 reachable return(s), delay slots included; bound 0x80108F40 |
| `0x80108F40` | 1320 | accepted | pointer: 0x801152A4, 0x801152C8 (+30 more); prologue: 0x80108F40; 1 reachable return(s), delay slots included; bound 0x80109468 |
| `0x80109468` | 1528 | accepted | pointer: 0x80115790; prologue: 0x80109468; 1 reachable return(s), delay slots included; bound 0x80109A60 |
| `0x80109A60` | 1268 | accepted | pointer: 0x801157D8, 0x801157FC (+6 more); prologue: 0x80109A60; 1 reachable return(s), delay slots included; bound 0x80109F54 |
| `0x80109F54` | 1504 | accepted | pointer: 0x801157B4; prologue: 0x80109F5C; 1 reachable return(s), delay slots included; bound 0x8010A53C |
| `0x8010A53C` | 616 | accepted | pointer: 0x80115A60; prologue: 0x8010A53C; 1 reachable return(s), delay slots included; bound 0x8010A7A4 |
| `0x8010B5D0` | 556 | accepted | pointer: 0x801172C4; prologue: 0x8010B5D0; 1 reachable return(s), delay slots included; bound 0x8010B7FC |
| `0x8010B7FC` | 460 | accepted | pointer: 0x801172E8; prologue: 0x8010B7FC; 1 reachable return(s), delay slots included; bound 0x8010B9C8 |
| `0x8010B9C8` | 700 | accepted | pointer: 0x8011730C; prologue: 0x8010B9C8; 1 reachable return(s), delay slots included; bound 0x8010BC84 |
| `0x8010BC84` | 928 | accepted | pointer: 0x80117330; prologue: 0x8010BC84; 1 reachable return(s), delay slots included; bound 0x8010C02C |
| `0x8010C02C` | 696 | accepted | pointer: 0x80117528; prologue: 0x8010C02C; 1 reachable return(s), delay slots included; bound 0x8010C2E4 |
| `0x8010C2E4` | 356 | accepted | pointer: 0x8011752C; 2 reachable return(s), delay slots included; bound 0x8010C448 |
| `0x8010C448` | 320 | accepted | pointer: 0x8011751C; prologue: 0x8010C450; 1 reachable return(s), delay slots included; bound 0x8010C588 |
| `0x8010C588` | 320 | accepted | pointer: 0x80117520; prologue: 0x8010C590; 1 reachable return(s), delay slots included; bound 0x8010C6C8 |
| `0x8010C7CC` | 24 | accepted | pointer: 0x80117524; 1 reachable return(s), delay slots included; bound 0x8010C7F4 |
| `0x8010C7F4` | 384 | accepted | pointer: 0x80118BB8; prologue: 0x8010C7F4; 1 reachable return(s), delay slots included; bound 0x8010C974 |
| `0x8010C974` | 2636 | accepted | pointer: 0x80118BBC; prologue: 0x8010C974; 1 reachable return(s), delay slots included; bound 0x8010D3C0 |
| `0x8010D3C0` | unproved | refused | pointer: 0x80118858, 0x80118888 (+10 more); prologue: 0x8010D3C0; indirect jump at 0x8010D498 |
| `0x8010D680` | unproved | refused | pointer: 0x8011885C, 0x8011888C (+10 more); prologue: 0x8010D680; indirect jump at 0x8010D76C |
| `0x8010D85C` | 368 | accepted | pointer: 0x8011789C; prologue: 0x8010D85C; 1 reachable return(s), delay slots included; bound 0x8010D9CC |
| `0x8010D9CC` | 300 | accepted | pointer: 0x801177A8; prologue: 0x8010D9CC; 1 reachable return(s), delay slots included; bound 0x8010DAF8 |
| `0x8010DAF8` | 192 | accepted | pointer: 0x80117928; prologue: 0x8010DAF8; 1 reachable return(s), delay slots included; bound 0x8010DBB8 |
| `0x8010DBB8` | 324 | accepted | pointer: 0x801180D8; prologue: 0x8010DBB8; 1 reachable return(s), delay slots included; bound 0x8010DCFC |
| `0x8010DCFC` | 660 | accepted | pointer: 0x80117808, 0x80117958 (+2 more); prologue: 0x8010DCFC; 1 reachable return(s), delay slots included; bound 0x8010DF90 |
| `0x8010DF90` | 364 | accepted | pointer: 0x80117538, 0x80117568; prologue: 0x8010DF90; 1 reachable return(s), delay slots included; bound 0x8010E0FC |
| `0x8010E0FC` | 1000 | accepted | pointer: 0x8011753C, 0x8011756C; prologue: 0x8010E0FC; 1 reachable return(s), delay slots included; bound 0x8010E4E4 |
| `0x8010E4E4` | 432 | accepted | pointer: 0x8011780C, 0x8011795C (+2 more); prologue: 0x8010E4EC; 1 reachable return(s), delay slots included; bound 0x8010E694 |
| `0x8010E694` | 152 | accepted | pointer: 0x8011873C, 0x80118ACC (+1 more); prologue: 0x8010E69C; 1 reachable return(s), delay slots included; bound 0x8010E72C |
| `0x8010E72C` | 252 | accepted | pointer: 0x80117778, 0x80117838 (+8 more); prologue: 0x8010E72C; 1 reachable return(s), delay slots included; bound 0x8010E828 |
| `0x8010E828` | 132 | accepted | pointer: 0x8011777C, 0x8011783C (+8 more); prologue: 0x8010E828; 1 reachable return(s), delay slots included; bound 0x8010E8B4 |
| `0x8010E8B4` | 340 | accepted | pointer: 0x8011ACBC, 0x8011AD04; prologue: 0x8010E8B4; 1 reachable return(s), delay slots included; bound 0x8010EA08 |
| `0x8010EA08` | 2056 | accepted | jal: 0x8010F810; prologue: 0x8010EA14; 1 reachable return(s), delay slots included; bound 0x8010F218 |
| `0x8010F218` | 2444 | accepted | pointer: 0x8011ACE0, 0x8011AD28; prologue: 0x8010F218; 1 reachable return(s), delay slots included; bound 0x8010FBB4 |
| `0x8010FD60` | 28 | accepted | pointer: 0x8011EAC0; 1 reachable return(s), delay slots included; bound 0x801249F0 |

The six `indirect_jump` refusals need an independently proved switch-table destination set before every return and branch can be accounted for. `0x80104704` branches to the existing `highscore_entry_anim` start at `0x80104A58`; claiming its full audit extent would absorb that known target. This pass leaves the suffix and all existing targets intact. No requirement/code contradiction was found: refusing an unproved bound is the instructed outcome.

## Verification and registration

Registration completed in one atomic pass: 45 new targets. An immediate post-registration dry run reports no accepted heads and zero registrations. The new layout has 1,210 functions and 551,536 identified function bytes (previously 1,165 and 513,128). Game C coverage is 482/1,210 functions and 54,596/647,072 bytes (8.4374%); the numerator is unchanged. At the registration checkpoint, both lock files were byte-identical to their snapshots; later concurrent changes are recorded below. The game and group checks report zero problems, and the static check confirms all 26 locked functions intact. The required full pytest command selected 1,193 tests (744 passed, 449 skipped; 3 deselected) and exits 0 (captured separately in `/tmp/register-heads-final-pytest.exit`, before printing the log). Context regeneration completed and the repeated prototype header was byte-identical. The builder ROM command printed `SHA-1 EXACT`, `MAKE=0 TEST=0`, and exited 0. Forty new targets upgraded to relocation-aware objects, with zero evidence rows purged; five retained their complete raw-word objects because the existing conversion failed its length gate: `func_80102448`, `func_80102980`, `func_80109468`, `func_80109A60`, and `func_80109F54`. All 45 targets were verified unmatched with stored objects. The cloud scorer independently sees all 1,210 targets and all 45 registered heads. The full population histogram artifact is complete and its context hash matches the current generated context: 1,607 extracted records, including 1,210 usable function targets and 397 extent-conflict records. Its command wrapper exited 143 after the completed artifact was written; that exit is not reported as a successful command exit. The synthetic suite covers fall-through, two returns, data in code, alternating code/data, hoisted prefixes, incoming fall-through, known boundaries, indirect jumps, escaping branches, stack balance, missing delay slots, likely-delay annulment, switch labels, decoder rejection, dry-run/idempotent unmatched registration, and atomic rollback on assembly failure.

## Concurrent checkout changes observed at final checks

After the successful ROM gate, final checks detected eight concurrent splices: `Effects_UpdateEmitters`, `func_8008B3C8`, `func_800A5488`, `func_800AB750`, `func_800CFDEC`, `func_800E8CB8`, `func_800FBE60`, and `input_init_flag_get`. None is a registered head from this lane. No original lock entry was removed. The `func_800EA3F4` entry changed its group source hash as its group definition gained the two new members. The lock now has 490 entries; game coverage is 490/1,210 functions and 55,652/647,072 bytes (8.60%). This increase is concurrent promotion work, not head-registration coverage.

The prompt's unchanged-lock exception only covers head-related splits or renames, so this difference has been reported to the coordinating user rather than reverted or silently adopted. Final baseline acceptance and commit of shared generated outputs are pending that clarification. The head detector, tests, and registration did not write the game lock or group definition.
