# Sourced by every trace71 script (Pi side).
#   SCR  (or TAG)  builder scratch root, relative to the builder's $HOME (default rush2049/scratch/frontier/$TAG); it
#                  holds include/, tools/conveyor (for reduced_tu), tgt/FN.o (w17k fetch_targets.py) and t71/ (install71.sh)
#   LOUT (optional) Pi-side output dir for fetched traces (default <repo>/build/trace71/<scratch basename>)
WD=${WD:-$(pwd)}
REPO=/home/cburnes/projects/rush2049-decomp
TK=$(cd "$(dirname "$0")" && pwd)
BUILDER=${BUILDER:-watchman2}
if [ -z "$SCR" ] && [ -z "$TAG" ]; then echo "set SCR=<builder scratch dir> (or TAG=<lane>)" >&2; exit 2; fi
SCR=${SCR:-rush2049/scratch/frontier/$TAG}
SCR=${SCR#\~/}; SCR=${SCR#$HOME/}
LOUT=${LOUT:-$REPO/build/trace71/$(basename $SCR)}
mkdir -p "$LOUT"
rsh() { ssh -o BatchMode=yes "$BUILDER" "$@"; }
realp() { (cd "$WD" 2>/dev/null; realpath "$1"); }
# push a local source to SCR/t71/in/ and echo its builder path
push() { _p=$(realp "$1"); _b=t71/in/$(echo "$_p" | md5sum | cut -c1-8)_$(basename "$_p")
  rsh "mkdir -p ~/$SCR/t71/in" && scp -q "$_p" "$BUILDER:$SCR/$_b" && echo "$_b"; }
# run remote/tk71.sh on the builder (re-copied each call so it never drifts)
rtk() { scp -q "$TK"/remote/tk71.sh "$BUILDER:$SCR/t71/tk71.sh" && rsh "SCHED='$SCHED' DIFF='$DIFF' FLAGS='$FLAGS' sh ~/$SCR/t71/tk71.sh ~/$SCR $*"; }
fetch() { scp -q "$BUILDER:$SCR/t71/run/$1" "$2"; }
