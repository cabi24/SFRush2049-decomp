# Surface impulse literal ownership audit

Frozen nonmatching research. No new coverage or acceptance claim.

`func_800E1F80@800E1F80` is a complete 1,060-byte /265-word vehicle surface impulse updater with an ordinary Model2056 pointer argument. B90 had already reconstructed the complete routine and tested actual arrays, force-carrier reuse and projection operand order. Those controls were reviewed and not repeated.

The single new control uses the actual floating mph-to-feet-per-second conversion `(f32)(5280.0 /3600.0)` in both places previously represented by mutable globals. Current canonical image SHA256 `bf7da3fa6283428a97372250cd4076d15e9eae10f9d5709c0387fe0742d43a1d` verifies that both original pool symbols at801243C4 and801243C8 hold1.4666666984558105. The source corrects names for world velocity544, world position556, mass1472 and air distance1516, preserving their actual offsets and operations. The normalizer8008E0B8 returns float; that return is unused here. The other helpers are the real two-argument scene-direction lookup800C1A00 and accepted three-pointer matrix transform800A61B0.

Arcade `stree.c:tiresurf` exports surface boost flags, establishing related subsystem ancestry. This N64 impulse extension has no identified exact whole-function arcade ancestor. The conversion formula is an equivalent source interpretation of the proven native constants, not a claim that the original N64 source has been recovered.

Independent ordinary O2 and O3 replay gives identical runtime output:265 true words,144-byte frame versus native136,93 diagnostic differing words. Four local-pool HI/LO references remain unverified. The strict stack-sensitive score is904, not a match; anonymous pool placement was not accepted or masked for coverage. There are no unresolved targets, errors or extra words. Receipt `independent_replay.json` records both exact flags, hashes, unverified sites and accepted:false. The compiled two-float pool cannot be counted until a complete protected relocation gate succeeds.

The bounded source change is inert. No further allocation, frame, flag or declaration controls were run. Objects and disassembly remain ignored. Accepted sources, locks, targets, build infrastructure and tests were not changed.
