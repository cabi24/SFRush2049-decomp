#!/bin/sh
# w29n: re-apply the game symbol rename (renames.json) to a fresh tree, mechanically.
#
#   sh cloud/work/frontier/w29n/replay.sh [MAIN_CHECKOUT]
#
# Run from the root of a clean checkout whose HEAD is the pre-rename tree plus the
# w29n lane inputs (this directory, docs/external/gc, tools/gcref). MAIN_CHECKOUT
# (default /home/cburnes/projects/rush2049-decomp) supplies build/ when the tree has
# none, reference/gc/features.json for the correlation, and the tools/mips_to_c +
# decomp-permuter submodules are NOT needed. The live Conveyor DB is only READ (a
# sqlite backup copy is renamed under $W29N_DATA, default build/w29n_data).
# The result is left uncommitted in the working tree; every step refuses on a
# failed proof (set -e). Steps:
#   1  inventory: copy the DB, db_rename.py on the copy, blob_layout derive from it
#   2  data names: gamesyms.py writes GAME_SYMBOLS' generated block
#   3  symbol_addrs (symaddr_rename.py) + sdk_initialize context re-pin (static_rename owned)
#      + src/rom/storage_overrides.mk regenerated (owned_storage generate)
#   4  sources: users.py finds every src/blob C/H file that USES an old name (all
#      singles and groups, incl. ones installed after the w29n base); permute_c; the same for
#      the lane drafts cloud/work/frontier/w2[89]?/<fn>/best.c
#   5  group.json/unit_overrides names, git mv of renamed singles (json_rename.py)
#   6  lock: rename_resplice.py apply (keys, pure-rename proofs, recompile on watchman2,
#      image gate) -> w29n/blob_lock_migration.json; blob_tu symbols
#   7  receipts: the two symbols.json pins, slot-contract audit --write, rebind.py,
#      rebind_packets.py (packet list below), test token renames
#   8  CLAIMS.md (claims.py), recomp follow-ups (followups.py --write), correlation
#   9  gate chain: blob_splice check, blob_group check, blob_unit manifest+check,
#      blob_rom rom (SHA-1 EXACT)
set -e
MAIN=${1:-/home/cburnes/projects/rush2049-decomp}
L=cloud/work/frontier/w29n
MAP=$L/renames.json
DATA=${W29N_DATA:-build/w29n_data}
[ -z "$(git status --porcelain --untracked-files=no)" ] || { echo "replay: tree not clean"; exit 1; }
BASE=$(git rev-parse HEAD)
if [ ! -f build/blob_layout.json ]; then
  mkdir -p build/blob_unit
  cp -a "$MAIN"/build/blob "$MAIN"/build/ovl "$MAIN"/build/us "$MAIN"/build/game_code.bin \
        "$MAIN"/build/blob_layout.json "$MAIN"/build/blob_unit.json "$MAIN"/build/m2c_datasyms.json build/
  cp -a "$MAIN"/build/blob_unit/cache build/blob_unit/
  [ -f baserom.us.z64 ] || cp "$MAIN"/baserom.us.z64 .
fi
echo "== 1 inventory (copy of the live DB)"
mkdir -p "$DATA"; rm -f "$DATA/conveyor.db"; ln -sfn "$HOME/.conveyor/blobs" "$DATA/blobs"
python3 -c "import sqlite3,sys; s=sqlite3.connect('file:'+sys.argv[1]+'?mode=ro',uri=True); d=sqlite3.connect(sys.argv[2]); s.backup(d); d.close()" \
  "$HOME/.conveyor/conveyor.db" "$DATA/conveyor.db"
python3 $L/tools/db_rename.py $MAP --data "$DATA" --apply
python3 -m tools.conveyor.pipeline.blob_layout --data "$DATA" derive | tail -1
echo "== 2 GAME_SYMBOLS"; python3 $L/tools/gamesyms.py $MAP
echo "== 3 symbol_addrs + owned context"
python3 $L/tools/symaddr_rename.py $MAP
rm -f $L/rebind/sdk_initialize_context_hashes.json
python3 -m tools.conveyor.pipeline.static_rename owned $L/rebind/empty_renames.json --base HEAD --rebind-dir $L/rebind
python3 -m tools.conveyor.pipeline.owned_storage generate   # storage_overrides.mk follows the moved context proof
echo "== 4 sources"
files=$(python3 $L/tools/users.py $MAP $(find src/blob \( -name '*.c' -o -name '*.h' \) | sort) | cut -d: -f1)
python3 -m tools.conveyor.pipeline.static_rename source $MAP $files
# lane drafts reused next wave (w28*/w29* <fn>/best.c, not this lane): same rule, so they keep linking
drafts=$(python3 $L/tools/users.py $MAP $(ls cloud/work/frontier/w2[89]?/*/best.c | grep -v '^cloud/work/frontier/w29n/' | sort) | cut -d: -f1)
[ -z "$drafts" ] || python3 -m tools.conveyor.pipeline.static_rename source $MAP $drafts
echo "== 5 group.json / unit_overrides / singles"; python3 $L/tools/json_rename.py $MAP
echo "== 6 lock"
python3 $L/tools/rename_resplice.py apply $MAP --base HEAD
python3 -m tools.conveyor.pipeline.blob_tu symbols
echo "== 7 receipts"
OLD=$(git show HEAD:asm/us/blob/symbols.json | sha256sum | cut -d' ' -f1)
NEW=$(sha256sum asm/us/blob/symbols.json | cut -d' ' -f1)
for r in cloud/work/frontier/dot_nearest_point_donor_20261005/verification.json \
         cloud/work/frontier/dot_target_orientation_20261005/verification.json; do
  grep -q "$OLD" $r && sed -i "s/$OLD/$NEW/" $r
done
python3 cloud/work/frontier/dot_slot_contract_audit_20261006/audit.py --write > /dev/null
python3 $L/tools/rebind.py --base HEAD --write
python3 $L/tools/rebind_packets.py --base HEAD --write \
  cloud/work/encoded_string_mapping cloud/work/frontier/dot_runtime_a_writer_bounds_20261012 \
  cloud/work/frontier/dot_audio_bus_reopen cloud/work/frontier/dot_billboard_context_20261005 \
  tests/conveyor/test_billboard_save_input_packet.py tests/conveyor/test_encoded_string_mapping_reconstruction.py \
  tests/cloud/test_runtime_a_writer_bounds.py tests/conveyor/test_dot_audio_bus_reopen.py \
  tests/cloud/test_slot_contract_audit.py \
  cloud/work/dot_substantial_scout tests/conveyor/test_dot_substantial_scout.py
echo "== 8 CLAIMS, recomp follow-ups, correlation"
python3 $L/tools/claims.py $MAP
python3 $L/tools/followups.py --write
if [ -f "$MAIN/reference/gc/features.json" ]; then
  # PYTHONHASHSEED: ties among equal-score candidates follow set order; pinned for a reproducible table
  PYTHONHASHSEED=0 nice python3 -m tools.gcref.correlate --gc-features "$MAIN/reference/gc/features.json" | tail -1
fi
echo "== 9 gates"
python3 -m tools.conveyor.pipeline.blob_splice check
python3 -m tools.conveyor.pipeline.blob_group check
python3 -m tools.conveyor.pipeline.blob_unit manifest
python3 -m tools.conveyor.pipeline.blob_unit --tag w29n --jobs 2 check
python3 -m tools.conveyor.pipeline.blob_rom rom 2>&1 | tee build/w29n_rom.log | grep -E "SHA-1|stale"
grep -q "SHA-1 EXACT" build/w29n_rom.log
find $L -name __pycache__ -prune -exec rm -rf {} +
echo "replay: done on top of $BASE (uncommitted)"
