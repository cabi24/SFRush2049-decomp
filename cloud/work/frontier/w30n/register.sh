#!/bin/sh
# w30n Job 1: register the game function heads the Conveyor inventory lacks (func_80104704),
# mechanically, on a tree that does not carry the registration yet.
#
#   sh cloud/work/frontier/w30n/register.sh [MAIN_CHECKOUT]
#
# Steps (refresh-game-context skill, then the receipts that summarise the whole target table):
#   1  copy the live DB (sqlite backup, read-only) and hard-link the blob store under $DATA
#   2  `closure run` on the copy, twice (the second must register 0): registers func_80104704 from the
#      jal at 0x80105730 (005 extent scanner: 260 words) and the suffix rule marks highscore_entry_anim
#      extent_conflict:func_80104704
#   3  blob_layout derive (from the copy); blob_splice rebuild-stale (builder, this lane's scratch);
#      blob_tu generate (asm/us/blob regions, blob.ld, symbols.json, SHA256SUMS)
#   4  repin_heads.py --write: proved re-pins (symbols.json pins, census digests, slot-contract audit +
#      its test counts, source-boundary scout word count, recomp census entry of the retired label)
# Tests/doc edits (test_ipakit_heads/deps, unregistered-heads.md, CLAIMS.md) are inputs, not replayed.
# The live DB is never written; the coordinator applies step 2 to it (RESULTS.md "Integration").
set -e
MAIN=${1:-/home/cburnes/projects/rush2049-decomp}
DATA=${W30N_DATA:-build/w30n_data}
if [ ! -f build/blob_layout.json ]; then
  mkdir -p build/blob_unit
  cp -a "$MAIN"/build/blob "$MAIN"/build/ovl "$MAIN"/build/us "$MAIN"/build/game_code.bin \
        "$MAIN"/build/blob_layout.json "$MAIN"/build/blob_unit.json "$MAIN"/build/m2c_datasyms.json build/
  cp -a "$MAIN"/build/blob_unit/cache build/blob_unit/
  [ -f baserom.us.z64 ] || cp "$MAIN"/baserom.us.z64 .
fi
mkdir -p "$DATA"; rm -f "$DATA/conveyor.db"
[ -d "$DATA/blobs" ] && [ ! -L "$DATA/blobs" ] || { rm -f "$DATA/blobs"; cp -al "$HOME/.conveyor/blobs" "$DATA/blobs"; }
python3 -c "import sqlite3,sys; s=sqlite3.connect('file:'+sys.argv[1]+'?mode=ro',uri=True); d=sqlite3.connect(sys.argv[2]); s.backup(d); d.close()" \
  "$HOME/.conveyor/conveyor.db" "$DATA/conveyor.db"
python3 -m tools.conveyor.pipeline.closure --data "$DATA" run --report build/w30n_closure.json | tail -4
python3 -m tools.conveyor.pipeline.closure --data "$DATA" run --report build/w30n_closure2.json | grep -q "registered=0 "
python3 -m tools.conveyor.pipeline.blob_layout --data "$DATA" derive | tail -3
python3 -m tools.conveyor.pipeline.blob_splice rebuild-stale --builder-dir rush2049/scratch/frontier/w30n/splice \
  --group-builder-dir rush2049/scratch/frontier/w30n/group | tail -2
python3 -m tools.conveyor.pipeline.blob_tu generate | tail -1
python3 cloud/work/frontier/w30n/tools/repin_heads.py --write
echo "register: done (uncommitted)"
