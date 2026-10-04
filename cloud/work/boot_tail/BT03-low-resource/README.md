# BT03 low resource registration packet

Exclusive claim `7d98cace`, branch `dot/boot-tail-bt03-low-resource`, base master
`301d9e7552ad4fd7f54a38796db84671e1000d35`. Five whole bodies / 644 B attempted:
four first-try strict O2 matches / **436 B**, and one complete NONMATCH / **208 B**.

| Function | Bytes | O2 | O1 |
|---|---:|---|---|
| 80014F14 | 108 | MATCH | 27/27 + 5 nonzero excess |
| 80014F80 | 108 | MATCH | 27/27 + 5 nonzero excess |
| 80014FEC | 108 | MATCH | 27/27 + 5 nonzero excess |
| 80015058 | 112 | MATCH | 28/28 + 4 nonzero excess |
| 800154A4 | 208 | 17/52 | 47/52 + 8 nonzero excess |

Only the four matching bodies are submitted. Every retained O2 result has zero
nonzero excess, unresolved symbols, unverified references, masks or relocation
errors. Matches have full relocated word equality. The three 108-byte functions
have one ordinary zero alignment word in 112-byte object text; the 112-byte body
has no trailing text. The nonmatch has exactly 208-byte object text and receives
no match credit. Targets, extents and scorer remain untouched.

All flags are `-g0 -O2/-O1 -mips2 -G 0 -non_shared`, with the scorer's automatic
`-Wab,-r4300_mul`. Native O2 signals include the 40-byte registration frames,
three real persistent values (cursor, resource pointer, sentinel), live reloads
and branch-likely loops. The dispatcher has the native 32-byte frame but still
fails strict equality; frame agreement alone does not confer a match.

## Whole-body contracts and source evidence

The four registration functions each receive exactly two real pointers: a
sentinel-terminated u16 identifier list and a four-offset resource table.
`14F14`, `14F80`, and `14FEC` call lookup helpers `14E64`, `14E90`, and `14EBC`
respectively, then call `1671C`, `15D68`, and `15720` for successful lookups with
entry+8. `15058` calls lookup `14EE8`, then `15A0C` with entry+12 and the unsigned
halfword at entry+10. The common resource-table type agrees with prior complete
lookup-wrapper sources. The extended header is a naturally aligned genuine
12-byte prefix, not artificial local padding.

Every external ABI was inspected in canonical code before writing candidates.
The registration helpers home a u16 identifier and full pointer; `15A0C` also
homes and reads the low halfword of its third argument. Their observed integer
statuses are ignored. The lookup helpers return a pointer/null. Exact original
status typedef/signedness is not recoverable here and is not claimed.
`abi.json` records canonical helper hashes and contracts; no helper body is
inserted into a candidate or altered elsewhere.

An important live effect is preserved: after lookup returns, the registration
call rereads `*ids`. It does not reuse the identifier cached for lookup. Likewise,
the next list entry is loaded after callbacks. Null lookup results skip only
registration, then continue the walk. The tests explicitly mutate both current
identifiers and subsequent terminators to check these details.

The complete `154A4` dispatcher checks the global enabled byte and signed short
stack count, pops one resource-group pointer, and calls the five now-reviewed
walkers in native order: `150C8`, `152A8`, `15120`, `15178`, `151D0`. Each pointer
is selected group + the corresponding relative offset +8. The group has id +4,
type +6 and five word offsets beginning +8. Type one triggers a final `15348(id)`
call; a successful pop returns one, disabled/empty returns zero. The final
external declaration follows the existing complete-source void `15348` wrapper,
which itself discards `1661C`'s status. Helper sources remain read-only.

The selected group is retained across callbacks, while later offsets, type and
id are genuinely reread. The count decrement occurs before the first call.
Native pointer-table slots are four bytes; host tests validate behavior and the
pointer-free group prefix, not native pointer-table stride. When enabled, the
signed count must be a valid positive index within the configured table, and
all relative offsets/terminated lists must remain inside allocated resources.
Negative/malformed configurations are not offered a safety guarantee.

## Bounded dispatcher diagnosis

The initial inline-pop body is 17/52 with no excess. Workbench diagnosis ran
before refinements on the canonical target and fully relocated object. Both
frames agree; residual operand allocation/order has no proved automatic lever.
The count normalization uses temporaries instead of the target's in-place value;
later pointer additions have different operand/temporary ordering.

Two natural body controls were bounded: separating the global decrement worsens
to 46/52, while a genuine short index local used for the count store and table
index returns to 17/52. Three body forms total, then stop. The final declaration
alignment for `15348` changes no machine result. Initial controls retain their
original ignored-word-return declaration as historical controls. No narrow-value
or flag sweep, forced register, dummy call, keeper or padding local was tried.
Next useful input is authentic source/compiler context for the signed count-pop
expression and pointer lowering, not masked relocation or an ABI exception.

`verification.json` reproduces all five retained bodies plus three archived
controls, each at O2 and O1: sixteen exact hash-bound compiler rows.
`diagnosis.json` contains metadata only; temporary native listings and objects
are never archived. No external source was copied; this packet is reconstructed
from canonical native bodies and previously reviewed local caller/callee evidence.

## Checks and reproduction

Fresh setup, all target manifest entries, 439 starts/sizes totaling 99,120 B, and
the existing getter pass before candidates. Six tests compile actual retained
C89 source with pedantic errors, warnings as errors, AddressSanitizer and
UndefinedBehaviorSanitizer. They cover **1,386 candidate invocations**: 324 for
each registration wrapper and 90 dispatcher cases. Lookups exercise missing,
all-found and alternating results; hooks test live identifier/parameter/offset/
count/type/id effects, payload offsets, call order and selected-group preservation.
The wrapper test contracts are synthetic, not reconstructed helper implementations.
LeakSanitizer alone is disabled because ptrace prevents it here; no harness or
candidate heap allocation occurs. See `host_verification.json` for exact hashes
and valid-input limits. Host tests supplement strict native equality.

```sh
python3 cloud/work/boot_tail/BT03-low-resource/verify.py --check
python3 -m unittest discover -s cloud/work/boot_tail/BT03-low-resource -p 'test_*.py' -v
python3 tools/cloud/check_submissions.py --base 301d9e75 --head HEAD
```

Only the four named matching files and this owned folder change. Central ledger,
D10 and prior frozen packets stay untouched. No protected target/scorer, shared
header, symbol/layout/lock, production gate, runtime-image/farm or unrelated
helper changes; no ROM bytes, raw native dumps, objects or private data.
The 631 existing cloud regression tests pass with zero skips.
Independent BT03-high review passed exact source commit
`521cf064bb6370755486a54b973edecb6aaec3b2`, tree
`5a74cae75191c2872a2c64deaa50e38516c31eeb`. The reviewer inspected all five
complete C/native bodies and actual external ABIs, then independently replayed
all sixteen rows and six sanitizer tests in a separate pinned checkout.
`REVIEW.json` records that source-bound PASS. Aggregate publication and exact-head
CI remain lead-owned. No cartridge
coverage or maintainer acceptance claim; merging stays checker-owned.
