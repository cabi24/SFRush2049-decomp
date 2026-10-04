# Caller-only reconstruction and contracts

This source-free note preserves conclusions from the pre-implementation read-only
ABI audit, /tmp/boot-tail-decoder-facing-abi-closure.md (lost in filesystem reset),
and the selected own-body inspection. The original receipt was read before any
source write. The note is recreated, not an immutable original receipt.

## Native input pins and scope

The audit's final central tree was 83ab900c10e65057f1c5e29f7e52dd80aad3e6bd;
all inputs agreed with the unchanged baseline 301d9e7552ad4fd7f54a38796db84671e1000d35.
The actual implementation reused a separate clean checkout at eae0295b.

- 8002574C, 604 B, target SHA256
  19d7bc111c647cce08476953e530bcd26f2cb4ce36a54e7bcbc3f420ccc603d4
- 80025F74, 840 B, target SHA256
  7650ca0132961f8bcc14735c6797092a1b4fd0b4c40ff3031b2e512204aa5896
- 800268D0, 3840 B, interface-only audit target SHA256
  68813deb46ea749a4a353975a5c2f7a39310cb27b9b819aacaacb9b1c7c5c811
- boot_tail_8000f3a4.s SHA256
  0dc55ce51ebe828207816f4f0b878b8928da64694f7f247bf8749f68cf91b224
- extents.json SHA256
  0d69358c528171a2c53e7468dd5b48afe7611006ab3800dbfbb10e079b008fcf
- symbols.json SHA256
  bd200aeb947e5da64c9e810dc6fbe15ef5c3f2f26633071cf7363cdb1b1fcba9

Only the two named caller bodies are source-eligible here. No decoder algorithm,
T050 opening, out-of-census destination body, text at/past800277D0, 800D1248 or its
helpers was inspected in this implementation. No source from outside the repo.
The audited decoder entry/exit/interface metadata is not decoder reconstruction.

## 8002574C: actual no-input callback

Reviewed 80025C68 installs it in D_80038024 and reviewed 80013DEC invokes the slot
without inputs or consuming a return. The caller can therefore be void(void).
When D_8002D480 enables it, it obtains the queue with no-input 80025150, processes
exactly two 4648-byte D_80056230 records, and releases with no-input 8002517C.
The release body is excluded corpus-owned and is only externally declared.

Every nonzero busy record first invokes 80025F74(stream), then re-reads busy/state.
Busy 1/state 2 computes unsigned(blocks)*160.0f/unsigned(rate), resets processed,
sets busy 2, calls one-pointer 80026328, and invokes real ten-input 8001C580:
byte request, short-buffer pointer, unsigned sample count, unsigned frequency,
four byte settings, genuine five-input callback and unsigned opaque context.
It passes 80024FD4 and selected index 0/1. Its actual callback type is
(pointer, unsigned count, pointer, unsigned count, context word), integer result;
unused pointer formals are real callback ABI inputs, not invented parameters.
The returned handle is stored. On -1, 80026348 sets state 4; hook +28 releases the
current buffer when D_800586A0 bit 0 is clear; busy is cleared afterward.

Busy2/state 4 sets busy 3 and request-state 4. Busy 3/request-state 0 conditionally
releases then clears busy. At nonzero request-state, value 2 calls 8001C7F4(handle),
stores handle -1, then re-reads the byte before decrement/narrowing. Other branches
retain the observed no-op behavior. The four globals and callback address resolve
through protected symbols or the existing address-named fallback. Float 160 and
unsigned-to-float correction are immediate sequences, not local data references.

## 80025F74: actual one-stream-pointer caller

Its sole caller is 8002574C and ignores any result; it is expressed void(StreamState*).
The transfer callback at +400 has the genuine(source, destination, unsigned count,
queue pointer) interface established by initializer 80025EB0, publisher 800252AC and
matched 8002506C. The callback/decoder may mutate stream fields; reloads are real.

1. Snapshot signed byte budget before external calls. If pending, poll queue+4524
   with nonblocking osRecvMesg. On successful receive and received==0, extract
   high/low halfwords from input-ring word +420, remaining-input low24 bits from
   word +424, and set bit position 96. Every successful receive adds 1024 to the
   unsigned received counter, saturates remaining-input by subtracting 256 only
   when greater than 256, and clears pending.
2. When not pending, state 1/2/3 and unsigned(received-consumed_input)<=3072,
   request 1024 initially or 4*min(remaining_input, 256) later. A positive request sets
   pending before callback. Source=input+received; ring destination is +416 plus
   ((received%4096)/8)*8; queue is +4524.
3. While unsigned(available-consumed)<unsigned(capacity-160), state1 or positive
   halfword remaining-blocks, at least 37 input bytes and positive signed budget,
   invoke int 800268D0(stream, output+8*((available%capacity)/4)); discard result.
   Re-read decoder-mutated bit position and set consumed-input=(bits>>3)&~3U;
   re-read available and add 160; re-read/decrement/narrow block count; if zero,
   re-read capacity and store it into signed remaining. Decrement local budget.
4. State 3/4 with nonpositive unsigned halfword count (therefore zero) fills 320
   bytes per iteration under the same unsigned output-space and signed-budget
   gates. Re-read output count/capacity after bzero and retain unsigned addition.
5. State 1 becomes 2 once buffered output reaches unsigned(capacity-320).

The external decoder interface is established by an earlier bounded 160-byte entry,
64-byte exit and stack/control-transfer audit: a0 is stream; a1 is saved at entry
SP+4 and later read; no incoming fifth/later input exists; a2/a3 are assigned
internally. Frame 248, sole indirect transfer is return, explicit integer-zero result.
This supports int(StreamState*, void*) without inferring a void decoder from the
caller's ignored result. Its unknown 400-byte state remains opaque.

## Local data view and valid domain

Native offsets: callback 400, input 404, output 408, capacity 412;4096-byte ring 416..4511;
unsigned halfwords 4512/4514; unsigned remaining-input 4516; signed remaining 4520;
queue 4524..4547, message 4548; bits 4552, consumed-input 4556, received 4560, consumed 4564,
available 4568 all unsigned; pending4572, state 4573, budget4574 signed bytes;
request-queue 4576, request-messages 4600; unsigned busy 4608, byte settings 4609..4612,
rate 4616, handle 4620, buffer 4624, count 4628, request-state 4632, mode 4633, processed 4636,
float 4640, token 4644; record stride 4648. Header word storage is exposed intentionally.
No shared production-header edits are needed.

Use aligned allocated stream/input/output storage and registered callback/queue
lifetimes. Normal initialized playback uses receive increments 1024, capacity/sample
positions aligned 160, positive sufficient capacity and finite positive rate.
The aligned ring destination fits its 4096-byte buffer and each 160-sample step fits
normal two-byte-per-sample allocation plus the 8 extra bytes from 8001C770. Prior
arithmetic checks over capacity 320..8000/step 160 are not runtime or decoder tests.
No malformed-input, zero-capacity, underflowed occupancy, invalid-float, concurrent-race
or decoder-safety claim. Modulus retains the native zero-capacity trap behavior.
Mocks must permit helper mutation; they are interface contracts, not a guessed decoder.

## Compiler experiments before reset

Initial whole bodies were O2 15/151 and 92/210 differing; O1 expanded. Workbench
ran before refinement and again at the 2-word 25F74 plateau. The final 2574C complete
array-index loop preserves actual ABI and matched 0/151, ELF 604 B. A pointer loop had
three preheader schedule differences and twelve outgoing-stack-argument schedule
differences; direct indexing removed those. Genuine parameter locals regressed;
word-width/unprototyped external diagnostics were inert and rejected, not retained.

25F74 high-halfword shift/mask, one-line callback expression and <=zero test reduced
its residual to 2/210, ELF 840 B. They are solely an adjacent copy/store ordering swap
at +0x2F4/+0x2F8. Unmodified as1 -R source-line/dependency trace was read. Same-line
update/decrement equalized the line keys but did not change ready-list order.
Explicit fill cursors/loop operands and split budget guard regressed; assignment,
for, comma-step and byte-output-offset forms were inert. Final source retains the
clear byte-offset form. Work stopped within 20 directed variants, with exact replay
of initial/final sources required after recovery. A source/loop-form lead or a
measured scheduler explanation is needed, not more blind mutations or ABI guesses.
