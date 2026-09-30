mode_byte_set, mode_byte2_set: strict MATCH (score.py group, and zbuild --as1=-r4300_mul).
Trick: real sound_update_channel takes `force` in IPA reg t0 (bnez t0 after a jal to func_80096288).
Stand-in callee + stand-in func_80096288 with 3 params but only 2 used (param count picks t0; used 3 -> t1, 4 -> t2);
func_80096288 needs 2 call sites in the callee or -O3 inlines it; dead if(0){switch} stops inlining of it into callers.
