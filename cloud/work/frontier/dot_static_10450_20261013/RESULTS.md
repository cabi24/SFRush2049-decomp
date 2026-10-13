# Static audio DMA batch callback: full native match

## Result

`cloud/matches/boot_tail/func_80010450.c` reconstructs the complete 372-byte
function at `[0x80010450, 0x800105C4)` on base `55e179413`. The first natural
reconstruction matches under stock pinned IDO 7.1. A subsequent type cleanup
uses a pointer-typed source field, like the existing request type, with identical
native code. There are no fabricated helpers, unused formals/locals, padding,
code-shaping devices or newly invented volatile accesses.

```
IDO_DIR=tools/cloud/ido71 python3 tools/cloud/score.py fn \
  cloud/matches/boot_tail/func_80010450.c func_80010450 \
  --targets asm/us/boot_tail --flags '-g0 -O2 -mips2 -G 0 -non_shared'
func_80010450:
  MATCH
```

This is **372 bytes of new strict static candidate source**, pending independent
checker installation. No accepted locks, production TUs, target data, inventory,
ROM or splice state change. No cartridge identity or gameplay claim. Keep the
PR draft; acceptance, installation, ROM gates and merging belong to the checker.

## Reconstructed contract

No arcade ancestor is proven. Accepted `func_800105C4` registers this void,
no-argument callback in `D_80038020`. There are no direct `jal` callers across
all four protected target populations. Accepted `func_80010628/func_80010714`
provide the twelve-byte request `{destination, source, size}` and the volatile
unsigned-byte pending count (capacity four). Accepted `func_80012730` provides
the two-argument unsigned command writer and twelve-byte DMA record layout.
The private record view uses a pointer for its source field; IDO layout checks
prove it has the same four-byte representation as the accepted unsigned field.

- If the preceding-batch flag is nonzero, send a null message with flag zero to
  the existing message queue. Its return is ignored, including errors.
- If pending requests exist, scan them using an unsigned-byte index and the live
  volatile bound. Advance the command cursor by twelve and call the command
  writer before reading and appending each request as `{size, source, destination}`.
- Reload the shared queue base and unsigned-halfword index as native code does.
  Increment the queue index after each record. Clear pending count and set the
  preceding-batch flag to one after the nonempty branch, even if a second live
  count read becomes zero and no request is consumed. Clear the flag if the
  initial pending-count test is zero.

Valid inputs require the existing four-request storage and adequate destination
queue capacity. Synchronous hooks deliberately exercise mutation of pending
count, queue index/base and request fields. One native-only scripted live-read
case covers a count dropping to zero between its first two volatile reads. These
are finite semantics/ABI tests, not a concurrency or hardware proof. Arbitrary
aliasing, invalid memory/capacity, larger counts, asynchronous schedules, original
source identity and gameplay are outside the proof.

## Evidence

The protocol-v2 `verification.json` is input-bound; `verify.py --deep` rebuilds it:

1. Protected target-manifest validation, exact native next boundary, and pinned
   stock IDO 7.1 selected by the actual `lib_11050` compiler configuration. All
   93 words match canonically, with no unresolved/unverified relocations, errors,
   differing words or extra nonzero words.
2. An independent ELF reader and GNU linker consume the entire original object:
   all **22 relocations / ten external bindings**, full STT_FUNC 372 bytes,
   complete text 384 bytes with separate twelve-byte zero suffix, **56-byte
   frame**, no owned data. All unmasked linked words match. Legacy IDO marks
   MIPS2, not an explicit o32 flag; native layout and ABI replay supply that proof.
3. Eleven IDO C89 assertions verify request/record sizes and fields plus pointer,
   word and halfword widths. Four accepted contract sources independently remain
   strict. A scratch promotion-shaped TU preserves all **four existing C bodies**
   and the new body is still strict.
4. **1,351 field-oracle fixtures / 2,702 native and GNU-linked executions** cover
   all 93 instructions and both outcomes of all four conditional branches. Cases
   include zero through four requests, zero/one/high-bit batch flags, high-bit
   source/size values, cursor/count wrap, send-hook pending changes, writer-hook
   queue/request/pending changes, and the live-count-read case. Full mapped
   nonstack memory and helper-entry snapshots agree. Caller-save GPRs and LO are
   poisoned after each call; callee-save GPRs, stack and return address must survive.
5. **1,350 actual-source C89 host cases** pass at O0, O2 and ASan/UBSan/bounds with
   recovery disabled. Host pointers represent logical objects; native layout is
   separately asserted. Only LeakSanitizer is disabled in this container.
6. Fresh wrong-cursor and wrong-source C mutations fail strict and behavioral
   checks. The complete-object gate rejects an unexplained allocated data object
   and an enlarged STT_FUNC that consumes otherwise-zero section padding. Unknown
   instructions and unmapped memory fail closed.
7. Receipt inputs bind source/proof/host/test files, selected accepted contracts,
   parsed native targets/extents, symbols, direct-call census and stock compiler
   fingerprints. Shared tooling, build files and raw-assembly formatting are not
   pinned. No native words, assembly dumps, compiled objects or ROM payloads are
   published.

### Installation detail for the checker

The new function is the first slot of `lib_11050`, preceding the existing
`TransferRequest` typedef. Prefix-only dedup keeps the new early declaration,
so a straightforward temporary promotion creates a second identical typedef at
its old location. The verifier removes **only that second exact declaration**
in the scratch TU, retaining the original spelling and all existing bodies.
This is explicit declaration hoisting/dedup, not a silently altered production
source. The checker must perform the equivalent declaration placement when
installing, then run the normal static and cartridge gates. No shared promotion
tool or production source is changed by this packet.

## Reproduction

```
python3 cloud/work/frontier/dot_static_10450_20261013/verify.py --deep
python3 -O cloud/work/frontier/dot_static_10450_20261013/verify.py --deep
REQUIRE_TOOLCHAIN=1 python3 -m pytest tests/cloud/test_static_10450_match.py -q -o addopts=''
make check-matched
```

Normal and optimized-Python deep replays pass. The cheap test passes in about
0.4 seconds and the missing-IDO path is handled by the shared test fixture.
`make check-matched` preserves all 630 static locks and all 102 runtime-image
entries. Hosted CI and independent peer review remain separate publication gates.
