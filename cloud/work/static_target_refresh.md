# Static relocation attribution refresh, 2026-10-01

osPiRawReadWord was tier raw_word with no_asm_region, while its converted slot already exists in asm/us/nonmatchings/rom/lib_8dd0. The recently corrected shared static index finds that slot. No scorer masks or overrides were added.

Used supported targets.populate(conn, store, names=["osPiRawReadWord"]) to rebuild only this target. Existing assembler round-trip gate passes, and tier becomes reloc_aware with gate_reason null. Old evidence is superseded through the standard transaction. Target SHA-256: 7e8a6ebc6cde1e21063f40c7e8c0f972c023980696832c8bf02146048854bbab.

Independent Rocky D compile of exact cloud/work/static_C/osPiRawReadWord.c at -g0 -O2 -mips2 -G 0 -non_shared passes stack-sensitive score zero and raw object words zero. Source SHA-256: 0c3fe44a14d6b667cab1f97fe6d4f85040e50452640e1b32e67dedc817efdfae. Standard lock add also returns score zero. Existing local declaration-order correction is retained. ROM promotion remains a separate coverage gate.

Three GU wrapper targets had a different metadata blocker: GNU as rejected splat's o32 float aliases ($fa0 etc.). Isolated target assembly now reuses only the float-register .set declarations from the existing canonical ROM assembler prelude, preserving the same numeric registers and retaining external relocations. A synthetic float-word/relocation round-trip regression passes. Scoped regeneration of guPerspective, guOrtho and guLookAt passes the existing round-trip gate and produces reloc_aware targets; standard supersession retires 1,735 stale evidence rows. Independent D strict/raw proofs for all three match. No scoring or round-trip masks were widened.

Real sound closure integration also exposed context-range overlap: compiled empty callee is eight bytes, retail extent sixteen; following sound function begins at offset eight. Previously its call mapped to empty+8. Context address ranges now stop at the next compiled function boundary. Regression verifies following real context receives its own image address and an uncovered stand-in remains refused.
