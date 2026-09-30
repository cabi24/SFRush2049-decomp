#!/usr/bin/env python3
"""Generate ../group.c from tools/zlib-1.0.4 (run from anywhere: python3 gen/gen.py).

Every change to the zlib source is an entry in EDITS (old -> new, must match
exactly once), so the diff from upstream stays reviewable.
"""
import re, sys
from pathlib import Path
HERE = Path(__file__).resolve().parent
REPO = HERE.parents[4]
Z = REPO / 'tools' / 'zlib-1.0.4'
trees = (Z / 'trees.c').read_text()
deflate = (Z / 'deflate.c').read_text()

def between(text, start, end):
    a = text.index(start); b = text.index(end, a)
    return text[a:b]

def func(text, header):
    """from `header` to the closing brace at column 0"""
    a = text.index(header)
    b = text.index('\n}\n', a) + 3
    return text[a:b]

header = (HERE / 'header.h').read_text()
tables = (HERE / 'tables.h').read_text()

# trees.c: constants + macros, then the functions the game has
t_consts = between(trees, '#define MAX_BL_BITS 7', 'local int extra_lbits')
t_macros = between(trees, '#ifndef DEBUG\n#  define send_code', 'local void tr_static_init()')
parts = [t_consts, tables, t_macros]
for h in ['local void tr_static_init()', 'void _tr_init(s)', 'local void init_block(s)']:
    parts.append(func(trees, h))
parts.append(between(trees, '#define SMALLEST 1', 'local void pqdownheap(s, tree, k)'))
for h in ['local void pqdownheap(s, tree, k)', 'local void gen_bitlen(s, desc)',
          'local void gen_codes (tree, max_code, bl_count)', 'local void build_tree(s, desc)',
          'local void scan_tree (s, tree, max_code)', 'local void send_tree (s, tree, max_code)',
          'local int build_bl_tree(s)', 'local void send_all_trees(s, lcodes, dcodes, blcodes)',
          'void _tr_stored_block(s, buf, stored_len, eof)', 'ulg _tr_flush_block(s, buf, stored_len, eof)',
          'int _tr_tally (s, dist, lc)', 'local void compress_block(s, ltree, dtree)',
          'local unsigned bi_reverse(code, len)', 'local void bi_windup(s)',
          'local void copy_block(s, buf, len, header)']:
    parts.append(func(trees, h))
# deflate.c
parts.append(between(deflate, '#define UPDATE_HASH(s,h,c)', '/* ========================================================================= */\nint deflateInit_'))
for h in ['local void flush_pending(strm)', 'local int read_buf(strm, buf, size)',
          'local uInt longest_match(s, cur_match)', 'local void fill_window(s)']:
    parts.append(func(deflate, h))
parts.append(between(deflate, '#define FLUSH_BLOCK_ONLY(s, eof)', '/* ===========================================================================\n * Copy without'))
parts.append(func(deflate, 'local block_state deflate_slow(s, flush)'))
body = '\n'.join(parts)

for old, new in eval((HERE / 'edits.py').read_text()):
    n = body.count(old)
    if n != 1:
        sys.exit(f'edit matched {n} times: {old[:80]!r}')
    body = body.replace(old, new)

driver = (HERE / 'driver.c').read_text()
out = header + '\n' + body + '\n' + driver
(REPO / 'cloud/work/ipa-groups/zlib_deflate/group.c').write_text(out)
print('wrote', len(out.splitlines()), 'lines')
