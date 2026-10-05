import itertools, sys
src = open('L2.c').read().replace('    s8 flag;\n', '')
S1 = '            if (p->ref->car->data == 0) {\n'
S2C = '            if (D_8014A118[i].ref->car->data == 0) {\n'
S2 = '            data = D_8014A118[i].ref->car->data;\n            if (data == 0) {\n                continue;\n            }\n            for (k = 0; k < 12; k++) {\n                sum += data->base'
S3 = '            data = D_8014A118[i].ref->car->data;\n            if (data == 0) {\n                continue;\n            }\n            cup2 = data->base->cups;\n            stunt = data->base->stunts;\n'
S4 = '            data4 = D_8014A118[i].ref->car->data;\n            if (data4 == 0) {\n                continue;\n            }\n            base = data4->base;\n'
for a in (S1, S2C, S2, S3, S4): assert a in src, a
n = 0
for c1, c2, c3, c4, c5 in itertools.product(['', 'data', 'data4'], ['', 'data', 'data4'], ['data', 'data4'], ['data', 'data4'], ['data', 'data4']):
    s = src
    if c1: s = s.replace(S1, f'            {c1} = p->ref->car->data;\n            if ({c1} == 0) {{\n', 1)
    if c2: s = s.replace(S2C, f'            {c2} = D_8014A118[i].ref->car->data;\n            if ({c2} == 0) {{\n', 1)
    s = s.replace(S2, S2.replace('data', c3).replace('->' + c3, '->data'), 1)
    s = s.replace(S3, S3.replace('data', c4).replace('->' + c4, '->data'), 1)
    s = s.replace(S4, S4.replace('data4', c5).replace('->' + c5, '->data'), 1)
    name = 'h_%s_%s_%s_%s_%s' % (c1 or 'x', c2 or 'x', c3, c4, c5)
    open(f'{sys.argv[1]}/{name}.c', 'w').write(s); n += 1
print(n)
