# mkh.py BASE OUTDIR: fx_ok helper variants (properly non-recursive). Sites: the three
# `active_player_count<4 && (D_80156994 || D_8014978C>=6)` tests.
import sys, os
src = open(sys.argv[1]).read(); out = sys.argv[2]
COND = 'active_player_count<4 && (D_80156994 || D_8014978C>=6)'
assert src.count(COND) == 3
bodies = {
 'hA': 'return active_player_count<4 && (D_80156994 || D_8014978C>=6);',
 'hB': 'if (active_player_count<4 && (D_80156994 || D_8014978C>=6)) return 1;\n    return 0;',
 'hC': 'if (active_player_count>=4) return 0;\n    if (D_80156994) return 1;\n    if (D_8014978C>=6) return 1;\n    return 0;',
 'hD': 'if (active_player_count<4) {\n        if (D_80156994 || D_8014978C>=6) return 1;\n    }\n    return 0;',
 'hE': 's32 r = 0;\n    if (active_player_count<4 && (D_80156994 || D_8014978C>=6)) r = 1;\n    return r;',
}
anchor = 'void func_80090308(s16 car)\n'
for name, body in bodies.items():
    for where in ('before', 'after'):
        for sites in ('all', 'epu'):
            helper = 'static s32 fx_ok(void)\n{\n    %s\n}\n' % body
            s = src
            if sites == 'all':
                s = s.replace(COND, 'fx_ok()')
            else:
                i = s.index('void entity_physics_update')
                s = s[:i] + s[i:].replace(COND, 'fx_ok()')
            if where == 'before':
                s = s.replace(anchor, 'static s32 fx_ok(void);\n' + helper + anchor, 1) if False else s.replace(anchor, helper + anchor, 1)
            else:
                s = s.replace(anchor, 'static s32 fx_ok(void);\n' + anchor, 1) + '\n' + helper
            assert s.count('fx_ok()') >= 1 and 'fx_ok();' not in s.split('static s32 fx_ok(void)\n{')[1][:300]
            open(os.path.join(out, '%s_%s_%s.c' % (name, where[0], sites)), 'w').write(s)
