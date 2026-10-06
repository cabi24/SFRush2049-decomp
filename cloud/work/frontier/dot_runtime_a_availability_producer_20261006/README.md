# Runtime-A availability producer: 177/180 words

Frozen base `f2e8380d3b368d00efc9cac3e37cdc7059b1f6e2`.
Stock IDO 5.3 O3 group pipeline and actual
`-g0 -O3 -mips2 -G 0 -non_shared` flags.

Complete `func_80394564` (720 bytes) differs at only **3/180 words**. No extra
words, unresolved symbols, unverified relocations or relocation errors occur
for this producer. The initial complete reconstruction was 5 words off; placing
its real loop store on a separate source line resolves two scheduling words.

All remaining differences are the three entry reads of the player selector:
native a1 versus candidate a0. Every subsequent instruction word matches.
The native caller context is explicitly included, but no fictitious first
formal is added to force a1. `claims` is empty; this is NONMATCH research and
zero accepted-byte credit.

The producer recomputes two 14-byte option-availability rows for one player or
all players, applies the mode 4 overrides and counts enabled entries. Rows use
signed byte flags/counts and real native mapping/state arrays. There is no
owned literal data in the producer.

## Genuine caller context

The source includes full real 97FFC (1,072-byte setup caller) and 94B00
(728-byte state-transition caller). Their observed results are 251/268 words
differing with nine extra words and four unverified own-data sites for 97FFC,
and 162/182 differing for 94B00. They remain unclaimed research; the retained
root's missing outer context and other private-call boundaries are not hidden.
In particular 97AD8's native stack-passed private parameter remains an ordinary
external declaration here, rather than an invented multi-argument signature.

94B00's fallback player mapping is expressed as the observed player ID, without
manufacturing pointer arithmetic solely to reproduce redundant native shifts.
The group includes no fake callback table, synthetic caller, forced register,
unused frame filler or artificial volatility. Named types describe accessed
layouts, not proven original source declarations.

The two non-integer setup literals 1.43f and 1.1f were read from the authenticated
runtime-A image. The source has complete control flow and no computed-table
mapping gap; the caller's shifted layout prevents owned-data acceptance.

## Reproduce

With stock IDO and GNU MIPS tools configured:

```
python cloud/work/frontier/dot_runtime_a_availability_producer_20261006/reproduce.py \
  --repo . --reference-root .
```

The small script reads the fixed-base existing asset in memory, checks its
length/SHA-256, loader pointer and runtime image identity, then uses the normal
scorer's owned-data binding. It emits no native binary or dump. Matching
compilation/scoring and residual diagnosis only; no acceptance suite, proof
packet, CI wait, lock, production edit or promotion is included.
