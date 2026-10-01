# B13 — five current-layout unlocked heads

Selected against the actual root `blob_matched.lock.json` and `build/blob_layout.json`, excluding completed B5/B10/A2 packets and the known B5D0 hidden-s2 caller. Exact current target object/assembly identifiers are recorded in `heads_B13/provenance.json`; objects were exported read-only to private scratch. Target assembly blobs were absent locally, so actual instruction inspection used those trusted objects, not inferred seed output. No source/layout/locks/farm state mutations, integration or commits by this worker. One compute process at a time.

| Target / address | Current extent | Baseline O2 | Final strict | Flags |
|---|---:|---|---|---|
| func_8010C7CC @8010C7CC |24 B /6 words|MATCH|MATCH|O2|
| func_8010FD60 @8010FD60 |28 B /7 words|MATCH|MATCH|O2|
| func_8010D85C @8010D85C |368 B /92 words|compile error|62/92 NONMATCH|O3|
| func_80108DA8 @80108DA8 |408 B /102 words|94/102 +4 extra nonzero|88/102 NONMATCH, no extras|O2|
| func_8010E4E4 @8010E4E4 |432 B /108 words|108/108|95/108 NONMATCH|O3|

All flags expand to `-g0 -O2 -mips2 -G 0 -non_shared`, except the stated O3 best seeds. `heads_B13/verification.json` freezes exact strict scorer output, flags, hashes and process exit codes for the existing two match artifacts and retained best three nonmatches. No masked/workbench score is counted as acceptance.

The two existing `cloud/matches` files were preserved and directly strict-rescored. Neither was in the actual root lock at selection. They represent at most 52 new bytes pending coordinator independent splice and ROM/lock gates, not worker integration. Their exact existing identities:

- C7CC: `cloud/matches/func_8010C7CC.c`, SHA256 `e8ca4a734d87a7f6d67a44975e70704e86ad7e141301c56b63a92dea8245c89f`.
- FD60: `cloud/matches/func_8010FD60.c`, SHA256 `f4a75ea6b155a413ce5132c8ee8c2131a7830e3aff12672a1e32b9399262146d`.

C7CC's six actual instructions home all four real formals and return zero. Natural four-argument return-zero C reproduces them; this is the actual registered callback, not a stand-in for another function. FD60 masks its input to 28 physical-address bits and ORs the uncached cartridge window. Independently tested minimal alternate TUs are archived in this packet, but existing match files were not replaced.

D85C's m2c source did not compile because func_800A464C was declared s32 then compared to NULL and assigned to a pointer. Corrected its real pointer return and pointer first formal, initial entity_transform_apply(arg0,1), D_80118E08 word table, word-sized output store to D_8012E738, and the fifth literal1 outgoing argument observed at sp+0x10. Inspected the actual sound_bank_load callee: its second pointer receives a u16 result at offset0, so the output local is u16 rather than an invented array. The fourth formal is signed byte; the fifth actual caller argument exists even though that callee body does not use it. Best62/92 O3 has frame72 vs retail88 and different discriminator register web; no fabricated large buffer was introduced to fill16 bytes. About eight directed full-TU probes plus O2/O3 controls. Retained `func_8010D85C_ptrformal.c` is a compileable lead, not spliceable.

8DA8's actual table byte at D_80152907 is signed; corrected it and count D_80151AD0 to signed16. Boolean temporaries are word-sized, eliminating four extra nonzero instructions caused by seed s8 narrowing. Actual typed MenuNode fields preserve word index44, word28 at40, signed bytes24/26 and signed short positions14/16/20/22. Removed dead m2c stack alias and redundant boolean copy; this restores the early count/index register assignment and gives88/102. Global-address materialization and scheduling still shift the conditional region and downstream words. Tested actual count/mode pointer/array/struct spellings, with no improvement. About eleven meaningful shape/type probes; no blind line reflow. Retained `func_80108DA8_directbool.c` is a nonmatch.

E4E4 corrected the initial call to preserve arg0 and set only arg1=1. Reconstructed the actual three-element output vector for sound_position_set, and moved short loop-index initialization into the active branch. A separate natural typed State/Object/Model variant preserves object12, object position20, velocity56, model pointer108, model position0 and velocity12; it does not improve the strict verdict. Best95/108 O3 retains original meaningful carrier variables and vector, frame88 vs retail104, with differences in global address hoisting and FP operand coloring. About five directed shape probes plus compiler controls. No extra arrays or dead locals were added to force the frame. Retained `func_8010E4E4_minimalrepair.c` is a nonmatch.
