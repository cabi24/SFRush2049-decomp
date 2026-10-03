# Minimal maintainer inputs for a disjoint execution lane

Base inspected: `53d0fba0c5f44e91fe921f722fee7ff63a2dd0fe`.
Status: INPUT-CONTRACT RESEARCH ONLY. Zero ready execution packets; no candidate,
compile, accepted bytes, matching claim, integration, or ROM claim. D1248 and
its proposed closure were neither inspected nor edited. No private original
input, private checkpoint, or session history was read. No repository code ran.

## Recommendation and ranking

1. **C974 external contract: first ask.** One missing external identity/contract
   at `0x8038D798` is a small, sharply bounded maintainer lookup if a map/header
   exists. Potential target is 2,636 bytes/659 words at `0x8010C974`. Expected
   matching payoff is uncertain: recovering the service only unlocks complete
   reconstruction; the existing generated seed has other type/semantic defects.
   Retrieval cost low to medium if metadata exists, unknown/high if it does not.
2. **FAF6C authentic caller context: second.** Target `init_state_continue` at
   `0x800FAF6C`, 712 bytes/178 words. No unknown external image is required merely
   to start source recovery: native source material is already tracked. The
   costly missing work is genuine `0x800FB2C8` C and its compiler visibility,
   not another copy of native bytes. Target writes s-registers without an
   ordinary save frame, so ordinary standalone C is not justified. Cost high;
   matching payoff uncertain until authentic IPA/caller convention is explained.
3. **FA9B4: defer for this non-graphics lane.** `render_viewport_init` at
   `0x800FA9B4`, 924 bytes/231 words, needs two unknown service contracts plus
   other genuine source repairs. It also involves rendering-named dependencies
   and is not a demonstrated independent non-graphics route. Cost medium/high,
   payoff uncertain; naming is historical and not subsystem proof.

No quantitative probability or time estimate is supported by these records.
There is no fresh compiler explanation here that justifies repeating a frozen
control. Do not reserve any of these as execution-ready on the strength of this
packet alone.

## Ask 1: narrow C974 service handoff

Please supply an authoritative identity for runtime address `0x8038D798` in the
US revision used by this repository, and either a verified interface contract
or the genuine source implementation needed to establish it. No raw ROM upload.

A small text/JSON metadata file should contain:

- runtime address, actual symbol/aliases, owning image/overlay/object, verified
  extent if known, and mapping/load evidence; identify the exact revision;
- provenance (map/header/source revision or local audit), relevant source/hash;
- parameter count, order, widths, signedness, pointer meaning, stack arguments,
  result, register clobbers/preservation, and caller-visible memory effects;
- whether the body is externally compiled, visible to IDO in this translation
  unit, or an inline/IPA candidate; actual compiler recipe when known;
- explicitly mark unknown facts, rather than filling in an O32 prototype.

Caller evidence is already established: native call at `0x8010D2C4`; same pointer
in a0/a1; signed state byte in a2; bit pattern `0x43C80000` in a3; integer1600 in
the fifth stack argument. The caller saves/reloads t0. Do not infer float400.0
from its bits alone. This asks the maintainer to resolve that ambiguity, not to
endorse a guessed ABI. A symbol name alone is insufficient.

The separate three-pointer `camera_trigger_check` contract is already proven
in PR46's packet: incoming f12 is not required. Do not reopen that solved issue.
After the external contract arrives, audit actual `camera_trigger_check` and
`math_utility` caller/callee preservation while reconstructing the whole C974
entry, initialization, animation, and cleanup. Stop again if that closure remains
unresolved; do not call the old generated seed a complete baseline.

## Ask 2: only if a non-graphics caller-context route is preferred

Supply any existing faithful C for `0x800FB2C8`, aliases `setup_state_main` /
`display_list_flush`, **2,356 bytes/589 words**, plus its declarations, necessary
layout/constant definitions, source revision/hash, compiler version/flags and
real translation-unit membership. Include how the true callers `countdown`
(`0x800FBF88`, 2,672 bytes) and `game_loop` (`0x800FD464`, 704 bytes) see FAF6C:
prototypes, definitions visible together, and actual live values across its call.
Source order/keep-root settings should come from a real recipe, not invented
keepers. Source can be `.c` + `.h`; recipe/context/provenance can be text or JSON.

If no such C exists, say so: this is a source-reconstruction prerequisite that
can be assigned separately, not a request for a ROM. `work/game/race/` already
contains native material, but the setup_state_main and init_state_continue C
files are empty stubs. `src/game/game.c`'s display_list_flush is a generic RSP
submission sketch, describes 5,944 bytes, and omits the native 256-byte-frame,
two-entry-input setup-state behavior. It cannot establish this context. Caller
source alone is not sufficient without the faithful setup-state body. These
observations support rejecting those sources, not a complete reconstructed ABI.

## Ask 3: defer FA9B4 unless scope changes

The two distinct unresolved direct services are:

- `0x8038FCE0`, called at `0x800FAA14`;
- `0x80390F60`, called at `0x800FAA1C`.

Use the same authoritative identity/interface/visibility metadata as Ask1.
No explicit argument setup at these sites does **not** prove a void(void) ABI.
Also recover the actual `save_write_data` service at `0x800AF06C`, called at
`0x800FAB50`: the current historical buffer-serialization sketch is not a verified
native implementation. Do not import it merely for its convenient name.

## Safe maintainer-local read-only metadata exports

Run only in the authorized repository. These commands inspect tracked text and
Git metadata, do not execute repository scripts or extracted code, and do not
read original ROM/image content. Send relevant output/provenance after review;
empty grep output is not proof that a runtime service is absent.

```sh
git rev-parse HEAD
git status --short
git grep -n -i -E '8038d798|8038fce0|80390f60' -- \
  '*.ld' '*symbol*.txt' '*.map' '*.h' 'src/**' 'include/**'
git grep -n -i -E '800fb2c8|setup_state_main|init_state_continue' -- \
  '*.ld' '*symbol*.txt' '*.map' '*.h' 'src/**' 'include/**'
git log -1 --format='%H %s' -- path/to/identified/source.c
```

The last path is a placeholder: substitute only the actual identified authorized
source file. If an existing authorized local linker map is outside Git, run:

```sh
grep -n -i -E '8038d798|8038fce0|80390f60' -- "$MAP"
```

`MAP` must be a maintainer-selected known text linker map, not an invented path
or any original binary input. Inspect locally whether the service's containing
section is mapped even if no exact function entry is present. A symbol-map hit
is identity evidence only; accompany it with the interface/source evidence above.
Do not regenerate builds, execute recovered machine code, or dump entire images
for this request. No object/ROM/disassembly export is necessary for this first
handoff.

## Fresh source evidence

- `cloud/work/c974_call_contract/README.md` and `evidence.json`: native caller
  SHA256 `038c6b05ae52d2f07622ac981c8b16d1f9759f447573dd8aca9e7d71b1ef9052`,
  missing-service status, corrected camera contract. Read existing evidence;
  audit script was not rerun.
- `cloud/work/dot_substantial_scout/STATUS.md`: C974 seed compile failure,
  source-recovery defects and whole-function prerequisite.
- `docs/dot_handoff_targets.csv`: exact registered target sizes above; historical
  work-folder descriptions are not authoritative extents.
- `work/game/race/setup_state_main/base.c`,
  `work/game/race/init_state_continue/base.c`: current stubs.
- Existing tracked `work/game/race/{setup_state_main,init_state_continue,render_viewport_init}/target.s`:
  corroborates the stated frame/register/callsite observations. No raw words or
  native dump are copied into this packet.
- `src/game/game.c` near display_list_flush and save_write_data: source claims
  checked, not accepted as native-faithful.
- Tracked address search across linker/symbol/header/source/reference and research
  metadata found no new verified bodies for the three external8038/8039 addresses.
  Historical prototype-flywheel JSON only records unresolved generated names /
  invalid `?` declarations, not a usable contract.
