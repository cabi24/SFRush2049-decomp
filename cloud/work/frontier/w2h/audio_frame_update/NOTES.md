# audio_frame_update (0x800B0C48, 600 bytes) - 21/150 words in the unit, final loop colouring

best.c (= alt_handle_first.c, also v4/h_first.c): `blob_unit --tag w2h score audio_frame_update --with best.c`
-> `FAIL audio_frame_update: 21 of 150 words differ` (size equal).  The callees (func_800B0A88 group,
func_800B08FC) are locked, so the unit is the real context; everything before the last loop is identical.
Residual: in the final `do { ... } while (i < 4)` retail colours i->a0, 24->a1, car->a2, anim cb->a3,
&D_80139320[slot]->t0 and leaves v1 unused; ours i->v1, 24->a0, car->a1, ctl->a2, anim->a3.  Retail therefore
has one more web interfering with i that occupies v1 (lowest-index-first colouring), and its anim/ctl order
differs.  All 30 v*/ variants of the previous agent re-scored in the unit: 21-142, none better.
Next: look for a value retail keeps live across the loop (the copy-source pointer D_8011B58C / mode byte use
v1 earlier), e.g. a pointer variable reused for the loop or the loop written over `m`.
