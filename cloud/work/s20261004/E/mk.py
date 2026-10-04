"""mk.py BASE OUT [func=file ...] : replace top-level definitions of func in BASE with file contents."""
import re, sys
s = open(sys.argv[1]).read()
for a in sys.argv[3:]:
    fn, path = a.split('=', 1)
    m = re.search(r'^[^\n;{}]*\b' + fn + r'\([^;{]*\)\s*\{', s, re.M)
    i = m.start(); depth = 0; j = m.end() - 1
    while True:
        if s[j] == '{': depth += 1
        elif s[j] == '}':
            depth -= 1
            if depth == 0: break
        j += 1
    s = s[:i] + open(path).read().strip() + '\n' + s[j+1:]
open(sys.argv[2], 'w').write(s)
