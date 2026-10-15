# Coordinator replay and acceptance gates

**Everything in the compile/integration sections is a future command, not a
reported local result.** Run in a new authorized private N64 checkout. Refresh
master, read current claims and rebase/review changed inputs first. C910 remains
an Astra-owned handoff; coordinate its scoring with that owner. Keep claims empty
until a complete protocol-v2 result exists. Do not run any of this in a canonical
GC or N64 checkout. The independent checker owns integration and merging.

## Compiler and prerequisites

All fourteen targets are extracted **IDO 5.3** game/runtime-image code. Initial
flags: `-g0 -O3 -mips2 -G 0 -non_shared`. The strict group scorer applies
`as1 -r4300_mul`; single-function diagnostics must retain the equivalent erratum
option where needed. Try O2 only as a separately recorded diagnostic after the
structurally correct baseline. Per-source headers and genuine unit context take
precedence over guessed flags. No static boot/library TU is rebuilt by this
packet. If future verification rebuilds one, obtain its compiler through
`tools.conveyor.compilers.tu_cc(tu, default_cc)` and `rom_compilers.json` (Rush
boot tail generally 7.1, libultra 5.3), never substitute the game compiler.

Official IDO 5.3 v1.2 Linux archive SHA-256:
`ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506`.
Record the actual identified executable/component fingerprints, flags and outputs
at replay time. These were unavailable here. Cloud strict scoring needs a full
current checkout, complete manifest-listed target streams/symbols and executable
IDO tools; it does not need a ROM. `blob_unit` additionally needs Unix `fcntl`,
the private `build/blob_layout.json`, its `image.path`, full locked-unit staging
and authorized coordinator/builder access. No denial workaround is authorized.

```sh
H=cloud/work/frontier/gc_informed_n64_unscored_20261015
S=.scratch/gc_n64_20261015
git status --short
git rev-parse HEAD
bash tools/cloud/setup.sh
python3 -m tools.conveyor.compilers identify tools/cloud/ido
python3 tools/cloud/score.py fn src/blob/sound_handles_clear.c sound_handles_clear
python3 tools/cloud/score.py group src/blob/groups/resource_slot_clear
# Both setup controls must print MATCH before candidate scoring.
python3 -B "$H/verify.py"
python3 -B -O "$H/verify.py"
python3 -B "$H/materialize.py" --repo . --out "$S"
```

`materialize.py` refuses an existing output directory and verifies genuine locked
blast source hashes before creating it. Re-run into another new private scratch
directory for a cold replay. It writes only scratch C/metadata, performs no
compiler operation and does not install or supersede a group. Full context copies
come from the checkout; their large declarations are not duplicated in this PR.

## Source/native checks runnable independently

```sh
python3 -B "$H/verify.py"
python3 -B -O "$H/verify.py"
# Reads protected native targets from the private checkout, not from this packet:
python3 -B "$H/c910/verify_native.py" --repo .
python3 -B -O "$H/c910/verify_native.py" --repo .
```

The main check pins packet sources and its own verifier, audits eight route
configurations, authentic helper/type identity, empty claims and internal/keep
roles, 200 fixed-width checkpoint offsets, 385 valid-domain successor model
cases and N64 visual/blast/defense layouts. It rejects source mutation, false
claim insertion and accidental keeping of the internal blast callee.

The C910 replay retains the earlier fail-closed interpreter, not an independent
compiler/relocation implementation. It authenticates complete selected bodies
against population manifests and input-bound body hashes. It exercises native
C910 only: 1,024 fixtures, 69 instructions, contract hooks for scene helpers,
caller-save poisoning, callee-save checks and nonstack-memory comparisons.
Wrong creation flag and unknown instruction controls must fail. It does not
execute the candidate C, genuine helper internals, unrestricted alias/lifetime
behavior or gameplay. Hashes pin source and selected target bodies, not mutable
shared tools, locks, Makefile or assembly text.

## Compile/score matrix

Use **two independent cold runs** per baseline/candidate, with the same source
hashes and fresh distinct unit tags. Save full stdout/stderr, actual compiler
identity, emitted object/section/function extents, relocations and literal notes
privately. A research NONMATCH is a result; setup failure is not a candidate
score. No `--claims`, `--allow-unverified`, offset masks or waived extra words.

| Target / population | First source and required context | Literal / ABI / preservation gate |
|---|---|---|
| entity_iterate / game | original seed then named-pointer candidate; real input_deadzone_apply and locked frontier_vector_normalize unit | Inline .5=`3F000000`, .25=`3E800000`; 56 words, frame88; six arguments and always-zero return; preserve normalization and every locked body. |
| audio_mixer_main / game | all eight materialized route groups; real audio_priority_find internal | 183+108 words; successor type/order axes, authentic helper/type identity; compare whole unit with priority helper internal and main kept. Provisional helper is not new credit. |
| 90308, entity_physics_update, save_write_data / game | materialized baseline then no_array groups; four genuine locked contexts | 282/178/300 words; internal90308, two genuine kept callers; timer `3D088880`, .005=`3BA3D70A`; validate own-data identity and initializer stub/ABI. |
| camera_look_at_point / game | current src/blob/groups/camera_aspect_ratio, full eight locked members | 199 words; N64 IDs1/18/97, integer abs/fallback then ratio; replay all eight old matches. |
| B9740 / game | current w14r best.c first, then a separately reviewed typed/indexed proposal | 102 words; s16 min/max ±32767, no dropped vertex assignment, preserve alias-visible reloads; whole loader unit after leaf diagnostic. |
| audio_effect_apply / game | w17h/groups/physics_response with real caller/callee context | 174 words; native endian/no-write loops and exact call order; func_800B930C verbatim above; provisional physics_response920 B stays excluded. |
| camera_play_script / game | current w18d full source, original real unit/context | Full880 words, frame608; -.707=`BF34FDF4`; resolve own-data failure before colour diagnosis. 27 aligned rows is not strict zero. |
| input_deadzone_apply / game | w12a best.c + hdr_ipcorder.h and original provisional cylinder context, then real whole unit | 895 words, frame400; preserve 35-entry surface list and all calls; input_process_controller provisional source is not a locked caller proof. |
| C910 / B | existing #364 baseline first, then owner-approved new candidate and prior-candidate control | `--targets asm/us/ovl_b`, 69 words, 16 historical RELs, no own-data substitution; authentic callback, seven-word creation contract, AF5E0 return/prototype reconciliation. |
| AA14, A95C, AA8C / B | native-led future source, real callback + both helpers together; no new C supplied | 30/46/1953 words; private s4/unsaved-s ABI, signed operation, 0x10C/0x148 strides; verify actual N64 table values rather than GC payloads. |

Concrete first commands:

```sh
python3 -m tools.conveyor.pipeline.blob_unit --tag gc14_entity_seed --jobs 2 score entity_iterate --with "$S/entity/original-seed.c" --neighbours
python3 -m tools.conveyor.pipeline.blob_unit --tag gc14_entity_candidate --jobs 2 score entity_iterate --with "$S/entity/candidate.c" --neighbours

# Strict group diagnostics include BOTH route members even though claims is empty.
for v in baseline_w6b distance_boundary_control signed_above signed_below unsigned_above unsigned_below negative_successor_plus2 negative_kept_helper; do
  python3 tools/cloud/score.py group "$S/routes/$v"
done
# Candidate failures in the loop are expected research outputs. Inspect each log.
python3 -m tools.conveyor.pipeline.blob_unit --tag gc14_routes_unsigned --jobs 2 score audio_mixer_main audio_priority_find --internal audio_priority_find --keep audio_mixer_main --with "$S/routes/unsigned_above/group.c" --neighbours

python3 tools/cloud/score.py group "$S/blast/baseline"
python3 tools/cloud/score.py group "$S/blast/no_array"
python3 -m tools.conveyor.pipeline.blob_unit --tag gc14_blast_baseline --jobs 2 score func_80090308 entity_physics_update save_write_data --with "$S/blast/baseline/effects.c" --with "$S/blast/baseline/start_blast.c" --internal func_80090308 --neighbours
python3 -m tools.conveyor.pipeline.blob_unit --tag gc14_blast_no_array --jobs 2 score func_80090308 entity_physics_update save_write_data --with "$S/blast/no_array/effects.c" --with "$S/blast/no_array/start_blast.c" --internal func_80090308 --neighbours

# Owner-coordinated C910 baseline and experiment; both remain research.
python3 cloud/work/frontier/dot_runtime_b_callback_init_20261012/verify.py --score
python3 cloud/work/frontier/dot_runtime_b_callback_init_20261012/verify.py --deep
python3 tools/cloud/score.py fn "$S/c910/candidate.c" func_8038C910 --targets asm/us/ovl_b --flags "-g0 -O3 -mips2 -G 0 -non_shared"
python3 tools/cloud/score.py fn "$S/c910/prior-candidate.c" func_8038C910 --targets asm/us/ovl_b --flags "-g0 -O3 -mips2 -G 0 -non_shared"

# Existing linked drafts, diagnostics only; no new source is claimed for them.
python3 tools/cloud/score.py fn cloud/work/frontier/w14r/func_800B9740/best.c func_800B9740 --flags "-g0 -O3 -mips2 -G 0 -non_shared"
python3 tools/cloud/score.py group src/blob/groups/camera_aspect_ratio
python3 tools/cloud/score.py group cloud/work/frontier/w17h/groups/physics_response
python3 -m tools.conveyor.pipeline.blob_unit --tag gc14_world_collision --jobs 2 score input_deadzone_apply --with cloud/work/frontier/w12a/ida/best.c --neighbours
python3 -m tools.conveyor.pipeline.blob_unit --tag gc14_body_surface --jobs 2 score camera_play_script --with cloud/work/frontier/w18d/camera_play_script/best.c --neighbours
```

The last collision unit commands depend on the approved existing unit overrides
and the historical header/provisional context. Confirm them in the actual
checkout; do not infer that standalone source overrides recreate an IPA group.
For every route variant, also repeat the shown unit command with that variant's
source and a distinct tag; for `negative_kept_helper`, intentionally keep the
helper to test ABI failure. Neither negative variant is installable.

## Diagnosis, protocol v2 and acceptance

On a near match, obtain authentic fresh native/candidate objects and run:

```sh
python3 tools/workbench.py diagnose TARGET.o CANDIDATE.o --function FUNCTION --objdump mips-linux-gnu-objdump
```

Then use the documented forced-colour oracle, ctrace/pretrace/ugen/as1 traces with
stock-versus-traced fidelity. An oracle result is diagnostic, never a candidate
match. Trace one motivated lever at a time; do not run a broad syntax sweep.

Before any ready/acceptance claim, the coordinator must satisfy all of these:

1. Fresh identified stock compiler, manifest-authenticated complete native extent,
   correct population, actual source/context hashes and reproducible cold result.
   No truncation at an apparent suffix or borrowed neighbor. All relocations,
   symbol bindings, owned literals/jump tables/statics, zero suffix and extra
   words are checked without masks or uncertainty waivers.
2. For an IPA group, protocol-v2 `members` includes every defined body that is or
   becomes locked (including implemented stubs); genuine real callers/callees,
   `context`, `keep` and device disclosures are complete. Strict `score.py group`
   without `--claims` proves every proposed member. Superseding an installed
   group retains its name and every old body byte-identically. No stand-in earns
   installable credit. Populate claims only in a separately reviewed match packet.
3. Run all locked-neighbor/unit preservation checks, `make check-matched`, runtime
   overlay preservation and applicable repository tests. A source hash alone
   does not prove compiled nonregression. All baseline and negative controls must
   behave as expected; missing compilers/targets fail or use the shared test skip
   policy, never a fabricated successful score.
4. The independent coordinator/checker runs game `blob_splice`→`blob_rom`, or B
   `ovl_splice`/`ovl_layout build`→`ovl_rom`, linked-image byte identity, owned-data
   placement, compression and full-ROM SHA-1 gates through the current documented
   workflow. No original compressed stream substitution, lockfile edits or
   production promotion is authorized by this research PR.

Rollback is source-only: discard the newly created scratch groups and their
private diagnostics, preserve current locks/unit overrides/production sources,
and retain historical baseline receipts. If a candidate changes any locked body,
native contract or protected input, reject that candidate and investigate before
another experiment. Leave this PR draft; do not merge it automatically.
