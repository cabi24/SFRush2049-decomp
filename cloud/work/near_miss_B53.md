# B53 complete pool node constructor

Frozen NONMATCH func_800B3704@800B3704, 228 bytes/57 target words. The historical automatic source had missing initialization via a fake goto and word writes through actual half/byte fields. Complete native source restores all original stores, real200-node limit/index/count increment, string/resource input pointer, signed-half first/second fields, actual byte busy/pending/enable flags, real next pointer, word sentinel fields, returned pool slot and both final real calls. Complete original callees collision_sound_play/func_800A79F4/Input_ApplyPadConfig were decoded first; first incoming string pointer and actual u16 slot/index views follow their loads/forwarding. The source uses no fake caller or larger context group.

Three bounded controls: O2 native43/57; actual count postincrement expression43/57 unchanged; O3 native43/57 unchanged. Original48-byte frame is present, but count/name register allocation and constant/store scheduling differ. No extra operation, unused buffer, invented argument, fake pressure or line search was attempted. Sanitized fresh recompile proofs preserve source hashes and full comparison exceptions/counts, no target byte streams or objects. No claims.

E762C was read-only scouted, then rejected before compilation when a late filename check found the existing complete tiny_A30 body. No duplicate replay was performed.
