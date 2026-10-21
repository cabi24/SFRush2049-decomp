import itertools, sys
src = open('v/q2.c').read()
head = src[:src.index("void physics_float_calc")].replace("void ShowObject(ObjInstance *obj);", "void ShowCull();")
pre = src[src.index("void physics_float_calc"):src.index("    parts = D_8011E748")]
pre = pre.replace("    s32 dbg;\n", "    ObjInstance *obj;\n    s32 dbg;\n")
walks = {
 'w':   ("            w++;\n", "ptr[w]", "ptr_no_clip[w]"),
 'p':   ("            ptr++;\n", "*ptr", "*ptr_no_clip"),
}
cargs = ['', 'i', '&obj[i]', 'ptr', 'mask', 'show', 'unused']
objpos = ('afterw', 'beforeparts')
man = []; n = 0
for (wk, (inc, pe, ne)), nc, ca, cp, df, op in itertools.product(walks.items(), (0, 1), cargs + ['NONE'], ('top', 'mid', 'end'), (0, 1), objpos):
    if ca == 'NONE' and cp != 'top': continue
    p = pre
    if not df: p = p.replace("        default: break;\n", "")
    if ca == 'NONE': p = p.replace("    s32 dbg;\n", "").replace("    dbg = 0;\n", "")
    call = "        if (dbg) {\n            ShowCull(%s);\n        }\n" % ca
    if ca == 'NONE': call = ''
    ncs = ("|| (ptr_no_clip ? mask & %s : 0) " % ne) if nc else ""
    ob = "    obj = &D_8012E700[D_80149D90];\n"
    L = (ob if op == 'beforeparts' else '') + "    parts = D_8011E748[D_80149B8C];\n    mask = 1;\n    w = 0;\n" + (ob if op == 'afterw' else '')
    L += "    for (i = 0; i < parts; i++) {\n"
    L += "        if (i && !(i & 31)) {\n            mask = 1;\n" + inc + "        }\n"
    if cp == 'top': L += call
    L += "        if (((mask & %s) %s|| show) && !D_8014061A) {\n" % (pe, ncs)
    L += "            obj[i].flags &= ~0x80000000;\n        } else {\n            obj[i].flags |= 0x80000000;\n        }\n"
    if cp == 'mid': L += call
    L += "        mask <<= 1;\n"
    if cp == 'end': L += call
    L += "    }\n}\n"
    fn = 'b4/v%04d.c' % n
    open(fn, 'w').write(head + p + L)
    man.append('%s\t%s\tnc=%d\t%s\t%s\tdf=%d\t%s' % (fn, wk, nc, ca, cp, df, op)); n += 1
open('b4/manifest.tsv', 'w').write('\n'.join(man) + '\n')
print(n)
