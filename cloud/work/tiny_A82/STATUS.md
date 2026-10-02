# A82 wheel_torque_apply — frozen non-match

0x800AC668..0x800AC6F4, 140 bytes / 35 words.
Final wheel_torque_apply.result.c is 11/35, no extras or verification exceptions,
under literal -g0 -O2 -mips2 -G 0 -non_shared. Initial full native source 20/35
under both O2 and O3. Fresh hashes/proofs for both are in verification.json.

Actual func_800A473C audit proves unbounded strcpy (initial scouting inference of
13-byte copy was incorrect). Actual entity_name_copy is binary search over
20-byte records; pointer_offset_wrapper compares their strings at +4. Thus true
key record is an object pointer and 16-byte name. No buffer capacity was invented
from register pressure. The actual key name is copied then searched with count,
record stride20, and original comparison callback. The found object's first word
is published to the original global, reloaded, and passed with the signed half
input to differential_output. The actual result global supplies final return.

Final consumed search-result pointer local naturally places the true key at retail
sp+32, name+36. A faithful volatile view of the published global preserves its
original observable store then reload. Native frame56 versus original64 remains,
plus global-address and sentinel-store scheduling. No enlargement/padding of the
actual key, unused locals, pressure work, fake logical formals, dummy helper
context, or weakened comparisons. All sources frozen; empty claims, no credit.
