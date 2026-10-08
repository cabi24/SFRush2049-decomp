import itertools, random, sys, os
out = sys.argv[1]; n = int(sys.argv[2]); seed = int(sys.argv[3]) if len(sys.argv) > 3 else 1
os.makedirs(out, exist_ok=True)
HEAD = open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'func_800E7A98/b0/w11a.c')).read().split('void func_800E7A98')[0]
decls = {'p': 'Heap *p;', 'h': 'Heap *h;', 'next': 'Heap *next;'}
rng = random.Random(seed)
space = []
for dperm in itertools.permutations(['p', 'h', 'next']):
  for arm1 in [0, 1]:
    for arm2 in [0, 1]:
      for cmp in ['next == h', 'h == next']:
        for pos in ['before', 'after']:
          for lever in ['none', 'h', 'next', 'p']:
            for empty in [1, 0]:
              for loop in ['goto', 'while']:
                space.append((dperm, arm1, arm2, cmp, pos, lever, empty, loop))
ctrl = (('p', 'h', 'next'), 0, 0, 'next == h', 'before', 'none', 1, 'goto')
rng.shuffle(space)
space = [ctrl] + [s for s in space if s != ctrl]
for i, (dperm, arm1, arm2, cmp, pos, lever, empty, loop) in enumerate(space[:n]):
    decl = '\n'.join('    ' + decls[k] for k in dperm)
    a1 = ['        h = heap;\n        next = D_801527C8;\n', '        next = D_801527C8;\n        h = heap;\n']
    arm_then = a1[arm1]
    arm_else = ['        next = D_801527C8;\n        h = next;\n', '        h = D_801527C8;\n        next = h;\n'][arm2]
    # ensure equivalent: else arm alt keeps semantics: h = next = D_801527C8
    if arm2 == 1:
        arm_else = '        h = D_801527C8;\n        next = h;\n'
    if arm1 == 1:
        arm_then = '        next = D_801527C8;\n        h = heap;\n'
    lev = {'none': '', 'h': '    if (h) {}\n', 'next': '    if (next) {}\n', 'p': '    if (p) {}\n'}[lever]
    emp = '    if (heap == NULL) {\n    }\n' if empty else ''
    cmpop = cmp
    pre = ''
    if loop == 'goto':
        body = (pre + 'loop:\n    if (p != NULL) {\n        next = p->next;\n        if (%s) {\n            p->next = h->next;\n        } else {\n            p = next;\n            goto loop;\n        }\n    }\n' % cmpop)
    else:
        body = (pre + 'while (p != NULL) {\n        next = p->next;\n        if (%s) {\n            p->next = h->next;\n            break;\n        }\n        p = next;\n    }\n' % cmpop)
    if pos == 'after':
        body = body.replace('    p = next;\n', '', 1) if False else body
    src = HEAD + 'void func_800E7A98(Heap *heap)\n{\n' + decl + '\n' + emp + '    osRecvMesg(&D_80152770, NULL, 1);\n    if (heap != NULL) {\n' + arm_then + '    } else {\n' + arm_else + '    }\n' + ('    p = next;\n' + lev if pos == 'before' else lev + '    p = next;\n') + body + '    audio_reverb_update((u32)h, 1);\n    osJamMesg(&D_80152770, NULL, 0);\n}\n'
    open(os.path.join(out, 'v%04d.c' % i), 'w').write(src)
print(min(n, len(space)), 'variants of', len(space))
