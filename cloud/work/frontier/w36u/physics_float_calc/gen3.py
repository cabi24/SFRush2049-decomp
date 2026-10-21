import itertools, sys
src = open('v/q2.c').read()
head = src[:src.index("void physics_float_calc")].replace("void ShowObject(ObjInstance *obj);", "void ShowCull();")
pre = src[src.index("void physics_float_calc"):src.index("    parts = D_8011E748")]
pre = pre.replace("    s32 dbg;\n", "    ObjInstance *obj;\n    s32 dbg;\n")
objforms = {
 'idx':  ("", "", "D_8012E700[D_80149D90 + i]", "&D_8012E700[D_80149D90 + i]", ""),
 'idx2': ("", "", "(D_8012E700 + D_80149D90)[i]", "(D_8012E700 + D_80149D90) + i", ""),
 'objp': ("    obj = &D_8012E700[D_80149D90];\n", "", "(*obj)", "obj", "        obj++;\n"),
 'objt': ("", "        obj = &D_8012E700[D_80149D90 + i];\n", "(*obj)", "obj", ""),
}
walks = {
 'w':   ("            w++;\n", "ptr[w]", "ptr_no_clip[w]"),
 'p':   ("            ptr++;\n", "*ptr", "*ptr_no_clip"),
 'pw':  ("            ptr++;\n            w++;\n", "*ptr", "*ptr_no_clip"),
 'pnc': ("            ptr++;\n            ptr_no_clip++;\n", "*ptr", "*ptr_no_clip"),
 'wnc': ("            w++;\n            ptr_no_clip++;\n", "ptr[w]", "*ptr_no_clip"),
}
man = []; n = 0
for (ok, (bl, lt, lv, ca, le)), (wk, (inc, pe, ne)), nc, cp, df in itertools.product(objforms.items(), walks.items(), (0, 1), ('none', 'top', 'mid', 'end'), (0, 1)):
    p = pre
    if not df: p = p.replace("        default: break;\n", "")
    if cp == 'none': p = p.replace("    s32 dbg;\n", "").replace("    dbg = 0;\n", "")
    if ok in ('idx', 'idx2'): p = p.replace("    ObjInstance *obj;\n", "")
    call = "        if (dbg) {\n            ShowCull(%s);\n        }\n" % ca
    ncs = ("|| (ptr_no_clip ? mask & %s : 0) " % ne) if nc else ""
    L = "    parts = D_8011E748[D_80149B8C];\n    mask = 1;\n    w = 0;\n" + bl
    L += "    for (i = 0; i < parts; i++) {\n" + lt
    L += "        if (i && !(i & 31)) {\n            mask = 1;\n" + inc + "        }\n"
    if cp == 'top': L += call
    L += "        if (((mask & %s) %s|| show) && !D_8014061A) {\n" % (pe, ncs)
    L += "            %s.flags &= ~0x80000000;\n        } else {\n            %s.flags |= 0x80000000;\n        }\n" % (lv, lv)
    if cp == 'mid': L += call
    L += "        mask <<= 1;\n" + le
    if cp == 'end': L += call
    L += "    }\n}\n"
    fn = 'b3/v%04d.c' % n
    open(fn, 'w').write(head + p + L)
    man.append('%s\t%s\t%s\tnc=%d\t%s\tdf=%d' % (fn, ok, wk, nc, cp, df)); n += 1
open('b3/manifest.tsv', 'w').write('\n'.join(man) + '\n')
print(n)
