#!/usr/bin/env python3
"""reorder.py IN OUT name1 name2 ... : reorder top-level function definitions (by name) to the given order,
placed where the first function definition was. Functions not listed keep relative order after."""
import sys, re
src = open(sys.argv[1]).read(); order = sys.argv[3:]
# split into chunks: find each top-level function def start (line starting with type and name( ... ) and body to \n}\n
pat = re.compile(r'^(?:/\*[^\n]*\*/\n)?(?:static )?(?:void|s8|s32|u32|f32) (\w+)\([^;{]*\)[^;{\n]*\n\{', re.M)
spans = []
for m in pat.finditer(src):
    a = m.start(); b = src.index('\n}\n', a) + 3
    spans.append((m.group(1), a, b))
head = src[:spans[0][1]]
chunks = {n: src[a:b] for n, a, b in spans}
between = []
prev = spans[0][1]
for n, a, b in spans:
    between.append(src[prev:a]); prev = b
tail = src[prev:]
rest = [n for n, a, b in spans if n not in order]
protos = 'void func_8038FFD8(void);\ns8 func_8038F744(s32);\nvoid func_8038FFD0(void);\nstatic void uninitButtons(void);\nstatic void uninitScreen(void);\nvoid func_8038F79C(void);\nvoid InitEgg(s32);\nvoid func_80390418(void);\n\n' if len(sys.argv) and 'start' in sys.argv[1] else ''
out = head + protos + ''.join(between) + '\n'.join(chunks[n] for n in order + rest) + tail
open(sys.argv[2], 'w').write(out)
