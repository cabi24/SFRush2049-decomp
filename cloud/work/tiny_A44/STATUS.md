# A44 frozen no-claims ordinary packet

Literal flags `-g0 -O2 -mips2 -G 0 -non_shared`; scorer includes canonical r4300 multiply erratum handling. Three unlocked ordinary ABI leaves, actual complete bodies, no callee stand-ins or accepted tree changes. Final source hashes/counts/error checks in scores.json. No source reaches strict MATCH, so claims stay empty.

| Target | Differing / retail words | Compiled words |
|---|---:|---:|
| func_8008AD6C |32/41|42|
| func_80092BF4 |22/25|20|
| func_800EC270 |20/34|34|

AD6C returns2 on every path; opcode05 swaps only the low two bytes of the first word, 06/07 swaps both words while preserving high halves. The actual second load precedes the first store; retained source observes that alias order. Two actual early-return/load-order controls do not improve the score. No speculative Gfx opcode reinterpretation or extra branch.

92BF4 consumes a signed-half key and two pointers to full words. Key64 word0 is cast to signed-half index into Slot68; output fields60/64 store dereferenced inputs in retail order. The compiler eliminates a repeated multiply/sign-extension web that exists in retail; actual byte-offset expression controls do not recover it. No volatile/fake rereads or dummy inputs were added.

EC270 initializes actual float788/792 to zero, float820 from real global constant, half824=0 and826=-1. Signed vehicle byte1996 controls full signed counter assigned to halves828/830 and wraps to0 after reaching4; the false path writes only828, leaving830 unchanged. Global signed-half mode controls vehicle byte2012. End halves250/252/254 are -1/0/0. No field stores or guards invented. The exact extent still has a large allocator/scheduling residual; no blind sweep.

Four directed controls are preserved as sanitized controls.json. Canonical match acceptance was not requested for these nonmatches; initial measurement uses amatch.builder backed by canonical score internals. Instruction streams remain private/ignored. Stop after bounded actual-source evidence.
