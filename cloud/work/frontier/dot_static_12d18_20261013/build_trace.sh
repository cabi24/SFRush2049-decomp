#!/bin/sh
# Optional local diagnostic build. All fetched sources and binaries stay ignored.
set -eu
ROOT=$(git rev-parse --show-toplevel)
cd "$ROOT"
S="$ROOT/build/static-12d18-tracer"
TK="$ROOT/cloud/work/frontier/tools/trace71"
mkdir -p "$S/src"
if [ ! -d "$S/isr71/.git" ]; then
    git clone --depth 1 --branch v1.2 https://github.com/decompals/ido-static-recomp "$S/isr71"
fi
[ "$(git -C "$S/isr71" rev-parse HEAD)" = 9c242adc890beef098020149d9554f48208f699d ]
(cd "$S/isr71" && git submodule update --init --depth 1 && make -j2 setup RELEASE=1 && make -j2 RELEASE=1 VERSION=7.1 build/7.1/uopt.c build/7.1/ugen.c)
printf '%s  %s\n' 0f2c66d2098e373fcc18d51a5220058f417ae6e91479f13f7b0b8d5291eadd01 "$S/isr71/build/7.1/uopt.c" 44c24d9d1d8c47eb16a32fd58b0d53cd6eefe6e5c27b7ed415f1521f41b5618a "$S/isr71/build/7.1/ugen.c" | sha256sum -c
python3 "$TK/patches/instrument71_uopt.py" "$S/isr71/build/7.1/uopt.c" "$S/src/uopt.cdx.c"
python3 "$TK/patches/spill71.py" "$S/src/uopt.cdx.c" "$S/src/uopt.c"
python3 tools/workbench.py instrument-ugen --emit-provenance "$S/isr71/build/7.1/ugen.c" "$S/src/ugen.c"
python3 "$TK/patches/ugen71_addrs.py" "$S/src/ugen.c"
cp "$S/isr71/header.h" "$S/isr71/libc_impl.h" "$S/isr71/helpers.h" "$S/isr71/libc_impl.c" "$S/isr71/version_info.c" "$S/src/"
python3 "$TK/../trace/patches/ecvt.patch.py" "$S/src/libc_impl.c"
python3 - "$S" <<'PY'
import shutil,sys
from pathlib import Path
from tools.conveyor import compilers
cc=compilers.tu_cc('lib_11640',Path('tools/cloud/ido/cc'))
if compilers.identify_dir(cc.parent)!='ido7.1':raise SystemExit('Run tools/cloud/setup.sh for pinned stock IDO7.1')
shutil.copytree(cc.parent,Path(sys.argv[1])/'ido',dirs_exist_ok=True)
PY
cd "$S/src"
gcc -std=c11 -Os -fno-strict-aliasing -DIDO71 -DPACKAGE_VERSION='"v1.2"' -DDATETIME='"t71"' -c libc_impl.c version_info.c
gcc -std=c11 -Os -fno-strict-aliasing uopt.c libc_impl.o version_info.o -lm -o ../ido/uopt &
p=$!
gcc -std=c11 -Os -fno-strict-aliasing ugen.c libc_impl.o version_info.o -lm -o ../ido/ugen
wait "$p"
printf '%s\n' "Diagnostic compiler: $S/ido/cc; verify --deep --trace requires stock/traced whole-object identity."
