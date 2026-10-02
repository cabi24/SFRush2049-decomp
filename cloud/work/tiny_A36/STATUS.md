# A36 frozen: two genuine ordinary claims

Claims: func_800A73FC11 words44 bytes; func_8008B3F412 words48 bytes. Total23 words /92 bytes. Flags `-g0 -O2 -mips2 -G 0 -non_shared`. Both canonical `score.py fn SOURCE NAME --flags` exit0, strict_diff0, exact extent, extra0 and no unverified/unresolved/error entries. Current locks checked: both unlocked at freeze.

Source SHA256:
- func_800A73FC: `d33e274819710c2cada5d35d88cb7f44a403aa50f5caff5f86c0997a034ea2f8`.
- func_8008B3F4: `9cc33209e4f6d72d91d3f719775b0d781c988327310a8803504fc9bc8d690ecc`.

A73FC consumes a signed full-width value. Nonpositive input returns the unsigned byte global D_80146204. Positive input stores its low byte there, clears the genuine unsigned-byte latch D_8017A63C, and returns the full untruncated input. Baseline matched directly.

8B3F4 consumes and returns a real float: compare against actual float D_80123884; replace input only when strictly lower; return 1.0f/sqrtf(input). Real comparison retains the retail NaN behavior. IDO `#pragma intrinsic(sqrtf)` supplies the actual sqrt.s opcode; this is ordinary compiler support for the genuine standard math operation, not a runtime/source stand-in. Initial external-call baseline10/12+5 extras; intrinsic declaration reaches strict0/12. No other source control needed.

Two unclaimed leads: real signed-byte row/column/delta updater F7E30 remains2/16 exact (signed snapshot worsens5, u8delta worsens9; original retained). Real three-float norm8E098 remains7/8 with9 emitted+1 extra using the genuine sqrtf intrinsic; real sum/result carriers identical and double intrinsic worsens. No invented formals, dummy pressure, guards or memory writes. Eight bounded controls total across four leaves, including the two useful sqrt declarations; no broader sweep.

Full TUs, literal flags, sanitized final counts/hashes, claims and rejected controls are reviewable here. Raw canonical output stays ignored/private build/codex-A36 on Rocky A. Shared accepted source/state untouched. Root independently scores and applies image/ROM gates before acceptance.
