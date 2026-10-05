import itertools, sys
src = open('M3.c').read()
# remove unused decls
src = src.replace('    Data *data4;\n', '').replace('    s8 flag;\n', '')
CP = [
  ('            total2 = 0;\n            total = 0;\n', ['            total2 = 0;\n            total = 0;\n', '            total = 0;\n            total2 = 0;\n']),
  ('            cup2 = data->base->cups;\n            stunt = data->base->stunts;\n', ['            cup2 = data->base->cups;\n            stunt = data->base->stunts;\n', '            stunt = data->base->stunts;\n            cup2 = data->base->cups;\n']),
  ('            sum = 0;\n            data = D_8014A118[i].ref->car->data;\n            if (data == 0) {\n                continue;\n            }\n',
   ['            sum = 0;\n            data = D_8014A118[i].ref->car->data;\n            if (data == 0) {\n                continue;\n            }\n',
    '            data = D_8014A118[i].ref->car->data;\n            if (data == 0) {\n                continue;\n            }\n            sum = 0;\n']),
  ('            n0 = 0;\n            n1 = 0;\n            n2 = 0;\n            n3 = 0;\n            if (p->ref->car->data == 0) {\n                continue;\n            }\n',
   ['            n0 = 0;\n            n1 = 0;\n            n2 = 0;\n            n3 = 0;\n            if (p->ref->car->data == 0) {\n                continue;\n            }\n',
    '            if (p->ref->car->data == 0) {\n                continue;\n            }\n            n0 = 0;\n            n1 = 0;\n            n2 = 0;\n            n3 = 0;\n']),
  ('    Data *data;\n', ['    Data *data;\n', '']),   # decl moved to top (handled below)
  ('    SaveBase *base;\n', ['    SaveBase *base;\n', '']),
]
out = sys.argv[1]
n = 0
for combo in itertools.product(*[range(len(c[1])) for c in CP]):
    s = src
    extra = []
    for (anchor, alts), ch in zip(CP, combo):
        assert anchor in s, anchor
        s = s.replace(anchor, alts[ch], 1)
    if combo[4] == 1: extra.append('    Data *data;\n')
    if combo[5] == 1: extra.append('    SaveBase *base;\n')
    if extra:
        s = s.replace('    s16 i;\n', ''.join(extra) + '    s16 i;\n', 1)
    name = 'g' + ''.join(map(str, combo))
    open(f'{out}/{name}.c', 'w').write(s)
    n += 1
print(n)
