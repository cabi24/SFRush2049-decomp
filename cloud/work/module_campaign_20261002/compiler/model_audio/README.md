# DEF68 independent read-only audit

The complete 2976-byte source was checked against current native assembly, both switch tables in the exact game image, its real caller and callee input usage. No compile or equality claim is made by this audit.

The actual audio model has 2056 bytes. Its four body-force vectors start at 196 with a 12-byte stride. The center vector passed to the impact accumulator starts at 304; this is distinct from the collision-force member at 292 used by another module. Speed is at 1008, crash at 1600, collision kind/history at 1602/1603, direction at 1604 and player at 1990. The declared typed fields agree with all accessed native offsets.

Each impact point is 24 bytes; the five-point player record is 120 bytes. Both four-point loops advance one whole point. The fifth point's three-component scan advances one float. Force-vector iteration advances 12 bytes. These repaired typed pointer strides agree with native byte increments.

The first table has seven entries: state 0 initializes, states 1–3 stop or advance after timeout, and states 4–6 stop only when the force mask becomes zero. The second table maps states 1–3 to transient sounds with positive, negative and zero directional parameters, and states 4–6 to the corresponding sustained sound handles. The final irregular collision-kind dispatch is 1/2/3 to sound IDs 9/5/3. All source mappings agree with the two independently decoded native tables and branch dispatch.

The single actual model input is carried privately in `s1` by the real parent caller. `best_times_display` takes one signed-short player value privately in `s2`. The impact accumulator takes three ordinary integer/pointer inputs and a real threshold float privately in `f18`. `high_scores_display` takes seven ordinary inputs: four integer values followed by three floats at outgoing stack offsets 16/20/24. No unused extra incoming float is warranted.

One declaration inconsistency was reported before compilation and corrected by the parent: the new header now declares the external audio result as `u32`, matching `audio_core.c`. The correction changes only the declaration; the audited function body is unchanged. No source-body semantic error was found in the requested function.

[DEF68_readonly_audit.json](DEF68_readonly_audit.json) records source and image hashes, native geometry, table hashes and the observed interfaces. It applies to the recorded source hashes; later edits require a fresh audit of the changed portions.
