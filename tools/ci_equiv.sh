#!/bin/bash
# CI-equivalent test run on a remote x86-64 host (docs/BUILDING.md,
# "CI-equivalent test hosts"). Run on the Pi from the repository:
#
#   git bundle create /tmp/X.bundle HEAD --remotes=origin
#   tools/ci_equiv.sh HOST /tmp/X.bundle
#
# Copies the bundle to HOST, checks its HEAD out in a disposable clone there
# (remote branches too: research packets `git show` commits that live only on
# old unmerged branches), confirms IDO 5.3 and 7.1 are installed, and runs
# tests/conveyor then tests/cloud with REQUIRE_TOOLCHAIN=1 and
# -m "not node_required", exactly like .github/workflows/verify.yml. Prints the
# HEAD it tested and one summary line per suite; exits non-zero if either
# suite fails. Full logs stay on HOST in $CI_LOGS.
#
# Host settings come from ~/rush2049/scratch/ci.env on HOST when present
# (shell assignments), else these defaults:
#   CI_CLONE  ~/rush2049/scratch/ciclone6   disposable clone (never a work tree)
#   CI_VENV   ~/rush2049/scratch/civenv     venv with pytest (+ pytest-xdist for CI_FAST)
#   CI_PATH   (empty)                       extra PATH entries, e.g. MIPS binutils
#   CI_LOGS   ~/rush2049/scratch/ci-logs
# Variables set in the caller's environment (CI_CLONE, CI_VENV, CI_LOGS,
# CI_FAST) win over ci.env. CI_BUNDLES (caller only; default
# rush2049/scratch/ci-bundles, relative to HOST's home) is where the bundle goes.
#
#   CI_FAST=N  run each suite as `make test-fast`'s split: the parallel part with
#              pytest-xdist -n N, then the serial part (tests marked `serial`).
#              Unset: the plain serial pytest commands CI runs.
set -euo pipefail

if [ $# -ne 2 ]; then
  echo "usage: $0 HOST BUNDLE" >&2
  exit 2
fi
HOST=$1
BUNDLE=$2
[ -f "$BUNDLE" ] || { echo "error: no bundle at $BUNDLE" >&2; exit 2; }
git bundle verify -q "$BUNDLE" >/dev/null 2>&1 || {
  echo "error: $BUNDLE is not a valid bundle for this repository" >&2; exit 2; }
SHA=$(git bundle list-heads "$BUNDLE" HEAD | awk '{print $1}')
[ -n "$SHA" ] || { echo "error: $BUNDLE has no HEAD (create it with HEAD --remotes=origin)" >&2; exit 2; }

BUNDLE_DIR=${CI_BUNDLES:-rush2049/scratch/ci-bundles}
REMOTE_BUNDLE="$BUNDLE_DIR/$(basename "$BUNDLE")"
ssh -o BatchMode=yes "$HOST" "mkdir -p $(printf '%q' "$BUNDLE_DIR")"
scp -q "$BUNDLE" "$HOST:$REMOTE_BUNDLE"

# The remote script takes its parameters as positional arguments, each quoted
# with %q (ssh joins its arguments into one command line; empty ones survive).
ARGS=$(printf '%q ' "$SHA" "$REMOTE_BUNDLE" "${CI_CLONE:-}" "${CI_VENV:-}" "${CI_FAST:-}" \
       "${CI_LOGS:-}")
ssh -o BatchMode=yes "$HOST" "bash -s -- $ARGS" <<'REMOTE'
set -uo pipefail
SHA=$1; BUNDLE=$2; case $BUNDLE in /*) ;; *) BUNDLE=$HOME/$BUNDLE ;; esac
OVR_CLONE=$3; OVR_VENV=$4; CI_FAST=$5; OVR_LOGS=$6
CI_CLONE=$HOME/rush2049/scratch/ciclone6
CI_VENV=$HOME/rush2049/scratch/civenv
CI_PATH=
CI_LOGS=$HOME/rush2049/scratch/ci-logs
[ -f "$HOME/rush2049/scratch/ci.env" ] && . "$HOME/rush2049/scratch/ci.env"
[ -n "$OVR_CLONE" ] && CI_CLONE=$OVR_CLONE
[ -n "$OVR_VENV" ] && CI_VENV=$OVR_VENV
[ -n "$OVR_LOGS" ] && CI_LOGS=$OVR_LOGS
export PATH="$CI_VENV/bin${CI_PATH:+:$CI_PATH}:$PATH"
export REQUIRE_TOOLCHAIN=1
cd "$CI_CLONE" || { echo "error: no clone at $CI_CLONE"; exit 2; }
mkdir -p "$CI_LOGS"

git checkout -q . && git clean -qfd cloud tests \
  && git fetch -q "$BUNDLE" HEAD "refs/remotes/origin/*:refs/remotes/origin/*" \
  && git checkout -q "$SHA" || { echo "error: checkout of $SHA failed"; exit 2; }
for ido in tools/cloud/ido tools/cloud/ido71; do
  [ -x "$ido/cc" ] || { echo "error: $CI_CLONE/$ido/cc missing (run bash tools/cloud/setup.sh there)"; exit 2; }
done
echo "host $(hostname)  clone $CI_CLONE  HEAD $(git log --oneline -1)"

rc=0
for suite in conveyor cloud; do
  log=$CI_LOGS/${SHA:0:12}-$suite.log
  start=$(date +%s)
  if [ -n "$CI_FAST" ]; then
    make --no-print-directory test-fast SUITES="tests/$suite" JOBS="$CI_FAST" \
        PYTEST=python >"$log" 2>&1
  else
    python -m pytest "tests/$suite" -rs -m "not node_required" \
        -p no:cacheprovider >"$log" 2>&1
  fi
  status=$?
  [ $status -eq 0 ] || rc=1
  summary=$(grep -E '^(=+ )?([0-9]+ (passed|failed|errors?|skipped|deselected|xfailed|xpassed)|no tests ran).* in [0-9.]+s' "$log" \
            | sed -E 's/^=+ //; s/ =+$//' | paste -sd '|' -)
  echo "tests/$suite: ${summary:-no summary line} (exit $status, $(( $(date +%s) - start ))s; log $log)"
done
exit $rc
REMOTE
