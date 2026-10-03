# BT03-high: six state-service bodies

Six strict local O2 matches, **560 B / 140 words**. Independent paired review passed;
aggregate exact-head CI remains required. No cartridge-coverage or promotion
claim is made.

- Branch `dot/boot-tail-bt03-high-state`, source base fresh master
  `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Exact activation `bf7b7405`: `1D4CC+76`, `1D518+96`, `1D8B0+120`,
  `20274+80`, `20370+124`, `20558+64`; all addresses begin `0x800...`.
- These targets are in the assigned BT03-high partition, outside C11. Previous
  packets are frozen. Central status, claims, generated files and D10 are not
  edited here. The branch has no source dependency on earlier matching drafts.
- Fresh canonical manifests, 439-function census equality (99,120 B), and the
  preexisting getter replay pass. The unchanged pinned compiler is reused through
  IDO_DIR; no protected build/target source is modified.

## Native operations and real input types

All six bodies check `D_8002C630`. Enabled operations are bracketed by the
previously audited no-argument `80014594` / `800145DC` nesting/queue helpers.

| Function | Bytes | Operation |
|---|---:|---|
| `8001D4CC` | 76 | Pass the genuine state pointer to `8001D084`; return one when enabled, zero otherwise |
| `8001D518` | 96 | Read word +52 only when state flag bit16 at +8 is set; otherwise return all-ones |
| `8001D8B0` | 120 | Unlink a genuine doubly linked node, updating neighbors or head `D_8004FD54`; return enabled status |
| `80020274` | 80 | Call `80018E6C`, `8001D578` and `8001FAE4(0)` in order |
| `80020370` | 124 | Find an object by halfword ID, store its channel byte or sentinel remapping, and notify channel state |
| `80020558` | 64 | Call declared-only `80014BF8` between nesting helpers |

`1D084` dereferences its actual state pointer's link and flag fields. `1D518`
uses a minimal `StatePrefix` with the evidenced unsigned flag at +8 and identifier
at +52. Unknown byte arrays represent untouched object ranges, not stack locals.
The initial equivalent word-indexed reconstruction already matched; this prefix
cleanup preserves the identical complete native body.

`1D8B0` uses natural aligned next/previous pointer fields at native +0/+4. If next
exists, its previous link is updated; if previous exists, its next link is updated;
otherwise the global head becomes next. The removed node's own fields are not
cleared, matching native behavior. Host pointers are wider on this executor;
only the 32-bit IDO/native proof establishes field offsets. No host offsetof claim
is made.

For `20274`, the two first helpers traverse global voice/list state and take no
input. `1FAE4` genuinely consumes a byte argument, corroborated by its entry mask;
the source declares that byte and passes the real constant zero. The earlier
word-prototype seed was ABI-safe for this constant but was narrowed to the actual
callee contract before freezing, without changing code generation.

`20370` has exactly two inputs: unsigned halfword ID and unsigned byte channel.
`17040` consumes that ID and returns an object pointer. On success, channel254
maps to byte31 at object+9; otherwise the supplied byte is stored and
`1C19C(channel,3)` is called. The real byte value is preserved across lookup and
helper calls; no keeper or fabricated input is used.

`20558`'s excluded callee `14BF8` was inspected only as read-only ABI context.
Its actual call chain reaches `11074`, which takes state from globals and requires
no incoming argument. The source declares a no-argument callee only. No excluded
body, census classification, symbol or target is edited or reclassified.

All called addresses are canonical in-census functions or established counted
static queue functions. No boundary-blocked dependency, local literal or jump
table is needed by these submitted bodies. Original public names, complete
object types and middleware version remain unknown; no external body is copied.

## Flags and strict proof

Every initial natural body matched at `-g0 -O2 -mips2 -G 0 -non_shared`, plus the
scorer's automatic `-Wab,-r4300_mul`. Only `1D4CC` also matches at O1; its tiny
shape does not uniquely identify the historical level. The other final O1
controls differ as follows:

- `1D518`: 23/24 words
- `1D8B0`: 25/30 plus three nonzero excess words
- `20274`: 2/20
- `20370`: 28/31 plus two nonzero excess words
- `20558`: 2/16

O2 is the prescribed first successful level. No near-match refinement, diagnosis,
flag sweep or artificial source mechanism was needed. The two evidence-backed
prototype/layout cleanups described above were strictly replayed before freezing.
All six final O2 bodies have zero differing/extra words, unresolved symbols,
unverified relocations or errors. The exact O1 control is not a seventh body.

## Replay and scope

```sh
python3 cloud/work/boot_tail/BT03-high-state/verify.py
```

`verification.json` binds twelve final O2/O1 rows to source hashes;
`initial_controls.json` preserves the seed results. `status_delta.csv` is the
central sole writer's six-row integration input. Only this packet directory and
six matching C files are changed. No target, compiler/scorer, lock, layout,
symbol, runtime image, farm, forbidden helper, ROM/raw instruction dump, object,
secret or production gate is changed or published. Independent review and
exact-head CI precede checker-owned merging.

## Independent review

The BT05/BT07 reviewer independently reproduced all twelve rows and exact source
hashes on `22b3d2b6`. Complete wrapper targets and actual source ABIs were audited,
including aligned node links, state offsets, selector remapping and the genuine
no-argument excluded-helper call chain. `independent_review.json` confirms six
distinct O2 bodies, with the single O1 MATCH treated only as flag evidence.
No artificial source construct or source/ABI blocker was found.
