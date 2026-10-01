# Codex real input-dispatch caller, 2026-10-01

Strict `python3 tools/cloud/score.py group cloud/work/ipa-groups/codex_input_aux --claims` on Rocky A reports **MATCH** for input_aux_handler: 121 words, 484 bytes. Exact final group.c and group.json were copied to Rocky before the final score. Flags: `-g0 -O3 -mips2 -G 0 -non_shared`. No claim overlaps the lock.

## Actual source and closure

The pilot's synthetic zz_a/zz_b were removed. The sole real caller game_loop (176 target words) is reconstructed from its full assembly and included as kept context. input_aux_handler is internal. Surprisingly, this real caller preserves input_aux_handler out of line with only its one actual callsite; no unreachable switch or other inline-prevention quirk was necessary. Other callees remain external real game/overlay symbols; no callee stand-ins or source stubs are present.

The conservative register-summary direct-closure tool proposes seven members / roughly 2,475 words, adding debug_collision, physics_sym, func_800D7634, func_800D91A0 and func_800DB1E0, then reports four missing RaceStateMachine_Update relationships. These are useful larger-module leads, but the exact claimed input body requires only the real game_loop source to reproduce the four-wide temp ring. The tool's inferred t2 argument is propagated from downstream call summaries: the complete input_aux_handler assembly contains no direct t2 read, and the retail game_loop call does not set an argument. The source therefore retains the real no-explicit-argument interface. This is evidence for the bounded claim, not a general assertion that the larger graph's semantics are fully inferred.

## Semantic audit of real game_loop context

Unlike work/game/race/game_loop/base.c (empty TODO), this is an actual body. Unlike generated m2c's void-return seed with M2C_ERROR arguments, it reproduces target return paths and real calls without placeholders:

- Initialize frame_counter and gstate when gstate is zero.
- Same-state branch calls game_mode_handler; transition branch polls D_8002EB70, clears D_80149438, calls attract_or_transition, copies msgq_ptr into D_801497F4 and calls process_inputs.
- Convert the modulo-32-bit tick delta to unsigned float, multiply by D_8002AFB8, and manage the sound handle when elapsed exceeds 300 and no 0x4000000F state flag is active.
- Call playgame_state_change, dispatch input for 0x7C03FFFE, update RaceStateMachine for 0x00600000, and perform the four object/physics/effects/pad updates with early return 1 when D_801170FC is set.
- Call countdown for 0x03FC0000; when state words agree, call countdown_handler and the same four updates, returning 1 after incrementing frame_counter. Otherwise increment frame_counter and return 0.

Polling D_8002EB70 is marked volatile because target reloads it inside the wait loop. Unsigned subtraction is explicit before float conversion, avoiding a signed-overflow source ambiguity. Sound handle/address casts follow the real bit-width and address despite historical header prototype types. No runtime instructions or calls were invented to alter IPA metadata.

## Material limits

The real context game_loop is **170/176 positions different**, not claimed. Target frame is 88 bytes with nine saved integer and two saved float registers, while this bounded group uses a much smaller ordinary frame. Many downstream callees use wider IPA conventions than the external prototypes capture, which explains why this caller is far from a byte match. The group uses context for compiler allocation only; coordinator must preserve retail game_loop and all external callees when splicing the one exact claimed body. Do not independently promote the context as a match.

input_aux_handler itself is the pilot's cleaned real if/else dispatch chain with the m2c 64-bit mask and pass-through noise removed. Its compiled 121 words, calls, argument setup and relocations all strictly match. No volatile/dead-read/dead-switch quirk was added to the claimed body.

Worker performed no splice, lock/layout mutation, commits, or coordinator-state changes. Independent image gate and full-ROM SHA-1 are still required before cartridge coverage is accepted.
