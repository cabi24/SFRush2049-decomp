# Sourced by every trace-toolkit script (Pi side). Lane conventions:
#   TAG  (required)  your lane tag: blob_unit --tag $TAG, unit stage on the builder in unit/$TAG/stage
#   SCR  (optional)  your builder scratch dir, relative to the builder's $HOME (default rush2049/scratch/frontier/$TAG);
#                    the traced binaries live in $SCR/bin (install.sh), snapshots in $SCR/st_<LABEL>
#   LOUT (optional)  Pi-side output dir for fetched objects/traces (default <repo>/build/trace/$TAG)
#   JOBS (optional)  builder parallelism for blob_unit (default 2: the builder is shared)
WD=${WD:-$(pwd)}
REPO=/home/cburnes/projects/rush2049-decomp
TK=$(cd "$(dirname "$0")" && pwd)    # the toolkit dir (scripts live at its top level)
BUILDER=${BUILDER:-watchman2}
IDO_REL=rush2049/cache/toolkits/796ae99a5cb7922e335e3afd87008753c573c82b45d04e8cfbce4f513cdbfbf5/ido
if [ -z "$TAG" ]; then echo "set TAG=<your lane tag> (and optionally SCR=<builder scratch dir>)" >&2; exit 2; fi
SCR=${SCR:-rush2049/scratch/frontier/$TAG}
SCR=${SCR#\~/}; SCR=${SCR#$HOME/}
LOUT=${LOUT:-$REPO/build/trace/$TAG}
JOBS=${JOBS:-2}
UNIT_STAGE=rush2049/scratch/frontier/unit/$TAG/stage
mkdir -p "$LOUT"
# IDO pass command lines, exactly as pipeline.blob_unit runs them
UOPT_ARGS="-G 0 -Olimit 5000 -mips2 -EB -g0 -O3"
UGEN_ARGS="-G 0 -mips2 -EB -g0 -O3"
AS1_ARGS="-elf -G 0 -p0 -mips2 -EB -g0 -O3 -r4300_mul -Olimit 5000"
rsh() { ssh -o BatchMode=yes "$BUILDER" "$@"; }
# unit score of NAME with FILE (+ extra blob_unit args); prints the verdict lines
unit_score() { _n=$1; _f=$2; shift 2
  (cd "$REPO" && python3 -m tools.conveyor.pipeline.blob_unit --tag "$TAG" --jobs "$JOBS" score $_n "$@" --with "$_f" 2>&1) |
    grep -E "(EQUAL|FAIL) |rror|locked bodies|inlined" | head -${SCN:-4}; }
# copy the unit stage's merged ucode + symbol table into $SCR/st_LABEL (the snapshot every tracer works on)
snap_unit() { rsh "mkdir -p ~/$SCR && cd ~/$SCR && rm -rf st_$1 && mkdir st_$1 && cp ~/$UNIT_STAGE/merged ~/$UNIT_STAGE/st st_$1/"; }
realp() { (cd "$WD" 2>/dev/null; realpath "$1"); }
# run remote/tk.sh on the builder against snapshot $SCR/st_LABEL (the helper is re-copied each call so it never drifts)
rtk() { scp -q "$TK"/remote/tk.sh "$BUILDER:$SCR/tk/tk.sh" && rsh "SCHED='$SCHED' VEC='$VEC' sh ~/$SCR/tk/tk.sh ~/$SCR $*"; }
fetch() { scp -q "$BUILDER:$SCR/st_$1/$2" "$3"; }
udiff() { python3 "$TK/udiff.py" "$@"; }
