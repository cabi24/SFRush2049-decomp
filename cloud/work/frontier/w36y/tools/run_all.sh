#!/bin/sh
# w36y: regenerate every deliverable (read-only on the repo; writes only cloud/work/frontier/w36y/). ~3 min on the Pi.
set -e
cd "$(dirname "$0")"
python3 harvest.py            # build/harvest.json   (identities other lanes stated)
python3 contentscan.py        # build/content_hits.json (GC initialised data found by content)
W36Y_FINAL=1 python3 output.py   # map_data.json / map_data.md (code + content + lane channels)
python3 fields.py             # build/field_votes.json (struct field offsets, two passes)
python3 types_out.py          # types.md / types_n64.h
python3 renames.py            # renames_proposed.json
python3 evaluate.py | tail -1 # agreement with harvested identities (circular after the lane merge)
