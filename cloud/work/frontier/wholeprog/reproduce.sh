#!/bin/sh
# Full experiment matrix.  Run on the builder from the isolated copy:
#   cd ~/rush2049/scratch/frontier/wholeprog && . wp/env.sh && wp/reproduce.sh
# Three lanes in parallel (the box is shared); about 15 minutes, nearly all of
# it the Python scorer (the IDO part of a full build is under 5 s).
set -u
python3 wp/wp.py inventory > wp/runs.inventory.txt 2>&1
mkdir -p wp/runs
python3 wp/wp.py baseline  > wp/runs/baseline.log 2>&1
python3 wp/wp.py o3single  > wp/runs/o3single.log 2>&1
KE=wp/lists/keep_extra.txt; PG=wp/lists/prefer_group_def.txt
(
  wp/runall.sh s50_all --set singles --limit 50 --keep all
  wp/runall.sh full_all --keep all
  wp/runall.sh strip_all --keep all --ctx strip
  wp/runall.sh full_all_si --keep all --standins
  wp/autoblock.sh ab_all --keep all
  wp/runall.sh singles_raddr --set singles --order raddr --keep all
  WP_NOINLINE=1 wp/runall.sh ni_all --keep all
  WP_NOMERGE=1 wp/runall.sh nm_all --keep all
) > wp/runs/lane1.log 2>&1 &
(
  wp/runall.sh full_roots --keep roots
  wp/runall.sh full_real --keep real
  wp/autoblock.sh ab_roots --keep roots
  wp/autoblock.sh ab_real --keep real
  wp/autoblock.sh ab_real_si --keep real --standins
  wp/autoblock.sh ab_roots_si --keep roots --standins
) > wp/runs/lane2.log 2>&1 &
(
  wp/runall.sh full_groups --keep groups
  wp/runall.sh full_groups_ck --keep groups --conflict keep
  wp/runall.sh full_groups_ns --keep groups --internal notsingle
  wp/runall.sh full_groups_m --keep groups --internal members
  wp/runall.sh full_groups_si --keep groups --standins
  wp/autoblock.sh ab_groups --keep groups
  B=wp/block_ab_groups.txt
  wp/runall.sh best_ke --keep groups --block $B --keep-extra $KE
  WP_PREFER_FROM=codex_entity_helpers wp/runall.sh best_name --keep groups --block $B --keep-extra $KE --prefer-group-def $PG
  WP_PREFER_FROM=codex_entity_helpers wp/runall.sh best_addr --order addr --keep groups --block $B --keep-extra $KE --prefer-group-def $PG
  WP_PREFER_FROM=codex_entity_helpers wp/runall.sh best_raddr --order raddr --keep groups --block $B --keep-extra $KE --prefer-group-def $PG
  WP_NOINLINE=1 wp/runall.sh ni_groups --keep groups
  WP_NOMERGE=1 wp/runall.sh nm_groups --keep groups
) > wp/runs/lane3.log 2>&1 &
wait
python3 wp/summary.py
