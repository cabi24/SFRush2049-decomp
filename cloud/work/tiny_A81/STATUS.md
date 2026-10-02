# A81 tire_compound_set — frozen native near match

Canonical registered target 0x800A5BB8..0x800A5D2C, 372 bytes / 93 words.
Full source tire_compound_set.c, literal -g0 -O2 -mips2 -G 0 -non_shared.
Fresh canonical 8/93, no extras, unresolved, unverified, or errors. O3 same.
A real consumed volatile busy-pointer control also remains 8/93. Hashes and
fresh comparisons for both are frozen in verification.json; claims are empty.

The native routine performs a nonblocking queue receive, retries blocking on -1,
jams the queue, waits on the true volatile signed halfword, resets five true
globals and the signed halfword sentinel, then invokes actual reset callees.
The exact first 64-slot sweep tests a real 20-byte slot resource and signed-byte
flag; the second records the last occupied slot's one-based index. All original
updates, calls, branches, signedness, and both loop bodies are reproduced.

Residual: native stack frame is 48 bytes and actual queue-message home sp+44;
retail frame is 64 bytes and message home sp+52 (three words differ). The other
five differences color the actual busy-halfword address as v1 rather than v0.
No unused locals, padded message/container, artificial stack demand, fake formals,
register-pressure sweeps, synthetic helper context, or weakened scoring used.
These constraints leave an honest 8/93 until the true original source/frame
contract is discovered. Coordinator owns archival; worker continues fresh.
