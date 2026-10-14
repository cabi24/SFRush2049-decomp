#!/bin/sh
# w29n/w30n: apply one batch of the game symbol rename table (renames.json) to a tree, mechanically.
#
#   RENAME_BATCH=w30n sh cloud/work/frontier/w29n/replay.sh [MAIN_CHECKOUT]
#
# renames.json is the single table; every accepted row names the `batch` that applies it
# (w29n: wave 29; w30n: the 10 rows w29n deferred, wave 30; w32n: 19 GC screen/ghost names, wave 32).
# RENAME_BATCH (default w30n)
# selects the batch; tools that act on names the tree must still carry (inventory, symbol_addrs,
# sources, lock re-key, CLAIMS) get only that batch (tools/batch.py), token-only tools the
# whole table (they are idempotent on applied rows). Builder dirs: rush2049/scratch/frontier/$RENAME_BATCH.
#
# Run from the root of a clean checkout whose HEAD is the pre-rename tree plus the
# w29n lane inputs (this directory, docs/external/gc, tools/gcref). MAIN_CHECKOUT
# (default /home/cburnes/projects/rush2049-decomp) supplies build/ when the tree has
# none, reference/gc/features.json for the correlation, and the tools/mips_to_c +
# decomp-permuter submodules are NOT needed. The live Conveyor DB is only READ (a
# sqlite backup copy is renamed under $W29N_DATA, default build/w29n_data).
# The result is left uncommitted in the working tree; every step refuses on a
# failed proof (set -e). Steps:
#   1  inventory: copy the DB, `closure run` on the copy (registers the heads the live DB
#      still lacks, e.g. w30n's func_80104704; a no-op on a DB at fixpoint), db_rename.py
#      on the copy, blob_layout derive from it
#   2  data names: gamesyms.py writes GAME_SYMBOLS' generated block
#   3  symbol_addrs (symaddr_rename.py) + sdk_initialize context re-pin (static_rename owned)
#      + src/rom/storage_overrides.mk regenerated (owned_storage generate)
#   4  sources: users.py finds every src/blob C/H file that USES an old name (all
#      singles and groups, incl. ones installed after the w29n base); permute_c; the same for
#      the lane drafts (every tracked C/H file of cloud/work/frontier/w2[89]?/ and w3[0-9]?/);
#      lane_rename.py: their dc.args argument lines and group.json name values
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
BATCH=${RENAME_BATCH:-w30n}
DELTA=build/rename_batch_$BATCH.json
DATA=${W29N_DATA:-build/${BATCH}_data}
export RENAME_WORK=rush2049/scratch/frontier/$BATCH
[ -z "$(git status --porcelain --untracked-files=no)" ] || { echo "replay: tree not clean"; exit 1; }
BASE=$(git rev-parse HEAD)
if [ ! -f build/blob_layout.json ]; then
  mkdir -p build/blob_unit
  cp -a "$MAIN"/build/blob "$MAIN"/build/ovl "$MAIN"/build/us "$MAIN"/build/game_code.bin \
        "$MAIN"/build/blob_layout.json "$MAIN"/build/blob_unit.json "$MAIN"/build/m2c_datasyms.json build/
  cp -a "$MAIN"/build/blob_unit/cache build/blob_unit/
  [ -f baserom.us.z64 ] || cp "$MAIN"/baserom.us.z64 .
fi
python3 $L/tools/batch.py $MAP $BATCH $DELTA
echo "== 1 inventory (copy of the live DB)"
mkdir -p "$DATA"; rm -f "$DATA/conveyor.db"
# hard-linked copy of the blob store: registrations add objects to the copy only (never to ~/.conveyor)
[ -d "$DATA/blobs" ] && [ ! -L "$DATA/blobs" ] || { rm -f "$DATA/blobs"; cp -al "$HOME/.conveyor/blobs" "$DATA/blobs"; }
python3 -c "import sqlite3,sys; s=sqlite3.connect('file:'+sys.argv[1]+'?mode=ro',uri=True); d=sqlite3.connect(sys.argv[2]); s.backup(d); d.close()" \
  "$HOME/.conveyor/conveyor.db" "$DATA/conveyor.db"
python3 -m tools.conveyor.pipeline.closure --data "$DATA" run --report build/${BATCH}_closure.json | tail -3
# w32n: objects copied from the main checkout can be stale (bound to pre-rename sources): rebuild them on
# the PRE-rename tree first, or the lock step cannot compose the image (LockedBodyError)
python3 -m tools.conveyor.pipeline.blob_layout --data "$DATA" derive | tail -1
python3 -m tools.conveyor.pipeline.blob_splice --data "$DATA" rebuild-stale | head -1
python3 $L/tools/db_rename.py $DELTA --data "$DATA" --apply
python3 -m tools.conveyor.pipeline.blob_layout --data "$DATA" derive | tail -1
echo "== 2 GAME_SYMBOLS"; python3 $L/tools/gamesyms.py $MAP
echo "== 3 symbol_addrs + owned context"
python3 $L/tools/symaddr_rename.py $DELTA
# each batch writes its own context proof (the previous batch's proof is what rom_owned_data.json points at)
RB=cloud/work/frontier/$BATCH/rebind; mkdir -p $RB; rm -f $RB/sdk_initialize_context_hashes.json
python3 -m tools.conveyor.pipeline.static_rename owned $L/rebind/empty_renames.json --base HEAD --rebind-dir $RB
python3 -m tools.conveyor.pipeline.owned_storage generate   # storage_overrides.mk follows the moved context proof
echo "== 4 sources"
files=$(python3 $L/tools/users.py $DELTA $(find src/blob \( -name '*.c' -o -name '*.h' \) | sort) | cut -d: -f1)
[ -z "$files" ] || python3 -m tools.conveyor.pipeline.static_rename source $DELTA $files
# lane drafts reused next wave (w28*/w29* <fn>/best.c, not this lane): same rule, so they keep linking
# + every tracked C/H file of the wave-30+ lane dirs (drafts, group packets with their ctx_*.c files)
# + (w32n) every tracked C/H file of the w28/w29 lane dirs too (e.g. w28c's dc_static.c, which a recomp test runs)
drafts=$( (ls cloud/work/frontier/w2[89]?/*/best.c 2>/dev/null; git ls-files 'cloud/work/frontier/w2[89]?/*.c' 'cloud/work/frontier/w2[89]?/*.h' \
           'cloud/work/frontier/w3[0-9]?/*.c' 'cloud/work/frontier/w3[0-9]?/*.h') \
  | grep -v '^cloud/work/frontier/w29n/\|^cloud/work/frontier/w30n/\|^cloud/work/frontier/w32n/' | sort -u | xargs -r python3 $L/tools/users.py $DELTA | cut -d: -f1)
[ -z "$drafts" ] || python3 -m tools.conveyor.pipeline.static_rename source $DELTA $drafts
# w32n: the lane dirs' dc.args argument lines and group packets' group.json name values
python3 $L/tools/lane_rename.py $DELTA --write
echo "== 5 group.json / unit_overrides / singles"; python3 $L/tools/json_rename.py $DELTA
echo "== 6 lock"
python3 $L/tools/rename_resplice.py apply $DELTA --base HEAD --out blob_lock_migration_$BATCH.json
python3 -m tools.conveyor.pipeline.blob_tu symbols
echo "== 7 receipts"
OLD=$(git show HEAD:asm/us/blob/symbols.json | sha256sum | cut -d' ' -f1)
NEW=$(sha256sum asm/us/blob/symbols.json | cut -d' ' -f1)
for r in cloud/work/frontier/dot_nearest_point_donor_20261005/verification.json \
         cloud/work/frontier/dot_target_orientation_20261005/verification.json; do
  grep -q "$OLD" $r && sed -i "s/$OLD/$NEW/" $r
done
python3 cloud/work/frontier/dot_slot_contract_audit_20261006/audit.py --write > /dev/null
python3 $L/tools/rebind.py --base HEAD --map $DELTA --write
python3 $L/tools/rebind_packets.py --base HEAD --map $DELTA --users-only --write \
  cloud/work/encoded_string_mapping cloud/work/frontier/dot_runtime_a_writer_bounds_20261012 \
  cloud/work/frontier/dot_audio_bus_reopen cloud/work/frontier/dot_billboard_context_20261005 \
  tests/conveyor/test_billboard_save_input_packet.py tests/conveyor/test_encoded_string_mapping_reconstruction.py \
  tests/cloud/test_runtime_a_writer_bounds.py tests/conveyor/test_dot_audio_bus_reopen.py \
  tests/cloud/test_slot_contract_audit.py \
  cloud/work/dot_substantial_scout tests/conveyor/test_dot_substantial_scout.py \
  cloud/work/frontier/dot_node_pool_init_20261005 tests/cloud/test_node_pool_init_match.py \
  cloud/work/d05_hud_context_audit tests/conveyor/test_d05_hud_context_audit.py \
  cloud/work/dot_name_cache tests/conveyor/test_name_cache_research.py \
  cloud/work/frontier/dot_material_selector_20261006 tests/conveyor/test_dot_material_selector_20261006.py \
  tests/conveyor/test_disasm.py \
  cloud/work/frontier/dot_steady_camera_20261005 tests/cloud/test_dot_steady_camera.py \
  cloud/work/frontier/dot_frame_dispatch_20261005 tests/cloud/test_dot_frame_dispatch_match.py \
  cloud/work/tiny_A68/func_800F1D04.c \
  cloud/work/frontier/dot_billboard_helpers_20261005 tests/conveyor/test_billboard_name_entry_packet.py \
  cloud/work/frontier/dot_vector_blend tests/cloud/test_dot_vector_blend_packet.py \
  cloud/work/ipa-groups/dot_steady_camera_20261005 cloud/work/near-miss/func_8008B000/base.c \
  cloud/work/dot_resource_init tests/conveyor/test_resource_init_research.py \
  cloud/work/frontier/dot_resource_initializer tests/conveyor/test_frontier_resource_initializer.py \
  cloud/work/frontier/dot_slot_setter_boundary_20261006 tests/conveyor/test_dot_slot_setter_boundary_20261006.py \
  tests/cloud/test_ipakit_deps.py tests/cloud/test_ipakit_heads.py tests/cloud/test_recomp_phase11.py \
  cloud/work/ipa-groups/dot_menu_row_caller_20261005 tests/conveyor/test_dot_menu_row_caller.py \
  cloud/work/dot_menu_options_root_20261005 tests/conveyor/test_dot_menu_options_root.py \
  cloud/work/frontier/dot_cone_start_donor_20261005 tests/conveyor/test_cone_start_donor.py \
  cloud/work/ipa-groups/dot_speed_flags_c9210_20261006 tests/conveyor/test_speed_flags_contract.py \
  cloud/work/ipa-groups/billboard_render/br.c cloud/work/heads_B14/func_8010DCFC_singleformal.c
# w30n: exact game-side edits in fixtures that also hold frozen runtime-image spellings
python3 $L/tools/rebind_edits.py --base HEAD --map $DELTA --write
echo "== 8 CLAIMS, recomp follow-ups, correlation"
python3 $L/tools/claims.py $DELTA
python3 $L/tools/followups.py --write
if [ -f "$MAIN/reference/gc/features.json" ]; then
  # PYTHONHASHSEED: ties among equal-score candidates follow set order; pinned for a reproducible table
  PYTHONHASHSEED=0 nice python3 -m tools.gcref.correlate --gc-features "$MAIN/reference/gc/features.json" | tail -1
fi
echo "== 9 gates"
python3 -m tools.conveyor.pipeline.blob_splice check
python3 -m tools.conveyor.pipeline.blob_group check
python3 -m tools.conveyor.pipeline.blob_unit manifest
python3 -m tools.conveyor.pipeline.blob_unit --tag $BATCH --jobs 2 check
python3 -m tools.conveyor.pipeline.blob_rom rom 2>&1 | tee build/${BATCH}_rom.log | grep -E "SHA-1|stale"
grep -q "SHA-1 EXACT" build/${BATCH}_rom.log
find $L -name __pycache__ -prune -exec rm -rf {} +
echo "replay: done on top of $BASE (uncommitted)"
