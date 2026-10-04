# BT02 audio pair: linked buffer-pool initialization

Two complete bounded **NONMATCH** reconstructions, **1,232 native bytes**; no new
matching credit. Both final O2 bodies retain exactly **74 differing words**, with
zero excess nonzero words, unresolved symbols, unverified relocations or errors.
All differences are register operands; complete body geometry, calls, constants,
stack frame/homes and control flow agree. Actual-source C89 and ASan/UBSan runs
pass **522 cases each**, and pinned IDO native-width layout assertions pass.
Independent paired source/native/ABI review passes for research only on
`0506807df02405c9f0090ad3988b381651828cf8`; the receipt is
`independent_review.json`. No C source changed after that review.

## Scope and inputs

- Branch `dot/boot-tail-bt02-audio-pair`, base
  `ea50bee96b5d732ba9efcac8f433134090f41950`.
- Parent activation preceded source edits; central claim persisted at `1535b793`.
  Fresh STATUS and all live-claim scans at `16763d4b` found both rows open and
  unclaimed. Latest inspected published draft was #68 at
  `c196e2126217abb91380b240c68e7ea9c52fd941`; later aggregate publication belongs
  to the central lead.
- `80011F60`: 604 B, ends `800121BC`; `800123A8`: 628 B, ends `8001261C`.
- Reused the clean prior BT02 checkout and preserved its frozen branch. No new
  full checkout or duplicate compiler installation.
- Only this packet directory changes. No edits to previous attempts, central
  STATUS/claims/D10, targets, scorer, locks, layout, symbols, shared headers,
  runtime images, farm or production gates.

The input verifier checks pinned compiler-file hashes, target manifest members,
all 439 inventory/extent pairs (99,120 B), protected tool/input hashes and a
strict replay of the historical 12-byte getter. That getter earns no new credit.
Both bodies contain no switch-table or literal dependency. The caller's table
contents are neither read nor assumed.

## Actual ABI and reconstruction

The sole direct callee in both bodies is `800084E0`. Its actual static body in
`asm/us/90E0.s` consumes an address and signed byte length and performs data-cache
invalidation; it does **not** zero memory. The callback at `D_80038018` receives
an allocation size and zero mode, returns a pointer in `v0`, and follows ordinary
O32. This agrees with the already reconstructed callback view in `80010C68` and
`8001E740`. No callback implementation or fake context helper is embedded here.

`80011F60` consumes one 32-bit count, retains it in saved `s0`, allocates a count
of 20-byte nodes, then allocates `count * 256` buffer bytes and invalidates that
buffer range. It clears the active-list head `80038344`, sets free-list head
`80038348`, and builds a doubly linked list of nodes with 256-byte buffer slices.
It terminates the last next pointer and clears `80038340`. Next/previous/buffer
are at native offsets 0/4/8. The remaining two words are preserved storage, not
invented operations. The already reconstructed countdown walker `80012200`
provides an additional next-pointer/offset-16 field view; this packet does not
change its declaration.

`800123A8` consumes one genuine unsigned-halfword count. Native masks it to saved
`s0` and homes the original argument word. It allocates and invalidates
`count * 1536` buffer bytes, then allocates a count of 24-byte nodes. It clears
active head/tail `80038354`/`80038358`, sets free head `8003835C`, and links all
nodes to consecutive 1536-byte slices. Next/previous/buffer offsets are 0/4/20;
the three unmodified middle words remain unnamed storage. The already matched
`80012660` operates the same linked-list prefix.

The actual sole direct caller `800114C0` passes an unsigned-halfword load to the
first initializer and an explicitly truncated halfword product to the second.
Its unproved lookup-table values are unnecessary to reconstruct these callees;
no specific result of that multiplication is assumed. Actual release partners
`800121BC` and `8001261C` separately free exactly the corresponding two pointers,
in their observed order. These callers, callee and releasers were inspected as
native bodies, independently of historical symbol names.

### Valid-domain limits

Both native bodies unconditionally touch the first node, and neither checks
allocation failure. The well-defined source/test domain is a **positive count**
and successful allocations with the requested capacities. The observed call
path limits counts to at most 65,535. Count zero and allocation failure are not
silently treated as safe: the actual bodies have no guards, and the caller's
external setup contract is not established here. Host tests do not claim safety
outside this domain or simulate hardware cache/interrupt behavior.

## Flags and bounded diagnosis

Exact primary flags: `-g0 -O2 -mips2 -G 0 -non_shared`; the unchanged scorer adds
`-Wab,-r4300_mul`.

| Function | Initial O2 | Final O2 | Final O1 | Native bytes |
|---|---:|---:|---:|---:|
| 80011F60 | 76/151 | 74/151 | 149/151 | 604 |
| 800123A8 | 79/157 | 74/157 | 155/157 | 628 |

O2 is supported by both native four-way unrolled loop bodies, remainder loops,
common expressions and saved count registers. The fixed O1 controls are worse.
No optimization sweep was made.

Workbench diagnosis preceded variants. Removing the named byte-count temporary
and repeating its expression lets IDO retain the actual expression across the
call, fixing displaced stack homes and the second function's oversized frame.
Both final ELF symbol sizes exactly equal their native extents and both frames
are 48 bytes. The final diagnosis is `allocation-mismatch` for each: 74 aligned
register differences, zero structural/constant differences or insertion/deletion
gaps. Every strict scorer relocation remains clean.

The residual begins at the compiler-generated loop remainder: native masks into
its existing `v0`/`v1` carrier; the candidate uses `t8`. The subsequent unrolled-loop register sequence is phase-shifted, consistent
with an extra temporary demand. Exact compiler-pass ownership and causality
remain unproved; the workbench correctly requests evidence rather than naming
a guaranteed source lever. Fourteen
bounded directed controls after each baseline tried declaration ordering,
expression-only byte counts, equivalent loop/array spellings, index type and a
buffer-array representation. None improves on 74. The first function's
halfword-formal probe is explicitly rejected without independent ABI evidence;
it is not a recommended source. `variants.json` and exact control sources record
all results, and `verify.py` replays all thirty baseline/control rows.

Stop rather than add fake locals, dummy reads, padding, formals, assembly or
context helpers. A new attempt requires concrete original loop-carrier/type
or compiler-pass evidence that explains this single generated remainder web.
The current nonmatches are frozen evidence, not candidates for blind replay.

## Reproduce and verification limits

With `IDO_DIR` set to the existing pinned IDO 5.3 directory:

```sh
python3 cloud/work/boot_tail/BT02-audio-pair/verify.py
python3 cloud/work/boot_tail/BT02-audio-pair/test_layout.py
python3 cloud/work/boot_tail/BT02-audio-pair/test_host.py
```

For workbench metadata, set `MIPS_OBJDUMP` to the existing GNU MIPS objdump,
provide its runtime library path if needed, and run `diagnose.py`. The adjacent
MIPS assembler is used only for temporary diagnostics. Full strict scoring
continues to use the unchanged canonical gate. Only zero object-alignment words
outside the exact ELF symbol size are omitted from workbench geometry analysis;
raw words, temporary assembly and objects are never included in this packet.

Host checks use actual final sources as separate translation units. Cases cover
both families with every count 1–257 and 511/512/1023/65,535, all four unroll
remainders, one-node termination, initial nonempty global heads, exact callback
allocation/cache-call order and arguments, all next/previous links, all buffer
slices, preserved unnamed words, and allocation guard canaries. Host pointer
width changes node allocation sizes; separate pinned IDO assertions establish
the native 20/24-byte sizes and offsets. ASan leak detection is disabled solely
for the established runtime ptrace limitation.

No third-party implementation is copied. No ROM/image bytes, raw native dumps,
objects, credentials or private data are published. No cartridge coverage,
promotion, maintainer acceptance or merge is claimed.
