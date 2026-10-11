# w19d: test-speed tooling (pytest-xdist, second CI-equivalent host)

Status: IN PROGRESS (skeleton written early; sections fill as runs finish).

## Hosts
- watchman2: ~/rush2049/scratch/ciclone-w19d (copy of ciclone6), venv
  ~/rush2049/scratch/frontier/w19d/venv (civenv's pins + pytest-xdist 3.8.0);
  timing runs pinned to 4 cores with `taskset -c 0-3`.
- rocky: ~/rush2049/scratch/ciclone (bundle clone + submodules), civenv,
  IDO 5.3 + 7.1 via tools/cloud/setup.sh, MIPS binutils 2.45 built from
  source into ~/rush2049/opt/mips-binutils (no sudo).

## Timings
(pending)

## Serial list
(pending)

## Commands
(pending)
