#!/bin/bash
# sync.sh [GNAME]: assemble the group from seg/ and push parts + cand/GNAME to the builder
cd "$(dirname "$0")/.."
G=${1:-g1}
python3 tools/mk.py >/dev/null
ssh watchman2 "mkdir -p ~/rush2049/scratch/frontier/w35a/cand/$G ~/rush2049/scratch/frontier/w35a/parts"
scp -q groups/dot_cleanup_colortrace_20261011/group.* watchman2:rush2049/scratch/frontier/w35a/cand/$G/
scp -q parts/* watchman2:rush2049/scratch/frontier/w35a/parts/
