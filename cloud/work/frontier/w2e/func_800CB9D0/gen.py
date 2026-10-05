#!/usr/bin/env python3
"""gen.py OUT: write func_800CB9D0 variants (complete files) into OUT/."""
import sys, itertools
from pathlib import Path
here = Path(__file__).resolve().parent
pre = (here / 'pre.c').read_text()
out = Path(sys.argv[1]); out.mkdir(parents=True, exist_ok=True)
LOOP = '''    for (b = heap->first; b != 0; b = next) {
        if (b == old) {
            b = 0;
            break;
        }
        if (b->used != 0) {
            next = b->next;
        } else {
            if (b->size >= old->size) {
                break;
            }
            next = b->next;
            if (old == next) {
                adjacent = 1;
                break;
            }
        }
    }
    if (b == 0) {
        osJamMesg(&D_80152770, 0, 0);
        return;
    }
    b->owner = old->owner;
    *b->owner = (u8 *)b + 32;
    b->used = 1;
    b->tag = old->tag;
    b->count = old->count;
'''
def body(decl, pre_call, call, final, used_form, tailfix):
    s = 'void func_800CB9D0(void *addr)\n{\n' + decl + '''
    osRecvMesg(&D_80152770, 0, 1);
    heap = func_80095F8C(addr);
    old = func_80095EF4(heap, addr, 0);
    after = old->next;
    adjacent = 0;
''' + LOOP + pre_call + call + '''    if (adjacent || b->size - size >= 64) {
        n = (Block *)((u8 *)b + size + 32);
        if (adjacent) {
            n->next = after;
        } else {
            n->next = b->next;
        }
        if (n->next != 0) {
            n->next->prev = n;
        } else {
            heap->last = n;
        }
        n->prev = b;
''' + used_form + '''        n->magic = 0xFEDCBA98;
        b->next = n;
        b->size = size;
''' + tailfix + '    }\n' + final + '    osJamMesg(&D_80152770, 0, 0);\n}\n'
    return pre + s
USED = {
 'u0': '''        if (adjacent) {
            used = 0;
        } else {
            used = size + 32;
        }
        n->owner = 0;
        n->used = adjacent;
        n->size = b->size - used;
        n->tag = 0;
        n->count = 0;
''',
}
def decls(order, pad):
    d = {'heap':'Heap *heap;','old':'Block *old;','after':'Block *after;','n':'Block *n;','b':'Block *b;','next':'Block *next;','used':'u32 used;','adjacent':'s32 adjacent;','size':'u32 size;','src':'void *src;','pad':'s32 pad[%d];' % pad}
    return ''.join('    ' + d[o] + '\n' for o in order)
if __name__ == '__main__':
    base_order = ['heap','old','after','n','b','next','used','adjacent','size','pad','src']
    PRE = {'a': '    size = old->size;\n    src = (u8 *)old + 32;\n    old->owner = 0;\n'}
    CALL = {'s': '    func_800A47C0((u8 *)b + 32, src, size);\n',
            'e1': '    func_800A47C0((u8 *)b + 32, (u8 *)old + 32, size);\n',
            'e2': '    func_800A47C0((u8 *)b + 32, src, old->size);\n',
            'e3': '    func_800A47C0((u8 *)b + 32, (u8 *)old + 32, old->size);\n'}
    FIN = {'s': ('    audio_reverb_update(src, 0);\n', '        if (adjacent) {\n            src = (u8 *)n + 32;\n        }\n'),
           't': ('    audio_reverb_update(src, 0);\n', '        if (adjacent != 0) {\n            src = (u8 *)(n + 1);\n        }\n'),
          }
    PRES = {'a': '    size = old->size;\n    src = (u8 *)old + 32;\n    old->owner = 0;\n',
            'g': '    src = old + 1;\n    size = old->size;\n    old->owner = 0;\n',
            'h': '    size = old->size;\n    old->owner = 0;\n'}
    orders = {'o0': base_order}
    DT = {'v': 'void *src;', 'u8': 'u8 *src;', 'u32': 'u32 src;', 'blk': 'Block *src;'}
    for (pn, p), (cn, c), (fn, f), (on, o), (dn, dt) in itertools.product(PRES.items(), CALL.items(), FIN.items(), orders.items(), DT.items()):
        if pn == 'h' and cn in ('s', 'e2'): continue
        if pn == 'g' and dn != 'blk': continue
        if pn != 'g' and dn == 'blk': continue
        pp = p
        if pn == 'h': pp = p + ''  # src assigned after call
        cc = c
        if pn == 'h': cc = c + '    src = (u8 *)old + 32;\n'
        t = body(decls(o, 7).replace('void *src;', dt), pp, cc, f[0], USED['u0'], f[1])
        if dn == 'u32': t = t.replace('src = (u8 *)old + 32', 'src = (u32)old + 32').replace('src = (u8 *)n + 32', 'src = (u32)n + 32').replace('src = (u8 *)(n + 1)', 'src = (u32)(n + 1)').replace(', src, ', ', (void *)src, ').replace('update(src, 0)', 'update((void *)src, 0)')
        if dn == 'blk': t = t.replace('src = (u8 *)n + 32', 'src = n + 1').replace('src = (u8 *)(n + 1)', 'src = n + 1')
        (out / ('%s_%s_%s_%s_%s.c' % (pn, cn, fn, on, dn))).write_text(t)
