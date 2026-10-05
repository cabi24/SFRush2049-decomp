#!/bin/sh
# gen.sh "DECLS" out.c
python3 - "$1" "$2" <<'PY'
import sys
s=open('race_setup_2/b.c').read().replace('    DECLS', sys.argv[1])
open(sys.argv[2],'w').write(s)
PY
