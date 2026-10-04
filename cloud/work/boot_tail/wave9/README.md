# Ninth frozen cut: twenty-eight additional runtime bodies

This cut freezes 54 fresh whole-function attempts / 10,224 B: 28 independently
reviewed strict matches / 4,248 B and 26 complete nonmatches / 5,976 B. Current
registry, medium-tail, timed, context and later work are excluded. Every source
in `reviewed_sources.json` is byte-identical to its final peer-reviewed commit.

| Packet | Matches | Complete nonmatches |
|---|---:|---:|
| BT02-final-small | 2 / 220 B | 2 / 320 B |
| BT02-next-pair | 0 / 0 B | 2 / 932 B |
| BT03-low-resource | 4 / 436 B | 1 / 208 B |
| BT03-low-service | 5 / 876 B | 0 / 0 B |
| BT03-low-removal | 0 / 0 B | 3 / 872 B |
| BT03-low-lookups | 0 / 0 B | 5 / 784 B |
| BT03-high-routing | 5 / 704 B | 1 / 128 B |
| BT03-high-transitions | 3 / 592 B | 3 / 608 B |
| BT03-high-runtime | 4 / 680 B | 2 / 352 B |
| BT03-high-params | 4 / 448 B | 2 / 208 B |
| BT05-larger-trio | 1 / 292 B | 2 / 672 B |
| BT05-larger-pair | 0 / 0 B | 2 / 696 B |
| BT05-control-dispatch | 0 / 0 B | 1 / 196 B |

## Semantic and ABI evidence

Each packet has independent actual-source/native/caller/callee review, selected
O2/O1 control receipts and its recorded host/layout/sanitizer checks. Native
argument counts, ignored status returns, mutable helper effects, packed fields,
unsigned low-word arithmetic and signed narrowing order are retained explicitly.
The rejected lookup control that stored a result pointer before reading its value
is not the retained reconstruction, even though its compiler residual was lower.
The complete retained source preserves native ordering. No declarations were
invented to force register allocation.

The 23754 caller is independently reconstructed with its real four-input control
prefix and an external translator declaration. Its own relocation proof is clean,
but its body remains NONMATCH. This does not solve or alter 21548's missing table
mapping. Larger randomization/timing sources retain genuine truncation, valid
nonzero-divisor and pointer-domain limitations. Unsupported native behavior is
not replaced with invented initialization or safe fallbacks.

The resource/service packets preserve real list mutation/reloads and callable
helper contracts, including the valid resource-relative geometry. High-runtime
and transition tests retain actual pointer domains, live-count behavior and
untouched bytes. The final BT02 small packet's volatile reset-byte declaration
is backed by native access signatures and cross-reference evidence; its paired
initializer residuals remain archived. Host tests supplement rather than replace
native equality. Rejected controls, including disclosed O1 relocation-window
errors, never receive match credit.

## Exact parent and unique totals

This explicitly stacks on [#68](https://github.com/cabi24/SFRush2049-decomp/pull/68)
head `c196e2126217abb91380b240c68e7ea9c52fd941`, tree
`bcf3213f3d6bc50b8e9a448c4268c3b1d135693f`. Exact
[Verify 37164693092](https://github.com/cabi24/SFRush2049-decomp/actions/runs/37164693092)
passed; its proof is carried in `../wave8/ci.json`. Prior new CI-verified bodies
total 183 / 14,260 B. This cut adds 28 / 4,248 B, yielding 211 unique new bodies /
18,508 B if its own exact-head CI passes. The historical 12-byte getter is separate.

Across nine cuts, 307 distinct attempted addresses comprise 211 matching
candidates, 95 complete nonmatches / 14,496 B and one earlier 96 B
SOURCE-LEAD / needs-rodata-proof at 80021548. Repeated probes add no duplicate
targets. See `unique_totals.json`. Later refills are not part of this frozen cut.

## Reproduction and scope

The aggregate checker recompiles the exact source hashes, validates native extents
and reproduces all 54 selected outcomes. All 28 submitted matches have complete
relocated word equality with no masks, unresolved or unverified fields,
relocation errors or nonzero excess words. Canonical changed-submission routing,
central tests, deterministic ledger/opcode checks, static locks, source hashes,
protected paths and whitespace are rerun after integration.

```sh
python3 cloud/work/boot_tail/scripts/verify_wave9.py --check
python3 tools/cloud/check_submissions.py --base c196e212 --head HEAD
python3 cloud/work/boot_tail/scripts/generate.py --check
python3 -m unittest discover -s cloud/work/boot_tail/tests -v
```

Only allowed cloud boot-tail source/work files differ from #68. D10 is unchanged;
its blocked update remains excluded. No ROM/image bytes, raw native dumps,
object files, credentials, protected input/scorer, shared types, layout/lock/symbol,
runtime-image/farm or production-gate edits are included. The draft targets
master for CI, with prior drafts as explicit dependencies. No merging, cartridge
coverage or maintainer acceptance is claimed. Leave merging to the owner's
independent checker.
