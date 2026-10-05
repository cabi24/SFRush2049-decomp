#!/usr/bin/env python3
"""Private C13 proof: remove original PI table32-byte slot from composed input."""
from pathlib import Path
import sys,hashlib,json
src=Path(sys.argv[1]);out=Path(sys.argv[2]);data=src.read_bytes();slot=0x1E460;size=0x20
expected='7cd466024e8a90377babb0f8d67c4da7d5c38af1acf59be025edb7b8456ebb9d'
assert hashlib.sha256(data[slot:slot+size]).hexdigest()==expected, 'original table byte-identity guard failed'
out.with_suffix('.prefix.bin').write_bytes(data[:slot]);out.with_suffix('.suffix.bin').write_bytes(data[slot+size:])
