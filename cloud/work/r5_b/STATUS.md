# r5_b: precursor callees of entity_spawn_init (0x8008EA10)

No strict MATCH yet; nothing copied to cloud/matches. All five are ABI (ipa_signals.py: no non-ABI
entry reads, no unsaved s-regs), so none needs a group. score.py adds -r4300_mul itself.

| fn | words | best | status |
|---|---|---|---|
| func_8008E0B8 (vec3 normalize, returns length, 0.0 if <= D_8012394C) | 35 | 13 differ, -O2 | only FP register numbering differs. Needs 6 unused named locals (frame 40) + address-taken `z` (`f32 *pz = &z`) so z is spilled at 4(sp). Target: x=f12 y=f16 sqrt=f2 inv=f14; ours x=f12 y=f2 sqrt=f14 inv=f16. ~500 variants tried (decl/load/sum/store order, result var). |
| func_8008E26C (alloc slot in 68-byte table D_8012E700, count D_80156990, hiwater D_801569A8; returns s16 idx) | 75 | 68 differ, -O3 | code shape equal (word count 76 vs 75 incl. pad). Needs `s16 idx` local (spilled at 24(sp)), params (s32 a, s32 b, s16 c, s32 d). Target moves all four args to t2..t5 at entry (c->t5,a->t4,b->t3,d->t2) and reuses a3 as the index temp; we move three and keep d in a3. -O3 not -O2 (only -O3 makes the arg moves). b is really a pointer (caller passes obj+4). |
| model_data_load (scene-node flag walk; mode 0/1/2/3) | 93 | 80 differ, -O2, shape 100% | Key finds: (1) target index read is `D_8012E700[(s16)a]`, write is `D_8012E700[a]` (s32 a); (2) read into a temp `u32 t` first; (3) mode 2 child call is a real self-recursive *tail call* (IDO turns it into `b top; li a1,3`), mode 3 is a do/while over siblings with a real recursive jal for children. Remaining diff: ours copies a0 into a3 at entry (web for `a` not colored a0); target keeps a in a0, and operand order of the `or`. |
| model_transform_setup | 101 | 88 differ, shape 98% | Same function shape with AND-NOT masks. Target has an unreachable duplicate `mode == 0` arm: `flags = (flags & 0x7FFFFFFF) \| (v << 8)` (kept when written as a second `else if (mode == 0)`). Same a0/a3 copy blocker. |
| func_8008E408 (386w, private; sole caller entity_spawn_init) | 386 | first pass 376 words, shape 74% | See below. |

## func_8008E408 decode (first pass in func_8008E408.c)
`void f(s16 car, s32 type)`: car = D_80152818[car] (stride 0x3B8); sp186 = car.h248 >> 2; sp184 = fabsf((f32)sp186) as s16;
per-car flag word D_801392D8[car] bit (0x100<<type) guards re-spawn; then spawns TWO pooled effect objects
(func_8008E3C0(D_8013F1E0)): copies car pos f20..f28 to a local vec, func_8008E0B8 (normalize), vector_normalize_length(dir, basis[9])
(0x8008B4C4, 95w, **unmatched, also a prerequisite**; calls func_8008B3C8 11w and vector_copy_scale), math_utility copies basis to obj+4,
scale factor f2 from sp184 (<90: speed/90*D_80123954+D_80123958 when sp186>=0, else speed/70*0.75+0.25) applied to the 3x3,
wheel offset car.wheel[type] (+116, stride 12) and car vel (+44..52) copied into obj, LCG rand (seed D_8011735C, 0x41C64E6D, 12345)
scales velocity by rand-0.5, pos += vel, pos.y += 1.25, obj.w52 = func_8008E26C(D_8014295A[obj.h86], obj+4, -1, 0x40000).
Second object repeats with h86=4 and constants D_8012395C, w80 reused.
Blockers: target has no s-registers (saves only ra; t4/t1/t5/t2/a2 spilled around every jal, ra used as a temp), frame 256; ours
uses s-regs, frame 144. Likely needs the IPA/-O3 register environment of the entity_spawn_init unit (consider a group with
entity_spawn_init + its callees) or many more source-shape trials. The flag-bit block looks like 4 if-arms with recomputed
`&D_801392D8[i]` (not a switch on a shared pointer): target recomputes the address per arm after a shared early v0 for arm 0.

scratch/ holds the search scripts (gen*.py) and discarded variants.
