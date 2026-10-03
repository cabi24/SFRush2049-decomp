# Fresh graphics-initialization closure: NONMATCH

Base `0bfebc7367ebc1ddb4d6105b2012ed07fb080faf`. No source/ROM coverage
claim, no accepted input edits, no private image or cartridge verification.
The historical `sound_init` name is misleading: its entire 400-byte body
initializes a double-buffered graphics command stream and graphics state.
The complete second root `func_800878E0` is 296 bytes and conditionally emits
state commands. Neither root had complete curated C in the inspected master
and PR1–45 source inventory; legacy work directories contain TODO stubs.

## New value and bounded result

Natural full C for both roots now exists, including 64-bit cached state,
19,200-byte command-buffer stride, the exact >=221 height threshold,
all state-command words, flag gating and call order. An authentic three-body
O3 experiment uses the accepted `func_80086A50` source byte-for-byte, without
its old synthetic callers. The only kept roots are the two real public
functions. No guessed parameters, padding locals, volatile steering or
invented helper bodies occur in the candidate.

- `sound_init`: standalone O2 75/100 differing words, padded symbol extent 416;
  genuine O3 75/100, extent 412. Both have zero nonzero excess words, no
  unresolved or unverified references in the root. It is **not a match**.
- `func_800878E0`: standalone O2 56/74 plus 3 nonzero excess words, extent 320;
  genuine O3 29/74, exact 296-byte extent, zero excess/unresolved/unverified.
- Accepted mode helper context: 0/387 differing masked words and exact
  1,548-byte extent, but **two local .rodata relocations remain unverified**.
  This is not a fresh fully resolved match or new claim.

`verification.json` records complete counts, extents and source/target hashes.
The only extra source control changed the 64-bit zero from `0` to `0LL`;
O3 results were identical and plain `0` is retained. No permutation sweep.

## ABI and data proof

`sound_init` takes no arguments. It preserves `ra` in a 24-byte native frame.
Its call to the mode helper has `a0=1`; the optional flag helper has
`a0=0x8000`. Native `t3` holds the address of `D_8002AFC4` over the mode
call, proving isolated external-call O32 assumptions are insufficient.
`func_800878E0` receives one 32-bit flags value in `a0`, copies it to `t0`,
and preserves that across mode calls; `t1` similarly holds the mode-global
address. Neither root needs an invented extra formal argument.

The native initializer increments the signed bank index and resets values
>=2 to zero; safe expected inputs are bank 0 or 1. It scales by 75*256 =
19,200 bytes, represented naturally as 2 banks of 2,400 eight-byte Gfx.
No claim is made about negative bank indices or allocated physical storage
ownership. Each command cursor advance is eight bytes. The original word
stores zero both halves of `D_8012E688`. Independent reads/writes in
`object_render` compare a sign-extended 32-bit value against both halves,
and later store a sign-extended pair; this supports signed 64-bit state,
not a pair of unrelated scalars or a floating-point constant.

The flags routine first returns if all requested bits are already set;
otherwise it ORs them into the global and tests the **full requested mask**,
not only newly set bits. 0x4000 emits a combiner command, conditionally resets
cycle state for modes <=0 or >=4, and sets mode -1. Bit 1 emits its command;
bit 0x10 emits its command and calls the current mode; 0x20 calls it again.
Unknown bits are still retained. No lookup tables are needed in either root.

## Diagnosis and stopping condition

Workbench diagnosis of the O3 flags root finds a pure register allocation
residual: identical 74-instruction structure, 24-byte frame and 24/24 temporary
ring sequence; three webs differ (t0->a2 four sites, v0->v1 sixteen sites,
t1->a3 nine sites). Its raw-object relocation warnings are expected because
the native object has resolved words; `score.py` supplies the authoritative
fully resolved 29/74 count. The initializer also has a scheduled 64-bit zero
store difference plus changed address-register allocation. The real closure
removes the flags root's frame/excess/structure defects but does not recover
the original allocator pool. Stop here; a future experiment needs actual
additional caller/context evidence, not fake register pressure.

## Reproduce

Use existing IDO 5.3 via `IDO_DIR`, then:

```
python cloud/work/dot_graphics_init_closure/verify.py
python -m pytest -q tests/conveyor/test_graphics_init_closure.py
```

The scorer includes the R4300 multiply workaround. `verify.py` leaves raw
objects only under ignored `build/`. Host tests cover 10 initialization and
idempotence cases, 28,672 flag/mode/prior-bit cases, and 10,000 full-width
random cases. The mode callee is an event-logging stub in these **contract**
tests; this verifies the roots' effects and order, not the helper's semantics
or the target machine. The same executable passes AddressSanitizer and UBSan
with `ASAN_OPTIONS=detect_leaks=0` (LeakSanitizer cannot run under this executor's
ptrace); no leak claim is made.

## Independent review

A separate reviewer replayed `verify.py` byte-for-byte against the JSON receipt,
ran the pytest wrapper and a separate ASan/UBSan build, and audited both full
native bodies and the entry/call ABI. PASS as NONMATCH research. Review caught
and corrected the initializer's signed declaration for the shared flags global;
all source units and the harness now consistently declare it unsigned. The
reviewer confirmed the accepted mode source remains unchanged. Four canonical
scorer preflight matches (`sound_handles_clear` and the three resource-slot
group members) also pass in this environment.
