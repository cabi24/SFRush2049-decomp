import itertools, random, sys, os
out = sys.argv[1]; n = int(sys.argv[2]); seed = int(sys.argv[3])
os.makedirs(out, exist_ok=True)
HEAD = open(os.path.join(os.path.dirname(os.path.abspath(__file__)), 'func_800E7A98/b0/w11a.c')).read().split('void func_800E7A98')[0]
decl = {'p': '    Heap *p;\n', 'h': '    Heap *h;\n', 'next': '    Heap *next;\n'}
levers = {'none': '', 'h': '    if (h) {}\n', 'next': '    if (next) {}\n', 'hn': '    if (h) {}\n    if (next) {}\n', 'nh': '    if (next) {}\n    if (h) {}\n'}
else_arms = {
 'copy': '        next = D_801527C8;\n        h = next;\n',
 'copy_lev': '        next = D_801527C8;\n        h = next;\n        if (h) {}\n',
 'hfirst': '        h = D_801527C8;\n        next = h;\n',
}
then_arms = {'hn': '        h = heap;\n        next = D_801527C8;\n', 'nh': '        next = D_801527C8;\n        h = heap;\n'}
rng = random.Random(seed)
space = []
for dp in itertools.permutations(['p', 'h', 'next']):
  for ea in else_arms:
    for ta in then_arms:
      for lev in levers:
        for cmp in ['next == h', 'h == next']:
          for loop in ['goto', 'while']:
            for emp in [1, 0]:
              space.append((dp, ea, ta, lev, cmp, loop, emp))
rng.shuffle(space)
ctrl = (('h', 'next', 'p'), 'copy', 'hn', 'none', 'next == h', 'goto', 1)
space = [ctrl] + [s for s in space if s != ctrl]
for i, (dp, ea, ta, lev, cmp, loop, emp) in enumerate(space[:n]):
    body_decl = ''.join(decl[k] for k in dp)
    lv = levers[lev]
    if loop == 'goto':
        loopsrc = 'loop:\n    if (p != NULL) {\n        next = p->next;\n        if (%s) {\n            p->next = h->next;\n        } else {\n            p = next;\n            goto loop;\n        }\n    }\n' % cmp
    else:
        loopsrc = 'while (p != NULL) {\n        next = p->next;\n        if (%s) {\n            p->next = h->next;\n            break;\n        }\n        p = next;\n    }\n' % cmp
    src = HEAD + 'void func_800E7A98(Heap *heap)\n{\n' + body_decl + ('    if (heap == NULL) {\n    }\n' if emp else '') + '    osRecvMesg(&D_80152770, NULL, 1);\n    if (heap != NULL) {\n' + then_arms[ta] + '    } else {\n' + else_arms[ea] + '    }\n' + lv + '    p = next;\n' + loopsrc + '    audio_reverb_update((u32)h, 1);\n    osJamMesg(&D_80152770, NULL, 0);\n}\n'
    open(os.path.join(out, 'v%04d.c' % i), 'w').write(src)
print(min(n, len(space)), 'of', len(space))
