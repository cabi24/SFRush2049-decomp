# BT05 core wait and constructor pair

Activated at central claim `b4da573c`: `8002193C` and `80024988`, each608 B.
This packet uses the existing isolated repository on `dot/boot-tail-bt05-core-pair`,
explicitly stacked on the frozen, independently reviewed resource head
`e6b304128ecaee3fb5c200d97fa1132aaad2fd95`. Only this owned packet changes.

Both whole sources are **COMPLETE-NONMATCH** research, totaling1,216 B and zero
matching credit. No matching submission is added.

## Actual source and native contracts

`2193C` consumes the dispatcher's real state and aligned two-word command. Its
packed state uses flags+36, identifier+96 and three unsigned timing words at
+156/+160/+164. The original nonzero high-halfword duration guard precedes every
random remainder. A later random result may be zero. The FFFF duration sentinel
sets deadlineFFFFFFFF; otherwise the command's mode bit selects the actual
address-taken duration conversion helper. Millisecond mode adds to the live
clock; beat mode adds to the base, advances an expired base and clears its
wait deadline. Unsigned word additions intentionally wrap.

The two command gates preserve existing state bits and their native early
returns. The playback query receives the identifier's low byte as a genuine
word argument. `1467C` reads the selected native playback record; `1E790` has no
inputs and returns an unsigned halfword. `1E930` consumes a real word pointer;
`1E940` consumes that pointer and the real state. Conversion details remain
external, including the registered state and nonzero external tempo domain.
The returned byte is a pending-wait predicate. Its initialized result local
carries this actual value through the zero-duration path; it is not a frame
padding object.

`24988` has eleven genuine O32 inputs: packed word, allocation halfword, key byte,
volume byte, panning byte, channel byte, set byte, offset halfword, section
halfword, start byte and group byte. Its native72-byte frame reads the last seven
arguments from old sp+16/+20/+24/+28/+32/+36/+40, with the corresponding byte or
halfword extraction. The previously reviewed resource caller forwards all eleven.
No parameter, floating context or helper implementation is invented.

The actual `16C20(u16)` result is a macro-program pointer or null. The real
four-input `1F13C(u8,u8,u16,u8)` returns a valid registered slot or -1. The
416-byte packed voice record has program pointers at0/4/8, link identifiers at
16/20, flags36, age40, age speed44, external byte76, note halfwords78/80,
volume/panning/channel/set/section bytes82..86, identifier96, macro halfword100,
deadline156, allocation halfword186, group191 and detune192. Untouched byte
arrays describe these actual record spans. Three native32-bit pointers make
host LP64 layout different; host tests are behavior checks, not416-byte proof.

The initializer preserves flag0x10 in the reset flags, queries actual playback,
then handles key bit128 as the external mode. That branch clears the key's
high bit and calls the real two-byte-input `20820` before installing its channel
and category. It initializes the program and valid command offset, note/volume
fields and sentinel links, constructs the internal identifier and calls the
real priority helper. The section halfword is intentionally narrowed to the
native byte field.

Return domains matter: when start is zero, the live voice identifier after the
priority helper is returned. When start is nonzero, the genuine one-pointer
`1ECE0` allocates a sequence node and returns its distinct queue key, orFFFFFFFF
on exhaustion. No rollback or invented fallback is added. This helper's actual
body and native16-byte sequence-node protocol were independently reviewed in
the paired chains packet. Program offsets, allocated slots, and helper callbacks
must preserve their valid object domains; malformed resources are not promised
safe.

## Bounded diagnosis and controls

O2 preceded O1. Unchanged workbench diagnosis ran at the initial and retained
checkpoints. `diagnosis_summary.json` contains only source hashes and numerical
classification counts; native listings and objects remain temporary.

- Wait starts146/152. A guarded block yields100/152 but two nonzero excess words;
  returning from each timing branch gives128/152. The natural initialized pending
  result yields103/152 with no excess and is retained. Splitting the mode-byte
  extraction from its mask leaves that score unchanged. Five total forms, stop.
- Constructor starts135/152. Spelling the actual external flag as a conditional
  1/0 yields93/152, removes the extra saved key register and is retained. Explicit
  macro-ID narrowing and a halfword macro local worsen to144/152+2 and147/152+1.
  Four total forms, stop. The remaining frame is80 rather than native72 and the
  local/argument register allocation differs; no padding or declaration sweep
  attempts to force it.

| Function | Retained O2 | O1 control |
|---|---:|---:|
| 2193C |103/152|152/152 +5 extras|
| 24988 |93/152|151/152 +34 extras|

Both retained O2 rows have zero nonzero excess words, unresolved or unverified
references and relocation errors, but neither has whole-word equality. All
four final rows and eighteen initial/directed rows bind exact source hashes.
Different formatting in the retained wait source changes only its hash, not its
score; archived controls remain unchanged.

No register/K&R/SDK-word prototype sweep, fake formal, keeper, local padding,
assembly, volatile trick, flag change or boundary change was used. Native
widths and genuine helper contracts remain authoritative.

## Verification

Two actual-source strict-C89 ASan/UBSan groups pass. Wait tests cover27,648 gate,
zero/FFFF/random duration, mode and wrapping-clock combinations, with exact
state and helper-call comparison. Constructor tests cover1,536 combinations of
all eleven input roles, external-key handling, valid offsets, active flags,
post-helper mutation and both return domains, plus missing-program and failed
allocation exits. Synthetic helpers express contracts and do not claim original
algorithms. LeakSanitizer alone is disabled under ptrace; no heap is allocated.

Run `verify.py`, `verify_controls.py`, and unittest discovery for `test_*.py`.
Use the existing pinned compiler through `IDO_DIR`. Fresh preflight validates all
439 target extents/99,120 B, protected manifests and the preexisting getter.
All161 static locks and whitespace pass. Central ledgers/D10, earlier frozen
sources, targets/scorer/compiler/layout/symbol/lock/spec inputs, runtime image,
farm, production gates, accepted800D1248 and restricted helper work are unchanged.
No ROM, raw assembly dump, object, credential or unrelated private data is
published. Independent paired review precedes central integration; central owns
aggregate publication and CI, and the independent checker alone merges.

Independent paired review PASS binds immutable source commit
`c04473b5cb22805dcf674a434d4fc4e155f2d739`, tree
`ccef0eacc854cf1b5cfa831e874a55cf6b6fd185`. Exact Git source copies reproduce
all four final rows, eighteen archived controls and both sanitizer groups.
The complete wait body and eleven-input constructor ABI, including their
sentinel, unsigned clock and distinct returned-key semantics, were independently
inspected with no source/domain blocker. `independent_review.json` records
1,216 B of complete nonmatching research and zero matching credit.
