# audio_channel_setup (332 B) - 3/83 words; one as1 address-temp register

Animation-frame stepper (setup callback installed by func_8010D85C): mode 0 -> entity_transform_apply(obj, 1).
Otherwise (unless D_801170FC) timer -= D_8002EB94 (volatile f32 frame time); when it runs out: frame++,
timer = 0.0625; at frame >= model->count look up the kind flags D_80117530[model->type].flags (48-byte records):
0x1000 -> remove (shared apply tail), 0x2000 -> entity_spawn_callback(model->id, 0, 0) then remove, else frame = 0.
Then f = (s16)(model->base + frame); if f != model->cur: D_8012E700[model->id].tex = D_801427C0[f] (u16 handle
table), model->cur = f. Layouts: Obj {+4 s16 frame, +0xC Model*, +0x10 f32 timer}; Model {+0xE id, +0x10 type,
+0x50 cur, +0x58 base, +0x5A count}; Ent (0x44, same table as func_8010D85C) +0x14 u16.

best.c (= e1.c) shaping: the removal call is one shared tail (goto into the `mode == 0` arm); `m = obj->model`
before the timer update; `id` and `tex` locals in the tail (each moved several words onto retail registers).

Unit: `blob_unit --tag w11h score audio_channel_setup --with .../best.c --neighbours` ->
`FAIL audio_channel_setup: 3 of 83 words differ` / `locked bodies that differ in this unit: 0`.
Residual: retail `lui a2; addu a2,a2,t2; lhu a1,%lo(a2)`, ours uses a1 for the address temp too
(`lhu $5, D_801427C0($10)` in the ugen listing; as1 normally uses the destination as temp). w10g notes the same
pattern as "an inlined accessor's result". Tried ~25 variants: tex/id types and order, pointer local, reuse of
`mode`, inlined value/pointer accessors (worse), hand-edited listing with an explicit address register (as1 then
does not fold %lo). Next hypothesis: find the ugen form whose as1 expansion uses a temp other than the
destination (as1t.sh trace of the macro expansion), then the source that yields it.
