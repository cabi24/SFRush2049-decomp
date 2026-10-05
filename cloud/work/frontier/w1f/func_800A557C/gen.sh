#!/bin/sh
python3 - "$1" "$2" "$3" <<'PY'
import sys
s=open(sys.argv[3]).read().replace('    DECLS', sys.argv[1])
open(sys.argv[2],'w').write(s)
PY
