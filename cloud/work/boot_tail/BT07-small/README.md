# BT07 small-body packet

Seven strict local matching bodies, **280 B**, reconstructed from the protected
boot-tail targets. These are matching source bytes, not cartridge coverage.
Independent source/ABI review and exact aggregate-head CI remain required.

- Branch: `dot/boot-tail-bt07-small`.
- Fresh master base: `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Central exact claim: `5b44ebd3`, `wave2/claims.json`; the prior unused BT07
  reservation was released before any source edits.
- Edit scope: these seven match sources and this packet directory only.
  The coordinator owns STATUS, generated central records and D10.
- PRs #52/#54 prerequisites are already merged. PR #59 orientation and draft
  PR #61 first-wave sources are context only; no edits to either are included.

## Verified result

| Function | Bytes | Supported behavior | O2 | O1 control |
|---|---:|---|---|---|
| `80024FB0` | 36 | Double signed byte +4574 when D_80038290 is nonzero | MATCH | MATCH |
| `800250F0` | 48 | Blocking receive from D_800586A8, then return zero | MATCH | MATCH |
| `80025120` | 48 | Nonblocking front-of-queue insertion, unused token input | MATCH | MATCH |
| `80025150` | 44 | Blocking receive from D_800586A8 | MATCH | MATCH |
| `80025D84` | 60 | Rescan two records until both busy bytes are clear | MATCH | 14/15 + 10 excess |
| `80026328` | 32 | Change signed state byte +4573 from 2 to 3 | MATCH | 3/8 |
| `80026348` | 12 | Set signed state byte +4573 to 4 | MATCH | MATCH |

All seven O2 checks have zero differing words, extra words, unresolved symbols,
unverified relocations and relocation errors. Every source has the required
line-one flags; the unchanged scorer adds `-Wab,-r4300_mul`. O2 is supported by
the loop's branch-likely/induction shape and the conditional state setter.
Five tiny wrappers/leaves are identical under O1, so those bodies cannot identify
the original optimization level independently.

Fresh setup downloaded the official pinned IDO 5.3 archive and checked SHA-256
`ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506`.
All three target manifest entries passed; 439 starts/sizes exactly match the
inventory at 99,120 B. The preexisting 12-byte getter replays strictly and adds
no new credit. `verification.json` binds final C files, verifier, compiler files,
scorer, setup and target metadata to SHA-256 hashes. `verify.py` is read-only.

## ABI and object evidence

`StreamState` is a partial, hypothesis-named 4,648-byte record. The three named
byte offsets and stride are directly evidenced by the selected bodies and their
in-scope callers. Unknown byte arrays describe object storage, not stack padding.
No production headers are changed. Full type identity and original field names
remain unknown. Busy is volatile to express polling of asynchronous state; the
native repeated loads and `80025DC0` shutdown sequence support that contract.
The declaration does not claim a recovered original qualifier spelling.

- `24FB0`: both `252AC+0x1E0` and `25C68+0x54` pass the record pointer and
  ignore v0; the latter caller advances records by 4,648 bytes. The global flag
  is read unsigned, while the field is read signed. Multiplication by two is
  defined in promoted int range; conversion back to signed char has the expected
  IDO low-byte truncation, also checked over all 256 inputs on the host.
- `250F0`: all four direct callers save its zero v0 and later pass that word to
  `25120`. The queue helper consumes no incoming arguments. `osRecvMesg` resolves
  to counted-static `80007270`; the prototype matches the local message header.
- `25120`: native stores a0 to its argument home before repurposing it. Callers
  `252AC+0x1F8`, `254D4+0xA8`, `25670+0xB8` and `259A8+0xE0` pass the saved
  token. Thus the unused word formal is genuine ABI evidence, not an invented
  allocator input. Signedness of the unused token is not established. Existing
  symbols resolve `800075E0` to `osJamMesg`, not osSendMesg. The message is null
  and the nonblocking flag is zero. All observed callers ignore the result.
- `25150`: caller `2574C+0x44` supplies no argument and ignores v0. Like `25120`,
  the native body leaves the queue routine's v0 intact, but this does not prove
  a meaningful source return contract. The reconstruction uses void based on
  observed use; neither source invents a return value or extra input.
- `25D84`: `25DC0+0x30` calls it after iterating the two stop operations. It
  takes no arguments and has no callees. The native scan starts at D_80056230,
  checks byte +4608, advances by 4,648 and ends at +9,296 (D_80058680). A busy
  entry restarts the whole scan. The source uses a natural two-element index loop.
- `26328` and `26348`: the caller windows pass the same record pointer and ignore
  v0. Only the state byte is changed. Neither function needs additional formals.

These are native reconstructions. No public source was imported and no original
SDK/middleware identity is claimed. No selected body depends on the four
out-of-census destinations identified by Packet 1; no destination bytes outside
the permitted target population were inspected.

## Bounded reconstruction of 80025D84

The initial complete pointer-loop form differed in 15/15 words at O2 and O1
(O1 also had nine extra words). Before refining, the vendored workbench's
`diagnose` reported structural mismatch, equal frameless geometry and no supported
register-only lever. Its relocation warning reflects a temporary raw-word native
object versus symbolic candidate references; only the unchanged strict scorer is
used for acceptance.

The natural index loop immediately matched O2. Three additional already-bounded
pointer controls (explicit end pointer, relational comparison and a while loop)
remained nonmatches and were discarded. Five complete source forms total, well
below the twenty-variant bound. `experiments.json` records numerical outcomes;
no native words, raw dumps or object files are included. No synthetic parameters,
keepers, padding locals, asm, helper bodies, flag sweeps or scorer/target edits
were used.

## Reproduction and limits

After `bash tools/cloud/setup.sh`, run from the repository root:

```sh
python3 cloud/work/boot_tail/BT07-small/verify.py
python3 cloud/work/boot_tail/BT07-small/test_semantics.py
python3 tools/cloud/check_submissions.py --base 301d9e75 --head HEAD
```

The C89 host harness links the actual seven submitted translation units. It checks
record offsets/stride, all 512 scale/enable inputs, all 256 state values, adjacent
byte preservation, queue address/arguments and zero-return behavior. Both-clear
scan returns. A permanently busy first or second record does not return during
each bounded observation. Concurrent clearing transitions are not host-tested.
These are source-level behavior checks, not execution of native targets or ROM
validation. The host helper definitions exist only in the temporary test harness;
no callee bodies are embedded in match files.

`status_delta.csv` is an integration input, with LOCAL-STRICT-MATCH qualification
pending independent replay and aggregate CI. No central totals, locks, layout,
symbols, runtime-image work, tools, targets, farm or production gates changed.
