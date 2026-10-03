# BT07 larger pair: whole-body reconstruction

Base 301d9e7552ad4fd7f54a38796db84671e1000d35. Exact pair was activated by the
coordinator before source editing: 800259A8 (268 B) and 80025C68 (284 B), 552 B.
This packet changes only these candidate sources and this packet directory.
Central claims/status and D10 are owned by the integration lead.

## 800259A8

Two actual inputs are consumed: an unsigned stream token and a float output
pointer. A token of all ones or a zero enable byte returns floating zero before
lookup. Lookup 25264 returns the signed index or -1; failure also returns zero.
The successful path obtains the queue token from 250F0, then selects the genuine
4,648-byte stream record. For busy state 2 it converts unsigned +4636 and +4616
to floats and divides; other states produce zero. In both successful-lookup paths
it copies float +4640 to the output pointer, returns the queue token through
25120, and returns the computed float. Native frame is 32 B: saved return at +20,
ratio spill at +24, actual input/index home at +32, output pointer home at +36.
There is no local rodata: zero and the standard unsigned-conversion 2^32 constant
are synthesized with immediate integer instructions. No original literal bytes,
table placement or unknown object contents are guessed.

## 80025C68

One actual unsigned flags input is stored to D_800586A0; D_8005868C is cleared.
It walks the two consecutive 4,648-byte records. Each receives 25EB0 with four
null/zero trailing arguments, then 24FB0, then busy +4608 is cleared. After both,
250AC creates the single-token queue. If flag bit zero is set, 1C770 transforms
constant 4320 to 8648 bytes. The allocator hook at D_80038000+24 is called twice
with that size and a zero second input; both results are stored to D_80058698
and each is passed with the same size to static osInvalDCache at 800084E0.
Finally the real no-input callback 2574C is stored to D_80038024, the enable byte
is set, and the next-token counter D_80058680 is zeroed. Native frame is 48 B,
with s0/s1/s2/s3 and return saved. No switch table or local literal relocation.

## Context and provenance

These are native reconstructions from the protected repository targets, with
actual caller/callee contracts cross-checked against the prior frozen BT07
packets. No external source is imported; arcade reference source is unavailable
in this repo-only environment. Names are behavioral hypotheses. The local stream
view preserves earlier offsets and stride; its unknown arrays represent genuine
object storage, never stack padding. The busy and enable volatile views follow
the prior callback/poller evidence, without asserting original qualifier spelling.
The allocator/release hook slots agree with the actual initializer 20598 and
shutdown 25DC0. No production shared header is changed.
