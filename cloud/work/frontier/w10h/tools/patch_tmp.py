# Patch the w3a traced uopt.c: wrap temp-allocation functions and print tempdisp (0x1001c4b4) changes.
# usage: python3 patch_tmp.py in.c out.c   (output only when env TMPLOG is set; to stderr)
import re, sys
src = open(sys.argv[1]).read()
fns = {
 'f_spilltemps': ('void', ['mem','sp'], '(uint8_t *mem, uint32_t sp)'),
 'func_424ddc': ('void', ['mem','sp','v0','a0','a1','a2','a3'], '(uint8_t *mem, uint32_t sp, uint32_t v0, uint32_t a0, uint32_t a1, uint32_t a2, uint32_t a3)'),
 'f_reemit': ('void', ['mem','sp'], '(uint8_t *mem, uint32_t sp)'),
 'func_464e50': ('void', ['mem','sp','v0','a0','a1'], '(uint8_t *mem, uint32_t sp, uint32_t v0, uint32_t a0, uint32_t a1)'),
 'func_466790': ('uint32_t', ['mem','sp','v0','a0','a1','a2'], '(uint8_t *mem, uint32_t sp, uint32_t v0, uint32_t a0, uint32_t a1, uint32_t a2)'),
 'func_42c548': ('void', ['mem','sp','v0'], '(uint8_t *mem, uint32_t sp, uint32_t v0)'),
 'f_gettemp': ('void', ['mem','sp','a0','a1'], '(uint8_t *mem, uint32_t sp, uint32_t a0, uint32_t a1)'),
 'f_procinit': ('void', ['mem','sp'], '(uint8_t *mem, uint32_t sp)'),
}
wr = ['\n#include <stdio.h>\n#include <stdlib.h>\nstatic int tmplog_on = -1; static int tmplog_proc = 0;\n#define TMPON (tmplog_on < 0 ? (tmplog_on = getenv("TMPLOG") != NULL) : tmplog_on)\n']
for fn, (rt, args, sig) in fns.items():
    pat = 'static %s %s%s {' % (rt, fn, sig)
    assert src.count(pat) == 1, fn
    src = src.replace(pat, 'static %s %s_X%s;\nstatic %s %s_X%s {' % (rt, fn, sig, rt, fn, sig))
    call = '%s_X(%s)' % (fn, ', '.join(args))
    body = 'uint32_t d0 = MEM_U32(0x1001c4b4);'
    if rt == 'void':
        body += call + ';'
    else:
        body += 'uint32_t r = ' + call + ';'
    if fn == 'f_procinit':
        body += 'tmplog_proc++; if (TMPON) fprintf(stderr, "[TMP] procinit proc=%d\\n", tmplog_proc);'
    elif fn == 'f_gettemp':
        body += ('if (TMPON) { uint32_t rec = MEM_U32(a0); fprintf(stderr, "[TMP] gettemp proc=%d size=%d num=%d off=%d recsize=%d disp %d->%d\\n", tmplog_proc, (int)a1, (int)MEM_U32(rec+0), (int)MEM_U32(rec+4), (int)MEM_U32(rec+8), (int)d0, (int)MEM_U32(0x1001c4b4)); }')
    else:
        body += 'if (TMPON && d0 != MEM_U32(0x1001c4b4)) fprintf(stderr, "[TMP] %s proc=%%d disp %%d->%%d\\n", tmplog_proc, (int)d0, (int)MEM_U32(0x1001c4b4));' % fn
    if rt != 'void':
        body += 'return r;'
    wr.append('static %s %s%s { %s }\n' % (rt, fn, sig, body))
src = src + ''.join(wr)
open(sys.argv[2], 'w').write(src)
