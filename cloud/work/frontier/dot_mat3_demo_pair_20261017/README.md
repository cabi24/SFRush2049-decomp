# Demo/attract pair: static-state source evidence

**Research only. Claims are empty; new accepted bytes are zero.**

The native helper `debug_stats` at FCDF8 is playDemoGame: a strict 532-byte match,
including every data relocation. Its genuine caller `debug_collision` at FD024
is updateDemoGame and remains 3/133 differing words, also 532 bytes. Their frames
are respectively24 and48 bytes. The three caller differences are at offsets
0xcc,0x11c,0x19c: native initializes a saved register, while the reconstruction
stores the extra actual argument in an unused argument slot. No register forcing,
extra formal, unused local or padding is added to repair those words.

All25 bodies from the independently reviewed PlayGame packet are unchanged and
strict exact, as are four adjacent locked8-byte stubs. The complete group has
31 scored functions: these29 preserved bodies and both new research functions.
The unmatched caller prevents accepted credit for the helper.

## Genuine source evidence and limits

Canonical later `demo.c` is configured Matching. Original debug ELF/DWARF
confirms updateDemoGame8000E814/180B, playDemoGame8000E9E8/216B,
endDemoGame8000EAC0/192B, initDemoGame8000E8C8/80B and
uninitDemoGame8000E918/76B. Play has one formal `int show` and no automatic
locals; end has exactly `signed short i`. Init/uninit/update have no automatic
locals. The native caller's meaningful `x` local preserves N64-only state-edge
behavior and is not claimed as original later-DWARF source.

The saved source calls playDemoGame with two actual arguments through an
unprototyped declaration but defines only one formal. This packet preserves
that legacy IDO input literally and discloses it. It is compiler-specific source
evidence, not a portable ISO C arity contract. The behavior proof executes the
compiled instructions; it does not assert portable C execution of that call.
The old reconstruction's invented second formal is excluded. Reinstating that
old formal does not fix the three caller words. A separately preserved fully
prototyped one-actual-argument control worsens the caller to81/133, while its
helper remains exact; it is not a match or an alternative accepted source.

Original signed-byte inited/preplayDone/gDemoInited and signed-short track state
are authenticated. The decisive change is restoring the function-static
**initialized** gDemoInited flag. The other three file-static declarations are
not required for the matching gain. Returning that flag to an extern declaration
restores117 helper word differences. A static flag without explicit initialization
retains the instruction shape but produces unsupported .bss references: the
native flag lives in initialized data. The .data layout and all five complete
four-byte storage cells are checked against native data before behavioral use.

The original file-static initialized blit pointer and compatible opaque Blit
return/removal contracts are also restored; this source correction is byte-neutral.

The real init, uninit, end, next-track, environment and state-setter boundaries
are restored, with native-specific track wrap, input-edge and four-player loop
behavior retained. RemoveEnvirons has accepted later private proof56B/3relocs,
no locals; it reloads and removes the environment-list head before InitSkids.
The named native deleted-helper correspondences FCDF0=end, FD00C=uninit,
FD014=next-track and FD01C=init are inferred from source call boundaries and the
exact adjacent native/compiler layout. Their existing8-byte targets are locked;
there is no claim of an independently authenticated historical N64 symbol name.

Later demo source SHA256:
951fa46bcec23ef007fd9735c2f5462d4c57ab5ff6f96a14fa0f3f0ff5f1d204.
Original debug ELF SHA256:
d4a06e4516ab9a75e6de780beee10ea07b8358016e4964a8baf9e8eaa282908c.
These are source/identity receipts, not a fresh later-platform function score.

## Independent bounded behavior

A fixture-field/event model is independently expressed from the compiled control
flow and pointer loads. Each of768 cases runs both native and candidate caller
and helper instructions, for1536 executions. All133 caller instructions and
all132 reachable helper instructions execute, with60 branch outcomes. Helper
word+0xac is a dead duplicate branch-likely prefix; a conservative CFG separately
checks reachability rather than counting it as executed.

The bounded domain includes signed active-car counts-1..4, four allocated model
records2056B with signed-halfword slot+1990, four complete player76B records,
0..3 valid environment nodes, zero/one state flags, demo tracks-1,0,3,4,5,
all eight low input-edge combinations, both reset/PrePlay/timer outcomes, and
both special race-state bits. Four-player writes and all memory accesses are
bounds/alignment checked. Saved GPRs/SP/FPRs are checked after the complete pair.

All other gameplay calls use deterministic event/argument/caller-clobber stubs.
ReleaseEnviron additionally advances the fixture list head. The model predicts
that list transition independently. These stubs do not validate actual rendering,
car setup, timing, audio, allocator, controller, thread or gameplay side effects.
No arbitrary-alias, invalid-pointer, concurrent-state or full-runtime claim is made.

The candidate relocator computes addresses from named functions and five checked
static-data slots; it never fills machine instructions from target words.
Three separately compiled negatives request the wrong next state, pass the wrong
PrePlay mask, or leave the teardown flag set. Each fails the field/event model.
Thirteen IDO-emitted layout facts and three layout negatives cover the native views.

## Reproduction

`verify.py` builds seven complete groups and checks all31 scores, exact extents,
frames, preservation, repeated complete-object identity, data, behavior, and
causal/semantic negatives. Protocol-v2 source/script/model/relocator, target and
baseline hashes are sealed; proof guards use explicit exceptions under Python-O.
Observation-only tracing reproduces the complete object. Focused tests cover ten
optimized refusal paths, including false claims, changed inputs, unverified
relocation and corrupt trace binding.

Use the existing configured IDO/MIPS toolchain:

```
python cloud/work/frontier/dot_mat3_demo_pair_20261017/verify.py
python -O cloud/work/frontier/dot_mat3_demo_pair_20261017/verify.py
python -m pytest -q tests/cloud/test_mat3_demo_pair_20261017.py
```

For a source-only checkout, pass `--repo /path/to/complete/unchanged/checkout`
and set N64_RESEARCH_REPO to that path for pytest. The baseline is master58425072.
Canonical files and protected targets are unchanged. Raw machine code, objects,
optimizer logs and native dumps are outside this packet.

The focused tests also relocate the packet directly beneath `/tmp`, then check optimized integrity acceptance and false-credit refusal with an explicit external repository. The verifier does not require a repository-depth path to import.
