import itertools, sys, os
out = sys.argv[1]
HDR = open('v1.c').read().split('void func_800F7F3C(void)')[0]
KEYS = {
 'stunt': ('gStuntPlayer[%s].totalPoints', '<'),
 'pts': ('gGameCar[%s].Points', '<'),
 'team': ('gTeamPoints[gCarTeam[%s]]', '<'),
 'place': ('gGameCar[%s].place', '>'),
}
def sort_arm(key, o):
    k, op = KEYS[key]
    decl_in = {'fn': '', 'inner_all': '%s a; %s b; %s ia; %s ib;\n                ' % (o['ab'], o['ab'], o['ix'], o['ix']),
               'inner_ix': '%s ia; %s ib;\n                ' % (o['ix'], o['ix'])}[o['decl']]
    if o['swap'] == 'ab':
        swap = 'gRankedPlayer[j + 1] = a;\n                    gRankedPlayer[j] = b;'
    else:
        swap = 's8 temp = gRankedPlayer[j + 1];\n                    gRankedPlayer[j + 1] = gRankedPlayer[j];\n                    gRankedPlayer[j] = temp;'
    return '''        for (i = 0; i < gNumPlayers - 1; i++) {
            for (j = 0; j < gNumPlayers - 1; j++) {
                %sa = gRankedPlayer[j];
                b = gRankedPlayer[j + 1];
                ia = model[a].slot;
                ib = model[b].slot;
                if (%s %s %s) {
                    %s
                }
            }
        }
''' % (decl_in, k % 'ia', op, k % 'ib', swap)
def tie_arm(key, o):
    k, op = KEYS[key]
    return '''        for (i = 1; i < gNumPlayers; i++) {
            f = model[gRankedPlayer[0]].slot;
            c = model[gRankedPlayer[i]].slot;
            if (%s == %s) {
                ties++;
                tied[i] = 1;
            }
        }
''' % (k % 'f', k % 'c')
axes = dict(decl=['fn', 'inner_all', 'inner_ix'], swap=['ab', 'temp'], ix=['s16', 's32'], ab=['s8', 's32'])
names = list(axes)
os.makedirs(out, exist_ok=True)
man = open(os.path.join(out, 'manifest.tsv'), 'w')
for n, combo in enumerate(itertools.product(*[axes[a] for a in names])):
    o = dict(zip(names, combo))
    fn_decl = '    s32 i;\n    s32 j;\n'
    if o['decl'] == 'fn':
        fn_decl += '    %s a;\n    %s b;\n' % (o['ab'], o['ab'])
    arm_decl = '        %s f; %s c;\n' % (o['ix'], o['ix'])
    if o['decl'] == 'fn':
        arm_decl = '        %s ia; %s ib; %s f; %s c;\n' % (o['ix'], o['ix'], o['ix'], o['ix'])
    if o['decl'] == 'inner_ix':
        fn_decl += '    %s a;\n    %s b;\n' % (o['ab'], o['ab'])
    body = 'void func_800F7F3C(void)\n{\n' + fn_decl + '\n    for (i = 0; i < gNumWindows; i++) gRankedPlayer[i] = i;\n'
    body += '    if (gRace == 4) {\n' + arm_decl + sort_arm('stunt', o) + tie_arm('stunt', o)
    body += '    } else if (gRace == 6) {\n' + arm_decl + sort_arm('pts', o) + sort_arm('team', o) + tie_arm('pts', o)
    body += '    } else {\n' + arm_decl + sort_arm('place', o) + tie_arm('place', o) + '    }\n}\n'
    open(os.path.join(out, 'v%04d.c' % n), 'w').write(HDR + body)
    man.write('v%04d.c\t%s\n' % (n, ' '.join('%s=%s' % kv for kv in o.items())))
