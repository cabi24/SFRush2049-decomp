#!/bin/sh
# install.sh SCRATCH [--reuse OTHER_SCRATCH]      (run on the Pi, from anywhere)
#
# Builds the traced IDO 5.3 passes into the builder scratch dir SCRATCH (e.g. ~/rush2049/scratch/frontier/w11a):
#   SCRATCH/bin/uopt  recompiled uopt.c + workbench globalcolor profile + w5d PRE hooks (patches/instrument_w5d.py)
#                     + spill-temp/area hooks (patches/patch_spill.py)        env: CDX_*, W5D_*, TMPLOG, SPLOG
#   SCRATCH/bin/ugen  recompiled ugen.c + `workbench instrument-ugen --emit-provenance`  env: DKWB_UGEN_TRACE, _SCHED
#   SCRATCH/bin/as1   recompiled as1.c unchanged; libc printf made real (patches/as1_printf.patch.py) so -R prints
#   all three link one libc_impl.o: ido-static-recomp libc_impl.c + patches/ecvt.patch.py + patches/as1_printf.patch.py
#   SCRATCH/bin/ido -> the stock toolkit IDO; SCRATCH/bin/MANIFEST: source and binary sha256, patch list
#   SCRATCH/tk/      builder-side helper scripts (remote/*.sh)
# Sources: the recompiled C on the builder in $RECOMP (default ~/rush2049/scratch/ci/tools/ido-static-recomp/build),
# pinned by sha256 below (the same uopt.c every lane since w3a instrumented). Instrumentation runs here on the Pi
# (it needs the vendored workbench); gcc runs on the builder, two jobs at most, niced.
# --reuse OTHER: copy OTHER/bin/{uopt,ugen,as1,err.english.cc,MANIFEST} instead of building (a scratch that this
# script installed earlier); the binaries' sha256 are checked against OTHER's MANIFEST.
# After installing, run fidelity.sh (needs one blob_unit score with your tag) to prove tracing-off == stock.
set -e
[ -n "$1" ] || { sed -n 2,20p "$0"; exit 2; }
REPO=/home/cburnes/projects/rush2049-decomp
TK=$(cd "$(dirname "$0")" && pwd)
B=${BUILDER:-watchman2}
SCR=${1#\~/}; SCR=${SCR#$HOME/}
RECOMP=${RECOMP:-rush2049/scratch/ci/tools/ido-static-recomp}
IDO_REL=rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
PIN_UOPT=627eff8f854eb0a9d3f0a080e9dabbcd23335acb768f88e916d7ffbcbbc69e89
PIN_UGEN=4079660f9ebb068a791c6f54abb67cf7c3bb792207c89a6eb9ac2d9e146341ec
rsh() { ssh -o BatchMode=yes "$B" "$@"; }

rsh "mkdir -p ~/$SCR/bin ~/$SCR/tk ~/$SCR/src ~/$SCR/patches && ln -sfn ~/$IDO_REL ~/$SCR/bin/ido"
scp -q "$TK"/remote/*.sh "$B:$SCR/tk/"
if [ "$2" = "--reuse" ]; then
  O=${3#\~/}; O=${O#$HOME/}
  rsh "set -e; cd ~/$O/bin && cp uopt ugen as1 err.english.cc MANIFEST ~/$SCR/bin/ && cd ~/$SCR/bin && grep '^bin ' MANIFEST | awk '{print \$3\"  \"\$2}' | sha256sum -c"
  echo "installed (reused from $O) into $B:~/$SCR/bin"; exit 0
fi

L=${TMPDIR:-/tmp}/wtk-install.$$; mkdir -p "$L"; trap 'rm -rf "$L"' EXIT
scp -q "$B:$RECOMP/build/uopt.c" "$B:$RECOMP/build/ugen.c" "$L/"
echo "$PIN_UOPT  $L/uopt.c" | sha256sum -c --quiet || { echo "uopt.c is not the pinned source"; exit 1; }
echo "$PIN_UGEN  $L/ugen.c" | sha256sum -c --quiet || { echo "ugen.c is not the pinned source"; exit 1; }
cd "$REPO"
python3 "$TK/patches/instrument_w5d.py" "$L/uopt.c" "$L/uopt.w5d.c"
python3 "$TK/patches/patch_spill.py" "$L/uopt.w5d.c" "$L/uopt.trace.c"
python3 tools/workbench.py instrument-ugen --emit-provenance "$L/ugen.c" "$L/ugen.trace.c"
scp -q "$L/uopt.trace.c" "$L/ugen.trace.c" "$B:$SCR/src/"
scp -q "$TK"/patches/ecvt.patch.py "$TK"/patches/as1_printf.patch.py "$B:$SCR/patches/"
rsh "sh ~/$SCR/tk/build.sh ~/$SCR ~/$RECOMP"
echo "installed into $B:~/$SCR/bin; now: TAG=<tag> SCR=$SCR $TK/fidelity.sh NAME CAND.c"
