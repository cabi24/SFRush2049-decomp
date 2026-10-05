# audio_channel_priority (0x80094A54, 468 bytes) - 11/117 words, one register rotation

Semantics: remove handle `id` from the 12 x 3 x 5 table D_80151690 (s32) and the parallel f32 weight table
D_80150F88[12].weight[3][5] (records of 0x60 bytes, weights at +4): shift the tail of the row down,
store 0.0f / -1 in the last cell, re-test the same slot.
best.c (= h.c): standalone and unit both `11/117 words differ`.
Residual: retail colours outer->v0, weights->v1, id->t0; ours id->v0, outer->v1, weights->t0 (all other
webs identical, size identical).  So id's web outranks outer and weights in ours and ranks below every loop
web in retail.
Tried (previous agent + this one, ~300 variants, no movement off 11): gen/ and gen2/ (272 generated
declaration-order / index-form / compare-order variants, all 10-11 aligned / 11 strict or worse), discarded
reads of id (`if (id) {}` at 7 places, 1-4 copies; no change at all), param used directly (stride.c, 24),
natural 3-D indexing (a.c, 99 words).
Next hypothesis: id is not a copy of the parameter but the parameter web itself with an extra definition
(e.g. a narrowing cast or a reassignment) that lowers its save; needs the instrumented uopt colouring trace.
