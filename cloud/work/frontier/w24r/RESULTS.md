# w24r: recomp Phase 4 (IPA-aware replacement build, RT64 test mode, scripts, gameplay-test package) — tooling lane, branch w24r

Status: IN PROGRESS (2026-10-12). Not a matching lane: no src/, asm/, lock or coverage change.
Host: rocky, `~/rush2049/recomp/w24r` (copy of w23r + this branch's tracked files; nothing ROM-derived committed).
Target tester OS (owner, 2026-10-12): **Windows 11** gaming laptop; the package's primary INSTALL path is Windows
native (VS 2022 Build Tools + clang-cl + CMake + Ninja), Linux secondary, WSL2 a fallback note.

## Flag agreement with w24t

- `--allow-ipa` (gen_config.py, build side, this lane): by default the replacement build REFUSES a replacement whose
  object calls a callee with IDO -O3 interprocedural register parameters (build/ipa_members.json `callees`) or a
  known inlined-in-retail `jr ra` stub, unless `tools/recomp/ipa_callees.json` says how to call it (then the build
  inserts a register-moving shim / links the stub's body). `--allow-ipa` builds it anyway with plain o32 calls (the
  Phase 2/3 behaviour, e.g. to reproduce the frame-706 artifact). Same spelling as w24t's detection flag; w24t keeps
  detection/reporting, this lane only the build.

(Sections below are filled as the lane proceeds.)
