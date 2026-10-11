# Recomp boot log (Phase 1)

Exact commands and what each run reached. Host: rocky (see `VERSIONS.md`). Everything under
`build/recomp/` is private (ROM-derived) and never committed.

## Commands

```bash
# on rocky, in a copy of the w21r worktree (tracked files only)
cd ~/rush2049/recomp/w21r
python3 tools/recomp/gen_config.py --rom ~/rush2049/recomp/rom/baserom.us.z64 \
        --n64recomp-src ~/rush2049/recomp/src/N64Recomp
(cd build/recomp && rm -rf RecompiledFuncs && ~/rush2049/recomp/build/N64Recomp/N64Recomp rush2049.recomp.toml)
CC=clang CXX=clang++ cmake -S tools/recomp/runtime -B build/recomp/host -G Ninja \
      -DN64MODERNRUNTIME_DIR=$HOME/rush2049/recomp/src/N64ModernRuntime -DRECOMP_OUT=$PWD/build/recomp
ninja -C build/recomp/host
build/recomp/host/rush2049_recomp --rom build/recomp/rom/rush2049.recomp.z64 \
      --home build/recomp/home --seconds 30 [--press 8:START,12:A] [--fb-dump build/recomp/fb]
```

`cmake` runs `tools/recomp/runtime/check_contract.py` first: every shim/hook the generator
recorded in `manifest.json` `runtime_contract` must be defined by the host or the runtime.
Crash triage: `gdb -batch -ex run -ex bt --args build/recomp/host/rush2049_recomp ...`.

## Runs
