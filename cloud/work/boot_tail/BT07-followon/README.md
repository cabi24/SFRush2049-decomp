# BT07 follow-on packet

Five strict local matching C bodies, **804 native bytes**, and one complete
**NONMATCH**, 128 bytes. This is matching-source research, not cartridge coverage
or promotion. Independent source/ABI replay and exact aggregate-head CI remain
required before VERIFIED-BODY credit.

- Fresh base: `301d9e7552ad4fd7f54a38796db84671e1000d35`.
- Branch: `dot/boot-tail-bt07-followon`; central exact claim: `3c55e780`.
- Six final fresh 64–255-byte BT07 rows: 2506C, 254D4, 25594, 255F0, 25670, 25DC0.
  Their total is 932 B. Earlier seven small and six medium targets stay excluded;
  the prior 24FD4 residual was not reopened.
- Only the five matching sources and this packet directory are edited. Central
  STATUS, claims, totals and D10 belong to the lead; D10 remains unchanged.
- The already approved IDO 5.3 installation is reused read-only. All 24 file
  hashes equal the pinned prior setup receipt. Its archive SHA-256 is
  `ab5c741561f80913d58c8b074771f23941a3edd312505a8ebed6d1dfeb65e506`.
- All three protected manifest entries pass; all 439 extents and 99,120 bytes
  equal the inventory. The existing getter is still strict MATCH and adds no credit.

## Results and native optimization evidence

| Address | Size | Supported behavior | O2 | O1 control |
|---|---:|---|---|---|
| 8002506C | 64 | Forward transfer request, then jam null message onto supplied queue | MATCH | 13/16 + 1 extra |
| 800254D4 | 192 | Stop selected stream according to busy state and ownership flag | MATCH | 48/48 + 48 extra; truncated-window HI16 error |
| 80025594 | 92 | Gate and look up token, stop found stream, return success | MATCH | 23/23 + 4 extra |
| 80025670 | 220 | Gate token update, pass four byte settings to handle, store three fields | MATCH | 52/55 + 31 extra |
| 80025DC0 | 236 | Stop both streams, wait, disable callback, release owned buffers | MATCH | 58/59 |
| 800255F0 | 128 | Gate and look up token, return normalized busy byte | NONMATCH 2/32 | 27/32 + 5 extra |

Each O2 match has zero differing and extra words, masks, unresolved or unverified
references, and relocation errors. Every complete relocated native body equals
its protected target, and object trailing alignment is zero. The unchanged scorer
adds `-Wab,-r4300_mul`. O2 is evidenced by the shared indexed addresses, argument
home reuse, branch-likely guards and the shutdown loop's common callback pointer.
O1 fails every body; 254D4 expands beyond its target window, so its O1 negative
control has an unpaired HI16 at that truncated window. This is recorded unchanged,
not suppressed or repaired, and is absent from its accepted O2 body.

## Actual object and call ABI evidence

These sources are native reconstructions. No external implementation was imported.
Names describe hypotheses; no original middleware identity is claimed. The local
partial `StreamState` has the same 4,648-byte stride as the earlier BT07 packets.
Busy is at +4608; byte settings +4609/+4610/+4611; handle +4620; buffer pointer
+4624; request-state byte +4632. Unknown arrays model genuine object storage,
never stack padding. Native repeated asynchronous reads in the prior poller and
callback packets support the volatile busy/enable views. Original qualifier
spelling remains unproved. Production headers and previous sources are untouched.

- **2506C:** this is an indirect callback. `252AC+0x1BC–0x1C0` materializes its address
  and `252AC+0x1D8` supplies it to 25EB0 as the real fifth argument. 25EB0 stores
  it at stream+400. `25F74+0x144` calls that slot with source address, destination
  address, count and embedded queue. The first two inputs are reversed when
  forwarded to 10714. That actual callee stores its three inputs into 12-byte
  transfer records and rounds the count up to 16 bytes. The callback then calls
  counted-static `osJamMesg` at 800075E0 with queue, null message and blocking flag
  one; the declaration agrees with `include/PR/os_message.h`. The callback's caller
  does not consume its v0, so the wrapper is void. No unused invented formal exists.
- **254D4:** 25594 supplies the successful lookup's signed index; 25DC0 supplies
  indices zero and one. Busy=1 clears the byte and, unless flag bit zero is set,
  calls the actual indirect release hook at D_8003801C with the buffer field at +4624. Busy=2
  gets the real integer queue token from 250F0, calls 26348 with the selected
  record, writes busy=3/request-state=4, and passes the saved token to 25120.
  All three actual callees were independently reconstructed in the frozen BT07-small
  packet; the token has a genuine caller/callee ABI even though its value is zero.
- **25594:** one token input is consumed. The sentinel -1 is rejected before the
  enabled-byte read; enabled calls pass the token to 25264. That actual callee
  returns an index or -1. A valid index is passed to 254D4; explicit v0 paths return
  zero/one. No caller is identified in the tail inventory, so public API identity
  is unresolved; argument and result behavior are proved by this body and callees.
- **25670:** five actual inputs are read: a token and four unsigned byte settings.
  The fifth byte is loaded from the caller's argument-home slot. 25264 supplies
  the signed index/-1. A valid index is held in the input word's real home across
  250F0. Handle+4620 is checked against -1 before calling 1C77C with the handle and
  all four bytes, including a real fifth stack argument. 1C77C itself reads those
  four byte homes and shifts them by 16 before forwarding to 14A74. Afterward
  fields +4610/+4611/+4609 receive inputs 3/4/2 and 25120 receives the queue token.
  The wrapper returns zero or one. No extra formal or stub context is used.
- **25DC0:** no incoming argument is read. It stops indices zero/one, invokes the
  actual polling helper 25D84, clears the enabled byte, enters 14594, clears the
  callback word D_80038024, resets through 1061C, and exits 145DC. The latter actual
  bodies implement queue-backed nesting/unnesting; 1061C clears D_80038020.
  It conditionally releases D_8005868C when D_8002D484 is set, then releases both
  D_80058698 pointers when flag bit zero is set. `20598` copies eight supplied
  words into D_80038000, proving the 32-byte hook table. `25C68` uses its +24 slot
  to allocate both buffers and stores those results into D_80058698; this body
  consumes its +28 release slot with one pointer and ignores v0. The allocation
  prototype follows that observed two-argument call; original hook definitions
  remain external. No external destination bytes or unproven boundary were used.
- **255F0 nonmatch:** one real token input and normalized zero/one result. The
  complete best source reuses the token word for the actual signed lookup result;
  its sentinel comparison uses the corresponding unsigned all-ones value. It
  matches all geometry and references but computes the final boolean in a
  temporary register before moving/narrowing into the result register.

## Directed work and stopping point

2506C, 25594 and 25DC0 matched their first source forms. The other three had
complete initial bodies and O1 controls. Workbench diagnosis preceded refinement:
254D4 had unchanged geometry with pointer/token homes and final store/load order;
25670 had identical opcode/register flow with an 8-byte larger frame; 255F0 had
pure result-register allocation differences after its input word was reused.

The successful natural form for 254D4 and 25670 uses the real indexed array fields
instead of declaring a separate pointer local. IDO preserves their common pointer
itself, closing native spill homes, scheduling and frame size with no added
operation. Both sources retain only the genuine queue-token local. Negative
controls covered real pointer lifetimes, declaration order, field/enable views,
input signedness and lexical initialization. Register hints and equivalent index
spelling were measured as ineffective. Captured busy values were real consumed
switch expressions, but did not match and were not submitted. No padding local,
dummy call, false formal, empty context body or compiler/target modification exists.

The 255F0 hypothesis reached **20 directed O2 forms** (one initial form and 19
refinements); it stops at 2/32 words. 254D4 and 25670 used 18 forms each, including
negative controls, and have exact matching sources. Fixed O1 controls and final
replays are not additional tuning. Every trial is in `experiments.json`, with no
raw words, disassembly or object files. The next 255F0 hypothesis requires authentic
caller/result-type or source-lifetime evidence to explain the boolean register,
rather than more spelling changes or fake allocation pressure.

## Reproduction and tests

With an existing pinned toolchain selected through `IDO_DIR`:

```sh
python3 cloud/work/boot_tail/BT07-followon/verify.py
python3 cloud/work/boot_tail/BT07-followon/test_semantics.py
```

`input_pins.json` binds compiler and protected/scoring inputs. `verification.json`
binds final source, target bodies, scripts and this note. The verifier is read-only.
`test_semantics.py` compiles 32-bit C89 size/offset assertions, then links the five
actual source files into a native-width ASan/UBSan harness: **2,169 cases** cover
all 256 busy states, both stream indices, ownership flags, sentinel/gated lookups,
five byte boundary values, real fifth-argument forwarding, transfer/queue order and
shutdown release order. The fixed-compiler match remains the 32-bit proof. The
host harness is complementary and does not execute the target or a ROM. Temporary
harness stubs represent callees only in testing. LeakSanitizer is disabled under
ptrace; ASan and UBSan remain enabled. No cartridge coverage or runtime execution
is claimed. Independent review and exact aggregate-head CI remain pending.
