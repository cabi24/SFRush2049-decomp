#!/bin/sh
# install71.sh SCRATCH [--reuse OTHER_SCRATCH]      (run on the Pi, from anywhere)
#
# Builds the traced IDO 7.1 passes into the builder scratch dir SCRATCH (e.g. ~/rush2049/scratch/frontier/w22c):
#   SCRATCH/isr71/       decompals/ido-static-recomp v1.2 (the release the pinned 7.1 toolkit binaries come from),
#                        with its IRIX 7.1 binaries; `make setup` + recomp of uopt/ugen/as1 -> build/7.1/*.c (pinned)
#   SCRATCH/t71/reb/     the release 7.1 ido dir with uopt/ugen/as1 rebuilt by us from that C, unpatched (control)
#   SCRATCH/t71/ido/     the release 7.1 ido dir with the TRACED uopt/ugen/as1 (cc finds its passes next to itself,
#                        so the dir is a full copy, not symlinks):
#       uopt = uopt.c + patches/instrument71_uopt.py (globalcolor CDX trace + CDX_FORCE oracle + names/CDX_FN)
#                     + patches/spill71.py (SPLOG: f_spilltemps homes, local-area growth)
#       ugen = ugen.c + `workbench instrument-ugen --emit-provenance` + patches/ugen71_addrs.py (7.1 globals)
#       as1  = as1.c unchanged (the v1.2 libc already prints, so `-Wb,-R` gives the scheduler trace)
#       libc = v1.2 libc_impl.c + ../trace/patches/ecvt.patch.py
#   SCRATCH/t71/MANIFEST sha256 of sources, generated sources and binaries; SCRATCH/t71/tk71.sh, fidelity71.sh
# --reuse OTHER: copy OTHER/t71/{ido,reb,MANIFEST} (an earlier install) and check the binaries against its MANIFEST.
# Then run fidelity71.sh (all 18 boot-tail TUs: release vs rebuilt vs traced-off vs traced-on).
set -e
[ -n "$1" ] || { sed -n 2,22p "$0"; exit 2; }
REPO=/home/cburnes/projects/rush2049-decomp
TK=$(cd "$(dirname "$0")" && pwd)
B=${BUILDER:-watchman2}
SCR=${1#\~/}; SCR=${SCR#$HOME/}
REL=rush2049/cache/toolkits/4586d230749de1bc8a64add52ae53db0ffde18a1eee76c8a73f3791fd98e2557/ido
PIN_UOPT=0f2c66d2098e373fcc18d51a5220058f417ae6e91479f13f7b0b8d5291eadd01
PIN_UGEN=44c24d9d1d8c47eb16a32fd58b0d53cd6eefe6e5c27b7ed415f1521f41b5618a
PIN_AS1=19494fb081b3463a06c4159dbefe3055aa50ed4254cdbfeb6e1d5d980757f5ab
rsh() { ssh -o BatchMode=yes "$B" "$@"; }

rsh "mkdir -p ~/$SCR/t71/src ~/$SCR/t71/bin ~/$SCR/t71/patches"
scp -q "$TK"/remote/*.sh "$B:$SCR/t71/"
if [ "$2" = "--reuse" ]; then
  O=${3#\~/}; O=${O#$HOME/}
  rsh "set -e; rm -rf ~/$SCR/t71/ido ~/$SCR/t71/reb; cp -a ~/$O/t71/ido ~/$O/t71/reb ~/$O/t71/MANIFEST ~/$SCR/t71/ && cd ~/$SCR/t71 && grep '^bin ' MANIFEST | awk '{print \$3\"  \"\$2}' | sha256sum -c"
  echo "installed (reused from $O) into $B:~/$SCR/t71"; exit 0
fi

# 1. upstream recomp at v1.2 + generated C (builder; ~1 min, 2 jobs)
rsh "set -e; cd ~/$SCR; [ -d isr71/.git ] || git clone -q --depth 1 --branch v1.2 https://github.com/decompals/ido-static-recomp isr71
  cd isr71 && git submodule update --init --depth 1 -q && [ -x build/recomp.elf ] || nice -n 10 make -j2 setup RELEASE=1 >setup.log 2>&1
  nice -n 10 make -j2 RELEASE=1 VERSION=7.1 build/7.1/uopt.c build/7.1/ugen.c build/7.1/as1.c >cgen.log 2>&1
  nice -n 10 make -j2 RELEASE=1 VERSION=7.1 build/7.1/out/uopt build/7.1/out/ugen build/7.1/out/as1 >bin.log 2>&1
  echo '$PIN_UOPT  build/7.1/uopt.c' | sha256sum -c --quiet; echo '$PIN_UGEN  build/7.1/ugen.c' | sha256sum -c --quiet
  echo '$PIN_AS1  build/7.1/as1.c' | sha256sum -c --quiet"
# 2. instrument on the Pi (needs the vendored workbench)
L=${TMPDIR:-/tmp}/t71-install.$$; mkdir -p "$L"; trap 'rm -rf "$L"' EXIT
scp -q "$B:$SCR/isr71/build/7.1/uopt.c" "$B:$SCR/isr71/build/7.1/ugen.c" "$L/"
cd "$REPO"
python3 "$TK/patches/instrument71_uopt.py" "$L/uopt.c" "$L/uopt.cdx.c"
python3 "$TK/patches/spill71.py" "$L/uopt.cdx.c" "$L/uopt71.trace.c"
python3 tools/workbench.py instrument-ugen --emit-provenance "$L/ugen.c" "$L/ugen71.trace.c"
python3 "$TK/patches/ugen71_addrs.py" "$L/ugen71.trace.c"
scp -q "$L/uopt71.trace.c" "$L/ugen71.trace.c" "$B:$SCR/t71/src/"
scp -q "$TK"/../trace/patches/ecvt.patch.py "$B:$SCR/t71/patches/"
# 3. build (builder, gcc -Os like the release Makefile, 2 jobs) and assemble the two ido dirs
rsh "sh ~/$SCR/t71/build71.sh ~/$SCR ~/$REL"
echo "installed into $B:~/$SCR/t71; now: SCR=$SCR $TK/fidelity71.sh"
