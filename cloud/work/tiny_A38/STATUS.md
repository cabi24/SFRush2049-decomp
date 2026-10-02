# A38 frozen ordinary byte updater claim

Claim func_800F7E30:16 words /64 bytes. Flags `-g0 -O2 -mips2 -G 0 -non_shared`. Canonical fn exit0, strict_diff0, exact size16, extra0, no unresolved/unverified/errors. Source SHA256 `616d94533ad40b0e8352ad54868475739a3d7fc78080a635984e5b3e8d12dd91`. Current lock checked unlocked at freeze.

Three genuine consumed inputs: signed-byte row and column, and byte delta. Actual destination address is D_80149428 plus row*4 plus column, with signed byte load and byte store after addition. Effective delta consumption is low eight bits because output is a byte; signed versus unsigned original delta spelling cannot be recovered from arithmetic alone, and the delivered signed-byte type exactly reproduces its genuine parameter-home store. No extra formal, wrapper, pressure or runtime operation introduced. There are no fabricated array extent checks: actual assembly imposes none.

Baseline frozen A36 source was2/16 differing words, only the order of parameter-home and output-byte stores around return. Single pilot33 token-preserving physical layout control puts the actual update on its own line and gives the target scheduling. No tokens or semantics changed; source layout is part of exact frozen proof. Existing A36 source remains untouched.

One parallel similar physical-layout control on the genuine three-float norm stayed7/8 plus1 extra; unclaimed source and sanitized norm_control.json retained, with no continued allocation/formatting sweep. Only byte updater is in packet claims.

Raw canonical/disassembly artifacts remain ignored/private build/codex-A38 on Rocky A. No accepted source/layout/lock/state modifications. Root independently verifies target source image and ROM before acceptance.
