# Runtime-A storage availability refresh

Frozen base `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`;
stock IDO 5.3 group pipeline, actual `-g0 -O3 -mips2 -G 0 -non_shared`.

New `func_80394834` is **100/100 strict**, **400 candidate bytes**. It has no
unresolved symbol, unverified own-data reference, relocation error or extra
word. Only this new helper is claimed.

Complete genuine caller/context is extended from #268, with its prior bodies
unchanged. The sole kept root `func_80397FFC` naturally retains this helper;
no synthetic second caller, formal, retention barrier or register pressure.

Current other scores, all research and unclaimed:

- `func_80394564`: 3/180 words differ, no data/extent gap; unchanged from #268.
- `func_80394B00`: 162/182 differ, no data/extent gap; unchanged from #268.
- `func_80397FFC`: 260/268 differ, 12 extra nonzero words, 4 unverified own-data
  sites and 3 errors (one HI16 outside the fixed comparison window plus two
  shifted literal-address conflicts). These are disclosed context failures,
  not a claim that the full group is ready to integrate.

## Genuine implementation

The refresh scans four player-presence records, assigns the four native error
statuses, invalidates mapped selections after a previous error, counts the
actual linked selection nodes, and checks both available bytes and remaining
entry capacity. Its ordinary no-argument source receives the native private
s0..s8 clobber contract from real group compilation.

Presence records are observed 16-byte records; the list uses the existing
#268 Owner/OwnerBody/Selection/Item pointer chain. Accessed offsets are retained,
not asserted as original source type names. The explicit zero-count store after
the traversal is a directly observed native write, retained even though it
stores zero when the count is already zero. The same-symbol `func_800A35BC`
unsigned byte-count and `func_800A3508` explicit unsigned comparison preserve
the observed `sltu` operation.

No assembly, synthetic context, unused locals, padding, forced registers,
volatile qualification or optimization exception. The earlier unused-argument
ABI gap of 94564 remains unrepaired, rather than fabricating an extra formal.
Other callback/data/initializer assumptions are inherited from the source and
notes in #268; do not install both copies into one unit or count old bytes twice.
This follow-on keeps #268 available for the independent integration decision.

## Reproduce

With IDO and GNU MIPS tools configured:

```
python cloud/work/frontier/dot_runtime_a_availability_refresh_20261007/reproduce.py --repo .
```

For an overlay use `--repo OVERLAY --reference-root GIT_REPO`. The script
verifies the pinned asset and image identity in memory, then uses the unchanged
strict scorer. No raw data image is included. Only matching compilation/scoring
was run; acceptance tests, integration and merging belong to the checker.
