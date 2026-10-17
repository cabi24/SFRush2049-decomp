# Elapsed-timer blit: original source flow and owned position data

Research-only recovery for historical N64 label `checkpoint_hit`, address
0x801073D8, 580 bytes. Its supported identity is `animElapsedTimer`, the blit
half of the elapsed-time display. The genuine retained text sibling is
`animElapsedTimerText`, 0x8010761C, 1,232 bytes.

The blit now emits exactly 145 words. Eight differ, all fixed stack/frame
immediates; every other instruction field agrees. Its frame is 128 bytes
versus native 144. This remaining 16-byte gap has no authenticated source
explanation. No padding local, forced register, fake caller or added volatile
qualifier was introduced. **Claims are empty and new accepted credit is zero.**

## Original source and native adaptation

The available MAT3 counterpart is `animElapsedTimer`, 0x8001F39C, 388 bytes.
Original DWARF records an int return, one Blit pointer, then exactly these
locals: int win, digitW, colonW, and signed char fixedWidth. The complete
source restores the whole animID assignment, two genuine Hidden calls,
font selection, width measurement order, signed-byte restoration and the
centering expression. The first hidden path clears animFunc; the second
leaves it intact. There is no masked animation ID.

The older MAT3 report lists 100%, but current hud.c is newer and carries a
partial-source freshness limitation. This packet uses saved source/type
evidence; no fresh donor matching verification is claimed. The source SHA256
is 93fc1f0f95d96be48706e68acf92a76c76e354197e6255fbed1227b603fe9924.

The GC source assigns alpha 128 and has no depth assignment. The N64 body
independently supplies the HUD alpha byte and depth 100. Keeping the original
alpha assignment before this native-only depth store removes four register
allocation differences. Reversing that source order is a diagnostic causal
control; it is semantically equivalent in the host trace and is not labeled
a behavioral bug.

The real Hidden body returns int and updates the blit only when its signed
hide byte changes. The character-width API is unsigned int(unsigned short).
The fixed-width API parameter and result are signed char, independently of
the font header's unsigned nonProp field. Queue types are opaque; the shared
font header and resource slot are 16 and 20 bytes, spacing/leading are signed
bytes, and foreground/background storage is eight bytes. Six target-compiler
checks verify this callback's Blit extent and used field offsets plus table
extent. The full 64-byte Blit view replaces the old misleading short record.

## One genuine owned static, two actual readers

Original DWARF DIE 0x65277 and the original ELF local-object symbol establish
file-static int elapsedTimerPos[4][2], 32 bytes, with no qualifier. Both elapsed
callbacks read it. The original GC initializer is {{272,144},{272,144},
{240,200},{160,136}}. Those values are not transferred to N64.

The authenticated native data window at 0x80115C88 independently verifies
{{272,142},{272,142},{240,197},{160,134}}, SHA256
c528ed12076d15a71b70f5cd0c4c23648859690a2f5c19a8e6bea9ec04b93a61.
The verifier requires the complete 32-byte initialized window for both
readers. All masked operands must have owned-data proof. It does not claim
neighboring storage or a complete translation-unit data layout. The wrong
GC initializer leaves the blit's eight instruction differences unchanged,
but fails actual content ownership; a mismatch count alone is insufficient.
Externalizing the table also fails this packet's ownership proof.

The actual existing locked text sibling is retained, rather than inventing a
caller or duplicate reader. It remains strict 308/308, including the shared
position object and the actual eight-byte justification object. Its font and
justification calls use the single genuine existing implementations. Its
historical unused formal and remaining body spelling are inherited from the
locked N64 reconstruction; this work does not authenticate that formal.

## Complete bounded graph and inherited failures

All 26 known native bodies are explicit members, with no hidden context list.
There are 22 strict bodies and four partial bodies:

- checkpoint_hit: 8/145 differing words, no excess words or ownership errors
- object_bytes23_sum: 4/18 differing words
- object_bytes_sum_global: 4/21 differing words
- world_velocity_integrate: 158/178 differing words, three excess words and
  two unverified own-section references

Three additional eight-byte emitted inlining stubs, Hidden, du_gfx_lock and
 du_gfx_unlock, are separately inventoried. They are not independent native
body claims. The existing font/resource context still contains disclosed
empty/unreachable inline-blocker syntax in ctx_gr3_b.c, peer_gr3.c and
peer_slot_group.c. The new donor does not authenticate those inherited devices.
HAL_SetFont's actual lock/set/unlock source and retained reset/resource graph
are used unchanged. External renderer/text calls remain actual boundaries;
this is not a complete transitive implementation.

A private 943-file source-shadow preserves the blit's eight differences and
the strict text sibling. It deletes HAL_SetFont's standalone body (28/28
failure) and changes the inherited replay result to 157/178 with two
unverified references. Both font-height partials remain. These results do
not establish an installable source boundary or complete-unit acceptance.
No canonical installation, ROM build, full-image proof or CI watcher is used.

## Reproduce the narrow proof

Configure the repository's existing authorized IDO/MIPS tools, then run:

    python cloud/work/frontier/dot_elapsed_timer_research_20261017/verify.py
    python -O cloud/work/frontier/dot_elapsed_timer_research_20261017/verify.py
    python -m unittest tests/cloud/test_dot_elapsed_timer_research.py
    python cloud/work/frontier/dot_elapsed_timer_research_20261017/host/replay.py

The verifier pins every source, all 26 native targets, comparison tools,
proof implementation, saved native receipts and fidelity report. Twelve
existing compiler components, including frontend/driver components, are
fingerprinted. Explicit RuntimeError checks remain active under Python -O.
The full stock object, retained-stage stock object and observation-only
traced object share SHA256
5208f3fb45144fb35b6f42580357a1dbbb35bd5e11a9d41e3973181d108b44f5.
No compiled object, private trace, ROM content or native instruction dump is
included in this packet.

Seven current-source negatives reject: masking animID, failing to restore
fixed width, wrong native depth, wrong alpha, reversed native field order,
wrong GC position data, and externalized position data. All unrelated 24
native receipts remain unchanged. Moving the real Hidden definition after
its callers preserves every native result. Error receipts scrub compared
byte values. Seven standard-library guards run with the toolchain available;
only the two environment-dependent guards skip when it is absent, while
five source/proof/report/scope guards still run. With REQUIRE_TOOLCHAIN=1,
a missing toolchain instead fails both environment-dependent guards.

The separate host differential compares the prior expanded visibility source
with the recovered Hidden/win source over 8,192 deterministic scenarios. It
uses the authenticated native positions and checks update/font/width/fixed
width call order and returned/modified fields. Both traces agree at SHA256
afeb8456ff62606f305171a948fd261dff136afd26d70d903a88f8e9164ba17d.
Four behavioral mutations reject (246, 2,979, 2,987 and 2,976 scenarios).
This is source-version differential evidence with test stubs, not an
independent executable specification or N64 ABI/matching proof.

Independent review and merging remain separate from this research draft.
