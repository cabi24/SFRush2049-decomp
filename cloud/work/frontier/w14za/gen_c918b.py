# func_800C8918 round 2: END/START forms, inner limit copy, outer compare, volatile. Unit-scored.
import itertools, os, sys
out, srcf = sys.argv[1], sys.argv[2]
os.makedirs(out, exist_ok=True)
src = open(srcf).read()
head = src[:src.index('void func_800C8918')]
ENDA = ['end = D_801439D8;', 'end = &D_80142DD8[128];', 'end = D_80142DD8 + 128;', 'end = (ShutdownMsg *)D_801439D8;']
LIM = ['m != end', 'm != lim', 'm != D_801439D8']
STARTS = ['m = D_80142DD8;', 'm = &D_80142DD8[0];']
OUTER = ['m < end', 'end > m']
VOL = [True, False]
n = 0
for ea, lm, st, ou, vo in itertools.product(ENDA, LIM, STARTS, OUTER, VOL):
    mdecl = 'volatile ShutdownMsg *m;' if vo else 'ShutdownMsg *m;'
    limdecl = '    ShutdownMsg *lim;\n' if lm == 'm != lim' else ''
    limset = '        lim = end;\n' if lm == 'm != lim' else ''
    inner = lm.replace('m != end', 'm != end')
    body = '''void func_800C8918(void) {
    %s
    ShutdownMsg *end;
%s
    if (D_8011023C) {
        if (D_8011025C) {
            audio_effect_process(D_8011025C);
            D_8011025C = 0;
        }
        if (D_80110260) {
            audio_effect_process(D_80110260);
            D_80110260 = 0;
        }
        object_type1_create();
        object_type7_create();
        %s
%s        do {
            %s
            do {
                if (m->used) {
                    break;
                }
                m++;
            } while (%s);
        } while (%s);
        D_8011023C = 0;
        init_wait_completion();
        audio_effect_process(D_80110244);
        audio_effect_process(D_80110248);
        audio_effect_process(D_80110270);
    }
}
''' % (mdecl, limdecl, ea, limset, st, lm, ou)
    open(os.path.join(out, 'v%04d.c' % n), 'w').write(head + body)
    n += 1
print(n)
