# BT07 stream pair: whole-body reconstruction

Base: 301d9e7552ad4fd7f54a38796db84671e1000d35. Central exclusive claim
b5723bfa activates 80025AB4 (436 bytes) and 800252AC (552 bytes), 988 bytes.
These observations were written before candidate source creation. Only the two
matching files and this packet directory are writable; central status and D10
remain the lead's responsibility.

## 80025AB4: table binding/loading hypothesis

The function consumes two genuine address-valued arguments and returns 0/1.
It returns zero if the enable byte at D_8002D480 is zero. When the current table
D_8005868C is nonnull and ownership byte D_8002D484 is nonzero, it calls the
release hook at D_80038000+28 with that table. It then tests the high nibble of
argument one against 8, identifying a CPU-address-shaped input.

For other high nibbles, it calls the one-argument translation hook at +20 with
argument one and stores the result to base D_80058684 and data base D_80058688.
It allocates four bytes through the two-argument +24 hook (size, zero), returning
zero if allocation fails. Synchronous transfer 80010628 receives allocation,
base, and 4, in that order. The transferred word becomes count D_80058690; the
allocation is released. A second allocation of count times four becomes the
table; failure returns zero. The second synchronous transfer copies count times
four bytes from base+4 to that table, ownership becomes 1, and the return is 1.

For high nibble 8, it stores argument one as base, translates argument two for
the data base, binds table to argument one+4, reads its first word as the count,
clears ownership, and returns 1. There is no table-content reconstruction:
callers supply the actual data. No local rodata, switch, or float literal.
The 32-byte frame saves return at +20, temporary four-byte allocation at +28,
and preserves actual input arguments through their genuine homes at +32/+36.

## 800252AC: stream startup hypothesis

There are seven real input words: unsigned table index, optional unsigned rate,
and five low-byte option values. After enable/range checks, it searches two
4,648-byte records for the first zero busy byte +4608. Failure returns all ones.
The selected record gets a two-message queue at +4576 with messages at +4600.
A zero rate chooses 16,000 if the selected table word's high byte is nonzero,
otherwise 8,000. Buffer count is ((rate*8 / 30 + 159) / 160) * 160 with unsigned
32-bit arithmetic. It stores rate +4616; options to +4612/+4609/+4610/+4611/+4633.

If D_800586A0 bit zero is set, it binds the existing per-record buffer from
D_80058698. Otherwise it computes allocation bytes through actual helper
8001C770, allocates through hook +24, stores the buffer, and invalidates cache
for that size, even if the returned pointer is null. A null buffer returns all
ones. Otherwise it takes the queue token from 800250F0, calls the five-input
80025EB0 initializer with record, data-base+(table[index]&0xFFFFFF), buffer,
buffer count, and actual four-input callback 8002506C. It then calls 80024FB0,
stores handle=-1 and busy=1, returns the queue token through 80025120, and returns
800251A8(selected). The 72-byte frame saves s0/s1/return, with actual allocator
size and queue-token spills; no fake locals or formals are needed.

## ABI provenance and limitations

Protected committed native bodies are the only reconstruction input. Names and
original type spelling remain hypotheses; no third-party source was imported.
Read-only matched context: BT02-larger 80010628 at da2834a8; BT07-medium
80025EB0/800251A8; BT07-small 800250F0/80025120; BT03-high 8001C770; prior BT07
callback 8002506C. Transfer is truly (destination, source, unsigned size), and
2506C reverses its own (source, destination, count, queue) before invoking the
asynchronous transfer. 25EB0 has five actual inputs, not a synthetic stub.
The stream view must preserve all earlier sizes/offsets. Unknown arrays denote
real global record storage, never stack padding. The genuine second queue and
its two-message storage refine the previously opaque bytes +4576 through +4607.
